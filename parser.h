#include <stdio.h>
#include <stdlib.h>


#ifndef __PARSER_H__
#define __PARSER_H__

#include "cmd.h"


#define MAX_IF_BITS         31
#define IF_SEPARATOR_COMA   0xFFFF
#define IF_SEPARATOR_DASH   IF_SEPARATOR_COMA-1

#define IF_TYPE_ETH         1
#define IF_TYPE_MGMT        2
#define IF_TYPE_SONET       3
#define IF_TYPE_ATM         4
#define IF_TYPE_PPP         5


#define cprintf             printf
#define dprintf(_dlevel, _dargs) \
                                if(g_cli_dbg & _dlevel) \
                                    printf _dargs

void parser(cdb_t *cur_sptr_cdb, char *cmdline);
void parserInit(char *cliHost, cdb_t *sptr_cdb);
void parserCliSrart(char *cliHost);
cdb_node_t * getCdbNode(char *);
cdb_node_t * getCdbNextNode(char *);
cdb_node_t * getCdbExecCli(char *cmdline);
bool ifLineToIfMap(char *cmdLine, unsigned int *if_map);

bool decimalLineToDecimal(char *cmdLine, unsigned int *decimal);
bool hexLinetoHex(char *cmdLine, unsigned int *hex);
bool stringLineToString(char *cmdLine, char *str);
bool ipLineToIp(char *cmdLine, unsigned int *ip);

extern cdb_t g_sptr_cdb;
#endif /*__PARSER_H__*/
