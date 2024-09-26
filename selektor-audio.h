// kody pilota
// mały czarny

//definicje pinów
#define PIN1 34
#define PIN2 35
#define PIN3 32
#define PIN4 33
#define PIN5 25
#define PIN6 26
#define PIN7 27
#define PIN8 14


//#define pilot_maly_czarny


#if defined pilot_maly_czarny
#define R_BT1 0xC  // button 1
#define R_BT2 0x18 // button 2
#define R_BT3 0x5E // button 3
#define R_BT4 0x8  // button 4
#define R_BT5 0x1C // button 5
#define R_BT6 0x5A // button 6
#define R_BT7 0x42 // button 7
#define R_BT8 0x52 // button 8
#define R_SET 0x9  // button EQ
#define R_BRG 0x47  // button CH+
#define R_BTPLUS 0x15  // button +
#define R_BTMINUS 0x9  // button VOL- do korekty
#define R_SAVE 0x18  // button + do korekty
#else
#define R_BT1 0xC  // button 1
#define R_BT2 0xD // button 2
#define R_BT3 0xE // button 3
#define R_BT4 0x10  // button 4
#define R_BT5 0x11 // button 5
#define R_BT6 0x12 // button 6
#define R_BT7 0x14 // button 7
#define R_BT8 0x15 // button 8
#define R_SET 0x1F  // button MENU
#define R_BRG 0x0  // button MODE
#define R_BTPLUS 0xB  // button VOL+
#define R_BTMINUS 0x9  // button VOL-
#define R_SAVE 0x2  // button +
#endif

// nazwy źródeł
#define LABEL1 "PHO "
#define LABEL2 "CD1 "
#define LABEL3 "CD2 "
#define LABEL4 "TAP "
#define LABEL5 "STR "
#define LABEL6 "BT  "
#define LABEL7 "AU1 "
#define LABEL8 "AU2 "


