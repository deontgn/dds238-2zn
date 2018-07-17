#include <ModbusMaster.h>

#define MAX485_DE      6
#define MAX485_RE_NEG  5

// instantiate ModbusMaster object
ModbusMaster node;

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
 // Serial.print("Setup");
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
  node.begin(1, Serial);
  // Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

bool state = true;

void loop()
{
  uint8_t result;

  result = node.readHoldingRegisters(0x0000, 18);
  if (result == node.ku8MBSuccess)
  {  
    Serial.println();  

    Serial.print("Energ: ");       
    Serial.println((node.getResponseBuffer(0x00)*65535+node.getResponseBuffer(0x01))/100.0f); 
        
    Serial.print("Voltage: ");
    Serial.print(node.getResponseBuffer(0x0C)/10.0f);   
    Serial.println(" V");

    Serial.print("Frequency:");
    Serial.print(node.getResponseBuffer(0x11)/100.0f); 
    Serial.println(" Hz");

    Serial.print("Power factor:");
    Serial.println(node.getResponseBuffer(0x10)/1000.0f);

    Serial.print("Active power: ");
    Serial.print(node.getResponseBuffer(0x0E)/1000.0f);
    Serial.println(" KW");

    Serial.print("Current: ");
    Serial.print(node.getResponseBuffer(0x0D)/100.0f);
    Serial.println(" A");
  
  }

  delay(10000);

}

