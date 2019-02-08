#include <ESP8266WiFi.h>
#include "blynk_helper.h"
#include "netpie_helper.h"

WiFiClient client;
bool hasInitWifi = false;
char serialBuffer[128];

void processCommand() 
{
    if (strlen(serialBuffer) == 0)
        return;

    char* command[7];
    int numParameter = 0;
    command[numParameter] = strtok(serialBuffer, ",\r"); 
    while (command[numParameter] != NULL && numParameter <= 5)
        command[++numParameter] = strtok(NULL, ",\r"); 
    
    // Serial.println(numParameter);
    // for (int i=0; i<numParameter; i++)
    // {
    //     Serial.println(command[i]);
    // }

    if (numParameter == 3 && command[0][0] == 'W')  // connect to wifi
    {
        WiFi.begin(command[1], command[2]);
        while (WiFi.status() != WL_CONNECTED) 
        {
            delay(500);
        }
        hasInitWifi = true;
        Serial.println("OK");
    }
    else if (Netpie_ProcessCommand(command, numParameter, hasInitWifi) 
        || Blynk_ProcessCommand(command, numParameter, hasInitWifi))
    {
        Serial.println("OK");
    }
    else
    {
        Serial.println("Error");
    }

    // clear buffer for the next command
    serialBuffer[0] = '\0';
}

void setup() 
{
    Serial.begin(115200);
    Serial.setTimeout(5);

    Netpie_Init(client);
    Blynk_Init();
}

void loop() 
{
    int numByteRead = Serial.readBytesUntil('\n', serialBuffer, 128);
    serialBuffer[numByteRead] = '\0';
    // Serial.print("Get command : ");
    // Serial.println(serialBuffer);
    processCommand();

    Netpie_Update();
    Blynk_Update();

    // delay(2000);
}