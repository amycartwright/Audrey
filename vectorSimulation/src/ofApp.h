#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    ofVec3f origin;
    ofVec3f pivotAbs;
    ofVec3f normal;
    ofVec3f m1, m2, m3; //motors
    ofVec3f s1, s2, s3; //springs??
    float tempPos1, tempPos2, tempPos3;
    
    
    
		
};
