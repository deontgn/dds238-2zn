#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

const char* ssid     = "";
const char* password = "";

const char* host = "home";
const int httpPort = 8085;

SoftwareSerial mySerial(D5, D6); // RX, TX
void setup() {
  Serial.begin(9600);
   
  delay(10);
  mySerial.begin(4800);
  
 /* Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);*/
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

 /* Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());*/
}



float CurrentTotalEnergy=0;
float Voltage=0;
float ActivePower=0;
float Current=0;

int Idx[4] = {99, 100, 103, 104  }; // Voltage, Current, Active power, Current total energy


float incoming_value;
unsigned char buffer[4];
int i=0;

WiFiClient client;

void loop() {


if(i>3) i=0;

if (mySerial.available()) {
  

  
  if (mySerial.readBytes(buffer, sizeof(float)) == sizeof(float)) memcpy(&incoming_value, buffer, sizeof(float)); else incoming_value = 0;

    switch (i) {
      case(0):
        Voltage = incoming_value;
      break;
      case(1):
        Current = incoming_value;
      break;
      case(2):
        ActivePower = incoming_value;
      break;
      case(3):
        CurrentTotalEnergy = incoming_value;        
      break;
    }

String url="/json.htm?type=command&param=udevice&idx=";
    url += Idx[i];
        url += "&nvalue=0&svalue=";
if(i<3){

    url += incoming_value; 
    
    
   
  }
  else
  {   
    url += ActivePower;
    url += ';';
    url += CurrentTotalEnergy;  
  }
  
  if (client.connect(host, httpPort)) client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" +  "Connection: close\r\n\r\n");
  
  Serial.println(url);
  if(i==3) i=0; else i++;
  

}
}



