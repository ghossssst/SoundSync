#include "DFRobot_UI.h"
#include "DFRobot_GDL.h"
#include <driver/rtc_io.h>
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

int buttonPin = D4;    //  Define board button pin
int rightsensorPin = A1;   //  Define analog sensor pin
int leftsensorPin = A2;   //  Define analog sensor pin
int dline = 9;
int lclockcycle = 0;
int rclockcycle = 0;
int lefttimestamp = 0;
int righttimestamp = 0;
int buttontoggle = 0;
int menuselected = 0;
int settingsmenuselected = 0;
int longpress = 0;
int timedelay = 0;
int synctimer = 0;
float audiosens = 0;
float synclenity = 24.50;
float timedif = 0;
uint16_t maindiscolor = 0xffff;
bool settingsexit = false;
bool settingschoosemenu = false;
bool choosemenu = false;
bool startup = true;

void setup()
{
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  pinMode(buttonPin, INPUT_PULLUP);
  screen.begin(); //start screen 
  screen.fillScreen(COLOR_RGB565_BLACK);  //wipe screen
  screensplash();
  screen.fillScreen(COLOR_RGB565_BLACK);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_27,1);
}

void loop()
{ 
  while (startup == true)
  {
    Serial.println("run audio sync");
    buttontoggle=1;
    screensetupfn2();
    while (buttontoggle==1) 
    {
      while (synctimer < synclenity+1)
      {
        if (analogRead(leftsensorPin)>audiosens && analogRead(rightsensorPin)>audiosens)
        {
          disamber();
          while (timedelay < 5000000 && buttontoggle == 1)
          {
            timedelay = timedelay+1;
            if (digitalRead(buttonPin) == LOW) 
            {    //If the button is pressed 
              buttontoggle = 0;
              while (digitalRead(buttonPin) == LOW)
              {
                delay(1);
              }
              startup = false; 
            }
          }
          timedelay = 0;
          screen.fillRect(9, 0, 151, 128, 0x0000);
        }
        else 
        {
          synctimer = synctimer+1;
        }
      }

      if (analogRead(leftsensorPin)>audiosens)
      {
        disgreen();
        while (timedelay < 5000000 && buttontoggle == 1)
        {
          timedelay = timedelay+1;
          if (digitalRead(buttonPin) == LOW) 
          {    //If the button is pressed 
            buttontoggle = 0;
            while (digitalRead(buttonPin) == LOW)
            {
              delay(1);
            }
            startup = false; 
          }
        }
        timedelay = 0;
        screen.fillRect(9, 0, 151, 128, 0x0000);
      }

      if (analogRead(rightsensorPin)>audiosens)
      {
        disred();
        while (timedelay < 5000000 && buttontoggle == 1)
        {
          timedelay = timedelay+1;
          if (digitalRead(buttonPin) == LOW) 
          {    //If the button is pressed 
            buttontoggle = 0;
            while (digitalRead(buttonPin) == LOW)
            {
              delay(1);
            }
            startup = false; 
          }
        }
        timedelay = 0;
        screen.fillRect(9, 0, 151, 128, 0x0000);
        
      }

      synctimer = 0;

      if (digitalRead(buttonPin) == LOW) 
      {    //If the button is pressed ;
        buttontoggle = 0;
        while (digitalRead(buttonPin) == LOW)
        {
          delay(1);
        }
        startup = false; 
        screen.fillScreen(COLOR_RGB565_BLACK);
        screen.fillRect(0, 0, 9, 130, maindiscolor);
      }
    }
  }





  if (digitalRead(buttonPin) == LOW)
  {
    while (digitalRead(buttonPin) == LOW && longpress < 400)
    {
      delay(1);

      longpress = longpress + 1;

      if (longpress > 399)
      {
        screen.fillScreen(COLOR_RGB565_BLACK);
        Serial.println("button longpressed");
        choosemenu = true;
      }
    }
    screen.fillRect(109, 0, 51, 128, 0x0000);

    longpress = 0;

    while (digitalRead(buttonPin) == LOW)
    {
      delay(1);
    }

    if (choosemenu == true && menuselected == 0)
    {
      Serial.println("run audio sync");
      buttontoggle=1;
      screensetupfn2();
      while (buttontoggle==1) 
      {
        while (synctimer < synclenity+1)
        {
          if (analogRead(leftsensorPin)>audiosens && analogRead(rightsensorPin)>audiosens)
          {
            disamber();
            while (timedelay < 5000000 && buttontoggle == 1)
            {
              timedelay = timedelay+1;
              if (digitalRead(buttonPin) == LOW) 
              {    //If the button is pressed 
                buttontoggle = 0;
                while (digitalRead(buttonPin) == LOW)
                {
                  delay(1);
                }
                startup = false; 
              }
            }
            timedelay = 0;
            screen.fillRect(9, 0, 151, 128, 0x0000);
          }
          else 
          {
            synctimer = synctimer+1;
          }
        }

        if (analogRead(leftsensorPin)>audiosens)
        {
          disgreen();
          while (timedelay < 5000000 && buttontoggle == 1)
          {
            timedelay = timedelay+1;
            if (digitalRead(buttonPin) == LOW) 
            {    //If the button is pressed 
              buttontoggle = 0;
              while (digitalRead(buttonPin) == LOW)
              {
                delay(1);
              }
              startup = false; 
            }
          }
          timedelay = 0;
          screen.fillRect(9, 0, 151, 128, 0x0000);
        }

        if (analogRead(rightsensorPin)>audiosens)
        {
          disred();
          while (timedelay < 5000000 && buttontoggle == 1)
          {
            timedelay = timedelay+1;
            if (digitalRead(buttonPin) == LOW) 
            {    //If the button is pressed 
              buttontoggle = 0;
              while (digitalRead(buttonPin) == LOW)
              {
                delay(1);
              }
              startup = false; 
            }
          }
          timedelay = 0;
          screen.fillRect(9, 0, 151, 128, 0x0000);
        }

        synctimer = 0;

        if (digitalRead(buttonPin) == LOW) 
        {    //If the button is pressed ;
          buttontoggle = 0;
          while (digitalRead(buttonPin) == LOW)
          {
            delay(1);
          }
          startup = false; 
          screen.fillScreen(COLOR_RGB565_BLACK);
        }
      }
    }
    
    if (choosemenu == true && menuselected == 1)
    {
      Serial.println("run sonograph");
      screensetupfn1();
      buttontoggle=1;
      while (buttontoggle==1) 
      {
        dline = dline+1;

        timedif = 0;

        if ((analogRead(leftsensorPin)>analogRead(rightsensorPin)) && (analogRead(leftsensorPin)>audiosens))
        {
          //Serial.println("left wins");
          screen.drawFastVLine(dline, 54, 20, 0x07e0);
        }

        if ((analogRead(rightsensorPin)>analogRead(leftsensorPin)) && (analogRead(rightsensorPin)>audiosens))
        {
          //Serial.println("right wins");
          screen.drawFastVLine(dline, 54, 20, 0xf800);
        }

        sonograph();

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

        if (analogRead(leftsensorPin)>audiosens && lclockcycle==0)
        {
          lefttimestamp = millis();
          lclockcycle = 100;
        }

        if (analogRead(rightsensorPin)>audiosens && rclockcycle==0)
        {
          righttimestamp = millis();
          rclockcycle = 100;
        }

        if (lclockcycle > 0 || rclockcycle > 0)
        {
          if (lefttimestamp > righttimestamp);
          {
          timedif = lefttimestamp - righttimestamp;
          }

          if (righttimestamp > lefttimestamp);
          {
          timedif = righttimestamp - lefttimestamp;
          }

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

        if (digitalRead(buttonPin) == LOW) 
        {    //If the button is pressed 
          Serial.println("button pressed");
          buttontoggle = 0;
          while (digitalRead(buttonPin) == LOW)
          {
            delay(1);
          }
          screen.fillScreen(COLOR_RGB565_BLACK);
          Serial.println(buttontoggle);
        } 
      }
    }
    
    if (choosemenu == true && menuselected == 2)
    {
      Serial.println("data");
      buttontoggle=1;
      screendatatext();
      while (buttontoggle==1) 
      {
        screen.setRotation(2);

        screen.fillRect( 2, 12, screen.width(), 10, 0x0000);
        screen.setTextSize(1);
        screen.setTextColor(0xffff); 
        screen.setCursor(2, 12);
        screen.println(analogRead(leftsensorPin));

        screen.fillRect( 2, 32, screen.width(), 10, 0x0000);
        screen.setTextSize(1);
        screen.setTextColor(0xffff); 
        screen.setCursor(2, 32);
        screen.println(analogRead(rightsensorPin));

        screen.fillRect( 2, 52, screen.width(), 10, 0x0000);
        screen.setTextSize(1);
        screen.setTextColor(0xffff); 
        screen.setCursor(2, 52);
        screen.println(millis());

        delay(10); 

        screen.setRotation(1);       

        if (digitalRead(buttonPin) == LOW) 
        {    //If the button is pressed 
          Serial.println("button pressed");
          buttontoggle = 0;
          while (digitalRead(buttonPin) == LOW)
          {
            delay(1);
          }
          screen.fillScreen(COLOR_RGB565_BLACK);
          Serial.println(buttontoggle);
        } 
      }
    }

    if (choosemenu == true && menuselected == 3)
    {
      Serial.println("Settings");
      screen.fillRect(0, 0, 9, 130, maindiscolor);
      settingsdisplay();
      while (settingsexit == false)
      {
        if (digitalRead(buttonPin) == LOW)
        {
          while (digitalRead(buttonPin) == LOW && longpress < 400)
          {
            delay(1);

            longpress = longpress + 1;

            if (longpress > 399)
            {
              screen.fillScreen(COLOR_RGB565_BLACK);
              Serial.println("button longpressed");
              settingschoosemenu = true;
            }
          }

          screen.fillRect(109, 0, 51, 128, 0x0000);

          longpress = 0;

          if (settingschoosemenu == true && settingsmenuselected == 0)
          {
            Serial.println("Synchronizer Lenity");
            buttontoggle=1;
            screen.fillRect(0, 0, 9, 130, maindiscolor);
            screen.setRotation(2);

            screen.setTextSize(1);
            screen.setTextColor(0xffff); 
            screen.setCursor(2, 2);
            screen.println(">Synchronizer Lenity:");

            screen.drawRect(2, 12, 126, 8, maindiscolor);
            screen.drawRect(3, 13, 124, 6, maindiscolor);
            screen.fillRect(4, 14, synclenity, 4, 0x7E0);

            screen.setCursor(2, 22);
            screen.println("Audio Sensitivity");

            screen.drawRect(2, 32, 126, 8, maindiscolor);
            screen.fillRect(4, 34, audiosens, 4, 0x7E0);

            screen.setCursor(2, 42);
            screen.println("Return to menu");

            screen.setCursor(2, 152);
            screen.setTextColor(0x0000); 
            screen.println("SETTINGS MENU: OPT 1");

            screen.setRotation(1);
            while (digitalRead(buttonPin) == LOW)
            {
              delay(1);
            }
            while (buttontoggle==1) 
            {     
              if (digitalRead(buttonPin) == LOW) 
              {    //If the button is pressed 
                Serial.println("button pressed");
                while (digitalRead(buttonPin) == LOW && longpress < 400)
                {
                  delay(1);

                  longpress = longpress + 1;

                  if (longpress > 399)
                  {
                    Serial.println("button longpressed");
                    buttontoggle = 0;
                    synclenity = synclenity - 12.25;
                  }
                }

                longpress = 0;

                synclenity = synclenity + 12.25;
                screen.setRotation(2); 
                screen.drawRect(2, 12, 126, 8, maindiscolor);
                screen.drawRect(3, 13, 124, 6, maindiscolor);
                
                if (synclenity > 128)
                {
                  synclenity = 0;                  
                  screen.fillRect(2, 12, 126, 8, 0x0000);
                  screen.drawRect(2, 12, 126, 8, maindiscolor);
                  screen.drawRect(3, 13, 124, 6, maindiscolor);
                }
                screen.fillRect(4, 14, synclenity, 4, 0x7E0);
                screen.setRotation(1);
                Serial.println(synclenity);
              } 
            }
            screen.fillScreen(COLOR_RGB565_BLACK);
          }

          if (settingschoosemenu == true && settingsmenuselected == 1)
          {
            Serial.println("Audio Sensitivity");
            buttontoggle=1;
            screen.fillRect(0, 0, 9, 130, maindiscolor);
            screen.setRotation(2);

            screen.setTextSize(1);
            screen.setTextColor(0xffff); 
            screen.setCursor(2, 2);
            screen.println("Synchronizer Lenity:");

            screen.drawRect(2, 12, 126, 8, maindiscolor);
            screen.fillRect(4, 14, synclenity, 4, 0x7E0);

            screen.setCursor(2, 22);
            screen.println(">Audio Sensitivity");

            screen.drawRect(2, 32, 126, 8, maindiscolor);
            screen.drawRect(3, 33, 124, 6, maindiscolor);
            screen.fillRect(4, 34, audiosens/8.15, 4, 0x7E0);

            screen.setCursor(2, 42);
            screen.println("Return to menu");

            screen.setCursor(2, 152);
            screen.setTextColor(0x0000); 
            screen.println("SETTINGS MENU: OPT 2");

            screen.setRotation(1);
            while (digitalRead(buttonPin) == LOW)
            {
              delay(1);
            }
            while (buttontoggle==1) 
            {     
              if (digitalRead(buttonPin) == LOW) 
              {    //If the button is pressed 
                Serial.println("button pressed");
                while (digitalRead(buttonPin) == LOW && longpress < 400)
                {
                  delay(1);

                  longpress = longpress + 1;

                  if (longpress > 399)
                  {
                    Serial.println("button longpressed");
                    buttontoggle = 0;
                    audiosens = audiosens - 100;
                  }
                }

                longpress = 0;

                audiosens = audiosens + 100;
                screen.setRotation(2); 
                screen.drawRect(2, 32, 126, 8, maindiscolor);
                screen.drawRect(3, 33, 124, 6, maindiscolor);
                
                if (audiosens > 1000)
                {
                  audiosens = 0;                  
                  screen.fillRect(2, 32, 126, 8, 0x0000);
                  screen.drawRect(2, 32, 126, 8, maindiscolor);
                  screen.drawRect(3, 33, 124, 6, maindiscolor);
                }
                screen.fillRect(4, 34, audiosens/8.15, 4, 0x7E0);
                screen.setRotation(1);
                Serial.println(audiosens);
              } 
            }
            screen.fillScreen(COLOR_RGB565_BLACK);
          }

          if (settingschoosemenu == true && settingsmenuselected == 2)
          {
            settingsexit = true;
          }

          if (settingsmenuselected == 2)
          {
            settingsmenuselected = settingsmenuselected-2;
          } else {
            settingsmenuselected = settingsmenuselected+1;  
          }
          while (digitalRead(buttonPin) == LOW)
          {
            delay(1);
          }
          screen.fillRect(0, 0, 9, 130, maindiscolor);
          settingschoosemenu = false;
        }

        if (settingsmenuselected == 0 && settingsexit == false)
        {
          screen.setRotation(2);

          screen.setTextSize(1);
          screen.setTextColor(0x0000); 
          screen.setCursor(2, 2);
          screen.fillRect(2, 1, 130, 9, maindiscolor);
          screen.println(">Synchronizer Lenity:");
          screen.setTextColor(0xffff); 

          screen.drawRect(2, 12, 126, 8, maindiscolor);
          screen.fillRect(4, 14, synclenity, 4, 0x7E0);

          screen.setCursor(2, 22);
          screen.println("Audio Sensitivity");

          screen.drawRect(2, 32, 126, 8, maindiscolor);
          screen.fillRect(4, 34, audiosens/8.15, 4, 0x7E0);

          screen.setCursor(2, 42);
          screen.println("Return to menu");

          screen.setCursor(2, 152);
          screen.setTextColor(0x0000); 
          screen.println("SETTINGS MENU: OPT 1");

          screen.setRotation(1);

          while (digitalRead(buttonPin) != LOW)
          {
            delay(1);
          }
        }

        if (settingsmenuselected == 1 && settingsexit == false)
        {
          screen.setRotation(2);

          screen.setTextSize(1);
          screen.setTextColor(0xffff); 
          screen.setCursor(2, 2);
          screen.println("Synchronizer Lenity:");

          screen.drawRect(2, 12, 126, 8, maindiscolor);
          screen.fillRect(4, 14, synclenity, 4, 0x7E0);

          screen.fillRect(2, 21, 130, 9, maindiscolor);
          screen.setCursor(2, 22);
          screen.setTextColor(0x0000); 
          screen.println(">Audio Sensitivity:");
          screen.setTextColor(0xffff); 

          screen.drawRect(2, 32, 126, 8, maindiscolor);
          screen.fillRect(4, 34, audiosens/8.15, 4, 0x7E0);

          screen.setCursor(2, 42);
          screen.println("Return to menu");

          screen.setCursor(2, 152);
          screen.setTextColor(0x0000); 
          screen.println("SETTINGS MENU: OPT 2");

          screen.setRotation(1);

          while (digitalRead(buttonPin) != LOW)
          {
            delay(1);
          }
        }

        if (settingsmenuselected == 2 && settingsexit == false)
        {
          screen.setRotation(2);

          screen.setTextSize(1);
          screen.setTextColor(0xffff); 
          screen.setCursor(2, 2);
          screen.println("Synchronizer Lenity:");

          screen.drawRect(2, 12, 126, 8, maindiscolor);
          screen.fillRect(4, 14, synclenity, 4, 0x7E0);

          screen.setCursor(2, 22);
          screen.println("Audio Sensitivity:");

          screen.drawRect(2, 32, 126, 8, maindiscolor);
          screen.fillRect(4, 34, audiosens/8.15, 4, 0x7E0);

          screen.fillRect(2, 41, 130, 9, maindiscolor);
          screen.setCursor(2, 42);
          screen.setTextColor(0x0000); 
          screen.println(">Return to menu");
          screen.setTextColor(0xffff); 

          screen.setCursor(2, 152);
          screen.setTextColor(0x0000); 
          screen.println("SETTINGS MENU: OPT 3");

          screen.setRotation(1);

          while (digitalRead(buttonPin) != LOW)
          {
            delay(1);
          }
        }
      }
      settingsexit = false;
      screen.fillRect(109, 0, 51, 128, 0x0000);
    }
    

    if (menuselected == 3)
    {
      menuselected = menuselected-3;
    } else {
      menuselected = menuselected+1;  
    }
    screen.fillRect(0, 0, 9, 130, maindiscolor);
    choosemenu = false;
  }

  if (menuselected == 0)
  {
    screen.setRotation(2);
    screen.fillRect(2, 1, 130, 9, maindiscolor);
    screen.setTextSize(1);
    screen.setTextColor(0x0000); 
    screen.setCursor(2, 2);
    screen.println(">Audio synchroniser");

    screen.setTextColor(0xffff); 
    screen.setCursor(2, 12);
    screen.println("Sonograph");

    screen.setTextColor(0xffff); 
    screen.setCursor(2, 22);
    screen.println("Raw Data Output");

    screen.setTextColor(0xffff); 
    screen.setCursor(2, 32);
    screen.println("Settings");

    screen.setCursor(2, 152);
    screen.setTextColor(0x0000); 
    screen.println("MENU: PRGM 1");

    screen.setRotation(1);

    while (digitalRead(buttonPin) != LOW)
    {
      delay(1);
    }
  }

  if (menuselected == 1)
  {
    screen.setRotation(2);

    screen.setTextSize(1);
    screen.setTextColor(0xffff); 
    screen.setCursor(2, 2);
    screen.println("Audio synchroniser");

    screen.fillRect(2, 11, 130, 9, maindiscolor);
    screen.setTextColor(0x0000); 
    screen.setCursor(2, 12);
    screen.println(">Sonograph");

    screen.setTextColor(0xffff); 
    screen.setCursor(2, 22);
    screen.println("Raw Data Output");

    screen.setTextColor(0xffff); 
    screen.setCursor(2, 32);
    screen.println("Settings");

    screen.setCursor(2, 152);
    screen.setTextColor(0x0000); 
    screen.println("MENU: PRGM 2");

    screen.setRotation(1);

    while (digitalRead(buttonPin) != LOW)
    {
      delay(1);
    }
  }

  if (menuselected == 2)
  {
    screen.setRotation(2);

    screen.setTextSize(1);
    screen.setTextColor(0xffff); 
    screen.setCursor(2, 2);
    screen.println("Audio synchroniser");

    screen.setTextColor(0xffff); 
    screen.setCursor(2, 12);
    screen.println("Sonograph");

    screen.fillRect(2, 21, 130, 9, maindiscolor);
    screen.setTextColor(0x0000); 
    screen.setCursor(2, 22);
    screen.println(">Raw Data Output");

    screen.setTextColor(0xffff); 
    screen.setCursor(2, 32);
    screen.println("Settings");

    screen.setCursor(2, 152);
    screen.setTextColor(0x0000); 
    screen.println("MENU: PRGM 3");

    screen.setRotation(1);

    while (digitalRead(buttonPin) != LOW)
    {
      delay(1);
    }
  }

  if (menuselected == 3)
  {
    screen.setRotation(2);

    screen.setTextSize(1);
    screen.setTextColor(0xffff); 
    screen.setCursor(2, 2);
    screen.println("Audio synchroniser");

    screen.setTextColor(0xffff); 
    screen.setCursor(2, 12);
    screen.println("Sonograph");

    screen.setTextColor(0xffff); 
    screen.setCursor(2, 22);
    screen.println("Raw Data Output");

    screen.fillRect(2, 31, 130, 9, maindiscolor);
    screen.setTextColor(0x0000); 
    screen.setCursor(2, 32);
    screen.println(">Settings");

    screen.setCursor(2, 152);
    screen.setTextColor(0x0000); 
    screen.println("MENU: PRGM 4");

    screen.setRotation(1);

    while (digitalRead(buttonPin) != LOW)
    {
      delay(1);
    }
  }
}

void screensplash()
{
  screen.setRotation(2);

  screen.setTextSize(1);
  screen.setTextColor(0xffff); 
  screen.setCursor(2, 2);
  screen.println("ghost's \"soundsync\"");
  Serial.println("plash1");
  delay(50);
  screen.setCursor(2, 12);
  screen.println("audio synchroniser");
  delay(50);
  screen.setCursor(2, 22);
  screen.println(".");
  delay(50);
  screen.setCursor(2, 32);
  screen.println(".");
  delay(50);
  screen.setCursor(2, 42);
  screen.println(".");
  delay(50);
  screen.setCursor(2, 52);
  screen.println(".");
  delay(50);
  screen.setCursor(2, 62);
  screen.println(".");
  delay(50);
  screen.setCursor(2, 72);
  screen.println(".");
  delay(50);
  screen.setCursor(2, 82);
  screen.println(".");
  delay(50);
  screen.setCursor(2, 92);
  screen.println(".");
  delay(50);
  screen.setCursor(2, 102);
  screen.println(".");
  delay(50);
  screen.setCursor(2, 112);
  screen.println(".");
  delay(50);
  screen.setCursor(2, 122);
  screen.println("firmware version:");
  delay(50);
  screen.setCursor(2, 132);
  screen.println("0.04.7");
  delay(50);
  screen.setCursor(2, 142);
  screen.println("code name:");
  delay(50);
  screen.setCursor(2, 152);
  screen.println("control");
  delay(500);

  screen.setRotation(1);
}

void screensetupfn1()
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
  screen.println("PRGM: 2");

  screen.setRotation(1);
}

