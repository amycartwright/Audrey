#include "ofApp.h"
//--------------------------------------------------------------

void ofApp::setup()
{

    //setup for the webcam
    video.listDevices();
    video.setDeviceID(0);
    video.initGrabber(ofGetWindowWidth(),ofGetWindowHeight());
    
    //setup for the data input film
    film.load("AudreyInputMovementData1.mp4");
    
    film.setVolume(0);
    
    //set up for the optical flow
    calculatedFlow = false;
    phase.x = 0;
    phase.y = 0;
    
    //setup for serial connection
    message.setup();
    
    //setup for translating the co-ordinates to the robots 3 point system
    vecSim.setup();
    
    
    //setup for gui
    gui.setup();
    gui.add(pyrScale.set("PyrScale", 0.7, 0, .99));
    gui.add(levels.set("Levels", 3, 1, 8));
    gui.add(winSize.set("WindowSize", 40, 0, 70));
    gui.add(iterations.set("Iterations", 5, 1, 8));
    gui.add(polyN.set("PolyN", 5, 5, 10));
    gui.add(polySigma.set("PolySigma", 1.1, 1.1, 2));
    gui.add(showAverage.set("Show Average", true));
    gui.add(showFlow.set("Show Flow", false));
    gui.add(threshold.set("Threshold", 1, 0, 20));
    gui.add(dampen.set("Dampen", 0.01, 0, 2));
    gui.add(showVecSim.set("Show Vector Simulation", true));
    gui.add(showWebCam.set("Show Web Cam Image", false));
    gui.add(showSoundPosition.set("Show SoundPosition", true));
    gui.add(vecSim.scale_rot.set("scale rot", 0.045, 0, 1));
    gui.add(vecSim.motor_rad.set("motor radius", 190, 10, 500));
    gui.add(vecSim.base_rad.set("base radius", 200, 10, 500));
    gui.add(vecSim.base_height.set("base height", 225, 10, 500));
    gui.add(toSteps.set("to steps scaling", 10, 10, 1000));
    gui.add(update_vecSys.setup("update base/motor position"));
    
    update_vecSys.addListener(this, &ofApp::upd_vecSysPressed);
    
    vecSim.setup();
    
    //setup for sound
    soundScore.load("Jakob_Glock_-_Granular_Voices.wav");
    
    
} /*END*/

//--------------------------------------------------------------
void ofApp::update(){
    
    message.update(); // check if arduino serial is free

    video.update(); //Decode the new frame if needed
    film.update();
    
//    vecSim.update(afterMapping.x, afterMapping.y);
    
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
    
    //End the program at the end of the music
    if (soundScore.getPosition() >= 0.97) {
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
    
    //Draw the loaded film
    if(soundScore.getPosition() > 0) {
        ofSetColor(255);
        film.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
        film.play();
    }
    
    //Draw the webcam footage
    if(showWebCam) {
        ofPushStyle();
        ofSetColor(255, 50);
        video.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
        ofPopStyle();
    }
    
    //Draw the sound position
    if(showSoundPosition) ofDrawBitmapString(ofToString(soundScore.getPosition()), 20, 20 );
    
    
    //Draw the vecSim diagram
    if(showVecSim) vecSim.draw();
    
    
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
    
    if(guiDraw) gui.draw();
    
    message.draw();
    //If flow is detected find the average flow and increase to get more movement
    if (numOfEntries>0){
        avg = sum / numOfEntries;
        avg *= 10;
        
    }
    
    //Low Pass filter - calculate the average of the current and previous positions
    ofVec2f tempAvg;
    currentPos.push_back(ofVec2f(avg.x, avg.y));
    
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
    
    //Always have a slight pull tot he center to avoid getting stuck on one side
    if(fabs(phase.x) > 1 ) phase.x -= dampen * ofSign(phase.x) * sqrt(fabs(phase.x));
    if(fabs(phase.y) > 1 ) phase.y -= dampen * ofSign(phase.y) * sqrt(fabs(phase.y));
    
    //Clamp the value so that it is not possible for the average data to exceed the bounds of the window
    phase.x = MAX(MIN(ofGetWidth()/2-40, phase.x),-ofGetWidth()/2+40);
    phase.y = MAX(MIN(ofGetHeight()/2-40, phase.y),-ofGetHeight()/2+40);
    
    //Draw a circle at the average point of flow
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    if(showAverage){
        ofSetColor(255);
        ofDrawCircle(phase.x, -phase.y, 40);
    }
    ofPopMatrix();

    //Map to screen width and height to get only positive numbers
    afterMapping.x = ofMap(phase.x, -ofGetWidth()/2, ofGetWidth()/2, 0, ofGetWidth());
    afterMapping.y = ofMap(-phase.y, -ofGetHeight()/2, ofGetHeight()/2, 0, ofGetHeight());

    //Send data to the vecSim to calculate the robots position
    vecSim.calculate(afterMapping.x, afterMapping.y, shiftKey);

    ofVec3f toSend = (vecSim.w - vecSim.base_height) * toSteps;
    
    //For debuging 
//    if (soundScore.getPosition() > 0){
//        message.sceneOne(toSend);
//    }
    
    ofDrawBitmapStringHighlight(ofToString((vecSim.w - vecSim.base_height)), 20, 80);
    
    
     //Choreographed movement
     //Begin sending data to arduino at scheduled intervals
     if (soundScore.getPosition() > 0.062 && soundScore.getPosition() < 0.074){
         message.sceneOne(toSend);
        }
    
        else if (soundScore.getPosition() > 0.131 && soundScore.getPosition() < 0.209){
            message.sceneOne(toSend);
        }
  
            else if(soundScore.getPosition() > 0.209 && soundScore.getPosition() < 0.294){
                    storedData.push_back(toSend);
            }

                else if (soundScore.getPosition() > 0.294 && soundScore.getPosition() < 0.385){
                    count++;
                    message.sceneTwo(storedData[count % storedData.size()]);
                    cout << "sd:" << storedData[count % storedData.size()]  << endl;
                }

                    else if(soundScore.getPosition() > 0.385 && soundScore.getPosition() < 0.460){
                        message.sceneOne(toSend);
                    }

                        else if (soundScore.getPosition() > 0.460 && soundScore.getPosition() < 0.581){
                            message.sceneOne(toSend);
                        }
    
                            else if (soundScore.getPosition() > 0.581 && soundScore.getPosition() < 0.770){
                                message.sceneOne(toSend);
                            }
    
                                else if (soundScore.getPosition() > 0.770 && soundScore.getPosition() < 0.970){
                                    message.sceneOne(toSend);
                                }
     
    
    
} /*END*/

//--------------------------------------------------------------
void ofApp::upd_vecSysPressed(){
    
    vecSim.setup();

}

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
        
        case ' ':
        vecSim.origin.z = 0;
        break;
            
        case 'g':
            message.toggleGui();
        break;

            
        case OF_KEY_LEFT_SHIFT:
        shiftKey = true;
        break;
            
            case 's':
            message.setMessage('a', ofVec3f(0,200,0));
            message.writeMessage(message.msg);
            break;
        case 'S':
            message.setMessage('a', ofVec3f(0,500,0));
            message.writeMessage(message.msg);
            break;
    }
     
} /*END*/

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if (key == OF_KEY_LEFT_SHIFT) {
        shiftKey = false;
    }
}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){
    
    vecSim.origin.z += scrollY;
}
//--------------------------------------------------------------
void ofApp::exit(){
//    message.sendReset();
}
