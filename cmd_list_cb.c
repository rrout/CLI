#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "cmd.h"
#include "test.h"

#define PRINT_LAST_TOKEN_AND_RETURN    { \
                                        dprintf(CMD_DBG_LEVEL_ALL,\
                                        ("%s: sptr_cdb->last_cmd_token = %d ... return\n", \
                                        __FUNCTION__, sptr_cdb->last_cmd_token)); \
                                        return;\
                                       }

#define PRINT_CFG_GEN_AND_RETURN    {\
                                        dprintf(CMD_DBG_LEVEL_ALL,\
                                        ("%s: sptr_cdb->curr_cmd_cfg_gen = %d ... return\n", \
                                        __FUNCTION__, sptr_cdb->curr_cmd_cfg_gen)); \
                                        return;\
                                    }

void enable_config_terminal(cdb_t *sptr_cdb)
{
    if (sptr_cdb->curr_cmd_cfg_gen) {
        printf ("!\n");
        printf ("config terminal\n");
        printf ("!\n");
        return;
    }
    setCmdModeParams(sptr_cdb, CMD_MODE_CONFIG);
}
void cmd_show_version(cdb_t *sptr_cdb)
{
    if (sptr_cdb->curr_cmd_cfg_gen)
        PRINT_CFG_GEN_AND_RETURN
    if (sptr_cdb->last_cmd_token == 0)
        PRINT_LAST_TOKEN_AND_RETURN
    printf ("CLI Parser V.0.01\n");
}
void cmd_show_configurations(cdb_t *sptr_cdb)
{
    if (sptr_cdb->curr_cmd_cfg_gen)
        PRINT_CFG_GEN_AND_RETURN
    if (sptr_cdb->last_cmd_token == 0)
        PRINT_LAST_TOKEN_AND_RETURN
    printf ("====== Current Saved Configuration ========\n");
}
void cmd_show_global_config_ver(cdb_t *sptr_cdb)
{
    if (sptr_cdb->curr_cmd_cfg_gen)
        PRINT_CFG_GEN_AND_RETURN
    if (sptr_cdb->last_cmd_token == 0)
        PRINT_LAST_TOKEN_AND_RETURN
    printf ("====== Current Global Config List ========\n");
    printf ("\n");
}

void enable_cmd_prompt(cdb_t *sptr_cdb)
{
    if (sptr_cdb->curr_cmd_cfg_gen) {
        printf ("!\n");
        printf ("enable\n");
        printf ("!\n");
        return;
    }
    setCmdModeParams(sptr_cdb, CMD_MODE_ENABLE);
}

void config_interface(cdb_t *sptr_cdb)
{
    if (sptr_cdb->curr_cmd_cfg_gen)
        PRINT_CFG_GEN_AND_RETURN
    setCmdModeParams(sptr_cdb, CMD_MODE_IF);
    printf ("Configuring if: %s\n",sptr_cdb->curr_mode_usr_str);
}

void config_if_type_enet(cdb_t *sptr_cdb)
{
    char str[255] = {0};
    char type_char = 'e';

    if (sptr_cdb->curr_cmd_cfg_gen) {
        cmdConfigIf(NULL);
        return;
    }
    sptr_cdb->if_type =  IF_TYPE_ETH;
    extern bool ifMapToIfLine(unsigned int *if_map, char *ifLine);
    ifMapToIfLine(&sptr_cdb->if_map, str);
    sprintf(sptr_cdb->curr_mode_usr_str, "(%c-%s)", type_char, str);
}

void config_if_type_mgmt(cdb_t *sptr_cdb)
{
    if (sptr_cdb->curr_cmd_cfg_gen)
        PRINT_CFG_GEN_AND_RETURN
    sptr_cdb->if_type =  IF_TYPE_MGMT;
}

