//
//  VecSim.hpp
//  AudreyTreeRobot
//
//  Created by Amy Cartwright on 24/08/2018.
//
//

#ifndef VecSim_hpp
#define VecSim_hpp

#include <stdio.h>
#include "ofMain.h"


#endif /* VecSim_hpp */

class VecSim{
    
    public:
    
    VecSim();
    
    void setup();
    void draw();
    void calculate(int, int, bool);
    void update(int, int);
   
    	
    
    ofVec3f origin;
    ofVec3f pivotAbs;
    ofVec3f m[3]; //motors
    ofVec3f normal;
    ofVec3f b[3]; //base
    
    
    
    //current state of the rotation
    ofQuaternion curRot;
    
    //a place to store the mouse position so we can measure incremental change
    ofVec2f lastPos;
    
    //slows down the rotation 1 = 1 degree per pixel
    float dampen = 0.35;
    
    ofQuaternion systemRot;
    
    ofVec3f w;
    
    
    ofParameter<float> scale_rot;
    
    ofParameter<float> base_rad;
    ofParameter<float> motor_rad;
    ofParameter<float> base_height;
    
};

