#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <string.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Adafruit_7segment segment = Adafruit_7segment();

struct{
  char title[100];
  char artist[100];
  char current_time[5];
} song;

int timer1 = 0;
int timer2 = 0;
int timer3 = 0;
int timer4 = 0;

boolean start_flg = false;

char last_title[100];

void setup()
{
  Serial.begin(9600);
  segment.begin(0x70);
  lcd.begin(16, 2);
  lcd.print("Now Playing:");
  segment.print(0, DEC);
  segment.writeDisplay();
  pinMode(13, OUTPUT);
}

void readSerial()
{
  
  int i = 0, j;
  char temp = NULL;
  boolean flag = false;

  for(j=0; j<100; j++){
    *(song.title + i) = *(song.artist + i) = ' ';
  }

  while(true){
    while(Serial.available() > 0){
      temp = Serial.read();
      if(temp == '\n'){
        flag = true;
        break;
      }
      song.title[i] = temp;
      i++;
    }
    if(flag) break;
  }
  song.title[i] = '\0';

  flag = false;
  temp = NULL;
  i = 0;

  while(true){
    while(Serial.available() > 0){
      temp = Serial.read();
      if(temp == '\n'){
        flag = true;
        break;
      }
      song.artist[i] = temp;
      i++;
    }
    if(flag) break;
  }
  song.artist[i] = '\0';

  flag = false;
  temp = NULL;
  i = 0;
  
  while(true){
    while(Serial.available() > 0){
      temp = Serial.read();
      if(temp == '\n'){
        flag = true;
        break;
      }
      song.current_time[i] = temp;
      i++;
    }
    if(flag) break;
  }
  song.current_time[i] = '\0';
}

void loop()
{
  boolean blinkColon = false;
  boolean trackChanged = false;
  char disp[17];
  int i;
  
  digitalWrite(13, HIGH);
  readSerial();
  digitalWrite(13, LOW);

  if(strcmp(song.title, last_title)){
    trackChanged = true;
    start_flg = true;
    strcpy(last_title, song.title);
  }
  else
    trackChanged = false;

  lcd.clear();
  lcd.print(song.artist);
  lcd.setCursor(0,1);

  if(strlen(song.title) < 16){
    lcd.print(song.title);
  }
  else{
    if(!trackChanged){

      if(start_flg){
        if(timer4 == 3){
          timer4 = 0;
          start_flg = false;
        }
        else{
          timer4++;
          timer2 = 0;
          timer1 = 0;
        }
      }
      
      int j = 0;
      for(i=timer2; i<timer2+16; i++){
        if(song.title[i] == '\0') break;
        disp[j] = song.title[i];
        j++;
      }

      disp[j] = '\0';
      lcd.print(disp);

      if(timer1 == 1){
        timer1 = 0;

        if(strlen(song.title) == i){
          if(timer3 == 3){
            timer3 = 0;
            timer2 = 0;
            start_flg = true;
          }
          else{
            timer3++;
          }
        }
        else{
          timer2++;
        }
      }
      
      timer1++;
    }
    else
      lcd.print(song.title);
  }
  
  segment.print(atoi(song.current_time), DEC);

  blinkColon = !blinkColon;
  segment.drawColon(blinkColon);
  
  segment.writeDisplay();

  delay(300);
}
