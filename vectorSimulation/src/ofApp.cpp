#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    origin.set(0, 0, 0);
    pivotAbs.set(ofGetWidth()/2, ofGetHeight()/2, 0); //Ball joint - central point
    normal.set(ofGetWidth()/2, ofGetHeight()/4, 0);
    
}

//--------------------------------------------------------------
void ofApp::update(){
   
    //These values will need to be changed to be variable dependant on each others length
    tempPos1 = ofGetWidth()/2 + 100;
    tempPos2 = ofGetHeight()/4 +50;
    tempPos3 = 0;
    
    m1.set(tempPos1, tempPos2, tempPos3);
    m2.set(tempPos1, tempPos2, tempPos3);
    m3.set(tempPos1, tempPos2, tempPos3);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
   
    ofSetColor(255, 0, 0);
    ofDrawArrow(origin, pivotAbs, 5);
    
    ofSetColor(0, 255, 0);
    ofDrawArrow(pivotAbs, normal, 5);
    
    ofSetColor(0, 0, 255);
    ofDrawArrow(pivotAbs, m1, 5);
    ofDrawArrow(pivotAbs, m2, 5);
    ofDrawArrow(pivotAbs, m3, 5);
    


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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
