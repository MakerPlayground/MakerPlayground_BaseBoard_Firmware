#include "ntp_helper.h"

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

bool hasInitNTP = false;

void NTP_Init()
{
}

void NTP_Update()
{
    if (hasInitNTP)
    {
        timeClient.update();
    }
}

bool NTP_Connected()
{
    return timeClient.isTimeSet();
}

bool NTP_ProcessCommand(char* command[], int numParameter, bool hasInitWifi)
{
    if (numParameter == 1 && command[0][0] == 'I')  // init connection to ntp server
    {
        if (hasInitWifi)
        {
            timeClient.begin();
            timeClient.setTimeOffset(25200);
            hasInitNTP = true;
            return true;
        }
        return false;
    }
    else if (numParameter == 1 && command[0][0] == 'E')
    {
        if (hasInitNTP)
        {
            Serial.println(timeClient.getEpochTime());
            return true;
        }
        return false;
    }
    else if (numParameter == 1 && command[0][0] == 'T')
    {
        if (hasInitNTP)
        {
            unsigned long currentEpoch = timeClient.getEpochTime();
            Serial.print(day(currentEpoch));
            Serial.print(' ');
            Serial.print(month(currentEpoch));
            Serial.print(' ');
            Serial.print(year(currentEpoch));
            Serial.print(' ');
            Serial.print(hour(currentEpoch));
            Serial.print(' ');
            Serial.print(minute(currentEpoch));
            Serial.print(' ');
            Serial.println(second(currentEpoch));
            return true;
        }
        return false;
    }
    return false;
}