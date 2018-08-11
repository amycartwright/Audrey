#include "ofApp.h"
//--------------------------------------------------------------

void ofApp::setup()
{
    ofSetFullscreen(true);
    
    //setup for the webcam
    video.listDevices();
    video.setDeviceID(0);
    video.initGrabber(ofGetWindowWidth(),ofGetWindowHeight());
    
    //setup for the data input film
    film.load("AudreyInputMovementData.mp4");
    film.play();
    film.setVolume(20);
    
    //set up for the optical flow
    calculatedFlow = false;
    phaseX = 0;
    phaseY = 0;
    
    //setup for serial connection
    message.setup();
    
} /*END*/

//--------------------------------------------------------------
void ofApp::update(){
    video.update(); //Decode the new frame if needed
    film.update();
    
    if(fabs (avgX) > 0.5) phaseX += avgX; //increase phaseX by avgX
    if(fabs (avgY) > 0.5) phaseY += avgY; //increase phaseY by avgY
    
    //Clamp the value so that it is not possible for the average data to exceed the bounds of the window
    phaseX = MAX(MIN(ofGetWidth()/2, phaseX),-ofGetWidth()/2);
    phaseY = MAX(MIN(ofGetHeight()/2, phaseY),-ofGetHeight()/2);
    
    if (video.isFrameNew()){
        //Only begin the flow algorithm if gray1 is true - this creates a short delay at the start of the program and ensures that movement data can be collected.
        if (gray1.bAllocated){
            gray2 = gray1;
            calculatedFlow = true;
        }
        
        //Convert to ofxCv images
        ofPixels & pixels = video.getPixels();
        currentColor.setFromPixels(pixels);
        //Decimate images to 25% (makes calculations faster + works like a blurr too)

        float decimate = 0.25;
        ofxCvColorImage imageDecimated1;
        imageDecimated1.allocate(currentColor.width * decimate, currentColor.height * decimate);
        //High-quality resize
        imageDecimated1.scaleIntoMe(currentColor, CV_INTER_AREA);
        gray1 = imageDecimated1;
        
        if (gray2.bAllocated){
            //Create OpenCV images
            Mat img1(gray1.getCvImage());
            Mat img2(gray2.getCvImage());
            //Image for flow
            Mat flow;
            //Computing optical flow (https://goo.gl/jm1Vfr - explanation of parameters)
            calcOpticalFlowFarneback(img1, img2, flow, 0.7, 3, 11, 5, 5, 1.1, OPTFLOW_FARNEBACK_GAUSSIAN);
            //Split flow into separate images
            vector<Mat> flowPlanes;
            split( flow, flowPlanes );
            //Copy float planes to ofxCv images flowX and flowY
            IplImage iplX( flowPlanes[0] );
            flowX = &iplX;
            IplImage iplY( flowPlanes[1] );
            flowY = &iplY;
        }
    }
    
//-------------- Timing the Program --------------
    
//    int minutes = 2;
//    int totalMillis = minutes * 60 * 1000; //180k
//    time = ofMap(ofGetElapsedTimeMillis(), 0, totalMillis, 0, 1, true);
    
    
    //program to exit at 3min.
   // if (time>=1) ofExit();
    
} /*END*/

