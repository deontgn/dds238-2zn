#include <ModbusMaster.h>
#include <SoftwareSerial.h>
#include <avr/wdt.h>

#define MAX485_DE      8 //ToModBus
#define MAX485_RE_NEG  9 //ToModBus


ModbusMaster node;
SoftwareSerial mySerial(10, 11); // RX, TX - ESP8266 Connection

void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

void setup()
{
  Serial.begin(9600);
  Serial.print("Setup");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
  node.begin(1, Serial);
  // Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  mySerial.begin(4800);

}

bool state = true;

double e=0;
double v=0;
double f=0;
double pf=0;
double ap=0;
double c=0;

 char buff[20];         
char *pointer_to_created_string;

void loop()
{
  uint8_t result;
  Serial.println("readHoldingRegisters"); 
  result = node.readHoldingRegisters(0x0000, 18);
  if (result == node.ku8MBSuccess)
  {  
    digitalWrite(LED_BUILTIN, HIGH);
    
    
     e=(node.getResponseBuffer(0x00)*65535+node.getResponseBuffer(0x01))*10.0f;
     v=node.getResponseBuffer(0x0C)/10.0f;
     f=node.getResponseBuffer(0x11)/100.0f;
     pf=node.getResponseBuffer(0x10)/1000.0f;
     ap = node.getResponseBuffer(0x0E);
     c = node.getResponseBuffer(0x0D)/100.0f;
    
    Serial.println();  
    Serial.print("Energ: ");     
    Serial.println(e);         
    Serial.print("Voltage: ");   
    Serial.print(v);   
    Serial.println(" V");
    Serial.print("Frequency:");    
    Serial.print(f); 
    Serial.println(" Hz");
    Serial.print("Power factor:");    
    Serial.println(pf);
    Serial.print("Active power: ");    
    Serial.print(ap);
    Serial.println(" KW");
    Serial.print("Current: ");
    Serial.print(c);
    Serial.println(" A");
    

  mySerial.write((const char *)&v, sizeof(float));
 
  mySerial.write((const char *)&c, sizeof(float));
 
  mySerial.write((const char *)&ap, sizeof(float));
 
  mySerial.write((const char *)&e, sizeof(float));
 
    delay(5000);
    digitalWrite(LED_BUILTIN, LOW);
  
  }
    
    
    delay(10000);

  }
  
 
  






