#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "Message.hpp"
#include "ofxGui.h"

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
    
    float time;
    int numOfEntries;
    
    ofVec2f sum;
    ofVec2f avg;
    ofVec2f phase;
    
    bool guiDraw = false;
    
    deque <ofVec2f> currentPos;
    
    ofxPanel gui;
    ofParameter<float> threshold;
    ofParameter<float> winSize;
    ofParameter<float> pyrScale;
    ofParameter<float> polySigma;
    ofParameter<int> levels;
    ofParameter<float> iterations;
    ofParameter<float> polyN;
    ofParameter<bool> showFlow;
    ofParameter<bool> showAverage;
    
    ofSoundPlayer soundScore;
    int lowPassSize  = 5;
   
    
};
