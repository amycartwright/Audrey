

#include <AccelStepper.h>

// Define three steppers and the pins they will use
AccelStepper stepper1(AccelStepper::DRIVER, 9, 8); //We use 'DRIVER' here as we are using a driver that requires two pins 
AccelStepper stepper2(AccelStepper::DRIVER, 7, 6);
AccelStepper stepper3(AccelStepper::DRIVER, 5, 4);

int pos1;
int pos2;
int pos3;
byte message;


void setup() { 
   
Serial.begin(115200);
  stepper1.setMaxSpeed(3000);
  stepper1.setAcceleration(500);
  stepper2.setMaxSpeed(3000);
  stepper2.setAcceleration(500);
  stepper3.setMaxSpeed(3000);
  stepper3.setAcceleration(500);

    pos1 = 3600;
    pos2 = 3600;
    pos3 = 3600;
}

void loop() {

  if(Serial.available() > 0){
    message = Serial.read();    
  }
  
  if (stepper1.distanceToGo() == 0)
  {
     pos1 = -pos1; 
     
    stepper1.moveTo(pos1);
  }
  
  if (stepper2.distanceToGo() == 0)
  {
    pos2 = -pos2;
    stepper2.moveTo(pos2);
  }
  
   if (stepper3.distanceToGo() == 0)
  {
    pos3 = -pos3;
    stepper3.moveTo(pos3);
  }
  
  stepper1.run();
  stepper2.run();
  stepper3.run();
  
}

