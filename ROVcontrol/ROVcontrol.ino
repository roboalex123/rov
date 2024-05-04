#include <ArduinoJson.h> //Load Json Library
#include <Servo.h>

byte camPin = 22;
Servo camServo;
byte frontLeftPin = 28;
Servo frontLeftMotor;
byte frontRightPin = 23;
Servo frontRightMotor;
byte midLeftPin = 25;
Servo midLeftMotor;
byte midRightPin = 26;
Servo midRightMotor;
byte backLeftPin = 27;
Servo backLeftMotor;
byte backRightPin = 24;
Servo backRightMotor;
int signal = 1500;

void setup() {
  camServo.attach(camPin);
  camServo.writeMicroseconds(signal);
  frontLeftMotor.attach(frontLeftPin);
  frontLeftMotor.writeMicroseconds(midLeftPin);
  frontRightMotor.attach(frontRightPin);
  frontRightMotor.writeMicroseconds(signal);
  midLeftMotor.attach(midLeftPin);
  midLeftMotor.writeMicroseconds(signal);
  midRightMotor.attach(midRightPin);
  midRightMotor.writeMicroseconds(signal);
  backLeftMotor.attach(backLeftPin);
  backLeftMotor.writeMicroseconds(signal);
  backRightMotor.attach(backRightPin);
  backRightMotor.writeMicroseconds(signal);
  delay(5000);
  Serial.begin(9600);
}

//void test(){
 // camServo.writeMicroseconds(1100);
 // delay(300);
 // camServo.writeMicroseconds(1200);
 // delay(300);
 // camServo.writeMicroseconds(1300);
//}

int thrustClamp (int thrust){
  if (thrust > 1900) {
    thrust = 1900;
  } else if (thrust < 1100) {
    thrust = 1100;
  }
  return thrust;
}


int thrust2send (float thrust){
  float normThrust = thrust + 1;
  if (normThrust < 1) {
    return (normThrust)*400+(1100-50);
  } else if (normThrust > 1) {
    return (normThrust)*400+(1100+50);
  } else {
  return (thrust + 1)*400+1100;
  }
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
     
     float camAngle = json_doc["camAngle"];
     float frontLeft = json_doc["frontLeft"];
     float frontRight = json_doc["frontRight"];
     float midLeft = json_doc["midLeft"];
     float midRight = json_doc["midRight"];
     float backLeft = json_doc["backLeft"];
     float backRight = json_doc["backRight"];

     camServo.writeMicroseconds(thrust2send(camAngle));
     frontLeftMotor.writeMicroseconds(thrustClamp(thrust2send(frontLeft)));
     frontRightMotor.writeMicroseconds(thrustClamp(thrust2send(frontRight)));
     midLeftMotor.writeMicroseconds(thrustClamp(thrust2send(midLeft)));
     midRightMotor.writeMicroseconds(thrustClamp(thrust2send(midRight)));
     backLeftMotor.writeMicroseconds(thrustClamp(thrust2send(backLeft)));
     backRightMotor.writeMicroseconds(thrustClamp(thrust2send(backRight)));

     delay(10);

  }

 }
