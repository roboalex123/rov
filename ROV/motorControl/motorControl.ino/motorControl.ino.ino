#include <ArduinoJson.h> //Load Json Library
#include <Servo.h>

byte camPin = 22;
Servo camServo;
byte flPin = 23;
Servo flMotor;
byte frPin = 24;
Servo frMotor;
byte mlPin = 25;
Servo mlMotor;
byte mrPin = 26;
Servo mrMotor;
byte blPin = 27;
Servo blMotor;
byte brPin = 28;
Servo brMotor;
int signal = 1500;

void setup() {
  servo.attach(servoPin);
  servo.writeMicroseconds(signal);
  delay(5000);
  Serial.begin(9600);
}

void test(){
  servo.writeMicroseconds(1100);
  delay(300);
  servo.writeMicroseconds(1200);
  delay(300);
  servo.writeMicroseconds(1300);
}

void loop() {
 String thruster;
  while (!Serial.available()){ 
   //Serial.print("No data");
   digitalWrite(13,HIGH);
   delay(100);
   digitalWrite(13,LOW);
   delay(100);
  }
  if(Serial.available()) {
      thruster=Serial.readStringUntil( '\x7D' );//Read data from Arduino until};
  
    StaticJsonDocument<1000> json_doc; //the StaticJsonDocument we write to
    deserializeJson(json_doc,thruster);
     
     float thrust = json_doc["thrust"];
     int thrust2send = (thrust+1)*400+1100;

     servo.writeMicroseconds(thrust2send);

     delay(10);
  }
 }