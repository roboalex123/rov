#pragma once
#include <Stepper.h>

class Test
{
private:
  Stepper stepper;
  int steps;
  float currentSpeed;

  String name;
  float mappedSpeed;

public:
  Test(int steps, int stepPin, int dirPin, String name) : stepper(steps,stepPin,dirPin)
  {
    this->name = name;
    this->steps =steps;
    currentSpeed = 0;
  }

  void setFast(int speed)
  {
    // Map speed ratio to the range of speeds for your stepper motor
    stepper.setSpeed(speed);
  }


  void run(bool direction = false)
  {
    int clicks = 1000;
    if(direction){
      clicks *= -1;
    }
    stepper.step(clicks);
  }

  float getCurrentSpeed()
  {
    return mappedSpeed;
  }

  String getName()
  {
    return name;
  }
};