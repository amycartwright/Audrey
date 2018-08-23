#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    origin.set(ofGetWidth()/2, ofGetHeight()/2, 0);
    pivotAbs.set(0, 0, 0); //Ball joint - central point
    normal.set(0, -1, 0);
    
    // set motor position
    m[0].set(0, 1, 1);
    
    m[1].set(m[0]);
    m[1].rotate(120, normal);

    m[2].set(m[0]);
    m[2].rotate(-120, normal);
    
    // set base attach position
    b[0].set(0, 0, 1);
    
    b[1].set(b[0]);
    b[1].rotate(120, normal);
    
    b[2].set(b[0]);
    b[2].rotate(-120, normal);
}

//--------------------------------------------------------------
void ofApp::update(){
   
//    //These values will need to be changed to be variable dependant on each others length
//    tempPos1 = ofGetWidth()/2 + 100;
//    tempPos2 = ofGetHeight()/4 + 50;
//    tempPos3 = 0;
//
//    m1.set(tempPos1, tempPos2, tempPos3);
//    m2.set(tempPos1, tempPos2, tempPos3);
//    m3.set(tempPos1, tempPos2, tempPos3);
    
    // rotate around Y
//    for (int i = 0; i < 3; i++) {
//        m[i].rotate(1, normal);
//    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
   
    ofPushMatrix();
    ofTranslate(origin);
    
    // *************************** start

    //Extract the rotation from the current rotation
    ofVec3f axis;
    float angle;
    curRot.getRotate(angle, axis);
    
    //apply the quaternion's rotation to the viewport and draw the sphere
    ofRotateDeg(angle, axis.x, axis.y, axis.z);

    // *************************** end
    
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

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if (key == ' ') {
        origin.z = 0;
    }
    
    if (key == OF_KEY_LEFT_SHIFT) {
        shiftKey = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if (key == OF_KEY_LEFT_SHIFT) {
        shiftKey = false;
    }

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

    if (shiftKey) {
        
    // ************************* start
    
    //every time the mouse is dragged, track the change
    //accumulate the changes inside of curRot through multiplication
    ofQuaternion yRot((x-lastMouse.x)*dampen, ofVec3f(0,1,0));
    ofQuaternion xRot((y-lastMouse.y)*dampen, ofVec3f(-1,0,0));
    curRot *= yRot*xRot;
    lastMouse.set(x, y);
        
    
    // ************************* end
    
    } else {
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

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

    // ************************* start

    //store the last mouse point when it's first pressed to prevent popping
    lastMouse = ofVec2f(x,y);
    
    // ************************* end

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){
    
    origin.z += scrollY;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
