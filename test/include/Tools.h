/*
    Tools.h
    JJTeam - 2021
*/

#ifndef _tools_h
#define _tools_h

const String getSerialNumber();
const char *getResetReason();
boolean isIp(String str);
String toStringIp(IPAddress ip);
void PrintWiFiStatus(uint8_t st);

#endif // _tools_h