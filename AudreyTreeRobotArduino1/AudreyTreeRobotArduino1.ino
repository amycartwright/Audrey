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

enum State{
  READ_SCENE = 0,
  READ_X,
  READ_Y,
  READ_Z,
  READ_DONE
  }state = READ_DONE;


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

  while (Serial.available()){ 
    int current = Serial.read();
    if(current == 'S' && state == READ_DONE) {
      state = READ_SCENE;
      messageComplete = false;
      break;
    }
 
    if (current == 'x'){
      state = READ_X;
    }else if (current == 'y'){
      state = READ_Y;
    }else if (current == 'z'){
      state = READ_Z;
    }else if(current == 'E'){
      state = READ_DONE;
      isStill = false;
      messageComplete = true;
    }
    if(state == READ_SCENE){
      scene = current;
      
    }else
    if(state == READ_X){
      motor[0] = Serial.parseInt();
      Serial.print(motor[0]);
      Serial.print(", ");
    }else
    if(state == READ_Y){
      motor[1] = Serial.parseInt();
      Serial.print(motor[1]);
      Serial.print(", ");
    }else
    if(state == READ_Z){
      motor[2] = Serial.parseInt();
      Serial.print(motor[2]);
      Serial.println("");
    }
  }
  

  if (isStill == false && messageComplete) {
    messageComplete = false;
    if (scene == 'a') {
        stepper1.moveTo(motor[0]);
        stepper2.moveTo(motor[1]);
        stepper3.moveTo(motor[2]);
    }else if (scene == 'r') {
      stepper1.moveTo(-stepper1.currentPosition());   
      stepper2.moveTo(-stepper2.currentPosition());   
      stepper3.moveTo(-stepper3.currentPosition());   
      
   }

  
  }
  stepper1.run();
    stepper2.run();
    stepper3.run();
}

