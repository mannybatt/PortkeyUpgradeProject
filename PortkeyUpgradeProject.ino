
// DS3231 RTC
#include <RTClib.h>
#include <TimeLib.h>
RTC_DS3231 rtc;
char t[32];
byte last_second, second_, minute_, hour_, day_, month_;
int year_;

// Oled 32x128 Screen
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 32     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
static const unsigned char PROGMEM logo_bmp[] = { 0b00000000, 0b11000000,
                                                  0b00000001, 0b11000000,
                                                  0b00000001, 0b11000000,
                                                  0b00000011, 0b11100000,
                                                  0b11110011, 0b11100000,
                                                  0b11111110, 0b11111000,
                                                  0b01111110, 0b11111111,
                                                  0b00110011, 0b10011111,
                                                  0b00011111, 0b11111100,
                                                  0b00001101, 0b01110000,
                                                  0b00011011, 0b10100000,
                                                  0b00111111, 0b11100000,
                                                  0b00111111, 0b11110000,
                                                  0b01111100, 0b11110000,
                                                  0b01110000, 0b01110000,
                                                  0b00000000, 0b00110000
                                                };

//UDP for Wled Control
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
const char *ssid = "portkey";
const char *password = "baubillius";
unsigned int localPort = 2390;
WiFiUDP Udp;
char ip[] = "192.168.4.2";

//Variables
unsigned long previousTime = 0;
unsigned long currentTime;

//Messages to Scroll

//Need to go through every year in case some holidays happen out of order

int x, minX;
char mBooting[] =  "Booting!";
char mCurrent[] =  "RGB Pattern: ";
char mNext[] =  "Next RGB Pattern: ";
char mSuffix[] = "            .";

