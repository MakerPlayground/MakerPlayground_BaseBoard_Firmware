#define BLYNK_SEND_GAP 100

#include "blynk_helper.h"

#include <BlynkSimpleEsp8266.h>

double virtualPinValue[8];
uint8_t virtualPinChanged;

unsigned long lastSendMillis = 0;
bool hasInitBlynk = false;

void Blynk_Init()
{
}

void Blynk_Update() 
{
    if (hasInitBlynk)
    {
        Blynk.run();
        if (millis() - lastSendMillis > BLYNK_SEND_GAP) 
        {
            for (uint8_t i=0; i<8; i++)
            {
                if (virtualPinChanged & (1 << i)) 
                {
                    Blynk.virtualWrite(i, virtualPinValue[i]);
                    virtualPinChanged &= ~(1 << i);
                }
            }
            lastSendMillis = millis();
        }
    }
}

bool Blynk_ProcessCommand(char* command[], int numParameter, bool hasInitWifi) 
{
    if (numParameter == 2 && command[0][0] == 'B')  // connect to blynk
    {
        if (hasInitWifi)
        {
            Blynk.config(command[1]);   // auth token
            if (Blynk.connect())
            {
                hasInitBlynk = true;
                return true;
            }
        }
        return false;
    } 
    else if (numParameter == 3 && command[0][0] == 'V')  // blynk write (value)
    {
        if (hasInitBlynk)
        {
            int pin = atoi(command[1]);
            virtualPinValue[pin] = atof(command[2]);
            virtualPinChanged |= (1 << pin); 
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (numParameter == 2 && command[0][0] == 'R')
    {
        if (hasInitBlynk)
        {
            Serial.println(virtualPinValue[atoi(command[1])]);
            return true;
        }
        return false;
    }
    else if (numParameter >= 3 && command[0][0] == 'C')  // blynk write (command)
    {
        if (hasInitBlynk)
        {
            if (numParameter == 3)
            {
                Blynk.virtualWrite(atoi(command[1]), command[2]); // pin, value (string)
                return true;
            }
            else if (numParameter == 6)
            {
                Blynk.virtualWrite(atoi(command[1]), atoi(command[2]), atof(command[3]), atof(command[4]), command[5]);
                return true;
            }
        }
        return false;
    }
    else if (numParameter == 2 && command[0][0] == 'A')  // blynk notify
    {
        if (hasInitBlynk)
        {
            Blynk.notify(command[1]);
            return true;
        }
        return false;
    }
    return false;
}

BLYNK_READ(V0) 
{
    Blynk.virtualWrite(0, virtualPinValue[0]);
    virtualPinChanged &= ~_BV(0);
}

BLYNK_READ(V1) 
{
    Blynk.virtualWrite(1, virtualPinValue[1]);
    virtualPinChanged &= ~_BV(1);
}

BLYNK_READ(V2) 
{
    Blynk.virtualWrite(2, virtualPinValue[2]);
    virtualPinChanged &= ~_BV(2);
}

BLYNK_READ(V3) 
{
    Blynk.virtualWrite(3, virtualPinValue[3]);
    virtualPinChanged &= ~_BV(3);
}

BLYNK_READ(V4) 
{
    Blynk.virtualWrite(4, virtualPinValue[4]);
    virtualPinChanged &= ~_BV(4);
}

BLYNK_READ(V5) 
{
    Blynk.virtualWrite(5, virtualPinValue[5]);
    virtualPinChanged &= ~_BV(5);
}

BLYNK_READ(V6) 
{
    Blynk.virtualWrite(6, virtualPinValue[6]);
    virtualPinChanged &= ~_BV(6);
}

BLYNK_READ(V7) 
{
    Blynk.virtualWrite(7, virtualPinValue[7]);
    virtualPinChanged &= ~_BV(7);
}

BLYNK_WRITE(V0) 
{
    virtualPinValue[0] = param.asInt();
}

BLYNK_WRITE(V1) 
{
    virtualPinValue[1] = param.asInt();
}

BLYNK_WRITE(V2) 
{
    virtualPinValue[2] = param.asInt();
}

BLYNK_WRITE(V3) 
{
    virtualPinValue[3] = param.asInt();
}

BLYNK_WRITE(V4) 
{
    virtualPinValue[4] = param.asInt();
}

BLYNK_WRITE(V5) 
{
    virtualPinValue[5] = param.asInt();
}

BLYNK_WRITE(V6) 
{
    virtualPinValue[6] = param.asInt();
}

BLYNK_WRITE(V7) 
{
    virtualPinValue[7] = param.asInt();
}