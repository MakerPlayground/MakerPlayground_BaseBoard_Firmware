#define NETPIE_PUBLISH_INTERVAL 500

#include <MicroGear.h>
#include "netpie_helper.h"
#include <map>

MicroGear* microgear;

std::map<String, double> topicValue;
std::map<String, bool> topicChanged;

unsigned long nextSendTime = 0;
bool hasInitNetpie = false;  

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) 
{
    msg[msglen] = '\0';
    
    // skip topic to the second '/' as the topic is in format /appid/topic
    // and we only want /topic
    while ((*(++topic) != '/') && (*topic != '\0'));
    
    topicValue[String(topic)] = atof((char *) msg); 
}


void Netpie_Init(WiFiClient& client)
{
    microgear = new MicroGear(client);
    microgear->on(MESSAGE, onMsghandler);
}

void Netpie_Update()
{
    if (hasInitNetpie && microgear->connected()) 
    {
        microgear->loop();

        if (nextSendTime <= millis())
        {
            for (const auto& c : topicChanged) 
            {
                if (c.second)
                {
                    microgear->publish((char*) (c.first.c_str()), topicValue[c.first]);
                    topicChanged[c.first] = false;
                }
            }
            nextSendTime = millis() + NETPIE_PUBLISH_INTERVAL;
        }

        delay(5);
    }
}

bool Netpie_ProcessCommand(char* command[], int numParameter, bool hasInitWifi) 
{
    if (numParameter == 5 && command[0][0] == 'N') // connect to netpie
    {
        if (hasInitWifi)
        {
            microgear->init(command[1], command[2], command[3]); // key, secret, alias
            microgear->connect(command[4]);  // appid
            hasInitNetpie = microgear->connected();
            return hasInitNetpie;
        }
        return false;
    }
    else if (numParameter == 3 && command[0][0] == 'P') // netpie's publish
    { 
        if (hasInitNetpie)
        {
            String topic(command[1]);
            topicValue[topic] = atof(command[2]);
            topicChanged[topic] = true;
            return true;
        }
        return false;
    }
    else if (numParameter == 2 && command[0][0] == 'S') // netpie's subscribe
    {
        if (hasInitNetpie)
        {
            microgear->subscribe(command[1]);
            return true;
        }
        return false;
    }
    else if (numParameter == 2 && command[0][0] == 'U') // netpie's unsubscribe
    {
        if (hasInitNetpie)
        {
            microgear->unsubscribe(command[1]);
            return true;
        }
        return false;
    }
    else if (numParameter == 2 && command[0][0] == 'G')  // get netpie's topic value
    {
        if (hasInitNetpie)
        {
            Serial.println(topicValue[String(command[1])]);
            return true;
        }
        return false;
    }
    else if (numParameter == 2 && command[0][0] == 'O') // netpie's notification
    {
        if (hasInitNetpie)
        {
            microgear->publish("/@push/owner", command[1]);
            return true;
        }
        return false;
    }
    else if (numParameter == 2 && command[0][0] == 'F') // netpie's feed
    {
        if (hasInitNetpie)
        {
            microgear->writeFeed(command[1], command[2], command[3]);   // feedid, data, apikey
            return true;
        }
        return false;
    }

    return false;
}

bool Netpie_Connected()
{
    return hasInitNetpie;
}