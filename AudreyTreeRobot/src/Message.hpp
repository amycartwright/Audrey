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
    
};