//--------------------------------------------------------------
void ofApp::draw(){

    numOfEntries = 0;
    sumX = 0;
    sumY = 0;
    avgX = 0;
    avgY = 0;
    
    //Draw the webcam footage
    //video.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    
    //Draw the loaded film
    film.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

        if (calculatedFlow){
            ofSetColor(255, 255, 255);
            
            int w = gray1.width;
            int h = gray1.height;
            
            //Input images + optical flow
            ofPushMatrix();
            ofScale(4, 4);
            
            //Optical flow
            float *flowXPixels = flowX.getPixelsAsFloats();
            float *flowYPixels = flowY.getPixelsAsFloats();
            
            for (int y=0; y<h; y+=5){
                for (int x=0; x<w; x+=5){
                    float fx = flowXPixels[ x + w * y ];
                    float fy = flowYPixels[ x + w * y ];

                    //Draw only long vectors
                    //Change the threshold (no. at the end) to change the ammount of flow detected
                    if (fabs(fx) + fabs(fy) > 5){
                        if(showFlow == true){
                            ofDrawRectangle(x-0.5, y-0.5, 1, 1);
                            ofDrawLine(x, y, x + fx, y + fy);
                        }
                        sumX += fx;
                        sumY += fy;
                        numOfEntries ++;
                    }
                }
            }
            ofPopMatrix();
        }
    
    //If flow is detected find the average flow
    if (numOfEntries>0){
        avgX = sumX / numOfEntries;
        avgY = sumY / numOfEntries;
        
        //More movement
        avgX *= 10;
        avgY *= 10;
    }
    
    //Low Pass filter - calculate the average of the current and previous positions
    ofVec3f avg = ofVec3f(avgX, avgY);
    currentPos.push_back(ofVec3f(avgX, avgY));
    
    if(currentPos.size() > 1){
        float f = 0.5; //adapt this to adjust the proportions between current and prev
        avg = f * currentPos[1] + (1-f) * currentPos[0]; //low pass filter for additional smoothing add more points to average ensurin that the coefficient is always equal to 1

    }
    
    //Draw a circle at the average point of flow
    ofPushMatrix();
    ofTranslate(ofGetWindowWidth()/2, ofGetWindowHeight()/2);
    if(showAverage == true){
        ofPushStyle();
        ofSetColor(255);
        ofDrawCircle(phaseX, phaseY, 40);
        ofPopStyle();
    }
    
//    if(ofGetFrameNum() % 20 == 0){
//         cout << 'x' << phaseX << endl;
//        // cout << 'a' << avgX << endl;
//         cout << 'y' << phaseY << endl;
//       //  cout << 'a' << avgY << endl;
//        
//   
//    }
    
    ofPopMatrix();
    
    ofVec3f vec = ofVec3f(phaseX,phaseY,1);
    delta(vec, dlt);
//    cout << 't' << dlt << endl;
//    cout << 'o' << vec << endl;
    
    
//-------------- Communicating with Arduino --------------
    
    if(phaseX >= 0){
      //  message.sceneOne();
    }
    
    if(phaseY <= 0){
        message.sceneTwo();
    }
} /*END*/

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == '1') showFlow = !showFlow;
    
    if(key == '2') showAverage = !showAverage;
    
} /*END*/
//--------------------------------------------------------------
float ofApp::sq(float d){
    
    return d * d;
} /*END*/
//--------------------------------------------------------------
void ofApp::delta(ofVec3f &cartesian, ofVec3f &delta){
    
    float deltaDiagonalRod = 300;
    float deltaSmoothRodOffset = 212.357;
    float deltaEffectorOffset = 30.0;
    float deltaCarriageOffset = 30.0;
    float deltaRadius = (deltaSmoothRodOffset - deltaEffectorOffset - deltaCarriageOffset);
    
    float sin60 = sin(PI/3);
    float cos60 = 0.5;
    
    float deltaDiagonalRod2 = deltaDiagonalRod * deltaDiagonalRod;
    float deltaTower1X = - sin60 * deltaRadius;
    float deltaTower1Y = -cos60 * deltaRadius;
    float deltaTower2X = sin60 * deltaRadius;
   float deltaTower2Y = -cos60 * deltaRadius;
    float deltaTower3X = 0.0;
    float deltaTower3Y = deltaRadius;
    
    
    delta.x = sqrt(deltaDiagonalRod2 - sq(deltaTower1X - cartesian.x) - sq(deltaTower1Y - cartesian.y)) + cartesian.z;
    
    delta.y = sqrt(deltaDiagonalRod2 - sq(deltaTower2X - cartesian.x) - sq(deltaTower2Y - cartesian.y)) + cartesian.z;
    
    delta.z = sqrt(deltaDiagonalRod2 - sq(deltaTower3X - cartesian.x) - sq(deltaTower3Y - cartesian.y)) + cartesian.z;
} /*END*/
                   
                   
                   
                   
                   
                   
                   
