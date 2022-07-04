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
#include <Ethernet.h> //Load Ethernet Library
#include <EthernetUdp.h> //Load UDP Library
#include "Picopixel.h"

//Definitions
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Make sure this is set to -1 for Pico
#define SCREEN_ADDRESS 0x3C //Use 3C with Pico for 128x64 OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Ethernet Declarations
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE}; //Assign a mac address
IPAddress ip(192, 168, 137, 121); //Assign my IP adress
unsigned int localPort = 5000; //Assign a Port to talk over
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
String datReq; //String for our data
int packetSize; //Size of Packet
EthernetUDP Udp; //Define UDP Object
String localip="192.168.2.1"; //sets a temporary local IP address

//initialize the control signal
int current_input = 10;

void setup()
{
  //sets and initialize the sockets
  ethstart();
  delay(1500); //delay

  //sets and initialize the SSD1306 display
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.fillScreen(0); //0 for filling with black dots. 1 for white
  display.display();

  //display the IP address to bind on the screen
  setip();
  display.display();

  //sets a delay allowing the user to copy the address to the python console
  delay(10000);

  //clears the display
  display.fillScreen(0);
  display.clearDisplay();
  display.display();
}


void loop()
{
  //starts the game
  main_menu();

  //clears and updates the display
  display.display();
  display.fillScreen(0);
  display.clearDisplay();
}

//sets the score to zero on start
int score = 0;

//display the game over screen with the score attained
int game_over()
{
  while(1)
  {
    display.fillScreen(0);
    display.clearDisplay();
    display.setCursor(64,32);
    display.print("Game Over");
    display.setCursor(80,55);
    display.print("SCORE:");
    display.setCursor(115,55);
    display.print(score);
    display.display();
  }
}

// read the control signals from the python code
int read_input()
{
  //initialize the packetSize
  packetSize = Udp.parsePacket(); //Read theh packetSize
  if (packetSize > 0) 
  { //Check to see if a request is present

    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); //Reading the data request on the Udp
    String datReq(packetBuffer); //Convert packetBuffer array to string datReq21
    current_input = datReq.toInt(); //converts the received control signal to integer value

    //if the recived data is "STOP" the capture
    if (datReq == "STOP") 
    { //See if Red was requested
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());  //Initialize Packet send
      Udp.print("GAME OVER"); //Send string back to client
      Udp.endPacket(); //Packet has been sent
      //display the end game screen
      game_over();
    }
  }
  memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
  return 0;
}

//to control the player paddle
int y=0;
int y_rate=5;
int action()
{
  if(current_input==5) //increment if 5 is recived
  {
    y+=y_rate;
  }
  if(current_input==0) //decrement 5 if 0 is recived
  {
    y-=y_rate;
  }

  //if player paddle reach max or min y make it stationary
  if(y>=64-20)
  {
    y=64-20;
  }
  if(y<=0)
  {
    y=0;
  }
  return 0;
}

//display the player paddle
int move_basket()
{
  display.fillRect(10,y,5,20,WHITE);
  return 0;
}

//initialize the apple variables.
int fire_when=3;
int bullet_x = 128-5;
int bullet_y = 32;
int bullet_rate = 1;

//fire the apples
int enemy_fire()
{
  if(fire_when==3)
  {
    bullet_x=bullet_x-bullet_rate;
    bullet_rate+=1;
    display.fillRect(bullet_x,bullet_y,3,3,WHITE);
    //collition detection
    if((bullet_y>=y&&bullet_y<=y+20))
    {
      if(bullet_x>=0&&bullet_x<=15)
      {
      display.setCursor(100,40);
      display.print("HIT");
      score+=1;
      }
    }
  }
  //reset apples and start from a random position
  if(bullet_x<=5)
  {
    bullet_x = 128-5;
    bullet_y = random(0,63);
    bullet_rate=1;
  }
  return 0;
}

//print the score to the bottom of the screen
int printscore()
{
  display.setCursor(80,55);
  display.print("SCORE:");
  display.setCursor(115,55);
  display.print(score);
  return 0;
}

//starts the game setting player paddle and apples
int main_menu()
{
  display.setCursor(80,0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  read_input();
  action();
  move_basket();
  enemy_fire();
  printscore();
  display.display();
  return 0;
}

//initialize the Ethernet Socket UDP connection

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

//converts the address to printable string
String DisplayAddress(IPAddress address)
  {
 return String(address[0]) + "." + 
        String(address[1]) + "." + 
        String(address[2]) + "." + 
        String(address[3]);
  }

// display the IP address of the pico board
int setip()
{
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("IP Address is:");
  writeip(DisplayAddress(Ethernet.localIP()));
//  writeip(localip);
  return 0;
}

//display the IP address to the screen
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
