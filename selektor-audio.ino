#include <Wire.h>
#include <RTClib.h>
#include <time.h>


const int change_time = 1000;

RTC_DS3231 rtc;
char daysOfTheWeek[7][5] = {"NIE", "PON", "WTO", "ŚRO", "CZW", "PIĄ", "SOB"};
char daysOfTheWeekAll[7][15] = {"Niedziela", "Poniedzialek", "Wtorek", "Sroda", "Czwartek", "Piatek", "Sobota"};
int pl_week[7] = {1, 2, 3, 4, 5, 6, 0};
DateTime now;

void setup()
{

    Serial.begin(115200);

    if (!rtc.begin())
    {
        Serial.println("Could not find RTC! Check circuit.");
        while (1)
            ;
    }

    if (rtc.lostPower())
    {
        Serial.println("RTC power failure, resetting the time!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    now = rtc.now();
    Serial.println(now.timestamp(DateTime::TIMESTAMP_TIME));
    Serial.println(now.timestamp(DateTime::TIMESTAMP_DATE));
}

void loop()
{

  
  static long last_loop_time = 0;
  long loop_time = millis();
  if (loop_time - last_loop_time > change_time)
  {
    now = rtc.now();

     last_loop_time = millis();
  }


 
}