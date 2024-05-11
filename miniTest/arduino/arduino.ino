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
  (Thruster(26, "frontLeft")),
  (Thruster(23, "frontRight")),
  (Thruster(28, "middleLeft")),
  (Thruster(27, "middleRight")),
  (Thruster(25, "backLeft")),
  (Thruster(24, "backRight"))
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
  int DELAY = 1500;
  for (int j = 0; j<6; j++){
    Serial.println(j);
    for (int i = 50; i < 55; i++) {
      Serial.println(i);
      Serial.println("POSITIVE");
      thrusters[j].setSpeedRaw(1500 + i);
      delay(DELAY);
      Serial.println("STOP");
      thrusters[j].setSpeedRaw(1500);
      delay(DELAY);
      Serial.println("");
      /*
      Serial.println("NEGATIVE");
      thrusters[j].setSpeedRaw(1500 - i); 
      delay(DELAY); 
      Serial.println("STOP");
      thrusters[j].setSpeedRaw(1500);
      delay(DELAY); 
      Serial.println("");
      */
    }
  }
}