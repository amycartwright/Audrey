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
    
    //    systemRot.getRotate(angle, axis);
    //    for (int i = 0; i < 3; i++) {
    //        b[i].rotate(angle, axis);
    //    }
    //    normal.rotate(angle, axis);

    systemRot.getRotate(angle, axis);
    
    ofVec3f normalRot; //normal rotation
    ofVec3f br[3]; //base rotation
    
    for (int i = 0; i < 3; i++) {
        br[i] = b[i].getRotated(angle, axis);
    }
    normalRot = normal.getRotated(angle, axis);
    
    // origin-base vectors
    ofSetColor(255, 0, 0);
    ofDrawArrow(pivotAbs, 50 * br[0], 4);
    ofDrawArrow(pivotAbs, 50 * br[1], 4);
    ofDrawArrow(pivotAbs, 50 * br[2], 4);
    
    // origin-normal vector
    ofSetColor(0, 255, 0);
    ofDrawArrow(pivotAbs, 50 * normalRot, 4);
    
    
    // origin-motor vectors
    ofSetColor(0, 0, 255);
    ofDrawArrow(pivotAbs, 50 * m[0], 4);
    ofDrawArrow(pivotAbs, 50 * m[1], 4);
    ofDrawArrow(pivotAbs, 50 * m[2], 4);
    
    // motor-base vectors
    ofSetColor(255, 255, 0);
    ofDrawArrow(50 * m[0], 50*br[0], 4);
    ofDrawArrow(50 * m[1], 50*br[1], 4);
    ofDrawArrow(50 * m[2], 50*br[2], 4);
    
    
    w[0] = (br[0] - m[0]).length();
    w[1] = (br[1] - m[1]).length();
    w[2] = (br[2] - m[2]).length();
    
    ofPopMatrix();
}
//--------------------------------------------------------------
void VecSim::calculate(int x, int y, bool pressed){
    
    if (pressed) {
        //every time the mouse is dragged, track the change
        //accumulate the changes inside of curRot through multiplication
        ofQuaternion yRot(  (x-lastPos.x) * dampen, ofVec3f(0,1,0));
        ofQuaternion xRot( -(y-lastPos.y) * dampen, ofVec3f(1,0,0));
        curRot *= yRot * xRot;
        
    } else {
        
        float nX, nY;
        nX = x - ofGetWidth()/2;
        nY = y - ofGetHeight()/2;
        ofQuaternion zRot( nX * dampen * dampen, ofVec3f(0,0,1));
        ofQuaternion xRot(-nY * dampen * dampen, ofVec3f(1,0,0));
        systemRot = zRot * xRot;
        
    }
    
    lastPos.set(x, y);
    
}
//--------------------------------------------------------------
void VecSim::update(int x, int y){

    lastPos.set(x, y);

}