void screensetupfn2()
{
  screen.setRotation(2);

  screen.fillRect(0, 151, 130, 9, maindiscolor);

  screen.setCursor(2, 152);
  screen.setTextColor(0x0000); 
  screen.println("PRGM: 1");

  screen.setRotation(1);
}
 
void sonograph()
{
  screen.setTextSize(0);
  screen.drawFastVLine(159, 0, screen.height(), maindiscolor);
  screen.drawFastVLine(10, 0, screen.height(), 0x0000);
  screen.drawFastVLine(dline, 0, analogRead(leftsensorPin)/18.518+1, 0x07e0);
  screen.drawFastVLine(dline, 128 - analogRead(rightsensorPin)/18.518-1, 120, 0xf800);
  screen.drawFastVLine(dline+1, 0, screen.height(), 0x0000);
  screen.drawFastVLine(dline+2, 0, screen.height(), maindiscolor);
  screen.drawFastHLine(0, 54, 149, maindiscolor);
  screen.drawFastHLine(0, 74, 149, maindiscolor);
  screen.drawFastHLine(0, audiosens/18.618, 149, maindiscolor);
  screen.drawFastHLine(0, 127 - (audiosens/18.618), 149, maindiscolor);
}

void screendatatext()
{
  screen.setRotation(2);

  screen.setTextSize(1);
  screen.setTextColor(0xffff); 
  screen.setCursor(2, 2);
  screen.println("Left microphone:");

  screen.setTextSize(1);
  screen.setTextColor(0xffff); 
  screen.setCursor(2, 22);
  screen.println("Right microphone:");

  screen.setTextSize(1);
  screen.setTextColor(0xffff); 
  screen.setCursor(2, 42);
  screen.println("System time(ms):");

  screen.fillRect(0, 151, 130, 9, maindiscolor);

  screen.setCursor(2, 152);
  screen.setTextColor(0x0000); 
  screen.println("PRGM: 3");

  screen.setRotation(2);
}

