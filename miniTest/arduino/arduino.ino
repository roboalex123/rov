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
  int DELAY = 2000;
  thrusters[0].setSpeed(0.5);
  delay(DELAY);
  thrusters[0].setSpeed(0);
  delay(DELAY);
}
