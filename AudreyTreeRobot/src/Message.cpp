//////////////////////              Amy Cartwright             ///////////////////////

//  Determines what specific data is being sent to the Arduino

#include "Message.hpp"
Message::Message(){
    
}
//--------------------------------------------------------------
void Message::setup(){
    //setup for the serial connection to the arduino IDE.
    serial.setup(1, 9600);
    serial.listDevices();
}
//--------------------------------------------------------------
void Message::sceneOne(ofVec3f avg){
    //reduce the framerate when sending message to ensure the arduino is not overwhelmed
    //Sends message indicating specific action to arduino as well as movement data.
    if(ofGetFrameNum() % 20 == 0){
        cout << 'a' << avg << endl;
        serial.writeByte(char('c'));
        serial.writeByte(avg.x / 640 * 255);
        serial.writeByte(avg.y / 480 * 255);
        serial.writeByte(avg.z / 255 * 255);
    }
}
//--------------------------------------------------------------
void Message::sceneTwo(ofVec3f storedData){
    //Sends movement data that has been stored in a buffer. Allows the arms to move when the dancer is not
    if(ofGetFrameNum() % 20 == 0){
        serial.writeByte(char('a'));
        serial.writeByte(storedData.x / 640 * 255);
        serial.writeByte(storedData.y / 480 * 255);
        serial.writeByte(storedData.z / 255 * 255);
    }
}
//--------------------------------------------------------------
void Message::sceneThree(ofVec3f avg){
    
    if(ofGetFrameNum() % 20 == 0){
        serial.writeByte(char('b'));
        serial.writeByte(avg.x / 640 * 255);
        serial.writeByte(avg.y / 480 * 255);
        serial.writeByte(avg.z / 255 * 255);
    }
}
//--------------------------------------------------------------
void Message::sceneFour(ofVec3f avg){
    
    if(ofGetFrameNum() % 20 == 0){
        serial.writeByte(char('d'));
        serial.writeByte(avg.x / 640 * 255);
        serial.writeByte(avg.y / 480 * 255);
        serial.writeByte(avg.z / 255 * 255);
    }
}




