///////////////////////////////////////////////////////  Dancers In White, Standing - Performance  ////////////////////////////////////////////
//////////////////////////////////////////////////////                 Amy Cartwright             ///////////////////////////////////////////

/*Code to accompany the main program in OpenFrameworks. This code is the other half of a serial connection that powers the servo motors used to control three robotic arms through an arduino.
  This is the code used for the 5min performance, the data is collected through color trackng in openFrameworks*/

#include <Servo.h>
//Identify each servo as an instance of the servo object
//1st Arm
Servo armBase;
Servo armShoulder;
Servo armElbow;
//2nd Arm
Servo arm1Base;
Servo arm1Shoulder;
Servo arm1Elbow;
//3rd Arm
Servo arm2Base;
Servo arm2Shoulder;
Servo arm2Elbow;

byte xyz[4]; //Array to recieve the serial data - a message followed by x, y, z coordinates
bool isStill = true;
float ratio = 180.0 / 255.0; //used to map the values recieved thrugh serial to the values reachable by the servos

void setup() {
  //attach each servo to a pin on the arduino
  armBase.attach(13);
  armShoulder.attach(12);
  armElbow.attach(11);

  arm1Base.attach(9);
  arm1Shoulder.attach(8);
  arm1Elbow.attach(7);

  arm2Base.attach(5);
  arm2Shoulder.attach(4);
  arm2Elbow.attach(3);

  Serial.begin(9600); // start the communication between the oF app and the Arduino
}

void loop() {
  //if the program is recieving serial messages then isStill is false and we assign the data to the array of bytes
  if (Serial.available() > 2) {
    isStill = false;
    xyz[0] = Serial.read();
    xyz[1] = Serial.read();
    xyz[2] = Serial.read();
    xyz[3] = Serial.read();
  }

  //If bool isStill is false and the first byte recieved is 'a' the arms should move in sync with each other in mirror image to the dancer
  if (isStill == false) {
    if (xyz[0] == 'a') {
      //base motion
      armBase.write(180 - (xyz[1] * ratio)); //-180 flips the data to get the opposing motion allows it to be a mirror image rather than in sync
      arm1Base.write(180 - (xyz[1] * ratio)); //shoulder motor is mounted in reverse and so does not require the mirror image
      arm2Base.write(180 - (xyz[1] * ratio));

      //shoulder motion
//      armShoulder.write(xyz[3] * ratio);
//      arm1Shoulder.write(xyz[3] * ratio);
//      arm2Shoulder.write(xyz[3] * ratio);

      //elbow motion
      armElbow.write(180 - (xyz[2] * ratio));
      arm1Elbow.write(180 - (xyz[2] * ratio));
      arm2Elbow.write(180 - (xyz[2] * ratio));

      delay(20); // make sure we give them a second to move
    }

    //If bool isStill is false and the first byte recieved is 'b' move in cannon (one after the other)
    if (xyz[0] == 'b') {
      armBase.write(180 - (xyz[1] * ratio));
      //armShoulder.write(xyz[3] * ratio);
      armElbow.write(180 - (xyz[2] * ratio));

      delay(1000); //The delay here creates the cannon effect

      arm1Base.write(180 - (xyz[1] * ratio));
     // arm1Shoulder.write(xyz[3] * ratio);
      arm1Elbow.write(180 - (xyz[2] * ratio));

      delay(1000);

      arm2Base.write(180 - (xyz[1] * ratio));
     // arm2Shoulder.write(xyz[3] * ratio);
      arm2Elbow.write(180 - (xyz[2] * ratio));

      delay(1000);
    }

    //If bool isStill is false and the first byte recieved is 'c', only one arm moves
    if (xyz[0] == 'c') {
      armBase.write(180 - (xyz[1] * ratio));
     // armShoulder.write(xyz[3] * ratio);
      armElbow.write(180 - (xyz[2] * ratio));
    }

    //If bool isStill is false and the first byte recieved is 'c', only two arms move this time not in mirror image
    if (xyz[0] == 'd') {
      arm1Base.write((xyz[1] * ratio));
      arm2Base.write((xyz[1] * ratio));

//      arm1Shoulder.write(180 - (xyz[3] * ratio));
//      arm2Shoulder.write(180 - (xyz[3] * ratio));

      arm1Elbow.write(180 - (xyz[2] * ratio));
      arm2Elbow.write(180 - (xyz[2] * ratio));

      delay(20);
    }
  }

  //if there is no serial data being recieved bool isStill is true.
  if (!Serial.available()) {
    isStill == true;
  }

  //If bool isStill is true motors should set to these positions
  if (isStill == true) {
    armBase.write(90);
    //armShoulder.write(90);
    armElbow.write(180);

    arm1Base.write(50);
   // arm1Shoulder.write(90);
    arm1Elbow.write(100);

    arm2Base.write(150);
   // arm2Shoulder.write(90);
    arm2Elbow.write(120);
  }
}
