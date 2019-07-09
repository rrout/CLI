#include <stdio.h>
#include <stdlib.h>

#ifndef __TEST_H__
#define __TEST_H__

#define MAX_PORTS           32

typedef enum {
    PORT_STATUS_UNKNOWN,
    PORT_STATUS_DISABLE,
    PORT_STATUS_ENABLE
}port_status_t;

typedef struct _port {
    unsigned int portNum;
    bool         portConfigured;
    unsigned int portStatus;
    unsigned int portSpeed;
    char         portName[20];
}PORT;

extern PORT g_port_list[MAX_PORTS];

void portConfigShow(PORT *portDb);
void portConfigChangeState(unsigned int if_map, port_status_t status);
void portConfigChangeName(unsigned int if_map, char *name);
void portShowBrief(unsigned int if_map);
void portShow(unsigned int if_map);


#endif /*__TEST_H__*/
