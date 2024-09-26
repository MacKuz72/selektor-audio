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

// Constructor object (GPIO STB , GPuint8_t MainMode = 2;IO CLOCK , GPIO DIO, use high freq MCU)
TM1638plus tm(STROBE_TM, CLOCK_TM, DIO_TM, high_freq);

uint8_t godzina;
uint8_t minuta;
char workstr[11];
char opisAudio[4] = {'_', '_', '_', '_'};
uint8_t MainMode = 2;
uint8_t SelectAudio = 0;
unsigned long previousMillis = 0;
const long interval = 225; // interval at which to debounce button readback function(milliseconds)

void Brightness(void);
uint8_t buttonsRead(void);
void SetTime(void);
void SetDate(uint8_t Hours, uint8_t Minutes);

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
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    now = rtc.now();
    godzina = now.hour();
    minuta = now.minute();

    Serial.println(now.timestamp(DateTime::TIMESTAMP_TIME));
    Serial.println(now.timestamp(DateTime::TIMESTAMP_DATE));

    tm.displayBegin();
    tm.displayText("00000000");
    delay(100);
    if (godzina == 24)
        godzina = 0;
    if (minuta == 59)
        minuta = 0;
    sprintf(workstr, "%s%02d.%02d", opisAudio, godzina, minuta);
    tm.displayText(workstr);

    Serial.println("selektor-audio");
    Serial.println(workstr);
    delay(5000);
}

void loop()
{

    static long last_loop_time = 0;
    long loop_time = millis();
    if (loop_time - last_loop_time > change_time)
    {
        now = rtc.now();
        godzina = now.hour();
        minuta = now.minute();

        if (godzina == 24)
            godzina = 0;
        if (minuta == 59)
            minuta = 0;
        sprintf(workstr, "%s%02d.%02d", opisAudio, godzina, minuta);
        tm.displayText(workstr);

        last_loop_time = millis();
    }
    uint8_t buttonsValue = buttonsRead();
    if (buttonsValue != 0)
    {
        updateDisplay(buttonsValue);

        switch (buttonsValue)
        {
        case 1:                 // SW1
            tm.setLEDs(0x0000); // all leds off
            tm.setLED(0, 1);
            SelectAudio = 1;
            sprintf(opisAudio, "PHO ");
            break;
        case 2: // SW2
            tm.setLEDs(0x0000);
            tm.setLED(1, 1);
            SelectAudio = 2;
            sprintf(opisAudio, "CD1 ");
            break;
        case 4: // SW3
            tm.setLEDs(0x0000);
            tm.setLED(2, 1);
            SelectAudio = 3;
            sprintf(opisAudio, "CD2 ");
            break;
        case 8: // SW4
            tm.setLEDs(0x0000);
            tm.setLED(3, 1);
            SelectAudio = 4;
            sprintf(opisAudio, "TAP ");
            break;
        case 16: // SW5
            tm.setLEDs(0x0000);
            tm.setLED(4, 1);
            SelectAudio = 5;
            sprintf(opisAudio, "RPI ");
            break;
        case 32: // SW6
            tm.setLEDs(0x0000);
            tm.setLED(5, 1);
            SelectAudio = 6;
            sprintf(opisAudio, "BT  ");
            break;
        case 64: // SW7
            tm.setLEDs(0x0000);
            tm.setLED(6, 1);
            SelectAudio = 7;
            sprintf(opisAudio, "AU1 ");
            break;
        case 128: // SW8
            tm.setLEDs(0x0000);
            tm.setLED(7, 1);
            SelectAudio = 8;
            sprintf(opisAudio, "AU2 ");
            break;
        case 129:
            MainMode = 8;
            SettingsMenuDisplay();
            break;

        default:
            break;
        }
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

// Function to display the settings menu Called from Main
void SettingsMenuDisplay(void)
{
    tm.displayText("SEttInGs");
    while (1)
    {
        uint8_t buttons = buttonsRead();
        switch (buttons)
        {
        case 0x02:
            MainMode = 2;
            break;
        case 0x04:
            SetTime();
            break;
        // case 0x08 : SetAlarm(); break;
        case 0x10:
            Brightness();
            break;
        }
        Serial.println(buttons);
        if (MainMode == 2)
            break;
    }
    Serial.println("DUPA");
    Serial.println(MainMode);
}

void Brightness(void)
{

    uint8_t bright = 0x00;
    tm.displayText("        ");
    while (1)
    {
        uint8_t buttons = buttonsRead();
        switch (buttons)
        {
        case 0x02:
            MainMode = 2;
            break;
        case 0x80:
            bright++;
            break;
        }
        if (bright == 0x08)
        {
            bright = 0x00;
        }
        buttons = 0;
        tm.brightness(bright);
        tm.displayText("Bright");
        tm.displayHex(7, bright);
        if (MainMode == 2)
            break;
    }
}

void SetTime(void)
{
    char workstr[11];
    uint8_t Hours = 0;
    uint8_t Minutes = 0;
    while (1)
    {
        uint8_t buttons = buttonsRead();
        switch (buttons)
        {
        case 0x02:
            MainMode = 2;
            break;
        case 0x04:
            SetDate(Hours, Minutes);
            MainMode = 2;
            break;
        case 0x40:
            Hours++;
            break;
        case 0x80:
            Minutes++;
            break;
        }
        if (Hours == 24)
            Hours = 0;
        if (Minutes == 59)
            Minutes = 0;
        sprintf(workstr, "tS.  %02d.%02d", Hours, Minutes);
        tm.displayText(workstr);
        buttons = 0;
        if (MainMode == 2)
            break;
    }
}

// Function to Set Date called from setTime
// passed two integers with the time to set in hours and minutes
void SetDate(uint8_t Hours, uint8_t Minutes)
{
    char workstr[13];
    uint8_t Days = 0;
    uint8_t Months = 0;
    uint8_t Years = 0;
    MainMode = 0;

    while (1)
    {
        uint8_t buttons = buttonsRead();

        switch (buttons)
        {
        case 0x02:
            MainMode = 2;
            break;
        case 0x04:
            MainMode = 3;
            break;
        case 0x20:
            Days++;
            break;
        case 0x40:
            Months++;
            break;
        case 0x80:
            Years++;
            break;
        }

        if (Days == 32)
            Days = 1;
        if (Months == 13)
            Months = 1;
        if (Years == 100)
            Years = 0;

        sprintf(workstr, "DS.%02d.%02d.%02d", Days, Months, Years);
        tm.displayText(workstr);
        buttons = 0;
        if (MainMode == 2)
            break;
        if (MainMode == 3)
        {
            //   time_t t;
            //   tmElements_t tmx;
            //   tmx.Year = Years;
            //   tmx.Month = Months;
            //   tmx.Day = Days;
            //   tmx.Hour = Hours;
            //   tmx.Minute = Minutes;
            //   tmx.Second = 0;
            //   t = makeTime(tmx);
            //   RTC.set(t);        // use the time_t value to ensure correct weekday is set
            //   setTime(t);
            tm.displayText("tIn SEt ");
            delay(intervalDisplay);
            break;
        }
    }
}
