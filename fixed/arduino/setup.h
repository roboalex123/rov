#pragma once
#include <ArduinoSTL.h>
#include <vector>
#include <Servo.h>
#include "./thruster.h"
#include "./rovServo.h"

std::vector<Thruster> thrusters;
thrusters.push_back(Thruster(23, "frontLeft"));
thrusters.push_back(Thruster(24, "frontRight"));
thrusters.push_back(Thruster(25, "middleLeft"));
thrusters.push_back(Thruster(26, "middleRight"));
thrusters.push_back(Thruster(27, "backLeft"));
thrusters.push_back(Thruster(28, "backRight"));

std::vector<rovServo> servos;
servos.push_back(rovServo(22, "camera"));
