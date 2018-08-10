#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "Message.hpp"

using namespace cv;

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void delta(ofVec3f&,ofVec3f&);
    ofVec3f dlt;
    float sq(float);
    
    ofVideoGrabber video;
    ofVideoPlayer film;
    bool calculatedFlow;
    
    ofxCvColorImage currentColor;		//First and second original images
    ofxCvGrayscaleImage gray1, gray2;	//Decimated grayscaled images
    ofxCvFloatImage flowX, flowY;		//Resulted optical flow in x and y axes
    Message message;
    
    float sumX, sumY, avgX, avgY;
    int numOfEntries;
    float phaseX, phaseY;
    float time;
    
    bool showFlow = false;
    bool showAverage = true;
    
    float circleXpos;
    float circleYpos;
};
