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
    
    gui.setup("Serial", "serialSettings.xml");
    
    gui.add(offset.set("Offset", {0,0,0}, {-1000,-1000, -1000}, {1000, 1000, 1000}));
    offset.addListener(this, &Message::offsetChanged);
//    gui.loadFromFile("serialSettings.xml");
    
}
//--------------------------------------------------------------
void Message::offsetChanged(ofVec3f&){
    setMessage('a', ofVec3f(0,0,0));
    writeMessage(msg);
}
//--------------------------------------------------------------
void Message::sceneOne(ofVec3f avg){
    //reduce the framerate when sending message to ensure the arduino is not overwhelmed
    //Sends message indicating specific action to arduino as well as movement data.
    
    if(ofGetFrameNum() % 5 == 0){
            
        setMessage('a', avg);
        writeMessage(msg);
    
    }
}
//--------------------------------------------------------------
void Message::sceneTwo(ofVec3f storedData){
    //Sends movement data that has been stored in a buffer. Allows the arms to move when the dancer is not
    if(ofGetFrameNum() % 20 == 0){
        
        setMessage('a', storedData);
        writeMessage(msg);
      
    }
}
//--------------------------------------------------------------
void Message::sceneThree(ofVec3f avg){
    
    if(ofGetFrameNum() % 5 == 0){
     
    }
}
//--------------------------------------------------------------
void Message::sceneFour(ofVec3f avg){
    
    if(ofGetFrameNum() % 5 == 0){
    
    }
}



//--------------------------------------------------------------
void Message::update(){
    if(serial.available() && expectMessage){
        cout << "serial Feedback: ";
    }
    while(serial.available()) {
         unsigned char b = serial.readByte();
        expectMessage = (b == '\n');
        cout << b;
    }
}

//--------------------------------------------------------------
void Message::draw(){
    if(bDrawGui){
        gui.draw();
    }
}
//--------------------------------------------------------------
void Message::setMessage(char scene, ofVec3f avg){
    
    msg = "S"; // start
    msg += ofToString(scene); // scene
    msg += "x";
    msg += ofToString((int) (avg.x + offset->x));
    msg += "y";
    msg += ofToString((int) (avg.y + offset->y));
    msg += "z";
    msg += ofToString((int) (avg.z + offset->z));
    msg += "E"; // end
    cout << msg << endl;
}
//--------------------------------------------------------------
void Message::sendReset(){
    writeMessage("SrE");
}
//--------------------------------------------------------------
void Message::writeMessage(string message){
    
    int size = message.size();
    unsigned char buffer[size];
    copy(message.begin(), message.end(), buffer);
    serial.writeBytes(buffer, size);

}
//--------------------------------------------------------------
void Message::toggleGui(){
    bDrawGui ^= true;
}