void config_if_list(cdb_t *sptr_cdb)
{
    unsigned int if_map = 0;
    bool ret = TRUE;

    if (sptr_cdb->curr_cmd_cfg_gen)
        PRINT_CFG_GEN_AND_RETURN

    if (sptr_cdb->last_cmd_token == 0)
        PRINT_LAST_TOKEN_AND_RETURN
    ret = ifLineToIfMap(sptr_cdb->curr_cmd_str, &if_map);
    if (ret == TRUE) {
        sptr_cdb->if_map = if_map;
        strncpy(sptr_cdb->if_str, sptr_cdb->curr_cmd_str, CMD_LINE_LEN);
    }
    char str[255] = {0};
    char type_char = ' ';
    if (sptr_cdb->if_type == IF_TYPE_ETH)
        type_char = 'e';
    if (sptr_cdb->if_type == IF_TYPE_MGMT)
        type_char = 'm';
    extern bool ifMapToIfLine(unsigned int *if_map, char *ifLine);
    ifMapToIfLine(&sptr_cdb->if_map, str);
    dprintf(CMD_DBG_LEVEL_ALL, ("Setting if list%s\n", str));
    printf ("Current if: %x(%s)---%c-%s\n", sptr_cdb->if_map, sptr_cdb->if_str, type_char, str);
    //sprintf(sptr_cdb->curr_mode_usr_str, "(%c-%s)", type_char, str);
}
void config_if_enable(cdb_t *sptr_cdb)
{
    if (sptr_cdb->curr_cmd_cfg_gen)
        PRINT_CFG_GEN_AND_RETURN

    if (sptr_cdb->last_cmd_token == 0)
        PRINT_LAST_TOKEN_AND_RETURN
    portConfigChangeState(sptr_cdb->if_map, PORT_STATUS_ENABLE);
}

void config_if_disable(cdb_t *sptr_cdb)
{
    if (sptr_cdb->curr_cmd_cfg_gen)
        PRINT_CFG_GEN_AND_RETURN

    if (sptr_cdb->last_cmd_token == 0)
        PRINT_LAST_TOKEN_AND_RETURN
    portConfigChangeState(sptr_cdb->if_map, PORT_STATUS_DISABLE);
}

void config_set_string(cdb_t *sptr_cdb)
{
    if(strlen(sptr_cdb->curr_cmd_str) > 0)
        strcpy(sptr_cdb->str[sptr_cdb->numStr], sptr_cdb->curr_cmd_str);
    dprintf(CMD_DBG_LEVEL_ALL, ("Setting String value %sin sptr_cdb\n", sptr_cdb->curr_cmd_str));
}

void config_if_name(cdb_t *sptr_cdb)
{
    if (sptr_cdb->curr_cmd_cfg_gen)
        PRINT_CFG_GEN_AND_RETURN
    portConfigChangeName(sptr_cdb->if_map, sptr_cdb->str[sptr_cdb->numStr]);
}

void cmd_show_interface_config(cdb_t *sptr_cdb)
{
    if (sptr_cdb->curr_cmd_cfg_gen)
        PRINT_CFG_GEN_AND_RETURN
    if (sptr_cdb->last_cmd_token == 0)
        PRINT_LAST_TOKEN_AND_RETURN
    if(sptr_cdb->mode == CMD_MODE_IF)
        portShow(sptr_cdb->if_map);
    else
        portShow(CMD_CDB_MAX_DEC_VAL-1);
}

void cmd_show_interface_brief(cdb_t *sptr_cdb)
{
    if (sptr_cdb->curr_cmd_cfg_gen)
        PRINT_CFG_GEN_AND_RETURN
    if (sptr_cdb->last_cmd_token == 0)
        PRINT_LAST_TOKEN_AND_RETURN
    if(sptr_cdb->mode == CMD_MODE_IF)
        portShowBrief(sptr_cdb->if_map);
    else
        portShowBrief(CMD_CDB_MAX_DEC_VAL);
}

void cmd_set_dbg_cli(cdb_t *sptr_cdb)
{
    if (sptr_cdb->curr_cmd_cfg_gen) {
        cmdConfigDebug(g_cli_dbg);
        return;
    }

    if (sptr_cdb->decimal[sptr_cdb->numDec] == CMD_CDB_MAX_DEC_VAL)
        g_cli_dbg = CMD_CDB_MAX_DEC_VAL;
    else if (sptr_cdb->decimal[sptr_cdb->numDec] > CMD_DBG_LEVEL_MAX)
        printf ("Level is beyond max level %d\n",CMD_DBG_LEVEL_MAX);
    else
        g_cli_dbg |= (1 << sptr_cdb->decimal[sptr_cdb->numDec]);
    dprintf(CMD_DBG_LEVEL_ALL, ("Setting Debug Cli\n"));
}

