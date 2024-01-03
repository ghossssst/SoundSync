#include "DFRobot_GDL.h"
/*M0*/
#if defined ARDUINO_SAM_ZERO
#define TFT_DC  7
#define TFT_CS  5
#define TFT_RST 6
/*ESP32 ESP8266*/
#elif defined(ESP32)
#define TFT_DC  D2
#define TFT_CS  D6
#define TFT_RST D3
/*ESP8266*/
#elif defined(ESP8266)
//#define TFT_DC  D4
#define TFT_CS  D6
#define TFT_RST D5
/* AVR series mainboard */
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#endif

DFRobot_ST7735_128x160_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);

int rightsensorPin = A1;   //  Define analog angle sensor pin
int leftsensorPin = A2;   //  Define analog angle sensor pin
int sensorValue = 0;
int dline = 9;
float timedif = 0;
int lclockcycle = 0;
int rclockcycle = 0;
int lefttimestamp = 0;
int righttimestamp = 0;
uint16_t maindiscolor = 0xffff;
int buttonPin = D4;
int buttonState = 0;
int buttontoggle = 0;

void setup()
{
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  pinMode(buttonPin, INPUT_PULLUP);
  screen.begin(); //start screen 
  screen.fillScreen(COLOR_RGB565_BLACK);  //wipe screen
  screentestsetupfn1();
}

void loop()
{
  buttonState = digitalRead(buttonPin);

  if(buttonState == LOW){    //If the button is pressed 
     Serial.println("button pressed");
     Serial.println(buttonState);
     buttontoggle = 1;
     Serial.println(buttontoggle);
     screen.fillScreen(COLOR_RGB565_BLACK);
     screentestsetupfn2();
     delay(200);
     buttonState = 1;
  } 

  while (buttontoggle==1) {

    if (analogRead(leftsensorPin)>0 && buttontoggle==1)
    {
      disgreen();
      //buttontoggle=0;
    }

    if (analogRead(rightsensorPin)>0 && buttontoggle==1)
    {
      disred();
      //buttontoggle=0;
    }

    buttonState = digitalRead(buttonPin);

    if (buttonState == LOW) {    //If the button is pressed 
      Serial.println("button pressed");
      buttontoggle = 0;
      Serial.println(buttontoggle);
      delay(200);
      screentestsetupfn1();
    } 
  }

  dline = dline+1;

  leftsens();
  rightsens();

  timedif = 0;

  screenscan();

  if (analogRead(leftsensorPin)>analogRead(rightsensorPin))
  {
    //Serial.println("left wins");
    screen.drawFastVLine(dline, 54, 20, 0x07e0);
  }

  if (analogRead(rightsensorPin)>analogRead(leftsensorPin))
  {
    //Serial.println("right wins");
    screen.drawFastVLine(dline, 54, 20, 0xf800);
  }

  if (lclockcycle>0)
  {
    lclockcycle = lclockcycle - 1;
    screen.drawFastHLine(150, lclockcycle/2.564+9, 10, 0x0000);
  }
  else {
      screen.fillRect(150, 9, 10, 39, maindiscolor);
  }

  if (rclockcycle>0)
  {
    rclockcycle = rclockcycle - 1;
    screen.setRotation(3);
    screen.drawFastHLine(0, rclockcycle/2.564+9, 10, 0x0000);
    screen.setRotation(1);
  }
  else {
      screen.fillRect(150, 80, 10, 39, maindiscolor);
  }

  if (analogRead(leftsensorPin)>0 && lclockcycle==0)
  {
    Serial.println("test");
    lefttimestamp = millis();
    lclockcycle = 100;
  }

  if (analogRead(rightsensorPin)>0 && rclockcycle==0)
  {
    righttimestamp = millis();
    rclockcycle = 100;
  }

  if (lclockcycle > 0 || rclockcycle > 0)
  {
    timedif = lefttimestamp - righttimestamp;

    if (timedif<0)
    {
      timedif=-timedif;
    }
  }
  
  if (timedif/1000>9)
  {
    timedif=0;
  }

  screentext();

  if (dline > 146) 
  {
    dline = 9;
  }

  //delay(1);
}

void screentestsetupfn1()
{
  screen.setRotation(2);

  screen.fillRect(0, 0, 9, 10, maindiscolor);
  screen.fillRect(119, 0, 9, 10, maindiscolor);

  screen.setCursor(2, 2);
  screen.setTextColor(0x0000); 
  screen.println("L");

  screen.setCursor(121, 2);
  screen.setTextColor(0x0000); 
  screen.println("R");

  screen.fillRect(0, 151, 130, 9, maindiscolor);

  screen.setCursor(2, 152);
  screen.setTextColor(0x0000); 
  screen.println("FN: 1");

  screen.setRotation(1);
}

void screentestsetupfn2()
{
  screen.setRotation(2);

  screen.fillRect(0, 151, 130, 9, maindiscolor);

  screen.setCursor(2, 152);
  screen.setTextColor(0x0000); 
  screen.println("FN: 2");

  screen.setRotation(1);
}

void leftsens() 
{
  int ldb;

  ldb=analogRead(leftsensorPin);
  
  if (ldb > 0) 
  {
    Serial.println(ldb,DEC);
  }
}

void rightsens() 
{
  int rdb;

  rdb=analogRead(rightsensorPin);

  if (rdb > 0) 
  {
    Serial.println(rdb,DEC);
  }
}
 
void screenscan()
{
  screen.setTextSize(0);
  screen.drawFastVLine(159, 0, screen.height(), maindiscolor);
  screen.drawFastVLine(10, 0, screen.height(), 0x0000);
  screen.drawFastVLine(dline, 0, analogRead(leftsensorPin)/18.518+1, 0x07e0);
  screen.drawFastVLine(dline, 128 - analogRead(rightsensorPin)/18.518-1, 120, 0xf800);
  screen.drawFastVLine(dline+1, 0, screen.height(), 0x0000);
  screen.drawFastVLine(dline+2, 0, screen.height(), maindiscolor);
  //screen.drawFastVLine(0, 0, screen.height(), maindiscolor);
  //screen.drawFastVLine(1, 0, screen.height(), maindiscolor);
  screen.drawFastHLine(0, 54, 149, maindiscolor);
  screen.drawFastHLine(0, 74, 149, maindiscolor);
}

void screentext()
{
  //Serial.println(timedif, DEC);

  screen.setRotation(2);

  screen.fillRect(48, 0, 32, 10, maindiscolor);

  screen.setCursor(50, 2);
  screen.setTextColor(0x0000); 
  screen.println(timedif / 1000, 3);
  
  screen.setRotation(1);
}

void modetwo()
{

}

void disgreen()
{
  screen.setRotation(2);
  Serial.println("disgreen");
  screen.setTextSize(3);
  screen.fillRect(0, 0, 128, 151, 0x07e0);
  screen.setCursor(50, 2);
  screen.println("L");
  screen.setTextSize(0);
  delay(1000);
  screen.fillRect(0, 0, 128, 151, 0x0000);
  screen.setRotation(1);
}

void disred()
{
  screen.setRotation(2);
  Serial.println("disred");
  screen.setTextSize(3);
  screen.fillRect(0, 0, 128, 151, 0xf800);
  screen.setCursor(50, 2);
  screen.println("R");
  screen.setTextSize(0);
  delay(1000);
  screen.fillRect(0, 0, 128, 151, 0x0000);
  screen.setRotation(1);
}
