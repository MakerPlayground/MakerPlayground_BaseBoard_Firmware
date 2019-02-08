#ifndef NETPIE_HELPER_H
#define NETPIE_HELPER_H

#include <ESP8266WiFi.h>

void Netpie_Init(WiFiClient& client);
void Netpie_Update();
bool Netpie_ProcessCommand(char* command[], int numParameter, bool hasInitWifi);

#endif