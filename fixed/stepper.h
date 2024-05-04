class Stepper {
  private:
    String name;

    byte directionPin;
    byte stepPin;

    static const int stepDelay = 20;

  public:
    stepper(byte newDirectionPin, byte newStepPin, String newName): directionPin(newDirectionPin), stepPin(newStepPin), name(newName) {
      pinMode(directionPin, OUTPUT);
      pinMode(stepPin, OUTPUT);
    }

    String getName() const {
      return name;
    }

    void left() const {
      digitalWrite(directionPin, HIGH);
      digitalWrite(stepPin, HIGH);
      delay(stepDelay);
      digitalWrite(stepPin, LOW);
    }

    void right() const {
      digitalWrite(directionPin, LOW);
      digitalWrite(stepPin, HIGH);
      delay(stepDelay);
      digitalWrite(stepPin, LOW);
    }
};
