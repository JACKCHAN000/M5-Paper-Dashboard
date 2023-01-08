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
 * @file Config.h
 *
 * configuration information
 */
#pragma once

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
