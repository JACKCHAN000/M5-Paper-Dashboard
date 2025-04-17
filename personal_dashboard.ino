
#include <M5EPD.h>
#include "EPDWifi.h"

M5EPD_Canvas canvas(&M5.EPD);

void setup()
{
  //M5.begin();
  Serial.begin(115200);
  M5.begin(false, true, false, true, true);
  M5.SHT30.Begin();
  M5.BatteryADCBegin();
  M5.TP.SetRotation(0);
  M5.EPD.SetRotation(0);
  M5.EPD.Clear(true);
  StartWiFi();
  
  canvas.loadFont("/GenSenRounded-R.ttf", SD); // Load font files from SD Card
  canvas.fillCanvas(0);
  canvas.createCanvas(960, 540);
  canvas.drawPngFile(SD, "/bg.png");
  canvas.createRender(24, 256);
  canvas.setTextSize(24);
  canvas.setTextColor(0);
  canvas.setTextDatum(TL_DATUM);
  canvas.drawString("Personal Dashboard", 10, 5);

  M5.SHT30.UpdateData();
  float tem = M5.SHT30.GetTemperature();
  float hum = M5.SHT30.GetRelHumidity();
  Serial.printf("Temperature: %2.2f°C  Humidity: %0.2f%%\r\n", tem, hum);
  char temStr[10];
  char humStr[10];
  dtostrf(tem, 2, 1 , temStr);
  dtostrf(hum, 2, 1 , humStr);
  canvas.setTextDatum(TL_DATUM);
  canvas.drawString(String(temStr) + "°C", 755, 5);
  canvas.drawString(String(humStr) + "%", 867, 5);
  //canvas.drawString("Temperature:" + String(temStr) + "°C" + String(humStr), 0, 200);
  M5.RTC.begin();

  rtc_time_t RTCtime;
  rtc_date_t RTCDate;

  char timeStrbuff[64];

  M5.RTC.getTime(&RTCtime);
  M5.RTC.getDate(&RTCDate);
  sprintf(timeStrbuff, "%d/%02d/%02d %d %02d:%02d:%02d",
          RTCDate.year, RTCDate.mon, RTCDate.day, RTCDate.week,
          RTCtime.hour, RTCtime.min, RTCtime.sec);
  Serial.printf(timeStrbuff);
  //canvas.drawString(timeStrbuff, 0, 0);
  canvas.setTextDatum(TC_DATUM);
  char timeStrbuff2[14];
  sprintf(timeStrbuff2, "%02d:%02d",RTCtime.hour, RTCtime.min);
  canvas.drawString("Last Update: " + String(timeStrbuff2), 480, 5);
  char *wd[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
  char *mon[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
  Serial.println(wd[RTCDate.week]);
  canvas.createRender(120, 256);
  canvas.setTextSize(120);
  canvas.setTextColor(15);
  canvas.setTextDatum(TL_DATUM);
  canvas.drawString(String(RTCDate.day) + "" + String(wd[RTCDate.week]), 20, 70);
  canvas.createRender(50, 256);
  canvas.setTextSize(50);
  canvas.drawString(String(mon[RTCDate.mon - 1]) + " " + String(RTCDate.year), 20, 185);
  Get_Weather_Data();
  get_message();
  PostSHT30Values(temStr, humStr);
  Get_Week_Weather_Data();
  canvas.pushCanvas(0, 0, UPDATE_MODE_GC16);
  // StopWiFi();
  uint32_t vol = M5.getBatteryVoltage();
  Serial.println(String("Vol") + String(vol));
  delay(1000);
  // M5.shutdown(30*60);
  next_boot_time(30);
}

void loop() {
  }

void next_boot_time(int sync_minutes) {
  // sync every 5/15/30 minutes
  time_t rawtime;
  time(&rawtime);

  // calculate next boot time
  int sync_seconds = sync_minutes * 60;
  time_t next_time = ((rawtime + sync_seconds) / sync_seconds) * sync_seconds;

  int seconds_to_next = next_time - rawtime;

  if (seconds_to_next < 5) {
    seconds_to_next = 5;
  }
  
  // wakeup time
  struct tm *wakeup_info = localtime(&next_time);
  char buf[64];
  sprintf(buf, "%04d/%02d/%02d %02d:%02d:%02d",
          wakeup_info->tm_year + 1900,
          wakeup_info->tm_mon + 1,
          wakeup_info->tm_mday,
          wakeup_info->tm_hour,
          wakeup_info->tm_min,
          wakeup_info->tm_sec);

  Serial.printf("Next boot in %d seconds\n", seconds_to_next);
  Serial.println(String("Wakeup Time: ") + String(buf));

  M5.shutdown(seconds_to_next);
}
