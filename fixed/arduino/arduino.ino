#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <Servo.h>
#include "thruster.h"
#include "rovServo.h"
#include "test.h"
using namespace std;

const int NUM_NORMAL_SERVOS = 1;
rovServo normalServos[NUM_NORMAL_SERVOS] = {
  // (pinNumber, name)

  // camera servos
  (rovServo(22, "frontCamera"))
};

const int NUM_THRUSTERS = 8;
Thruster thrusters[NUM_THRUSTERS] = {
  // (pinNumber, name)


  // xy_plane thrusters
  (Thruster(28, "frontLeft")),
  (Thruster(23, "frontRight")),
  (Thruster(27, "backLeft")),
  (Thruster(24, "backRight")),

  // z_axis thrusters
  (Thruster(25, "midLeft")),
  (Thruster(26, "midRight")),

  // claw thrusters
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
