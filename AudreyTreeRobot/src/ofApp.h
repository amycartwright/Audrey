#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "Message.hpp"
#include "VecSim.hpp"
#include "ofxGui.h"

using namespace cv;

class ofApp : public ofBaseApp{
    
    public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseDragged(int x, int y, int button);
    void mouseScrolled(int x, int y, float scrollX, float scrollY);
    void mousePressed(int x, int y, int button);
    
    ofVideoGrabber video;
    ofVideoPlayer film;
    bool calculatedFlow;
    
    ofxCvColorImage currentColor;		//First and second original images
    ofxCvGrayscaleImage gray1, gray2;	//Decimated grayscaled images
    ofxCvFloatImage flowX, flowY;		//Resulted optical flow in x and y axes
    
    Message message;
    VecSim vecSim;
    
    float time;
    int numOfEntries;
    
    ofVec2f sum;
    ofVec2f avg;
    ofVec2f phase;
    
    float sumX, sumY;
    float avgX, avgY;
    float phaseX, phaseY;
    
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
    ofParameter<bool> showVecSim;
    
    ofParameter<float> dampen;
    
    
    ofSoundPlayer soundScore;
    int lowPassSize  = 5;
    bool shiftKey;
    
};
