#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd.h"
#include "parser.h"
#include "test.h"


PORT g_port_list[MAX_PORTS] = {{0}};

void portConfigShow(PORT *portDb)
{
    int i;
    if (!portDb)
        return;
    for( i = 1 ; i <= MAX_PORTS ; i++ )
    {
        if(portDb[i].portConfigured) {
            printf ("!\n");
            printf ("interface ethernet %d\n", i);
            if(strlen(portDb[i].portName) > 0)
                printf ("    name %s\n", portDb[i].portName);
            if (portDb[i].portStatus == PORT_STATUS_ENABLE)
                printf ("    enable\n");
            else if (portDb[i].portStatus == PORT_STATUS_DISABLE)
                printf ("    disable\n");
            printf ("!\n");
        }
    }
}

void portConfigChangeState(unsigned int if_map, port_status_t status)
{
    int i;
    for (i = 0; i <= MAX_IF_BITS; i++) {
        if (if_map & (1 << i)) {
            g_port_list[i].portStatus = status;
            if(g_port_list[i].portConfigured == FALSE)
                g_port_list[i].portConfigured =TRUE;
        }
    }
}

void portConfigChangeName(unsigned int if_map, char *name)
{
    int i;
    if (!name)
        return;
    for (i = 0; i <= MAX_IF_BITS; i++) {
        if (if_map & (1 << i)) {
            strcpy(g_port_list[i].portName, name);
            if(g_port_list[i].portConfigured == FALSE)
                g_port_list[i].portConfigured =TRUE;
        }
    }
}

void portShowBrief(unsigned int if_map)
{
    int i;
    char status_str[10];
    printf("Port cfg  speed  status   name\n");
    printf("---- ---- ------ -------- --------------\n");
    for (i = 0; i <= MAX_IF_BITS; i++) {
        if (if_map & (1 << i)) {
            if (g_port_list[i].portStatus == PORT_STATUS_ENABLE)
                strcpy(status_str,"enable");
            else if (g_port_list[i].portStatus == PORT_STATUS_DISABLE)
                strcpy(status_str, "disable");
            else
                strcpy(status_str, "unknown");
            printf("%-4d %-4s %-6d %-8s %s\n", i, (g_port_list[i].portConfigured)?"Yes":"No",
                g_port_list[i].portSpeed, status_str, g_port_list[i].portName);
        }
    }
}


void portShow(unsigned int if_map)
{
    int i;
    for (i = 0; i <= MAX_IF_BITS; i++) {
        if (if_map & (1 << i)) {
            if(g_port_list[i].portConfigured) {
                printf ("!\n");
                printf ("interface ethernet %d\n", i);
                if(strlen(g_port_list[i].portName) > 0)
                    printf ("    name %s\n", g_port_list[i].portName);
                if (g_port_list[i].portStatus == PORT_STATUS_ENABLE)
                    printf ("    status enable\n");
                else if (g_port_list[i].portStatus == PORT_STATUS_DISABLE)
                    printf ("    status disable\n");
                else
                    printf ("    status unknown\n");
                printf ("    speed %d\n", g_port_list[i].portSpeed);
                printf ("!\n");
            } else {
                printf ("interface ethernet %d -- Unconfigured\n", i);
            }
        }
    }
}

int main()
{
	/*parserCliSrart("Switch");*/
	extern void cmdCLIStart(char *cliHost);
	cmdCLIStart("Switch");
	return 1;
}


