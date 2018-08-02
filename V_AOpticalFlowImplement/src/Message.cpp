//////////////////////              Amy Cartwright             ///////////////////////

//  OSC Sender

#include "Message.hpp"
Message::Message(){
    
}
//--------------------------------------------------------------
void Message::setup(){
    
    serial.setup(1, 9600);
    serial.listDevices();
}
//--------------------------------------------------------------
void Message::sceneOne(ofVec3f avg){
    
    
  
    
    //reduce the framerate when sending message to ensure the arduino is not overwhelmed
    //Sends message indicating specific action to arduino as well as movement data.
    if(ofGetFrameNum() % 20 == 0){
        serial.writeByte(char('c'));
        serial.writeByte(avg.x / 640 * 255);
        serial.writeByte(avg.y / 480 * 255);
        serial.writeByte(avg.z / 255 * 255);
    }


}
//--------------------------------------------------------------
void Message::sceneTwo(){
    
   

}



