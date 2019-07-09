#include <stdio.h>
#include <stdlib.h>


#ifndef __CMD_H__
#define __CMD_H__
#define CMD_LEN				        20
#define CMD_DESC_LEN		        255
#define CMD_LINE_LEN		        255
#define CMD_MAX_TOKEN               20
#define CMD_MAX_TOKEN_LEN           50
#define CMD_HOST_NAME_LEN	        55

#define CMD_CDB_MAX_DEC             20
#define CMD_CDB_MAX_HEX             20
#define CMD_CDB_MAX_STR             20
#define CMD_CDB_MAX_IP              20

#define CMD_CDB_MAX_DEC_VAL         0xFFFFFFFF
#define CMD_CDB_MAX_HEX_VAL         0xFFFFFFFF


#define CMD_FLAG_FIRST              1 << 0
#define CMD_FLAG_LAST               1 << 1
#define CMD_FLAG_NEXT               1 << 2
#define CMD_FLAG_CR_ALLOWED         1 << 3

#define CMD_DBG_LEVEL_ALL   1 << 0
#define CMD_DBG_LEVEL_1     1 << 1
#define CMD_DBG_LEVEL_2     1 << 2
#define CMD_DBG_LEVEL_MAX   31

typedef enum {
    FALSE = 0,
    TRUE  = 1,
}bool;

typedef enum {
	CMD_MODE_ROOT,
	CMD_MODE_ENABLE,
	CMD_MODE_CONFIG,
	CMD_MODE_IF,
	CMD_MODE_MIF,
	CMD_MODE_NONE,
	CMD_MODE_MAX,
}cdb_cmd_mode_t;

typedef enum {
    CMD_TYPE_CMD,
    CMD_TYPE_IF_STRING,
    CMD_TYPE_STRING,
    CMD_TYPE_DECIMAL,
    CMD_TYPE_HEX,
    CMD_TYPE_IP,
    CMD_TYPE_MAX,
}cdb_cmd_type_t;

typedef struct _cdb {
	char cmd_hostname[CMD_HOST_NAME_LEN];
	cdb_cmd_mode_t mode;
	char curr_mode_str[CMD_LEN];
    char curr_mode_usr_str[CMD_LINE_LEN];
	char cmd_prompt[2];
    unsigned int last_cmd_token;

    /*
     * each time callback is called
     * we should know what command token
     * is getting executed
     */
    cdb_cmd_type_t curr_cmd_type;
    char curr_cmd_str[CMD_LEN];
    bool curr_cmd_cfg_gen;

    /*
     * Command Params
     */
    unsigned char if_type;
    unsigned int if_map;
    char if_str[CMD_LINE_LEN];

    unsigned int numDec;
    unsigned int decimal[CMD_CDB_MAX_DEC];

	unsigned int number1;
	unsigned int number2;
	unsigned int number3;
	unsigned int number4;
	unsigned int number5;
	unsigned int number6;
	unsigned int number7;
	unsigned int number8;
	unsigned int number9;
	unsigned int number10;
	unsigned int number11;

    unsigned int numStr;
    char str[CMD_CDB_MAX_STR][CMD_LEN];

	char str1[CMD_LEN];
	char str2[CMD_LEN];
	char str3[CMD_LEN];
	char str4[CMD_LEN];
	char str5[CMD_LEN];
	char str6[CMD_LEN];
	char str7[CMD_LEN];
	char str8[CMD_LEN];
	char str9[CMD_LEN];
	char str10[CMD_LEN];
	char str11[CMD_LEN];

    unsigned int numHex;
    unsigned int hex[CMD_CDB_MAX_HEX];

    unsigned int numIp;
    unsigned int ip[CMD_CDB_MAX_IP];
}cdb_t;

typedef struct _cdb_node {
	unsigned int mode_flags;
    cdb_cmd_type_t cmd_type;
	char cmd[CMD_LEN];
	char cmd_desc[CMD_DESC_LEN];
	void (*cmd_callback) (cdb_t *sptr_cdb);
	struct _cdb_node *next_node;
	unsigned int flags;
}cdb_node_t;

typedef struct _cdb_cb {
    char cmd[CMD_LEN];
    char cmd_str[CMD_LEN];
    cdb_cmd_type_t cmd_type;
    bool last_node;
    void (*cmd_callback) (cdb_t *sptr_cdb);
}cdb_cb_t;

typedef struct _cdb_cb_stack {
    int numNodes;
    cdb_cb_t cb[CMD_MAX_TOKEN];
}cdb_cb_stack_t;

typedef struct _mode_map {
    cdb_cmd_mode_t prevMode;
    cdb_cmd_mode_t mode;
    char *mode_str;
    char *mode_prompt;
    cdb_node_t *modeRootNode;
}mode_map_t;



/*void (cmd_callback *) (cdb_t *sptr_cdb);*/
/*typedef enum {
	CMD_FLAG_FIRST,
	CMD_FLAG_LAST,
	CMD_FLAG_NEXT,
    CMD_FLAG_CR_ALLOWED
}cdb_cmd_flag_t;*/


extern unsigned int g_cli_dbg;


extern cdb_node_t cmd_root[];
extern cdb_node_t cmd_enable[];
extern cdb_node_t cmd_cfg[];
extern cdb_node_t cmd_cfg_if[];
extern cdb_node_t cmd_cfg_terminal[];
extern cdb_node_t cmd_show[];


char * getCmdModeStr(cdb_cmd_mode_t mode);
char * getCmdModePrompt(cdb_cmd_mode_t mode);
cdb_cmd_mode_t getCurrCmdMode();
cdb_cmd_mode_t getPrevCmdMode(cdb_cmd_mode_t mode);
cdb_cmd_mode_t getCmdModeFromModeStr(char *str);
void setCmdModeParams(cdb_t *sptr_cdb, cdb_cmd_mode_t mode);
cdb_node_t * getCmdNodeFromMode(cdb_cmd_mode_t mode);
cdb_node_t * getPrevCmdNodeFromMode(cdb_cmd_mode_t mode);
cdb_cmd_mode_t getPrevCmdModeFromMode(cdb_cmd_mode_t mode);
bool cmdPreExecuteCmdToken(int last_token, cdb_cmd_type_t cmd_type, char *cmdToken, cdb_t *sptr_cdb);
bool cmdValidateCmdToken(int last_token, cdb_cmd_type_t cmd_type, char *cmdToken);
bool cmdNodeInsertCbStack(cdb_cb_stack_t *cbStack, cdb_node_t *node, char *cmd_str, bool last_node);
bool cmdCheckBuildCallBackStack(int is_full_cmd, char *cmd_str, cdb_node_t *node, cdb_cb_stack_t *cbStack);
void cmdPrintCbStack(cdb_cb_stack_t *cbStack);
void cmdExecCbStack(cdb_cb_stack_t *cbStack, cdb_t *sptr_cdb);
void cleanUpCdbCmdParam(cdb_t *sptr_cdb);
void cleanUpCdb(cdb_t *sptr_cdb);
void cmdBrowseNode(cdb_cmd_mode_t mode,cdb_node_t *node, cdb_t *sptr_cdb);
void cmdPrintNode(cdb_cmd_mode_t mode,cdb_node_t *node, cdb_t *sptr_cdb);


void cmdConfigDebug(int dbg);
void cmdConfigIf(int *ifDb);

#endif /*__CMD_H__*/
