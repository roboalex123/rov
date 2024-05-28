#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <Servo.h>
#include "thruster.h"
using namespace std;

Thruster thruster(28, "test"); // front left

void setup() {
  Serial.begin(9600);
  thruster.init();

  delay(7000);
}

void loop() {
    String data;

    if (Serial.available()) {
        data = Serial.readStringUntil('\x7D');

        StaticJsonDocument<200> doc;
        deserializeJson(doc, data);

        thruster.setSpeed(doc[thruster.getName()]);
    }
}