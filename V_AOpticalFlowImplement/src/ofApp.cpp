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
    film.setVolume(0);
    
    //set up for the optical flow
    calculatedFlow = false;
    phase.x = 0;
    phase.y = 0;
    
    //setup for serial connection
    message.setup();
    
    //setup for gui
    gui.setup();
    gui.add(pyrScale.set("PyrScale", 0.7, 0, .99));
    gui.add(levels.set("Levels", 3, 1, 8));
    gui.add(winSize.set("WindowSize", 40, 0, 70));
    gui.add(iterations.set("Iterations", 5, 1, 8));
    gui.add(polyN.set("PolyN", 5, 5, 10));
    gui.add(polySigma.set("PolySigma", 1.1, 1.1, 2));
    gui.add(showAverage.set("Show Average", true));
    gui.add(showFlow.set("Show Flow", true));
    gui.add(threshold.set("Threshold", 1, 0, 20));

    //setup for sound
    soundScore.load("Vocal.wav");
    
} /*END*/

//--------------------------------------------------------------
void ofApp::update(){
    video.update(); //Decode the new frame if needed
    film.update();
    
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
            calcOpticalFlowFarneback(img1, img2, flow, pyrScale, levels, winSize, iterations, polyN, polySigma, OPTFLOW_FARNEBACK_GAUSSIAN);
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
    
        //End the program at the end of the music
        if (soundScore.getPosition() >= 0.96) {
            ofExit();
        }
    
} /*END*/

//--------------------------------------------------------------
void ofApp::draw(){

    numOfEntries = 0;
    sum.x = 0;
    sum.y = 0;
    avg.x = 0;
    avg.y = 0;
    
    //Draw the webcam footage
    video.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    
    //Draw the loaded film
    if(soundScore.getPosition() > 0) film.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    
    if(guiDraw == true) gui.draw();

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
                    if (fabs(fx) + fabs(fy) > threshold){
                        if(showFlow == true){
                            ofDrawRectangle(x-0.5, y-0.5, 1, 1);
                            ofDrawLine(x, y, x + fx, y + fy);
                        }
                        sum.x += fx;
                        sum.y += fy;
                        numOfEntries ++;
                    }
                }
            }
            ofPopMatrix();
        }
    
    //If flow is detected find the average flow and increase to get more movement
    if (numOfEntries>0){
        avg = sum / numOfEntries;
        avg *= 10;
    }
    
    //Low Pass filter - calculate the average of the current and previous positions
    ofVec2f tempAvg;
    currentPos.push_back(ofVec2f(avg.x, avg.y));
    
    float distX;
    float distY;
    
    if(currentPos.size() > 1){
        //low pass filter for additional smoothing add more points to average ensuring that the coefficient is always equal to 1
        for(int i = 0; i < currentPos.size(); i++){
            tempAvg += currentPos[i];
        }
        tempAvg /= currentPos.size();
    }
    
    while(currentPos.size() > lowPassSize){
        currentPos.pop_front();
    }
    
    //avg is updated after being passed through the low pass filter
    avg = tempAvg;
    
    //Increment phase with avg only if the absolute of avg is greater than 0.5
    if(fabs (avg.x) > 0.5) phase.x += avg.x;
    if(fabs (avg.y) > 0.5) phase.y += avg.y;
    
    //Clamp the value so that it is not possible for the average data to exceed the bounds of the window
    phase.x = MAX(MIN(ofGetWidth()/2-40, phase.x),-ofGetWidth()/2+40);
    phase.y = MAX(MIN(ofGetHeight()/2-40, phase.y),-ofGetHeight()/2+40);
    
    //Draw a circle at the average point of flow
    ofPushMatrix();
    ofTranslate(ofGetWindowWidth()/2, ofGetWindowHeight()/2);
    if(showAverage == true){
        ofPushStyle();
        ofSetColor(255);
        ofDrawCircle(-phase.x, -phase.y, 40);
        ofPopStyle();
    }
    
    ofPopMatrix();
    
    //Passing to the delta algorithm
    ofVec3f vec = ofVec3f(phase.x,phase.y,1);
    delta(vec, dlt);
    
    //For Debugging
    if(ofGetFrameNum() % 20 == 0){
        //cout << 'p' << phase << endl;
        //cout << 't' << tempAvg << endl;
        //cout << 't' << dlt << endl;
        //cout << 'o' << vec << endl;
    }
    
    ofDrawBitmapString(ofToString(soundScore.getPosition()), ofGetWidth()/2, ofGetHeight()/2 + 20);
    
//-------------- Communicating with Arduino --------------
    
    /*
     //Choreographed movement
     //Begin sending data to arduino at 1min into music, stop at 2min
     if (soundScore.getPosition() > 0.18 && soundScore.getPosition() < 0.28){
        message.sceneOne(avg);
     }
        //Send the data to the arduino
        else if (soundScore.getPosition() > 0.28 && soundScore.getPosition() < 0.36){
            message.sceneThree(avg);
        }
            //Store the data that is collected from movement during this time
            else if(soundScore.getPosition() > 0.36 && soundScore.getPosition() < 0.50){
                storedData.push_back(avg);
            }
                //Send the stored data to the arduino
                else if (soundScore.getPosition() > 0.50 && soundScore.getPosition() < 0.69){
                    count++;
                    message.sceneTwo(storedData[count % storedData.size()]);
                }
                    //Send the stored data that has randomness interjected
                        else if(soundScore.getPosition() > 0.69 && soundScore.getPosition() < 0.81){
                        message.sceneFour(dataWithRandom);
                    }
                        //Begin sending data to arduino at 4:01 and run until tae end
                            else if (soundScore.getPosition() > 0.92 && soundScore.getPosition() < 0.94){
                            message.sceneOne(avg);
                        }
     */

    
} /*END*/

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
 
     switch (key) {
   
    case '1':
        guiDraw = !guiDraw;
        break;
    
    //Start sound score when letter 'p' is pressed
    case 'p':
         soundScore.play();
         break;
    
}

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
                   
                   
                   
                   
                   
                   
                   
