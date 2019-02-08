/*  NETPIE ESP8266 basic sample                            */
/*  More information visit : https://netpie.io             */

#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include <map>

// const char* ssid     = <WIFI_SSID>;
// const char* password = <WIFI_KEY>;

// #define APPID   <APPID>
// #define KEY     <APPKEY>
// #define SECRET  <APPSECRET>
// #define ALIAS   "esp8266"

WiFiClient client;

int timer = 0;
MicroGear microgear(client);

char serialBuffer[128];
std::map<String, double> topicValue;

// char* appid;
bool hasInitWifi = false;
bool hasInitNetpie = false;  

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) 
{
    msg[msglen] = '\0';
    
    // skip topic to the second '/' as the topic is in format /appid/topic
    // and we only want /topic
    while ((*(++topic) != '/') && (*topic != '\0'));
    
    topicValue[String(topic)] = atof((char *) msg); 
}

// void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) 
// {
// }

// void initWifi(const char* ssid, const char* pass) 
// {
//     if (WiFi.begin(ssid, pass)) 
//     {
//         while (WiFi.status() != WL_CONNECTED) 
//         {
//             delay(500);
//             Serial.print(".");
//         }
//     }
//     // Serial.println(WiFi.localIP());   
// }

// void initNetpie(char* key, char* secret, char* alias, char* appid) 
// {
//     microgear.init(key, secret, alias);
//     microgear.connect(appid);
// }

// void printLog(const char* message)
// {
//     Serial.println(message);
// }

void processCommand() 
{
    // if (strlen(serialBuffer) < 2)
    // {
    //     return;
    // }

    char* command[6];
    int numParameter = 0;
    command[numParameter] = strtok(serialBuffer, ",\r"); 
    while (command[numParameter] != NULL && numParameter <= 4)
        command[++numParameter] = strtok(NULL, ",\r"); 
    
    Serial.println(numParameter);
    for (int i=0; i<numParameter; i++)
    {
        Serial.println(command[i]);
    }

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
    else if (numParameter == 5 && command[0][0] == 'N') // connect to netpie
    {
        if (hasInitWifi)
        {
            microgear.init(command[1], command[2], command[3]); // key, secret, alias
            // appid = strdup(command[4]);
            microgear.connect(command[4]);  // appid
            hasInitNetpie = true;
            Serial.println("OK");
        }
        else
        {
            Serial.println("Error");
        }
        
    }
    else if (numParameter == 3 && command[0][0] == 'P') // netpie's publish
    { 
        if (hasInitNetpie)
        {
            microgear.publish(command[1], command[2]); 
            Serial.println("OK");
        }
        else
        {
            Serial.println("Error");
        }
    }
    else if (numParameter == 2 && command[0][0] == 'S') // netpie's subscribe
    {
        if (hasInitNetpie)
        {
            microgear.subscribe(command[1]);
            Serial.println("OK");
        }
        else
        {
            Serial.println("Error");
        }
    }
    else if (numParameter == 2 && command[0][0] == 'G')  // get netpie's topic value
    {
        if (hasInitNetpie)
        {
            Serial.println(topicValue[String(command[1])]);
        }
        else
        {
            Serial.println("Error");
        }
    } 
    // else 
    // {
    //     Serial.println("Error");
    // }

    // clear buffer for the next command
    serialBuffer[0] = '\0';
}

void setup() 
{
    Serial.begin(115200);
    Serial.setTimeout(5);

    microgear.on(MESSAGE, onMsghandler);
    // microgear.on(PRESENT,onFoundgear);
    // microgear.on(ABSENT,onLostgear);
    // microgear.on(CONNECTED,onConnected);
}

void loop() 
{
    int numByteRead = Serial.readBytesUntil('\n', serialBuffer, 128);
    serialBuffer[numByteRead] = '\0';
    Serial.print("Get command : ");
    Serial.println(serialBuffer);
    processCommand();

    if (microgear.connected()) 
    {
        microgear.loop();
    }

    delay(2000);
    // else if (hasInitNetpie)
    // {
    //     microgear.connect(appid);
    // }
}

// while (Serial.available() > 0) 
//     {
//         serialBuffer[numChar] = Serial.read();
//         if (serialBuffer[numChar] == '\n') 
//         {
//             processCommand();
//             break;
//         } 
//         else 
//         {
//             numChar++;
//         }
//     }