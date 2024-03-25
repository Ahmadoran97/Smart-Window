
#define CUSTOM_SETTINGS
#define INCLUDE_TERMINAL_MODULE
#include <Dabble.h>
#include <Servo.h>
#include <SoftwareSerial.h>

//SoftwareSerial mySerial(2, 3); // RX, TX

Servo servo1;
Servo servo2;
int motor1=9;
int motor2=10;

int measurePin = A5;
int ledPower = 12;

unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

String Bdata = "";

void setup() 
{
Serial.begin(9600);
Dabble.begin(9600);

servo1.attach(5); //Window
servo2.attach(6); //Kink
pinMode(motor1,OUTPUT); 
pinMode(motor2,OUTPUT);

servo1.write(90);
servo2.write(90);

pinMode(ledPower,OUTPUT);

}

void loop() 
{
  Dabble.processInput();             
  
  if(Terminal.available()!=0)
  {
    Bdata = Terminal.readString();
    Serial.println(Bdata);
  }

 
//LM35
 int Temp = analogRead(A2);
 Temp = (Temp*5.0/1023)*100;
 //Serial.print(Temp);

//LDR 
 int LDR = analogRead(A3);
 //Serial.println(LDR);

//Dut sensor
 digitalWrite(ledPower,LOW);
 delayMicroseconds(samplingTime);
 voMeasured = analogRead(measurePin);
 delayMicroseconds(deltaTime);
 digitalWrite(ledPower,HIGH);
 delayMicroseconds(sleepTime);
 calcVoltage = voMeasured*(5.0/1024);
 dustDensity = 0.17*calcVoltage-0.1;
 if ( dustDensity < 0)
  {
    dustDensity = 0.00;
  }
   //Serial.println(dustDensity);

  
 //open Curtain
 if (LDR <= 35 || Bdata== "Con")
 {
   digitalWrite(9,HIGH);
   digitalWrite(10,LOW);
   delay(3000);
   digitalWrite(9,LOW);
   digitalWrite(10,LOW);
   delay(1000);

 }

 //close Curtain
 if (LDR > 35 || Bdata== "Coff")
 {
   digitalWrite(9,LOW);
   digitalWrite(10,HIGH);
   delay(3000);
   digitalWrite(9,LOW);
   digitalWrite(10,LOW);
   delay(1000);
 }

 //open window
 if (Temp > 35 || Bdata== "Won")
 {
   servo1.write(180);
 }
 
 //close window
 if (Temp <= 35 || Bdata== "Woff")
 {
   servo1.write(90);
 }

 //open kink
 if ( dustDensity < 0.3 || Bdata== "Kon")
  {
   servo2.write(180);
  }

 //close kink
 if ( dustDensity >= 0.3 || Bdata== "Koff")
  {
   servo2.write(90);
  }

}  
