//////////////////////              Amy Cartwright             ///////////////////////

//  Determines what specific data is being sent to the Arduino

#include "Message.hpp"
Message::Message(){
    
}
//--------------------------------------------------------------
void Message::setup(){
    //setup for the serial connection to the arduino IDE.
    serial.setup(1, 57600);
    serial.listDevices();
}
//--------------------------------------------------------------
void Message::sceneOne(ofVec3f avg){
    //reduce the framerate when sending message to ensure the arduino is not overwhelmed
    //Sends message indicating specific action to arduino as well as movement data.
    /*
    if(flag){
        flag = false;
        
         serial.writeByte(char('S')); // start
         serial.writeByte(char('a')); // scene
         serial.writeByte(char('x'));
         serial.writeByte(int ((avg.x / 500) * 255)); //Here 500 should be changed to the maximum possible wire length
         serial.writeByte(char('y'));
         serial.writeByte(int ((avg.y / 500) * 255));
         serial.writeByte(char('z'));
         serial.writeByte(int ((avg.z / 500) * 255));
         serial.writeByte(char('E')); // end
     }
         */
        
        setMessage('a', avg);
        
        writeMessage(msg);
    
    //    if(ofGetFrameNum() % 20 == 0){
    //        serial.writeByte(char('a'));
    //        serial.writeByte(avg.x*0.1);
    //        serial.writeByte(avg.y*0.1);
    //        serial.writeByte(avg.z*0.1);
    //    }
}
//--------------------------------------------------------------
void Message::sceneTwo(ofVec3f storedData){
    //Sends movement data that has been stored in a buffer. Allows the arms to move when the dancer is not
    if(ofGetFrameNum() % 20 == 0){
        serial.writeByte(char('b'));
        serial.writeByte(int (storedData.x / 640 * 255));
        serial.writeByte(int (storedData.y / 480 * 255));
        serial.writeByte(int (storedData.z / 255 * 255));
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



//--------------------------------------------------------------
void Message::update(){
    
    flag = (serial.readByte() == 'F');
}


//--------------------------------------------------------------
void Message::setMessage(char scene, ofVec3f avg){
    
    msg = "S"; // start
    msg += ofToString(scene); // scene
    msg += "x";
    msg += ofToString((int) avg.x);
    msg += "y";
    msg += ofToString((int) avg.y);
    msg += "z";
    msg += ofToString((int) avg.z);
    msg += "E"; // end
    cout << msg << endl;
}

//--------------------------------------------------------------
void Message::writeMessage(string message){
    
    if(true){
        flag = false;
        
      // if(ofGetFrameNum() % 20 == 0){
        int size = message.size();
        unsigned char buffer[size];
        copy(message.begin(), message.end(), buffer);
        serial.writeBytes(buffer, size);
       //}
    }
}
