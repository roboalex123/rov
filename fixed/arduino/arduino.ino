#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <Servo.h>
#include "thruster.h"
#include "rovServo.h"
#include "test.h"
using namespace std;

const int NUM_NORMAL_SERVOS = 1;
rovServo normalServos[NUM_NORMAL_SERVOS] = {rovServo(22, "frontCamera")};

const int NUM_THRUSTERS = 6;
Thruster thrusters[NUM_THRUSTERS] = {
  (Thruster(28, "frontLeft")),
  (Thruster(23, "frontRight")),
  (Thruster(25, "midLeft")),
  (Thruster(26, "midRight")),
  (Thruster(27, "backLeft")),
  (Thruster(24, "backRight")),
  (Thruster(53, "openClaw")),
  (Thruster(51, "spinClaw"))
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

  if (Serial.available())
  {
    data = Serial.readStringUntil('\x7D');
    // Serial.println(data);
    StaticJsonDocument<500> readDoc;
    deserializeJson(readDoc, data);

    for (int i = 0; i < NUM_THRUSTERS; i++)
    {
      thrusters[i].setSpeed(readDoc[thrusters[i].getName()]);
    }

    for (int i = 0; i < NUM_NORMAL_SERVOS; i++)
    {
      normalServos[i].setAngle(readDoc[normalServos[i].getName()]);
    }
  }

}