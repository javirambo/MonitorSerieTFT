/*
  RTC.h
  Toma la hora de un servidor NTP
  Para eso necesita conectarse al WiFi

  JJTeam 2021
*/

#pragma once

class Rtc
{
private:
    char buf[22];

public:
    bool isTimeSetted();
    bool getTimeFromNtpServer();
    struct tm getTimeStruct();
    time_t getSystemTime();
    const char *getTime();
    const char *getDate();
    const char *getDateTime();
    const char *getDateTimeOrElse(const char *s);
};
