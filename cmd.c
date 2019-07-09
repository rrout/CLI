#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd.h"
#include "cmd_list.h"
#include "parser.h"
#include "test.h"

unsigned int g_cli_dbg = 0;

mode_map_t g_mode_map[CMD_MODE_MAX] = {
    {CMD_MODE_MAX,     CMD_MODE_ROOT,   "root",          "$", cmd_root},
    {CMD_MODE_ROOT,    CMD_MODE_ENABLE, "config-enable", ">", cmd_enable},
    {CMD_MODE_ENABLE , CMD_MODE_CONFIG, "config-term",   "#", cmd_cfg},
    {CMD_MODE_CONFIG,  CMD_MODE_IF,     "config-if",     "#", cmd_cfg_if},
    {CMD_MODE_CONFIG,  CMD_MODE_MIF,    "config-mif",    "#", cmd_cfg},
    {CMD_MODE_CONFIG , CMD_MODE_NONE,   "config-none",   "#", cmd_cfg},
};

char * getCmdModeStr(cdb_cmd_mode_t mode)
{
    int i = 0;
    for( i = 0 ; i < CMD_MODE_MAX ; i++ )
    {
        if (mode == i)
            return g_mode_map[i].mode_str;
    }
    return "Unknown";
}

char * getCmdModePrompt(cdb_cmd_mode_t mode)
{
    int i = 0;
    for( i = 0 ; i < CMD_MODE_MAX ; i++ )
    {
        if (mode == i)
            return g_mode_map[i].mode_prompt;
    }
    return "*";
}

cdb_cmd_mode_t getCurrCmdMode()
{
    return getCmdModeFromModeStr(g_sptr_cdb.curr_mode_str);
}

cdb_cmd_mode_t getPrevCmdMode(cdb_cmd_mode_t mode)
{
    int i;
    for( i = 0 ; i < CMD_MODE_MAX ; i++ )
    {
        if (g_mode_map[i].mode == mode)
            return g_mode_map[i].prevMode;
    }
    return CMD_MODE_MAX;
}

cdb_cmd_mode_t getCmdModeFromModeStr(char *str)
{
    int i;
    for( i = 0 ; i < CMD_MODE_MAX ; i++ )
    {
        if (0 == strcmp(str, getCmdModeStr(i)))
        {
            return i;
        }
    }
    return CMD_MODE_MAX;
}

void setCmdModeParams(cdb_t *sptr_cdb, cdb_cmd_mode_t mode)
{
	sptr_cdb->mode = mode;
	snprintf(sptr_cdb->curr_mode_str, CMD_LEN, "%s", getCmdModeStr(mode));
	snprintf(sptr_cdb->cmd_prompt,CMD_LEN,"%s",getCmdModePrompt(mode));
    /* reset if_map id mode ts changing from CFG_IF */
    //TODO
}

void cleanUpCdbCmdParam(cdb_t *sptr_cdb)
{
    //int i = 0;
    sptr_cdb->curr_cmd_type = CMD_TYPE_MAX;
    strcpy(sptr_cdb->curr_cmd_str, "\0");
    sptr_cdb->numDec = 0;
    sptr_cdb->numHex = 0;
    sptr_cdb->numStr = 0;
    sptr_cdb->numIp = 0;
}

void cleanUpCdb(cdb_t *sptr_cdb)
{
    snprintf(sptr_cdb->curr_mode_usr_str, CMD_LINE_LEN, "%s", "");
}


cdb_node_t * getCmdNodeFromMode(cdb_cmd_mode_t mode)
{
    int i;
    for( i = 0 ; i < CMD_MODE_MAX ; i++ )
    {
        if (mode == i)
            return g_mode_map[i].modeRootNode;
    }
    return NULL;
}

