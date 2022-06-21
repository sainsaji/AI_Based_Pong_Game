#include <Ethernet.h> //Load Ethernet Library
#include <EthernetUdp.h> //Load UDP Library
#include <SPI.h> //Load the SPI Library

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE}; //Assign a mac address
IPAddress ip(192, 168, 1, 14); //Assign my IP adress
unsigned int localPort = 5000; //Assign a Port to talk over
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
String datReq; //String for our data
int packetSize; //Size of Packet
EthernetUDP Udp; //Define UDP Object

void setup() {
  
Serial.begin(9600); //Turn on Serial Port
Ethernet.init(17);
Ethernet.begin(mac, ip); //Initialize Ethernet
Udp.begin(localPort); //Initialize Udp
while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // no point in carrying on, so do nothing forevermore:
    while (true) {
      delay(1);
    }
  }
  // print your local IP address:
  Serial.print("Type this IP to Python: ");
  Serial.println(Ethernet.localIP());
delay(1500); //delay
}
int redval,blueval=0;
void loop() {
  
  packetSize = Udp.parsePacket(); //Read theh packetSize
  
  if(packetSize>0){ //Check to see if a request is present
  
  Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); //Reading the data request on the Udp
  String datReq(packetBuffer); //Convert packetBuffer array to string datReq
  
  if (datReq =="Red") { //See if Red was requested
    Serial.println("Red Value:");
    Serial.println(redval);
    redval+=1;
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());  //Initialize Packet send
    Udp.print("You are Asking for Red"); //Send string back to client 
    Udp.endPacket(); //Packet has been sent
  }
   if (datReq =="Green") { //See if Green was requested
  
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());  //Initialize Packet send
    Udp.print("You are Asking for Green"); //Send string back to client 
    Udp.endPacket(); //Packet has been sent
   }
    if (datReq =="Blue") {
      blueval+=1;//See if Red was requested
      Serial.println("Blue Value:");
    Serial.println(blueval);
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());  //Initialize Packet send
    Udp.print("You are Asking for Blue"); //Send string back to client 
    Udp.endPacket(); //Packet has been sent
    }
  }
  memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
}
