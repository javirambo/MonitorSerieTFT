/*
  RTC.cpp
  Toma la hora de un servidor NTP
  Para eso necesita conectarse al WiFi

  JJTeam 2021
*/

#include <Arduino.h>
#include "lwip/apps/sntp.h"
#include "time.h"
#include "RTC.h"
#include "../logs/FSLog.h"

bool Rtc::isTimeSetted()
{
    // default (1609459200) = 1st Jan 2021
    return time(NULL) > 1609459200;
}

bool Rtc::getTimeFromNtpServer()
{
    // connect to NTP server
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, (char *)"pool.ntp.org");
    sntp_setservername(1, (char *)"time.nist.gov");
    sntp_setservername(2, (char *)"time1.google.com");
    sntp_init();

    // wait to get the time
    long now = millis();
    struct tm tm;
    time_t secs;
    time(&secs);
    localtime_r(&secs, &tm);
    while (tm.tm_year < (2019 - 1900))
    {
        LogTrace("Time not set, trying...");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&secs);
        localtime_r(&secs, &tm);

        if (millis() - now > 15000)
        {
            LogTrace("Timeout getting time from ntp servers, abort!");
            sntp_stop();
            return false;
        }
    }
    sntp_stop();
    // set the internal RTC time
    setenv("TZ", "GMT+3", 1); // Argentina
    tzset();
    timeval tv = {secs, 0};
    settimeofday(&tv, NULL);
    return true;
}

// get the internal RTC time as a tm struct
struct tm Rtc::getTimeStruct()
{
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    return timeinfo;
}

time_t Rtc::getSystemTime()
{
    return time(NULL);
}

const char *Rtc::getTime()
{
    struct tm timeinfo = getTimeStruct();
    strftime(buf, sizeof(buf), "%H:%M:%S", &timeinfo);
    return buf;
}

const char *Rtc::getDate()
{
    struct tm timeinfo = getTimeStruct();
    strftime(buf, sizeof(buf), "%d/%m/%Y", &timeinfo);
    return buf;
}

const char *Rtc::getDateTime()
{
    struct tm timeinfo = getTimeStruct();
    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", &timeinfo);
    return buf;
}

const char *Rtc::getDateTimeOrElse(const char *s)
{
    return isTimeSetted() ? getDateTime() : s;
}
