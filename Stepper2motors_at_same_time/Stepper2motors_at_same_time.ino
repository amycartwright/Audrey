#include <AccelStepper.h>

// Define three steppers and the pins they will use
AccelStepper stepper1(AccelStepper::DRIVER, 9, 8); //We use 'DRIVER' here as we are using a driver that requires two pins 
AccelStepper stepper2(AccelStepper::DRIVER, 7, 6);
AccelStepper stepper3(AccelStepper::DRIVER, 5, 4);

  int pos[][3] = {{1000,  0, 0  },
                 {00,  1000, 0  },
                 {0,    0, 1000}};

int count;


void setup() { 
   
Serial.begin(115200);

stepper1.setCurrentPosition(0);
stepper2.setCurrentPosition(0);
stepper2.setCurrentPosition(0);

  stepper1.setMaxSpeed(3000);
  stepper1.setAcceleration(500);
  stepper2.setMaxSpeed(3000);
  stepper2.setAcceleration(500);
  stepper3.setMaxSpeed(3000);
  stepper3.setAcceleration(500);

    count = 0;
}

void loop() {

  
  if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0 && stepper3.distanceToGo() == 0)
  {
    if (++count > 2){
      count = 0;
    }
    
    stepper1.runToNewPosition(pos[count][0]);
    stepper2.moveTo(pos[count][1]);
    stepper3.moveTo(pos[count][2]);

  }
  stepper1.run();
  stepper2.run();
  stepper3.run();
  
}

