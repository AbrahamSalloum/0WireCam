 #include <SPI.h>
#include <Ethernet.h>
#include <Adafruit_VC0706.h>
#include <SD.h>
#include <SoftwareSerial.h> 
#define chipSelect 4
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
Adafruit_VC0706 cam = Adafruit_VC0706(&Serial2);
IPAddress ip(10,1,1,111);
IPAddress gateway(10,1,1,1); 
IPAddress subnet(255, 255, 255, 0);


EthernetServer server(8181);
EthernetServer server1(9090);

char switch0;
int statsw = 4;
int val = 0; 


void setup() {
  
Ethernet.begin(mac,ip, gateway, subnet);
server.begin(); 
server1.begin();

#if (!defined(SOFTWARE_SPI))
if(chipSelect != 53) pinMode(53, OUTPUT); // SS on Mega
#endif

 if (!cam.begin()) {
 Serial.println("No camera found?");
 return;
 }
  
   cam.setImageSize(VC0706_640x480); 
  
  Serial.begin(9600);
delay(2000); 
pinMode(2, INPUT); 
pinMode(statsw, OUTPUT);  
Serial.println(Ethernet.localIP());
//delay(20000); 

}

void loop(){
  Serial.print("hey"); 
  delay(2000); 
EthernetClient client = server.available();
delay(2000); 
 
 //
 //
 //
 val = digitalRead(2);
 Serial.write(val); 

 if (client) {
 while (client.connected()) {  
    if (client.available()) { 
      switch0 = client.read(); 
     client.flush();
       delay(1000);
    }
    
    
       if(val == HIGH){
                 Serial.write("hi"); 
                 client.print("hi"); 
             
             }
           
           
           if(val == LOW){
                 Serial.write("lo"); 
                 client.print("lo"); 
               
           }   
client.flush();
 Serial.write(switch0);
 Serial.write("\n");
 if(switch0 == '0'){

   digitalWrite(statsw, LOW);    
 }
  else if(switch0 == '1'){
   digitalWrite(statsw, HIGH);     
 }
else if(switch0 == '2'){

 delay(2000); 
  Serial.print("after\n");  
EthernetClient client1 = server1.available();
//delay(2000); 
 if(client1){
 
          char c = client1.read();
          Serial.write("Client1 says: ");
          Serial.write(c);
   
    if (! cam.takePicture()) 
    Serial.println("Failed to snap!");
  else 
    Serial.println("Picture taken!");

  uint16_t jpglen = cam.frameLength();
    
    Serial.print(jpglen);
  client1.print(jpglen);
  
  Serial.print("Storing ");
  Serial.print(jpglen, DEC);
  Serial.print(" byte image.");
  pinMode(8, OUTPUT);

  byte wCount = 0; 
  while (jpglen > 0) {

    uint8_t *buffer;
    uint8_t bytesToRead = min(64, jpglen); 
    buffer = cam.readPicture(bytesToRead);
    client1.write(buffer, bytesToRead);
    if(++wCount >= 64) { 
      Serial.print('.');
      wCount = 0;
    }
    jpglen -= bytesToRead;
  }
cam.reset(); 
   
  Serial.println("done!");
   delay(2000);  
  Serial.print("close\n"); 
client.flush(); 
}

}}}}
