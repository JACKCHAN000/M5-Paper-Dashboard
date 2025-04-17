#include <WiFi.h>
#include "Config.h"
#include "EPDWifi.h"
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <TimeLib.h>

int monthStringToInt(String month) {
  if (month == "Jan") return 1;
  if (month == "Feb") return 2;
  if (month == "Mar") return 3;
  if (month == "Apr") return 4;
  if (month == "May") return 5;
  if (month == "Jun") return 6;
  if (month == "Jul") return 7;
  if (month == "Aug") return 8;
  if (month == "Sep") return 9;
  if (month == "Oct") return 10;
  if (month == "Nov") return 11;
  if (month == "Dec") return 12;
  return 0;
}

void get_message()
{
  String url = cal_url;
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    http.begin(url);
    http.addHeader("X-AIO-Key", IO_KEY);
    int httpCode = http.GET();
    if (httpCode > 0)
    {
      String res = http.getString();
      Serial.println(res);
      DynamicJsonDocument root(10240);
      auto error = deserializeJson(root, res);
      if (error)
      {
        Serial.println(F("deserializeJson() failed with code "));
        Serial.println(error.c_str());
        return;
      }

      String value = root[0]["value"]; // "Apr 12, 2025 04:32PM-This is a test event."
      int dashIndex = value.indexOf('-');
      if (dashIndex == -1) return;

      String timePart = value.substring(0, dashIndex); // "Apr 12, 2025 04:32PM"
      String event = value.substring(dashIndex + 1);   // event content

      // decode date and time
      int month = monthStringToInt(timePart.substring(0, 3));
      int day = timePart.substring(4, 6).toInt();
      int year = timePart.substring(8, 12).toInt();
      String timeString = timePart.substring(13); // "04:32PM"

      int colonIndex = timeString.indexOf(':');
      int hour = timeString.substring(0, colonIndex).toInt();
      int minute = timeString.substring(colonIndex + 1, colonIndex + 3).toInt();
      String meridiem = timeString.substring(colonIndex + 3);
      if (meridiem == "PM" && hour != 12) hour += 12;
      if (meridiem == "AM" && hour == 12) hour = 0;

      // get current time
      M5.RTC.getTime(&RTCtime);
      M5.RTC.getDate(&RTCDate);

      if (RTCDate.day != day || RTCDate.mon != month || (RTCDate.year + 2000) != year) {
        canvas.createRender(50, 256);
        canvas.setTextSize(50);
        canvas.setTextColor(15);
        canvas.setTextDatum(TL_DATUM);
        canvas.drawString("Have a nice day!", 100, 475);
        return;
      }

      int eventMinutes = hour * 60 + minute;
      int nowMinutes = RTCtime.hour * 60 + RTCtime.min;
      int diff = eventMinutes - nowMinutes;

      canvas.createRender(50, 256);
      canvas.setTextSize(50);
      canvas.setTextColor(15);
      canvas.setTextDatum(TL_DATUM);

      if (diff >= -180 && diff <= 360) {
        canvas.drawString(event, 100, 475);
        Serial.println(event);
      } else {
        canvas.drawString("Have a nice day!", 100, 475);
        Serial.println("Have a nice day!");
      }
    }
    http.end();
  }
}
