#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <Servo.h>
#include "thruster.h"
#include "rovServo.h"
using namespace std;

const int NUM_NORMAL_SERVOS = 1;
rovServo normalServos[NUM_NORMAL_SERVOS] = {rovServo(22, "camera", 180)};

const int NUM_THRUSTERS = 6;
Thruster thrusters[NUM_THRUSTERS] = {
  (Thruster(23, "frontLeft")),
  (Thruster(24, "frontRight")),
  (Thruster(25, "middleLeft")),
  (Thruster(26, "middleRight")),
  (Thruster(27, "backLeft")),
  (Thruster(28, "backRight"))
};


void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_THRUSTERS; i++) {
    thrusters[i].init();
  }

  for (int i = 0; i < NUM_NORMAL_SERVOS; i++) {
    normalServos[i].init();
  }

  delay(7000);
}

void loop() {
  String data;
  int smallDelay = 100;
  while (!Serial.available()) {
    digitalWrite(13, HIGH);
    delay(smallDelay);
    digitalWrite(13, LOW);
    delay(smallDelay);
  }

  if (Serial.available()) {
    data = Serial.readStringUntil( '\x7D' );
   // Serial.println(data);
    StaticJsonDocument<1000> doc;
    deserializeJson(doc, data);
 

    for (int i = 0; i < NUM_THRUSTERS; i++) {
      thrusters[i].setSpeed(doc[thrusters[i].getName()]);
    }

    for (int i = 0; i < NUM_NORMAL_SERVOS; i++) {
      normalServos[i].setAngle(doc[normalServos[i].getName()]);
    }

    delay(10);
  }
}
