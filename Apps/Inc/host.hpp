#ifndef HOST_H
#define HOST_H

#include "main.h"
#include "bsp_core.h"

class Host
{
public:
    static PKT_ERR hostCommandInvoke(bool from_startup = false);
    static char* getHostName();
    static char* getConnectUser();
    static char* getDeviceName();
};

#endif
