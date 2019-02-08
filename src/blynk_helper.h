#ifndef BLYNK_HELPER_H
#define BLYNK_HELPER_H

void Blynk_Init();
void Blynk_Update();
bool Blynk_ProcessCommand(char* command[], int numParameter, bool hasInitWifi);

#endif