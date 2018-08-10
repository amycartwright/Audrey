#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    origin.set(0, 0, 0);
    pivotAbs.set(400, 400, 0);
    normal.set(0, -100, 0);
}

//--------------------------------------------------------------
void ofApp::update(){
//    normal.set(0, mouseY);
    pivotAbs.set(mouseX, mouseY);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofTranslate(400, 400);
    ofDrawArrow(origin, pivotAbs);

    ofDrawArrow(pivotAbs, normal);

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
