
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
  M5.EPD.Clear(true);
  M5.TP.SetRotation(0);
  M5.EPD.SetRotation(0);
  StartWiFi();
  canvas.loadFont("/GenSenRounded-R.ttf", SD); // Load font files from SD Card


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
  char *wd[] = {"SUN", "MON", "TUE", "WED", "Thu", "FRI", "SAT"};
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
  StopWiFi();
  uint32_t vol = M5.getBatteryVoltage();
  if (vol > 4200){
    Serial.println(String("Charing Mode") + String(vol));
    delay(6000 * 3600);
  }
}

void loop()
{
  next_boot_Time();// turn on every 30 minutes
  canvas.fillCanvas(0);
  M5.EPD.Clear(true);
  setup();

}

void next_boot_Time() {
  M5.RTC.getTime(&RTCtime);
  M5.RTC.getDate(&RTCDate);

  time_t rawtime;
  time(&rawtime);

  // convert the time_t value to a struct tm object
  struct tm *timeinfo = localtime(&rawtime);

  // set the time
  timeinfo->tm_sec = 0;  // set the second to 0
  Serial.println(timeinfo->tm_min);
  int tf = 30 - ((timeinfo->tm_min) % 30);
  timeinfo->tm_min += tf;
  rtc_date_t RTC_DateStruct;
  rtc_time_t RTC_TimeStruct;
  RTC_DateStruct.year = timeinfo->tm_year + 1900;
  RTC_DateStruct.mon = timeinfo->tm_mon + 1;
  RTC_DateStruct.day = timeinfo->tm_mday;
  RTC_DateStruct.week = timeinfo->tm_wday;
  RTC_TimeStruct.hour = timeinfo->tm_hour;
  RTC_TimeStruct.min = timeinfo->tm_min;
  RTC_TimeStruct.sec = timeinfo->tm_sec;
  if (RTC_TimeStruct.min == 60)
  {
    RTC_TimeStruct.min = 0;
    RTC_TimeStruct.hour = RTC_TimeStruct.hour + 1;
  }
  if (RTC_TimeStruct.hour == 24)
  {
    RTC_TimeStruct.hour = 0;
    RTC_DateStruct.day = RTC_DateStruct.day + 1;
    RTC_DateStruct.week = RTC_DateStruct.week + 1;
  }
  if ((RTC_DateStruct.day == 32) or ((RTC_DateStruct.year % 4 == 0) and (RTC_DateStruct.mon == 2) and (RTC_DateStruct.day == 30)) or ((RTC_DateStruct.year % 4 != 0) and (RTC_DateStruct.mon == 2) and (RTC_DateStruct.day == 29)) or (((RTC_DateStruct.mon == 4) or (RTC_DateStruct.mon == 6) or (RTC_DateStruct.mon == 9) or (RTC_DateStruct.mon == 11)) and (RTC_DateStruct.day == 31)))
  {
    RTC_DateStruct.day = 1;
    RTC_DateStruct.mon = RTC_DateStruct.mon + 1;
  }
  if (RTC_DateStruct.mon == 13)
  {
    RTC_DateStruct.mon = 1;
    RTC_DateStruct.year = RTC_DateStruct.year + 1;
  }
  if (RTC_DateStruct.week == 8)
  {
    RTC_DateStruct.week = 1;
  }
  sprintf(timeStrbuff, "%d/%02d/%02d %d %02d:%02d:%02d",
          RTC_DateStruct.year, RTC_DateStruct.mon, RTC_DateStruct.day, RTC_DateStruct.week,
          RTC_TimeStruct.hour, RTC_TimeStruct.min, RTC_TimeStruct.sec);
  Serial.println(String("Next boot time: ") + String(timeStrbuff));
  M5.shutdown(RTC_DateStruct, RTC_TimeStruct); // wait 30 minutes

}
