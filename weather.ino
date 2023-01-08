
#include <M5EPD.h>
#include "EPDWifi.h"
#include "Config.h"
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

void Get_Weather_Data()
{
  if (WiFi.status() == WL_CONNECTED) // Check WiFi connection status
  {
    HTTPClient http; // Declare an object of class HTTPClient

    // specify request destination
    http.begin(OPENWEATHER_URL); // !!

    int httpCode = http.GET(); // send the request

    if (httpCode > 0) // check the returning code
    {
      String res = http.getString(); // Get the request response payload

      DynamicJsonDocument root(1024);

      // Parse JSON object
      deserializeJson(root, res);
      auto error = deserializeJson(root, res);
      if (error)
      {
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(error.c_str());
        return;
      }
      String weather_icon = root["weather"][0]["icon"];
      String weather_main = root["weather"][0]["main"];
      float temp = root["main"]["temp"];
      float temp_min = root["main"]["temp_min"];
      float temp_max = root["main"]["temp_max"];                 // get temperature in °C
      int humidity = root["main"]["humidity"];                   // get humidity in %
      //float pressure = (float)(root["main"]["pressure"]) / 1000; // get pressure in bar
      float wind_speed = root["wind"]["speed"];                  // get wind speed in m/s
      //int wind_degree = root["wind"]["deg"];                     // get wind degree in °

      // print data
      Serial.printf("Temperature = %.2f°C\r\n", temp);
      Serial.printf("Min Temperature = %.2f°C\r\n", temp_min);
      Serial.printf("Max Temperature = %.2f°C\r\n", temp_max);
      Serial.printf("Humidity = %d %%\r\n", humidity);
      //Serial.printf("Pressure = %.3f bar\r\n", pressure);
      Serial.printf("Wind speed = %.1f m/s\r\n", wind_speed);
      Serial.println(weather_main);
      //Serial.printf("Wind degree = %d°\r\n\r\n", wind_degree);
      char strtemp[10] = "";
      dtostrf(temp, 2, 0, strtemp);
      char strtemp_min[10] = "";
      dtostrf(temp_min, 2, 0, strtemp_min);
      char strtemp_max[10] = "";
      dtostrf(temp_max, 2, 0, strtemp_max);
      char strhumidity[10] = "";
      dtostrf(humidity, 2, 0, strhumidity);

      canvas.createRender(120, 256);
      canvas.setTextSize(120);
      canvas.setTextColor(15);
      canvas.setTextDatum(TL_DATUM);
      canvas.drawString(String(strtemp) + String("°C"), 630, 70);
      canvas.createRender(50, 256);
      canvas.setTextSize(50);
      canvas.drawString(strhumidity + String("%"), 695, 185);
      canvas.setTextDatum(TC_DATUM);
      canvas.drawString(String(weather_main), 480, 185);
      String icon_path = "/weather/" + weather_icon + ".png";
      int str_len = icon_path.length() + 1;
      char char_array[str_len];
      icon_path.toCharArray(char_array, str_len);
      //canvas.drawPngFile(SD, char_array, 390, 50, 180, 180, 0, 0, 1.2, 1.2);
      canvas.drawPngFile(SD, char_array, 390, 30, 180, 180, 0, 0, 1.2, 1.2);
      canvas.createRender(30, 256);
      canvas.setTextSize(30);
      canvas.setTextDatum(TL_DATUM);
      canvas.drawString(String("↑") + strtemp_max + String("°"), 880, 60);
      canvas.drawString(String("↓") + strtemp_min + String("°"), 880, 165);
    }

    http.end(); // Close connection
  }
}