void settingsdisplay()
{
  screen.setRotation(2);

  screen.setTextSize(1);
  screen.setTextColor(0xffff); 
  screen.setCursor(2, 2);
  screen.println(">Synchronizer Lenity:");

  screen.drawRect(2, 12, 126, 8, maindiscolor);

  screen.setCursor(2, 22);
  screen.println("Audio Sensitivity");

  screen.drawRect(2, 32, 126, 8, maindiscolor);

  screen.setCursor(2, 42);
  screen.println("Return to menu");

  screen.setCursor(2, 152);
  screen.setTextColor(0x0000); 
  screen.println("SETTINGS MENU: OPT 1");

  screen.setRotation(1);
}

void screentext()
{
  screen.setRotation(2);

  screen.fillRect(48, 0, 32, 10, maindiscolor);

  screen.setCursor(50, 2);
  screen.setTextColor(0x0000); 
  screen.println(timedif / 1000, 3);
  
  screen.setRotation(1);
}

void disgreen()
{
  screen.setRotation(2);
  Serial.println("disgreen");
  screen.setTextSize(3);
  screen.fillRect(0, 0, 128, 151, 0x07e0);
  screen.setCursor(55, 2);
  screen.println("L");
  screen.setTextSize(0);
  screen.setRotation(1);
}

void disred()
{
  screen.setRotation(2);
  Serial.println("disred");
  screen.setTextSize(3);
  screen.fillRect(0, 0, 128, 151, 0xf800);
  screen.setCursor(55, 2);
  screen.println("R");
  screen.setTextSize(0);
  screen.setRotation(1);
}

void disamber()
{
  screen.setRotation(2);
  Serial.println("disamber");
  screen.setTextSize(3);
  screen.fillRect(0, 0, 128, 151, 0xfdc0);
  screen.setCursor(10, 2);
  screen.println("synced");
  screen.setTextSize(0);
  screen.setRotation(1);
}
