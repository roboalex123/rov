#include <ArduinoJson.h>
#include <ArduinoSTL.h> // download from IDE
#include <vector>
#include <Servo.h>
#include "thruster.h"
#include "rovServo.h"
#include "setup.h"
using namespace std;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < thrusters.size(); i++) {
    thrusters[i].init();
  }

  for (int i = 0; i < servos.size(); i++) {
    servos[i].init();
  }

  delay(1000);
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
    Serial.println(data);
    StaticJsonDocument<1000> doc;
    DeserializationError error = deserializeJson(doc, data);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }

    for (int i = 0; i < thrusters.size(); i++) {
      thrusters[i].setSpeed(doc[thrusters[i].getName()]);
    }

    for (int i = 0; i < servos.size(); i++) {
      servos[i].setAngle(doc[servos[i].getName()]);
    }

    delay(10);
}
