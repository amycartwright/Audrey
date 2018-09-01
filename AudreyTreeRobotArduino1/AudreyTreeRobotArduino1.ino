#include <AccelStepper.h>

// Define three steppers and the pins they will use
AccelStepper stepper1(AccelStepper::DRIVER, 9, 8); //We use 'DRIVER' here as we are using a driver that requires two pins
AccelStepper stepper2(AccelStepper::DRIVER, 7, 6);
AccelStepper stepper3(AccelStepper::DRIVER, 5, 4);

char scene;
bool messageComplete = false;
int motor[3];
bool isStill = true;
char ignoreX, ignoreY, ignoreZ;
unsigned long wait;

void setup() {

  Serial.begin(57600);

  stepper1.setMaxSpeed(3000);
  stepper1.setAcceleration(500);
  stepper2.setMaxSpeed(3000);
  stepper2.setAcceleration(500);
  stepper3.setMaxSpeed(3000);
  stepper3.setAcceleration(500);

  wait =  millis();
}

void loop() {

//    if (millis() > wait){
//      Serial.write('F');
//      wait += 5000;
//    }

  if (Serial.available() && Serial.read() == 'S') {
    isStill = false;
    scene = Serial.read();
    if (Serial.read() == 'x') motor[0] = Serial.parseInt();
    if (Serial.read() == 'y') motor[1] = Serial.parseInt();
    if (Serial.read() == 'z') motor[2] = Serial.parseInt();

    if (messageComplete = (Serial.read() == 'E')) Serial.write('F');
        Serial.write(motor[0]);
        Serial.write(motor[1]);
        Serial.write(motor[2]);
  }

  if (isStill == false && messageComplete) {
    messageComplete = false;
    if (scene == 'a') {
      if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0 && stepper3.distanceToGo() == 0) {
        stepper1.moveTo(motor[0]);
        stepper2.moveTo(motor[1]);
        stepper3.moveTo(motor[2]);
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

