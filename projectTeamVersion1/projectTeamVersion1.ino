#include <SPI.h>         
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 3
//=====================================================================

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//IPAddress ip(192, 168, 137, 20);
byte ip[] = {192, 168, 137, 20};
EthernetServer server(23);
unsigned int localPort = 8888;
IPAddress timeServer(203,158,4,156);
const int NTP_PACKET_SIZE= 48;
byte packetBuffer[ NTP_PACKET_SIZE];
EthernetUDP Udp;

OneWire ourWire(ONE_WIRE_BUS);
DallasTemperature sensors(&ourWire);

LiquidCrystal lcd(9, 8, 7, 6, 5, 4);
int backLight = 13;
unsigned long epoch2 = 0;
String command;
//=====================================================================

void setup() 
{
  setup0();
  setup1();
  setup2();
  lcd.setCursor(0,0);           
  lcd.print("MyArduinoProject");
  lcd.setCursor(0,1);           
  lcd.print("TimeSync@SUT2013");
}
//=====================================================================

void loop()
{
  if( epoch2 == 0)
  {
  loop2();
  }else
  {
  loop2();
  
  loop3();
  loop4();
  delay(100);

  loop3();
  loop4();
  delay(100);

  loop3();
  loop4();
  delay(100);

  loop3();
  loop4();
  delay(100);

  loop3();
  loop4();
  delay(100);
 
  loop3();
  loop4();
  delay(100);

  loop3();
  loop4();
  delay(100);

  loop3();
  loop4();
  delay(100);
 
  loop3();
  loop4();
  delay(100);

  loop3();
  loop4();
  delay(100);
  
  loop3();
  loop4();
  delay(100);
  }
 
  EthernetClient client = server.available();
  if (client) {
    command = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
        if (c != '\r')
          command += c;
        
        
        if (c == '\r') {
          if (command == "time")
            sendTime(client);
          client.stop();
        }
      }
    }
  client.stop();
  }
}
//=====================================================================

void setup0()
{
  pinMode(backLight, OUTPUT);
  digitalWrite(backLight, 0);
  lcd.begin(16,2);
  Serial.begin(9600);
}
//=====================================================================

void setup1()
{
  while (!Serial)
  {
    ;
  }
  Ethernet.begin(mac, ip);
  //Ethernet.begin(mac);
  /*if (Ethernet.begin(mac) == 0)
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    for(;;)
    ;
  }*/
  Serial.println(Ethernet.localIP());
  server.begin();
  Udp.begin(localPort);
}
//=====================================================================

void setup2()
{
  sensors.begin();
}
//=====================================================================

void loop2()
{
  sendNTPpacket(timeServer);
  
  delay(1000);  

  if ( Udp.parsePacket() )
  {  
    Udp.read(packetBuffer,NTP_PACKET_SIZE);

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);  
    unsigned long secsSince1900 = highWord << 16 | lowWord;  
    const unsigned long seventyYears = 2208988800UL;     
    unsigned long epoch = secsSince1900 - seventyYears;
    epoch2 = epoch;

    Serial.print("The UTC time is ");
    Serial.print((epoch  % 86400L) / 3600+7);
    Serial.print(':');
    if ( ((epoch % 3600) / 60) < 10 )
    {
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60);
    Serial.print(':');
    if ( (epoch % 60) < 10 )
    {
      Serial.print('0');
    }
    Serial.println(epoch %60);
  }
  
}
//=====================================================================

unsigned long sendNTPpacket(IPAddress& address)
{
memset(packetBuffer, 0, NTP_PACKET_SIZE); 
packetBuffer[0] = 0b11100011;
packetBuffer[1] = 0;
packetBuffer[2] = 6;
packetBuffer[3] = 0xEC;
packetBuffer[12]  = 49; 
packetBuffer[13]  = 0x4E;
packetBuffer[14]  = 49;
packetBuffer[15]  = 52;

Udp.beginPacket(address, 123);
Udp.write(packetBuffer,NTP_PACKET_SIZE);
Udp.endPacket(); 
}
//=====================================================================

void loop3()
{
  sensors.requestTemperatures();
  Serial.print("Temperatures = ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.println(" C");
  lcd.setCursor(0,1);
  lcd.print("Temp = ");
  lcd.print(sensors.getTempCByIndex(0));
  lcd.println(" C  ");
}
//=====================================================================

void sendTime(EthernetClient client)
{
  client.print("Time is ");
  client.print((epoch2  % 86400L) / 3600+7);
  client.print(':');
  if ( ((epoch2 % 3600) / 60) < 10 )
  {
    client.print('0');
  }
  client.print((epoch2  % 3600) / 60);
  client.print(':');
  if ( (epoch2 % 60) < 10 )
  {
    client.print('0');
  }
  client.println(epoch2 %60);
}

void loop4()
{
  Serial.print("Time is ");
  lcd.setCursor(0,0);
  lcd.print("Time is ");
  Serial.print((epoch2  % 86400L) / 3600+7);
  lcd.print((epoch2  % 86400L) / 3600+7);
  Serial.print(':');
  lcd.print(':');
  if ( ((epoch2 % 3600) / 60) < 10 )
  {
    Serial.print('0');
    lcd.print('0');
  }
  Serial.print((epoch2  % 3600) / 60);
  lcd.print((epoch2  % 3600) / 60);
  Serial.print(':');
  lcd.print(':');
  if ( (epoch2 % 60) < 10 )
  {
    Serial.print('0');
    lcd.print('0');
  }
  Serial.println(epoch2 %60);
  lcd.println(epoch2 %60);
  epoch2++;
}
//=====================================================================

