//////////////////////              Amy Cartwright             ///////////////////////


#ifndef message_h
#define message_h

#include <stdio.h>
#include "ofMain.h"

#endif /* message_hpp */
class Message{
    
    public:
    
    Message();
    
    ofSerial serial; //set up a serial object
    
    void setup();
    void sceneOne(ofVec3f);
    void sceneTwo(ofVec3f);
    void sceneThree(ofVec3f);
    void sceneFour(ofVec3f);
    
    void update();
    bool flag = true;
    
    string msg;
    void setMessage(char, ofVec3f);
    void writeMessage(string);
    bool expectMessage = true;
};
