#include <Wire.h>
#include <RTClib.h>
#include <time.h>
#include <TM1638plus.h> // include the library

#define STROBE_TM 4
#define CLOCK_TM 16
#define DIO_TM 17

const long intervalButton = 200;   // interval to read button (milliseconds)
const long intervalDisplay = 1000; // interval at which to change display (milliseconds)
bool high_freq = true;             // default false, If using a high freq CPU > ~100 MHZ set to true.

const int change_time = 1000;

RTC_DS3231 rtc;
char daysOfTheWeek[7][5] = {"NIE", "PON", "WTO", "ŚRO", "CZW", "PIĄ", "SOB"};
char daysOfTheWeekAll[7][15] = {"Niedziela", "Poniedzialek", "Wtorek", "Sroda", "Czwartek", "Piatek", "Sobota"};
int pl_week[7] = {1, 2, 3, 4, 5, 6, 0};
DateTime now;

// Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO, use high freq MCU)
TM1638plus tm(STROBE_TM, CLOCK_TM, DIO_TM, high_freq);

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

    tm.displayBegin();
    tm.displayText("00000000");
    Serial.begin(38400);
    delay(100);
    Serial.println("selektor-audio");
    delay(5000);
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
    uint8_t buttonsValue = buttonsRead();
    if (buttonsValue != 0)
    {
        updateDisplay(buttonsValue);
    }
}

// Read and debounce the buttons from TM1638  every interval
uint8_t buttonsRead(void)
{
    uint8_t buttons = 0;
    static unsigned long previousMillisButton = 0; // executed once
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillisButton >= intervalButton)
    {
        previousMillisButton = currentMillis;
        buttons = tm.readButtons();
    }
    return buttons;
}

// Change display every interval
void updateDisplay(uint8_t buttonsValue)
{
    unsigned long currentMillis = millis();
    static unsigned long previousMillisDisplay = 0; // executed once
    if (currentMillis - previousMillisDisplay >= intervalDisplay)
    {
        previousMillisDisplay = currentMillis;
        tm.displayIntNum(buttonsValue, true, TMAlignTextLeft);
        Serial.println(buttonsValue, HEX);
    }
}