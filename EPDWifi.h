/*
   Copyright (C) 2021 SFini

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * @file EPDWifi.h
 *
 * Helper functions for the wifi connection.
 */
#pragma once
#include <WiFi.h>
#include "Config.h"

rtc_time_t RTCtime;
rtc_date_t RTCDate;

char timeStrbuff[64];


RTC_SLOW_ATTR bool ntpDataFlag = false;
time_t t;
struct tm *tm;

void setupTime() {
  t = time(NULL);
  tm = localtime(&t);

  RTCtime.hour = tm->tm_hour;
  RTCtime.min = tm->tm_min;
  RTCtime.sec = tm->tm_sec;
  M5.RTC.setTime(&RTCtime);

  RTCDate.year = tm->tm_year + 1900;
  RTCDate.mon = tm->tm_mon + 1;
  RTCDate.day = tm->tm_mday;
  RTCDate.week = tm->tm_wday;
  M5.RTC.setDate(&RTCDate);
}

/* Start and connect to the wifi */
void StartWiFi()
{
   IPAddress dns(8, 8, 8, 8); // Google DNS
   WiFi.mode(WIFI_STA);
   WiFi.disconnect();
   WiFi.setAutoConnect(true);
   WiFi.setAutoReconnect(true);

   Serial.print("Connecting to ");
   Serial.println(WIFI_SSID);
   delay(100);

   WiFi.begin(WIFI_SSID, WIFI_PW);

   for (int retry = 0; WiFi.status() != WL_CONNECTED && retry < 30; retry++)
   {
      delay(500);
      Serial.print(".");
   }

   if (WiFi.status() == WL_CONNECTED)
   {
      Serial.println("WiFi connected at: " + WiFi.localIP().toString());
      configTime(3600*TIMEZONE, 0, "time.google.com");
      delay(6000);
      setupTime();
      sprintf(timeStrbuff, "%d/%02d/%02d %d %02d:%02d:%02d",RTCDate.year, RTCDate.mon, RTCDate.day, RTCDate.week,
      RTCtime.hour, RTCtime.min, RTCtime.sec);
      Serial.printf(timeStrbuff);
   }
   else
   {
      Serial.println("WiFi connection *** FAILED ***");
   }
}

/* Stop the wifi connection */
void StopWiFi()
{
   Serial.println("Stop WiFi");
   WiFi.disconnect();
   WiFi.mode(WIFI_OFF);
}
