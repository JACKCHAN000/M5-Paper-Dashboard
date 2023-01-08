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
    @file SHT30.h

    Helper function to read the SHT30 environment data.
*/
#pragma once
#include <WiFi.h>
#include "Config.h"
#include "EPDWifi.h"

#include <HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
/* Read the SHT30 environment chip data */

void PostSHT30Values(String tem, String hum)
{
  String url = sensor_url;

  if (WiFi.status() == WL_CONNECTED) // Check WiFi connection status
  {
    HTTPClient http;
    http.begin(url); // !!
    http.addHeader("X-AIO-Key", IO_KEY);
    http.addHeader("Content-Type", "application/json");
    StaticJsonDocument<200> doc;
    StaticJsonDocument<50> f_1;
    StaticJsonDocument<50> f_2;
    f_1["key"] = "temperature";
    f_1["value"] = tem;
    f_2["key"] = "humidity";
    f_2["value"] = hum;
    JsonArray feeds = doc.createNestedArray("feeds");
    feeds.add(f_1);
    feeds.add(f_2);
    String output;
    serializeJson(doc, output);
    //{"feeds":[{"key": "temperature","value":"String(tem)"},{"key": "humidity","String(hum)"}]}
    int httpCode = http.POST(output); // send the request
    //Serial.printf(httpCode);
    if (httpCode > 0) // check the returning code
    {
      String payload = http.getString();
      Serial.println(payload);
    }
    else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  }
}
