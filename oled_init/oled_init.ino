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

//Definitions
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Make sure this is set to -1 for Pico
#define SCREEN_ADDRESS 0x3C //Use 3C with Pico for 128x64 OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


int writeip(String ipaddress)
{
  display.setCursor(0,10);
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.println(ipaddress);
  display.setCursor(0,20);
  display.setTextSize(1.5);
  display.println("Set this IP to Python");
  display.println("The Screen will exit in 5 Seconds:");
  return 0;
}

int setip()
{
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("IP Address is:");
  String ipaddress = "192.168.137.1";
  writeip(ipaddress);
  return 0;
}
int startcode=1;

int main_menu()
{
  display.setCursor(32,0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("[MAIN MENU]");
  display.setCursor(0,10);
  display.println("[1] Start Game\n[2] View High Score");
  return 0;
}


void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.fillScreen(0); //0 for filling with black dots. 1 for white
  display.display();
  setip();
  display.display();
  delay(5000);
  display.fillScreen(0);
  display.clearDisplay();
  display.display();
  main_menu();
  display.display();
}

void loop()
{
  
}