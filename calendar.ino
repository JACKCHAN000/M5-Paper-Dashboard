#include <WiFi.h>
#include "Config.h"
#include "EPDWifi.h"

#include <HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

void get_message()
{
  String url = cal_url;
  if (WiFi.status() == WL_CONNECTED) // Check WiFi connection status
  {
    HTTPClient http;
    http.begin(url); // !!
    http.addHeader("X-AIO-Key", IO_KEY);
    int httpCode = http.GET(); // send the request
    //Serial.printf(httpCode);
    if (httpCode > 0) // check the returning code
    {
      String res = http.getString(); // Get the request response payload
      Serial.println(res);
      DynamicJsonDocument root(10240);
      // Parse JSON object
      deserializeJson(root, res);
      auto error = deserializeJson(root, res);
      if (error)
      {
        Serial.println(F("deserializeJson() failed with code "));
        Serial.println(error.c_str());
        return;
      }
      String event = root[0]["value"];
      String event_time = root[0]["created_at"];
      canvas.createRender(50, 256);
      canvas.setTextSize(50);
      canvas.setTextColor(15);
      canvas.setTextDatum(TL_DATUM);
      M5.RTC.getTime(&RTCtime);
      M5.RTC.getDate(&RTCDate);
      int d1 = RTCDate.day;
      int t1 = (RTCtime.hour * 60) + RTCtime.min;
      String sub = event_time.substring(11, 19);
      String sub2 = event_time.substring(8, 10);
      int d2 = sub2.toInt();
      String substrings[50];  // create an array to store the substrings
      splitString(sub, ':', substrings);
      int t2 = ((substrings[0].toInt() + TIMEZONE) % 24) * 60 + substrings[1].toInt();
      //Serial.println(event_time);
      //Serial.println(t1 - t2);
      //Serial.println(d1 - d2);
      if ((abs(t1 - t2) > 240) or (abs(d1 - d2) > 0))
      {
        Serial.println("Have a nice day!");
        canvas.drawString("Have a nice day!", 100, 475);
      } else {
        Serial.println(event);
        canvas.drawString(event, 100, 475);
      }
    }
    http.end();
  }

}

void splitString(String str, char delimiter, String substrings[]) {
  int count = 0;
  int startIndex = 0;
  int endIndex = 0;

  // iterate through the string until we reach the end
  while (endIndex < str.length()) {
    // find the next occurrence of the delimiter
    endIndex = str.indexOf(delimiter, startIndex);

    // if the delimiter was not found, set the end index to the last character in the string
    if (endIndex == -1) {
      endIndex = str.length();
    }

    // store the substring between the start index and the end index
    substrings[count] = str.substring(startIndex, endIndex);

    // update the start index to the character after the delimiter
    startIndex = endIndex + 1;

    // increment the count
    count++;

    // exit the loop if we have reached the maximum number of substrings
    if (count >= 50) {
      break;
    }
  }
}
