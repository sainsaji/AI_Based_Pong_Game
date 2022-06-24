// Authour Sain Saji
// BaseCode for SSD1306 128x64 OLED Displays.
// GITHUB Link for Complete Tutorial: 
// https://github.com/sainsaji/SSD1306-Base-Code-for-Raspberry-Pico-Ardunio-Library/edit/main/README.md
// Connect Ground PIN of OLED with PICO Ground PIN : PIN: 38
// Connect VCC PIN of OLED with the 36th PIN of PICO
// Connect SCL [CLOCK] to PIN 7 [GPIO PIN 5]
// Connect SDA [DATA] to PIN 6 [GPIO PIN 4]

//FYI: The sketch won't complie in wowki due to some library issue,Works fine with Arduino IDE

//Libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Picopixel.h"

//Definitions
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Make sure this is set to -1 for Pico
#define SCREEN_ADDRESS 0x3C //Use 3C with Pico for 128x64 OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


int current_input = 10;

void setup()
{
  delay(1500); //delay
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.fillScreen(0); //0 for filling with black dots. 1 for white
  display.display();
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP); //for zero
}


void loop()
{
  main_menu();
  display.display();
  display.fillScreen(0);
  display.clearDisplay();
}


int read_input()
{
  current_input=0;
  for(int i = 1; i<=5;i++)
  {
  if(digitalRead(i)==0)
  current_input = i;
  }
  if(digitalRead(8)==0)
  current_input = 8;
}

int y=0;
int action()
{
  if(current_input==1) //increement
  {
    y+=1;
  }
  if(current_input==2) //decrement
  {
    y=0;
  }
  if(current_input==5) //increment
  {
    y-=1;
  }
  if(y>=64-20)
  {
    y=64-20;
  }
  if(y<=0)
  {
    y=0;
  }
  display.setCursor(100,10);
  display.print(y);
}

int move_basket()
{
  display.fillRect(10,y,5,20,WHITE);
}

int enemy_y=0;

int move_enemy()
{
  enemy_y+=1;
  display.fillRect(120,enemy_y,10,10,WHITE);
  if(enemy_y>=64)
  {
    enemy_y=0;
  }
  if(enemy_y<=0)
  {
    enemy_y=0;
  }
}

int fire_when=3;
int bullet_x = 128-5;
int bullet_y;
int bullet_rate = 5;
int enemy_fire()
{
  if(fire_when==3)
  {
    bullet_x=bullet_x-bullet_rate;
    bullet_rate+=1;
    display.fillRect(bullet_x,0,3,3,WHITE);
    if(y>=0&&y<=20)
    {
      display.setCursor(100,40);
      display.print("HIT");
    }
    else if(y>20)
    {
    display.setCursor(100,40);
    display.print("NO");
    }
  }
  if(bullet_x<=5)
  {
    bullet_x = 128-5;
    bullet_rate=0;
  }
}
int score = 0;
int printscore()
{
  display.setCursor(100,50);
  display.print(score);
}
int main_menu()
{
  display.setCursor(80,0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("INP:");
  display.setCursor(100,0);
  display.print(current_input);
  display.setCursor(100,30);
  display.print(bullet_x);
  read_input();
  action();
  move_basket();
  move_enemy();
  enemy_fire();
  printscore();
  display.display();
  return 0;
}
