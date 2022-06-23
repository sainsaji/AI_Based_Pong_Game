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
#include <Ethernet.h> //Load Ethernet Library
#include <EthernetUdp.h> //Load UDP Library
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "fonts/Picopixel.h"

//Definitions
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Make sure this is set to -1 for Pico
#define SCREEN_ADDRESS 0x3C //Use 3C with Pico for 128x64 OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Ethernet declarations
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE}; //Assign a mac address
IPAddress ip(192, 168, 137, 121); //Assign my IP adress
unsigned int localPort = 5000; //Assign a Port to talk over
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
String datReq; //String for our data
int packetSize; //Size of Packet
EthernetUDP Udp; //Define UDP Object
String localip="192.168.2.1";
int incoming;
void setup()
{
  ethstart();
  delay(1500); //delay
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.fillScreen(0); //0 for filling with black dots. 1 for white
  display.display();
  setip();
  display.display();
  delay(5000);
  display.fillScreen(0);
  display.clearDisplay();
  display.display();
  
}
int gamestatus=1;
void loop()
{
  main_menu();
  display.display();
  packetSize = Udp.parsePacket(); //Read theh packetSize
  if (packetSize > 0) 
  { //Check to see if a request is present

    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); //Reading the data request on the Udp
    String datReq(packetBuffer); //Convert packetBuffer array to string datReq
    incoming = datReq.toInt();
    if (datReq == "Start") 
    { //See if Red was requested
      gamestatus = 1; //to start the game
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());  //Initialize Packet send
      Udp.print("You are Asking to start the game"); //Send string back to client
      Udp.endPacket(); //Packet has been sent
    }
    if (datReq == "HS") 
    { //See if Red was requested
      gamestatus = 2;
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());  //Initialize Packet send
      Udp.print("You are Asking for the high score"); //Send string back to client
      Udp.endPacket(); //Packet has been sent
    }
  }
  memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
  display.fillScreen(0);
  display.clearDisplay();
}
int ethstart()
{
  Serial.begin(9600); //Turn on Serial Port
  Ethernet.init(17);
  Ethernet.begin(mac, ip); //Initialize Ethernet
  Udp.begin(localPort); //Initialize Udp
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
    {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF)
    {
      Serial.println("Ethernet cable is not connected.");
    }
    // no point in carrying on, so do nothing forevermore:
    while (true)
    {
      delay(1);
    }
  }
  // print your local IP address:
  Serial.print("Type this IP to Python: ");
  Serial.println(Ethernet.localIP());
  localip = String(Ethernet.localIP());
  return 0;
}
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
String DisplayAddress(IPAddress address)
  {
 return String(address[0]) + "." + 
        String(address[1]) + "." + 
        String(address[2]) + "." + 
        String(address[3]);
  }
int setip()
{
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("IP Address is:");
  writeip(DisplayAddress(Ethernet.localIP()));
  writeip(localip);

  return 0;
}

int startgame()
{
  display.fillScreen(0);
  display.clearDisplay();
  display.setCursor(100,40);
  display.print(incoming);
  display.drawRect(40, incoming, 5, 10, WHITE);
  display.display();
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
  display.setTextSize(2);
  display.setFont(&Picopixel);
  display.println("[1] Start Game\n[2] View High Score");
  display.setCursor(0,60);
  display.setTextSize(1);
  display.setFont(NULL);
  display.println(DisplayAddress(Ethernet.localIP()));
  display.setCursor(0,40);
  display.println("[Selected]:");
  display.setCursor(80,40);
  if(gamestatus==3)
  display.print("None");
  else
  {
  display.print(gamestatus);
  if(gamestatus==1)
  startgame();
  }
  display.setCursor(100,40);
  display.print(incoming);
  display.display();
  return 0;
}
