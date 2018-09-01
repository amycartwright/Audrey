#include <AccelStepper.h>

// Define three steppers and the pins they will use
AccelStepper stepper1(AccelStepper::DRIVER, 9, 8); //We use 'DRIVER' here as we are using a driver that requires two pins
AccelStepper stepper2(AccelStepper::DRIVER, 7, 6);
AccelStepper stepper3(AccelStepper::DRIVER, 5, 4);

byte motor[4];
bool isStill = true;

void setup() {

  Serial.begin(9600);

  stepper1.setMaxSpeed(3000);
  stepper1.setAcceleration(500);
  stepper2.setMaxSpeed(3000);
  stepper2.setAcceleration(500);
  stepper3.setMaxSpeed(3000);
  stepper3.setAcceleration(500);
}

void loop() {
  if (Serial.available() > 4) {
    isStill = false;
    motor[0] = Serial.read();
    motor[1] = Serial.read();
    motor[2] = Serial.read();
    motor[3] = Serial.read();

    Serial.write(motor[0]);
    Serial.write(motor[1]);
    Serial.write(motor[2]);
    Serial.write(motor[3]);

  }

  if (isStill == false) {
    if (motor[0] == 'a') {
      if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0 && stepper3.distanceToGo() == 0) {
        stepper1.moveTo(motor[1] * 10);
        stepper2.moveTo(motor[2] * 10);
        stepper3.moveTo(motor[3] * 10);
        //    delay(1000);
      }
    }

    stepper1.run();
    stepper2.run();
    stepper3.run();
  }

  if (!Serial.available()) {
    isStill = true;
  }
}