void cmd_set_undbg_cli(cdb_t *sptr_cdb)
{
    if (sptr_cdb->curr_cmd_cfg_gen)
        PRINT_CFG_GEN_AND_RETURN

    if (sptr_cdb->decimal[sptr_cdb->numDec] == CMD_CDB_MAX_DEC_VAL)
        g_cli_dbg = 0;
    else
        g_cli_dbg &= ~(1 << sptr_cdb->decimal[sptr_cdb->numDec]);
    dprintf(CMD_DBG_LEVEL_ALL, ("Setting Un-Debug ALL\n"));
}

void cmd_set_undbg_all(cdb_t *sptr_cdb)
{
    if (sptr_cdb->curr_cmd_cfg_gen)
        PRINT_CFG_GEN_AND_RETURN

    if (sptr_cdb->last_cmd_token == 0)
        PRINT_LAST_TOKEN_AND_RETURN
    /* reset all debug variables */
    g_cli_dbg = 0;
}

void cmd_set_dbg_level_val(cdb_t *sptr_cdb)
{
    unsigned int decimal;

    if (sptr_cdb->curr_cmd_cfg_gen)
        PRINT_CFG_GEN_AND_RETURN

    if(strlen(sptr_cdb->curr_cmd_str) > 0)
        decimalLineToDecimal(sptr_cdb->curr_cmd_str, &decimal);
        sptr_cdb->decimal[sptr_cdb->numDec] = decimal;
    dprintf(CMD_DBG_LEVEL_ALL, ("Setting Debug value %d in sptr_cdb\n", decimal));
}

void cmd_set_dbg_level_all(cdb_t *sptr_cdb)
{
    if (sptr_cdb->curr_cmd_cfg_gen)
        PRINT_CFG_GEN_AND_RETURN

    if (sptr_cdb->last_cmd_token == 0)
        PRINT_LAST_TOKEN_AND_RETURN
    sptr_cdb->decimal[sptr_cdb->numDec] = 0xFFFFFFFF;
}

void cmd_show_debug(cdb_t *sptr_cdb)
{
    if (sptr_cdb->curr_cmd_cfg_gen)
        PRINT_CFG_GEN_AND_RETURN

    if (sptr_cdb->last_cmd_token == 0)
        PRINT_LAST_TOKEN_AND_RETURN

    cmdConfigDebug(g_cli_dbg);
}

void cmd_show_running_config(cdb_t *sptr_cdb)
{
    if (sptr_cdb->curr_cmd_cfg_gen)
        PRINT_CFG_GEN_AND_RETURN

    if (sptr_cdb->last_cmd_token == 0)
        PRINT_LAST_TOKEN_AND_RETURN

    sptr_cdb->curr_cmd_cfg_gen = TRUE;

    /* Browse root node */
    cmdBrowseNode(CMD_MODE_ROOT, cmd_root, sptr_cdb);
    cmdBrowseNode(CMD_MODE_ENABLE, cmd_enable, sptr_cdb);
    cmdBrowseNode(CMD_MODE_CONFIG, cmd_cfg, sptr_cdb);
    cmdBrowseNode(CMD_MODE_IF, cmd_cfg_if, sptr_cdb);

    sptr_cdb->curr_cmd_cfg_gen = FALSE;
}

void cmd_show_cli(cdb_t *sptr_cdb)
{
    if (sptr_cdb->curr_cmd_cfg_gen)
        PRINT_CFG_GEN_AND_RETURN
    if (sptr_cdb->last_cmd_token == 0)
        PRINT_LAST_TOKEN_AND_RETURN
    printf("===== Mode : ROOT             ==\n");
    cmdPrintNode(CMD_MODE_ROOT, cmd_root, sptr_cdb);
    printf("\n===== Mode : ENABLE           ==\n");
    cmdPrintNode(CMD_MODE_ENABLE, cmd_enable, sptr_cdb);
    printf("\n===== Mode : CONFIG           ==\n");
    cmdPrintNode(CMD_MODE_CONFIG, cmd_cfg, sptr_cdb);
    printf("\n===== Mode : CONFIG-INTERFACE ==\n");
    cmdPrintNode(CMD_MODE_IF, cmd_cfg_if, sptr_cdb);
}