bool cmdPreExecuteCmdToken(int last_token, cdb_cmd_type_t cmd_type, char *cmdToken, cdb_t *sptr_cdb)
{
    unsigned int if_map = 0;
    unsigned int decimal;
    unsigned int hex;
    unsigned int ip;
    char str[CMD_LINE_LEN] = {0};
    bool ret = TRUE;

    if (!cmdToken || !sptr_cdb)
    {
        printf ("Unknown Args\n");
        return FALSE;
    }
    switch (cmd_type)
    {
        case CMD_TYPE_IF_STRING:
            {
                ret = ifLineToIfMap(cmdToken, &if_map);
                if (ret == TRUE) {
                    sptr_cdb->if_map = if_map;
                    strncpy(sptr_cdb->if_str, cmdToken, CMD_LINE_LEN);
                }
            }
            break;
        case CMD_TYPE_DECIMAL:
            {
                ret = decimalLineToDecimal(cmdToken, &decimal);
                if (ret == TRUE) {
                   sptr_cdb->decimal[sptr_cdb->numDec] = decimal;
                   sptr_cdb->numDec++;
                }
            }
            break;
        case CMD_TYPE_HEX:
            {
                ret = hexLinetoHex(cmdToken, &hex);
                if (ret == TRUE) {
                    sptr_cdb->hex[sptr_cdb->numHex] = hex;
                    sptr_cdb->numHex++;
                }
            }
            break;
        case CMD_TYPE_STRING:
            {
                ret = stringLineToString(cmdToken, str);
                if (ret == TRUE) {
                    strcpy (sptr_cdb->str[sptr_cdb->numStr], str);
                    sptr_cdb->numStr++;
                }
            }
            break;
        case CMD_TYPE_IP:
            {
                ret = ipLineToIp(cmdToken, &ip);
            }
            break;
        default:
            ret = TRUE;
            break;
    }

    return ret;
}

bool cmdValidateCmdToken(int last_token, cdb_cmd_type_t cmd_type, char *cmdToken)
{
    unsigned int if_map = 0;
    unsigned int decimal;
    unsigned int hex;
    unsigned int ip;
    char str[CMD_LINE_LEN] = {0};
    bool ret = TRUE;

    if (!cmdToken)
    {
        printf ("Unknown Args\n");
        return FALSE;
    }
    switch (cmd_type)
    {
        case CMD_TYPE_IF_STRING:
            {
                ret = ifLineToIfMap(cmdToken, &if_map);
            }
            break;
        case CMD_TYPE_DECIMAL:
            {
                ret = decimalLineToDecimal(cmdToken, &decimal);
            }
            break;
        case CMD_TYPE_HEX:
            {
                ret = hexLinetoHex(cmdToken, &hex);
            }
            break;
        case CMD_TYPE_STRING:
            {
                ret = stringLineToString(cmdToken, str);
            }
            break;
        case CMD_TYPE_IP:
            {
                ret = ipLineToIp(cmdToken, &ip);
            }
            break;
        default:
            ret = TRUE;
            break;
    }

    return ret;
}

bool cmdNodeInsertCbStack(cdb_cb_stack_t *cbStack, cdb_node_t *node, char * cmd_str, bool last_node)
{
    int nodeNum = 0;
    if (!cbStack || !node || cbStack->numNodes > CMD_MAX_TOKEN)
        return FALSE;

    nodeNum = cbStack->numNodes;
    strcpy(cbStack->cb[nodeNum].cmd, node->cmd);
    strcpy(cbStack->cb[nodeNum].cmd_str, cmd_str);
    cbStack->cb[nodeNum].cmd_type = node->cmd_type;
    cbStack->cb[nodeNum].cmd_callback = node->cmd_callback;
    if (last_node == TRUE)
        cbStack->cb[nodeNum].last_node = TRUE;
    cbStack->numNodes++;

    return TRUE;
}

bool cmdCheckBuildCallBackStack(int is_full_cmd,
    char *cmd_str, cdb_node_t *node, cdb_cb_stack_t *cbStack)
{
    if (node)
    {
       if (is_full_cmd)
       {
           if (!(node->flags & CMD_FLAG_CR_ALLOWED)) {
               printf("Incomplete command !\n");
               return FALSE;
            } else {
                cmdNodeInsertCbStack(cbStack, node, cmd_str, is_full_cmd);
           }
       } else {
           cmdNodeInsertCbStack(cbStack, node, cmd_str, is_full_cmd);
       }
    }
    else
    {
        printf("Invalid command node!\n");
    }
    return TRUE;
}