/*

  //Messages in chrono order

  char m1[] = "Manny's Choice! (Aug 1 - Aug 14)";
  char m1[] = "Road to Halloween Pt.1 (Aug 15 - Aug 31)"
  char m2[] = "First day of School at Hogwarts (Sep 1)"
  char m1[] = "Road to Halloween Pt.1 (Sep 2 - Sep 7)"
  char m2[] = "Road to Halloween Pt.2 (Sep 8 - Sep 30)"
  char m3[] = "Road to Halloween Pt.3 (Oct 1 - Oct 30)"
  char m4[] = "Halloween (Oct 31)"



  char m5[] = "Road to Thanksgiving (Nov 1 - Nov 27)" //2024
  char m6[] = "Thanksgiving (Nov 28)"

  char m5[] = "Road to Thanksgiving (Nov 1 - Nov 26)" //2025
  char m6[] = "Thanksgiving (Nov 27)"

  char m5[] = "Road to Thanksgiving (Nov 1 - Nov 25)" //2026
  char m6[] = "Thanksgiving (Nov 26)"

  char m5[] = "Road to Thanksgiving (Nov 1 - Nov 24)" //2027
  char m6[] = "Thanksgiving (Nov 25)"

  char m5[] = "Road to Thanksgiving (Nov 1 - Nov 22)" //2028
  char m6[] = "Thanksgiving (Nov 23)"

  char m5[] = "Road to Thanksgiving (Nov 1 - Nov 21)" //2029
  char m6[] = "Thanksgiving (Nov 22)"

  char m5[] = "Road to Thanksgiving (Nov 1 - Nov 27)" //2030
  char m6[] = "Thanksgiving (Nov 28)"

  char m5[] = "Road to Thanksgiving (Nov 1 - Nov 26)" //2031
  char m6[] = "Thanksgiving (Nov 27)"

  char m5[] = "Road to Thanksgiving (Nov 1 - Nov 24)" //2032
  char m6[] = "Thanksgiving (Nov 25)"

  char m5[] = "Road to Thanksgiving (Nov 1 - Nov 23)" //2033
  char m6[] = "Thanksgiving (Nov 24)"

  char m5[] = "Road to Thanksgiving (Nov 1 - Nov 22)" //2034
  char m6[] = "Thanksgiving (Nov 23)"

  char m5[] = "Road to Thanksgiving (Nov 1 - Nov 21)" //2035
  char m6[] = "Thanksgiving (Nov 22)"



  char m5[] = "Road to Christmas Pt.1 (Nov 29 - Dec 12)" //2024

  char m5[] = "Road to Christmas Pt.1 (Nov 28 - Dec 12)" //2025

  char m5[] = "Road to Christmas Pt.1 (Nov 27 - Dec 12)" //2026

  char m5[] = "Road to Christmas Pt.1 (Nov 26 - Dec 12)" //2027

  char m5[] = "Road to Christmas Pt.1 (Nov 24 - Dec 12)" //2028

  char m5[] = "Road to Christmas Pt.1 (Nov 23 - Dec 12)" //2029

  char m5[] = "Road to Christmas Pt.1 (Nov 29 - Dec 12)" //2030

  char m5[] = "Road to Christmas Pt.1 (Nov 28 - Dec 12)" //2031

  char m5[] = "Road to Christmas Pt.1 (Nov 26 - Dec 12)" //2032

  char m5[] = "Road to Christmas Pt.1 (Nov 25 - Dec 12)" //2033

  char m5[] = "Road to Christmas Pt.1 (Nov 24 - Dec 12)" //2034

  char m5[] = "Road to Christmas Pt.1 (Nov 23 - Dec 12)" //2035

  char m5[] = "Road to Christmas Pt.2 (Dec 13 - Dec 23)"
  char m6[] = "Christmas Eve (Dec 24)"
  char mC[] = "Christmas Day (Dec 25)"



  char m5[] = "Road to New Years (Dec 26 - Dec 30)"
  char m6[] = "New Years Eve (Dec 31)"
  char mC[] = "New Years Day (Jan 1)"



  char m5[] = "January Embers (Jan 2 - Jan 30)"



  char m7[] = "Road to Valentine's Day (Feb 1 - Feb 13)" //2025
  char m7[] = "Valentine's Day (Feb 14)"
  char m7[] = "Mardis Gras Celebration (Feb 15 - Mar 4)"

  char m7[] = "Road to Valentine's Day (Feb 1 - Feb 13)" //2026
  char m7[] = "Valentine's Day (Feb 14)"
  char m7[] = "Mardis Gras Celebration (Feb 15 - Feb 28)"

  char m7[] = "Mardis Gras Celebration (Feb 1 - Feb 9)" //2027
  char m7[] = "Road to Valentine's Day (Feb 10 - Feb 13)"
  char m7[] = "Valentine's Day (Feb 14)" //Big Gap

  char m7[] = "Road to Valentine's Day (Feb 1 - Feb 13)" //2028
  char m7[] = "Valentine's Day (Feb 14)"
  char m7[] = "Mardis Gras Celebration (Feb 15 - Feb 29)"

  char m7[] = "Mardis Gras Celebration (Feb 1 - Feb 13)" //2029
  char m7[] = "Valentine's Day (Feb 14)" //Big Gap

  char m7[] = "Road to Valentine's Day (Feb 1 - Feb 13)" //2030
  char m7[] = "Valentine's Day (Feb 14)"
  char m7[] = "Mardis Gras Celebration (Feb 15 - Mar 5)"

  char m7[] = "Road to Valentine's Day (Feb 1 - Feb 13)" //2031
  char m7[] = "Valentine's Day (Feb 14)"
  char m7[] = "Mardis Gras Celebration (Feb 15 - Feb 25)" //Big gap

  char m7[] = "Mardis Gras Celebration (Feb 1 - Feb 10)" //2032
  char m7[] = "Road to Valentine's Day (Feb 11 - Feb 13)"
  char m7[] = "Valentine's Day (Feb 14)" //Big Gap

  char m7[] = "Road to Valentine's Day (Feb 1 - Feb 13)" //2033
  char m7[] = "Valentine's Day (Feb 14)"
  char m7[] = "Mardis Gras Celebration (Feb 15 - Mar 1)"

  char m7[] = "Road to Valentine's Day (Feb 1 - Feb 13)" //2034
  char m7[] = "Valentine's Day (Feb 14)"
  char m7[] = "Mardis Gras Celebration (Feb 15 - Feb 21)" //Big gap

  char m7[] = "Mardis Gras Celebration (Feb 1 - Feb 6)" //2035
  char m7[] = "Road to Valentine's Day (Feb 7 - Feb 13)"
  char m7[] = "Valentine's Day (Feb 14)" //Big Gap



  char m7[] = "Road to St Patrick's Day (March 5 - March 16"); //2025
  char m7[] = "St Patrick's Day (March 17");

  char m7[] = "Road to St Patrick's Day (March 1 - March 16"); //2026
  char m7[] = "St Patrick's Day (March 17");

  char m7[] = "Road to St Patrick's Day (Feb 15 - March 16"); //2027
  char m7[] = "St Patrick's Day (March 17");

  char m7[] = "Road to St Patrick's Day (March 1 - March 16"); //2028
  char m7[] = "St Patrick's Day (March 17");

  char m7[] = "Road to St Patrick's Day (Feb 15 - March 16"); //2029
  char m7[] = "St Patrick's Day (March 17");

  char m7[] = "Road to St Patrick's Day (March 6 - March 16"); //2030
  char m7[] = "St Patrick's Day (March 17");

  char m7[] = "Road to St Patrick's Day (Feb 26 - March 16"); //2031
  char m7[] = "St Patrick's Day (March 17");

  char m7[] = "Road to St Patrick's Day (Feb 15 - March 16"); //2032
  char m7[] = "St Patrick's Day (March 17");

  char m7[] = "Road to St Patrick's Day (March 2 - March 16"); //2033
  char m7[] = "St Patrick's Day (March 17");

  char m7[] = "Road to St Patrick's Day (Feb 22 - March 16"); //2034
  char m7[] = "St Patrick's Day (March 17");

  char m7[] = "Road to St Patrick's Day (Feb 15 - March 16"); //2035
  char m7[] = "St Patrick's Day (March 17");



  char m8[] = "House Pride: Gryffindor!"; //(March 18 through April 30) using modulus 4 to divide

  char m8[] = "House Pride: Slytherin!";

  char m8[] = "House Pride: Hufflepuff!";

  char m8[] = "House Pride: Ravenclaw!";



  char m9[] = "Cinco de Mayo Celebration (May 1 - May 5)";
  char m9[] = "Manny's Choice Month! (May 6 - May 31)";



  char m10[] = "Pride Month! (June 1 - June 11)";
  char m10[] = "PULSE Memorial Day (June 12)";
  char m10[] = "Pride Month! (June 13 - June 17)";
  char m10[] = "Harry Potter and the Forbidden Journey's Birthday! (June 18)";
  char m10[] = "Pride Month! (June 19 - June 30)";



  char m1[] = "Fourth of July Celebration (Jul 1 - Jul 4)";
  char m3[] = "Celebrating Hogwarts (July 5 - July 30)";
  char m1[] = "Harry Potter's Birthday (Jul 31)";



*/




