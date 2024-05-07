#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <Servo.h>
#include "thruster.h"
#include "rovServo.h"
#include "stepper.h"
using namespace std;

const int NUM_NORMAL_SERVOS = 1;
rovServo normalServos[NUM_NORMAL_SERVOS] = {rovServo(22, "camera", 180)};

const int NUM_THRUSTERS = 6;
Thruster thrusters[NUM_THRUSTERS] = {
  (Thruster(28, "frontLeft")),
  (Thruster(23, "frontRight")),
  (Thruster(25, "midLeft")),
  (Thruster(26, "midRight")),
  (Thruster(27, "backLeft")),
  (Thruster(24, "backRight"))
};

const int NUM_STEPPER_MOTORS = 3;
Stepper stepperMotors[NUM_STEPPER_MOTORS] = {
  (Stepper(5, 2, "X")),
  (Stepper(6, 3, "Y")),
  (Stepper(7, 4, "Z"))
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
    StaticJsonDocument<500> readDoc;
    deserializeJson(readDoc, data);
 
    thrusters[0].setSpeed(readDoc[thrusters[0].getName()]);
    thrusters[1].setSpeed(readDoc[thrusters[1].getName()]);
    thrusters[2].setSpeed(readDoc[thrusters[2].getName()]);
    thrusters[3].setSpeed(readDoc[thrusters[3].getName()]);
    thrusters[4].setSpeed(readDoc[thrusters[4].getName()]);
    thrusters[5].setSpeed(readDoc[thrusters[5].getName()]);
    for (int i = 0; i < NUM_THRUSTERS; i++) {
      thrusters[i].setSpeed(readDoc[thrusters[i].getName()]);
    }

    for (int i = 0; i < NUM_NORMAL_SERVOS; i++) {
      normalServos[i].setAngle(readDoc[normalServos[i].getName()]);
    }

    /*for (int i = 0; i < NUM_STEPPER_MOTORS; i++) {
      float left = readDoc[stepperMotors[i].getName()];

      if (left < -0.001) {
        stepperMotors[i].left();
      } else if (left > 0.001) {
        stepperMotors[i].right();
      }
    }*/
  }
}
