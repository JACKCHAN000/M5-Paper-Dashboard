
#include <M5EPD.h>
#include "EPDWifi.h"
#include "Config.h"
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

void Get_Week_Weather_Data()
{
  if (WiFi.status() == WL_CONNECTED) // Check WiFi connection status
  {
    HTTPClient http; // Declare an object of class HTTPClient
    // specify request destination
    http.begin(OPENWEATHER_URL_WEEK); // !!

    int httpCode = http.GET(); // send the request
    if (httpCode > 0) // check the returning code
    {
      String res = http.getString(); // Get the request response payload
      DynamicJsonDocument root(5120);

      // Parse JSON object
      deserializeJson(root, res);
      auto error = deserializeJson(root, res);
      if (error)
      {
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(error.c_str());
        return;
      }
      Serial.println("--------------");
      // print data
      int x_start = 40;
      int y_start = 270;
      int x_text_start = 90;
      int y_text_start = 390;
      int x_offset = 130;
      for (int i = 0; i < 7; i++) {
        String weather_icon = root["list"][i]["weather"][0]["icon"];
        float temp_min = root["list"][i]["temp"]["min"];
        float temp_max = root["list"][i]["temp"]["max"];
        char strtemp_min[10] = "";
        dtostrf(temp_min, 2, 0, strtemp_min);
        char strtemp_max[10] = "";
        dtostrf(temp_max, 2, 0, strtemp_max);
        String icon_path = "/weather2/" + weather_icon + ".png";
        int str_len = icon_path.length() + 1;
        char char_array[str_len];
        icon_path.toCharArray(char_array, str_len);
        canvas.drawPngFile(SD, char_array, x_start, y_start);
        canvas.createRender(30, 256);
        canvas.setTextSize(30);
        canvas.setTextDatum(TC_DATUM);
        canvas.drawString(strtemp_min + String("°-") + strtemp_max + String("°"), x_text_start, y_text_start);
        x_start += x_offset;
        x_text_start += x_offset;
      }
    }

    http.end(); // Close connection
  }
}
