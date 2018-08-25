//
//  VecSim.cpp
//  AudreyTreeRobot
//
//  Created by Amy Cartwright on 24/08/2018.
//
//

#include "VecSim.hpp"

VecSim::VecSim(){
    
}
//--------------------------------------------------------------
void VecSim::setup(){
    origin.set(ofGetWidth()/2, ofGetHeight()/2, 0);
    pivotAbs.set(0, 0, 0); //Ball joint - central point
    normal.set(0, -1, 0);
    
    // set motors position
    m[0].set(0, 1, 1);
    
    m[1].set(m[0]);
    m[1].rotate(120, normal);
    
    m[2].set(m[0]);
    m[2].rotate(-120, normal);
    
    // set positions of base to motors points
    b[0].set(0, 0, 1);
    
    b[1].set(b[0]);
    b[1].rotate(120, normal);
    
    b[2].set(b[0]);
    b[2].rotate(-120, normal);

    
}
//--------------------------------------------------------------
void VecSim::draw(){
    
    ofPushMatrix();
    ofTranslate(origin);
    
    //Extract the rotation from the current rotation
    ofVec3f axis;
    float angle;
    curRot.getRotate(angle, axis);
    
    //apply the quaternion's rotation to the viewport and draw the sphere
    ofRotate(angle, axis.x, axis.y, axis.z);
    ofRotate(angle, axis.x, axis.y, axis.z);
    
    //    systemRot.getRotate(angle, axis);
    //    for (int i = 0; i < 3; i++) {
    //        b[i].rotate(angle, axis);
    //    }
    //    normal.rotate(angle, axis);
    ////    ofRotateDeg(angle, axis.x, axis.y, axis.z);
    
    // origin-base vectors
    ofSetColor(255, 0, 0);
    ofDrawArrow(pivotAbs, 50*b[0], 4);
    ofDrawArrow(pivotAbs, 50*b[1], 4);
    ofDrawArrow(pivotAbs, 50*b[2], 4);
    
    // origin-normal vector
    ofSetColor(0, 255, 0);
    ofDrawArrow(pivotAbs, 50*normal, 4);
    
    
    // origin-motor vectors
    ofSetColor(0, 0, 255);
    ofDrawArrow(pivotAbs, 50*m[0], 4);
    ofDrawArrow(pivotAbs, 50*m[1], 4);
    ofDrawArrow(pivotAbs, 50*m[2], 4);
    
    // motor-base vectors
    ofSetColor(255, 255, 0);
    ofDrawArrow(50*m[0], 50*b[0], 4);
    ofDrawArrow(50*m[1], 50*b[1], 4);
    ofDrawArrow(50*m[2], 50*b[2], 4);
    
    ofPopMatrix();
}

void VecSim::calculate(int x, int y, bool pressed){
    
    if (pressed) {
        //every time the mouse is dragged, track the change
        //accumulate the changes inside of curRot through multiplication
        ofQuaternion yRot((x-lastMouse.x)*dampen, ofVec3f(0,1,0));
        ofQuaternion xRot((y-lastMouse.y)*dampen, ofVec3f(-1,0,0));
        curRot *= yRot*xRot;
        lastMouse.set(x, y);
    }
    else {
        ofQuaternion zRot((x-lastMouse.x)*dampen, ofVec3f(0,0,1));
        ofQuaternion xRot((y-lastMouse.y)*dampen, ofVec3f(-1,0,0));
        systemRot = zRot*xRot;
        lastMouse.set(x, y);
    
        ofVec3f axis;
        float angle;
        systemRot.getRotate(angle, axis);
        for (int i = 0; i < 3; i++) {
            b[i].rotate(angle, axis);
        }
        normal.rotate(angle, axis);
    //    ofRotateDeg(angle, axis.x, axis.y, axis.z);
    
}

}
