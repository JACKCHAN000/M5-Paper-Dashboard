# M5-Paper-Dashboard
Arduino M5 Paper Dashboard

Feature:
Weather Station
Calendar
Environment Monitor
![IMG_3056](https://user-images.githubusercontent.com/61930699/211181751-0a6f5524-7451-4a99-86e6-3efdb1db319d.jpg)


Noted：

The weather display requires openweatherAPI

The environment sensor monitor requires Adafruit IO API

The calendar using the IFTTT to send feed data into Adafruit IO

Usage:
1. Please config all the setting on Config.h

```
// WIFI_SETTING
#define WIFI_SSID "xxxxxxxxxxxxxx"
#define WIFI_PW "xxxxxxxxxxxx"
#define TIMEZONE 8

//OpenWeather API
#define city "1675151"
#define openWeatherMapApiKey "xxxxxxxxxx"

#define OPENWEATHER_URL "http://api.openweathermap.org/data/2.5/weather?id=" + String(city) + "&lang=en_US&units=metric&APPID=" + String(openWeatherMapApiKey)
#define OPENWEATHER_URL_WEEK "http://api.openweathermap.org/data/2.5/forecast/daily?id=" + String(city) + "&lang=en_US&units=metric&cnt=7&APPID=" + String(openWeatherMapApiKey)

//Adafruit IO
#define IO_USERNAME  "xxxxxxxx"
#define IO_KEY       "xxxxxxxxxxxxxxxx"
#define sensor_url   "https://io.adafruit.com/api/v2/xxxxxxxxxxxxxx/groups/sensor/data"
#define cal_url   "https://io.adafruit.com/api/v2/xxxxxxxxxxxxxx/feeds/message/data"
```
2. Copy SD folder items to your SD card
3. Complie on the arduino IDE

