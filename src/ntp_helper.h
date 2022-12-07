#ifndef NTP_HELPER_H
#define NTP_HELPER_H

void NTP_Init();
void NTP_Update();
bool NTP_Connected();
bool NTP_ProcessCommand(char* command[], int numParameter, bool hasInitWifi);

#endif