void cmdPrintCbStack(cdb_cb_stack_t *cbStack)
{
    int i = 0;
    if (!cbStack)
        return;
    dprintf(CMD_DBG_LEVEL_ALL,("===========CB STACK===============\n"));
    for( i = 0 ; i < cbStack->numNodes ; i++ )
    {
        dprintf(CMD_DBG_LEVEL_ALL,
            ("Cmd:%-10s(%-5d)[%-10s] Callback:%p\n",cbStack->cb[i].cmd, cbStack->cb[i].cmd_type, cbStack->cb[i].cmd_str, cbStack->cb[i].cmd_callback));
    }
    dprintf(CMD_DBG_LEVEL_ALL,("===========STACK END===============\n"));
}

void cmdExecCbStack(cdb_cb_stack_t *cbStack, cdb_t *sptr_cdb)
{
    int i = 0;
    if (!cbStack || !sptr_cdb)
        return;

    for( i = cbStack->numNodes ; i > 0 ; i-- )
    {
        dprintf(CMD_DBG_LEVEL_ALL,("Exec Cmd : %-10s (%-10s) CB:%p\n",cbStack->cb[i-1].cmd, cbStack->cb[i-1].cmd_str, cbStack->cb[i-1].cmd_callback));
        if (cbStack->cb[i-1].cmd_callback) {
            sptr_cdb->last_cmd_token = cbStack->cb[i-1].last_node;
            strcpy(sptr_cdb->curr_cmd_str, cbStack->cb[i-1].cmd_str);
            sptr_cdb->curr_cmd_type = cbStack->cb[i-1].cmd_type;
            cbStack->cb[i-1].cmd_callback(sptr_cdb);
            sptr_cdb->last_cmd_token = FALSE;
        }
    }
}

void cmdBrowseNode(cdb_cmd_mode_t mode,cdb_node_t *node, cdb_t *sptr_cdb)
{
    int curnode = 0, subnode = 0;
    if(!node)
        return;

    do {
        curnode = subnode;
        dprintf(CMD_DBG_LEVEL_ALL, ("Exec callback for cmd %s\n",node[subnode].cmd));
        if(node[subnode].cmd_callback) {
            if (node[subnode].mode_flags == mode)
                node[subnode].cmd_callback(sptr_cdb);
        }
        if (node[subnode].next_node)
            cmdBrowseNode(mode, node[subnode].next_node, sptr_cdb);
        subnode++;
    }while (!(node[curnode].flags & CMD_FLAG_LAST));
}

void cmdPrintNode(cdb_cmd_mode_t mode,cdb_node_t *node, cdb_t *sptr_cdb)
{
    int curnode = 0, subnode = 0, i = 0;
    static char cmd_list[CMD_CDB_MAX_STR][CMD_LEN];
    static int cmd_cnt;
    if(!node) {
        if (cmd_cnt > 0)
            cmd_cnt--;
        return;
    }

    do {
        curnode = subnode;
        dprintf(CMD_DBG_LEVEL_ALL, ("Exec callback for cmd %s\n",node[subnode].cmd));
        if(node[subnode].cmd_type == CMD_TYPE_CMD) {
            strcpy(cmd_list[cmd_cnt++], node[subnode].cmd);
        } else {
            strcpy(cmd_list[cmd_cnt++], "<>");
        }
        if (node[subnode].flags & CMD_FLAG_CR_ALLOWED) {
            for( i = 0 ; i < cmd_cnt ; i++ )
            {
                printf("%s ", cmd_list[i]);
            }
            printf("\n");
        }
        cmdPrintNode(mode, node[subnode].next_node, sptr_cdb);
        subnode++;
    }while (!(node[curnode].flags & CMD_FLAG_LAST));

    if (cmd_cnt > 0)
        cmd_cnt--;
}

void cmdConfigDebug(int dbg)
{
    int i;
    if (!dbg)
        return;
    printf ("!\n");
    if(dbg == CMD_CDB_MAX_DEC_VAL) {
        printf ("debug cli all\n");
    } else {
        for (i = 0; i <= MAX_IF_BITS; i++) {
            if (dbg & (1 << i))
                printf ("debug cli level %d\n", i);
        }
    }
    printf ("!\n");
}

void cmdConfigIf(int *ifDb)
{
    portConfigShow(g_port_list);
}