void setup() {

  Serial.begin(115200);  // Initialize serial communication with a baud rate of 9600
  Wire.begin();          // Begin I2C communication
  rtc.begin();           // Initialize DS3231 RTC module

  DateTime now = rtc.now();    // Get initial time from RTC
  last_second = now.second();  // Store initial second

  /*

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
    }
    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display.display();
    delay(100);  // Pause for 2 seconds
    display.clearDisplay();

    // Show the display buffer on the screen. You MUST call display() after
    // drawing commands to make them visible on screen!
    display.clearDisplay();
    display.display();
    delay(100);

    display.clearDisplay();
    display.setTextSize(3); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Booting..."));
    display.display();      // Show initial text
    display.startscrollleft(0x00, 0x0F);
    delay(5000);
    display.clearDisplay();
    display.display();
  */


  display.begin(SSD1306_SWITCHCAPVCC);
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setTextWrap(false);
  resetTicker();


  //Udp Setup
  WiFi.softAP(ssid, password);
  Udp.begin(localPort);
}









void loop() {

  //oledClock();
  //wledUdpTest();

  //tickerText("msg1", 5000);
  tickerText("msg1", 4000);
  resetTicker();
  delay(500);
  //tickerText("msg2", 14000);
  tickerText("msg2", 4000);
  resetTicker();
  delay(500);








  //*** Date Checker ***//
  //Get Time from RTC
  DateTime rtcTime = rtc.now();
  printTime(rtcTime);
  int m = rtcTime.month();
  int d = rtcTime.day();
  int y = rtcTime.year();
  Serial.println(m);
  Serial.println(d);
  Serial.println(y);



  //The Ultimate Date Case

  /** Year Specific **/

    //2024
  if (y== 2024 && m==11 && d>=1 && d<=27) {
    char m1 = "Road to Thanksgiving (Nov 1 - Nov 27)" //2024
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (y== 2024 && m==11 && d==28) {
    char m1 = "Thanksgiving (Nov 28)" //2024
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (y== 2024 && ((m==11 && d>=29) && (m== 12 && d<=12))) {
    char m1 = "Road to Christmas Pt.1 (Nov 29 - Dec 12)" //2024
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  
    //2025  
  else if (y==2025 && m==2 && d>=1 && d<=13) {
    char m1 = "Road to Valentine's Day (Feb 1 - Feb 13)" //2025
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if ((y==2025 && m==2 && d>=15 && d<=28) || (y==2025 && m==3 && d>=1 && d<=4)){
    char m1 = "Mardis Gras Celebration (Feb 15 - Mar 4)" //2025
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (y==2025 && m==3 && d>=5 && d<=16) {
    char m1 = "Road to St Patrick's Day (March 5 - March 16"); //2025
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (y==2025 && m==11 && d>=1 && d<=26) {
    char m1 = "Road to Thanksgiving (Nov 1 - Nov 26)" //2025
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if ((y==2025 && m==11 && d>=28 && d<=30) || (y==2025 && m==12 && d>=1 && d<=12)){
    char m1 = "Road to Christmas Pt.1 (Nov 28 - Dec 12)" //2025
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  
    //2026
  else if (y==2026 && m==2 && d>=1 && d<=13) {
    char m1 = "Road to Valentine's Day (Feb 1 - Feb 13)" //2026
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if ((y==2026 && m==2 && d>=15 && d<=28)){
    char m1 = "Mardis Gras Celebration (Feb 15 - Feb 28)" //2026
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (y==2026 && m==3 && d>=1 && d<=16) {
    char m1 = "Road to St Patrick's Day (March 1 - March 16"); //2026
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (y==2026 && m==11 && d>=1 && d<=25) {
    char m1 = "Road to Thanksgiving (Nov 1 - Nov 25)" //2026
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if ((y==2026 && m==11 && d>=27 && d<=30) || (y==2026 && m==12 && d>=1 && d<=12)){
    char m1 = "Road to Christmas Pt.1 (Nov 27 - Dec 12)" //2026
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }

    //2027
  else if (y==2027 && m==2 && d>=1 && d<=9) {
    char m1 = "Mardis Gras Celebration (Feb 1 - Feb 9)" //2027
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if ((y==2027 && m==2 && d>=10 && d<=13)){
    char m1 = "Road to Valentine's Day (Feb 10 - Feb 13)" //2027
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if ((y==2027 && m==2 && d>=15 && d<=28) || (y==2027 && m==3 && d>=1 && d<=16)) {
    char m1 = "Road to St Patrick's Day (Feb 15 - March 16"); //2027
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (y==2027 && m==11 && d>=1 && d<=24) {
    char m1 = "Road to Thanksgiving (Nov 1 - Nov 24)" //2027
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if ((y==2027 && m==11 && d>=26 && d<=30) || (y==2027 && m==12 && d>=1 && d<=12)){
    char m1 = "Road to Christmas Pt.1 (Nov 26 - Dec 12)" //2027
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }

  /** This Section is year agnostic **/

  if (m==8 && d>=1 && d<=14) {
    char m1 = "Manny's Choice! (Aug 1 - Aug 14)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==8 && d>=15 && d<=31) {
    char m1 = "Road to Halloween Pt.1 (Aug 15 - Aug 31)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==9 && d==1) {
    char m1 = "First day of School at Hogwarts (Sep 1)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==8 && d>=2 && d<=7) {
    char m1 = "Road to Halloween Pt.1 (Sep 2 - Sep 7)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==9 && d>=8 && d<=30) {
    char m1 = "Road to Halloween Pt.2 (Sep 8 - Sep 30)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==10 && d>=1 && d<=30) {
    char m1 = "Road to Halloween Pt.3 (Oct 1 - Oct 30)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==10 && d==31) {
    char m1 = "Halloween (Oct 31)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==12 && d>=13 && d<=23) {
    char m1 = "Road to Christmas Pt.2 (Dec 13 - Dec 23)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==12 && d==24) {
    char m1 = "Christmas Eve (Dec 24)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==12 && d==25) {
    char m1 = "Christmas Day (Dec 25)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==12 && d>=26 && d<=30) {
    char m1 = "Road to New Years (Dec 26 - Dec 30)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==12 && d==31) {
    char m1 = "New Years Eve (Dec 31)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==1 && d==1) {
    char m1 = "New Years Day (Jan 1)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==1 && d>=2 && d<=30) {
    char m1 = "Road to New Years (Dec 26 - Dec 30)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==2 && d==14) {
    char m1 = "Valentine's Day (Feb 14)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==3 && d==17) {
    char m1 = "St Patrick's Day (March 17)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if ( ((m==3 && d>=19 && d<=31) || (m==4 && d>=1 && d<=30)) && (d%4 == 0)){
    char m1 = "House Pride: Gryffindor!";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if ( ((m==3 && d>=19 && d<=31) || (m==4 && d>=1 && d<=30)) && (d%4 == 1)){
    char m1 = "House Pride: Slytherin!";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if ( ((m==3 && d>=19 && d<=31) || (m==4 && d>=1 && d<=30)) && (d%4 == 2)){
    char m1 = "House Pride: Hufflepuff!";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if ( ((m==3 && d>=19 && d<=31) || (m==4 && d>=1 && d<=30)) && (d%4 == 3)){
    char m1 = "House Pride: Ravenclaw!";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==5 && d>=1 && d<=5) {
    char m1 = "Cinco de Mayo Celebration (May 1 - May 5)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==5 && d>=6 && d<=31) {
    char m1 = "Manny's Choice Month! (May 6 - May 31)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==6 && d>=1 && d<=11) {
    char m1 = "Pride Month! (June 1 - June 11)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==6 && d==12) {
    char m1 = "PULSE Memorial Day (June 12)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==6 && d>=13 && d<=17) {
    char m1 = "Pride Month! (June 13 - June 17)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==6 && d==18) {
    char m1 = "Harry Potter and the Forbidden Journey's Birthday! (June 18)"
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==6 && d>=6 && d<=31) {
    char m1 = "Pride Month! (June 19 - June 30)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==7 && d>=1 && d<=4) {
    char m1 = "Fourth of July Celebration (Jul 1 - Jul 4)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==7 && d>=5 && d<=30) {
    char m1 = "Celebrating Hogwarts (July 5 - July 30)"; 
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  else if (m==7 && d==31) {
    char m1 = "Harry Potter's Birthday (Jul 31)";
    Serial.println(m1);
    ScreenMsg(m1);
    wledUdp(999);
  }
  
}








delay(100);
}













void printTime(DateTime rctTime) {
  sprintf(t, "%02d:%02d:%02d  %02d/%02d/%02d",  rtcTime.hour(),
          rtcTime.minute(),
          rtcTime.second(),
          rtcTime.month(),
          rtcTime.day(),
          rtcTime.year());
}

void resetTicker() {
  x = display.width();
  if (--x < minX) x = display.width();
}

void tickerText(char message[], int timeToScroll) {
  display.setTextColor(SSD1306_WHITE);
  int stop = 0;
  currentTime = millis();
  previousTime = millis();
  while (stop == 0) {
    currentTime = millis();
    //Serial.println(currentTime - previousTime);
    if ((currentTime - previousTime) > timeToScroll) {
      stop = 1;
    }

    //minX = -12 * strlen(message);  // 12 = 6 pixels/character * text size 2
    minX = -28 * strlen(message);
    //minX = -38 * strlen(message);
    display.clearDisplay();
    display.setCursor(x, 0);
    display.print(message);
    display.display();
    if (--x < minX) x = display.width();
  }
  display.clearDisplay();
  display.display();
  delay(100);
}


void wledUdp(char msg) {
  char toSend[] = "{\"ps\": " + msg + "}}");

  Udp.beginPacket(ip, 21324);
  //Udp.write("{\"ps\": 1}}");
  Udp.write(toSend);
  Udp.endPacket();
  Serial.print("Sending: ");
  Serial.println(msg);
  delay(1000);
}


void oledClock() {

  DateTime rtcTime = rtc.now();  // Get current time from RTC
  display.setTextColor(SSD1306_WHITE);
  sprintf(t, "%02d:%02d:%02d  %02d/%02d/%02d", (rtcTime.hour() + 14) % 23, (rtcTime.minute() + 30) % 60, rtcTime.second(), rtcTime.day(), rtcTime.month(), rtcTime.year());
  //sprintf(t, "%02d:%02d:%02d  %02d/%02d/%02d", (rtcTime.hour(), rtcTime.minute(), rtcTime.second(), rtcTime.day(), rtcTime.month(), rtcTime.year()));
  Serial.println(t);
  last_second = second_;  // Update last second  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(t);
  display.display();

  //display.startscrollright(0x00, 0x0F);
  delay(1000);
  //display.stopscroll();
  display.clearDisplay();
  display.display();

  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  rtcTime = rtc.now();  // Get current time from RTC
  sprintf(t, "%02d:%02d:%02d  %02d/%02d/%02d", (rtcTime.hour() + 14) % 23, (rtcTime.minute() + 30) % 60, rtcTime.second(), rtcTime.day(), rtcTime.month(), rtcTime.year());
  //sprintf(t, "%02d:%02d:%02d  %02d/%02d/%02d", (rtcTime.hour(), rtcTime.minute(), rtcTime.second(), rtcTime.day(), rtcTime.month(), rtcTime.year()));
  Serial.println(t);
  last_second = second_;  // Update last second  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(t);
  display.display();

  //display.startscrollleft(0x00, 0x0F);
  delay(1000);
  //display.stopscroll();
  display.clearDisplay();
  display.display();
}
