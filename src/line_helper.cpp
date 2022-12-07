#include "line_helper.h"

#include <TridentTD_LineNotify.h>

bool hasInitLINE = false;

void LINE_Init()
{
}

void LINE_Update()
{
}

bool LINE_Connected()
{
    return false;
}

bool LINE_ProcessCommand(char* command[], int numParameter, bool hasInitWifi)
{
    if (numParameter == 2 && command[0][0] == 'L')  // set token
    {
        if (hasInitWifi)
        {
            LINE.setToken(command[1]);
            hasInitLINE = true;
            return true;
        }
        return false;
    }
    else if (numParameter == 2 && command[0][0] == 'N')
    {
        if (hasInitLINE)
        {
            return LINE.notify(command[1]);
        }
        return false;
    }
    else if (numParameter == 3 && command[0][0] == 'S')
    {
        if (hasInitLINE)
        {
            return LINE.notifySticker(atoi(command[1]), atoi(command[2]));
        }
        return false;
    }
    else if (numParameter == 4 && command[0][0] == 'M')
    {
        if (hasInitLINE)
        {
            return LINE.notifySticker(command[1], atoi(command[2]), atoi(command[3]));
        }
        return false;
    }
    return false;
}