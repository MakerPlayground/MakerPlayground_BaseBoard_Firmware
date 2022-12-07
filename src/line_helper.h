#ifndef LINE_HELPER_H
#define LINE_HELPER_H

void LINE_Init();
void LINE_Update();
bool LINE_Connected();
bool LINE_ProcessCommand(char* command[], int numParameter, bool hasInitWifi);

#endif