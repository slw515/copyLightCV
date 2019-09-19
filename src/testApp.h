#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "LightSystem2D.h"


class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void mousePressed(int x, int y, int button);
    
    ofVideoGrabber movie;
    
    ofxCvColorImage rgb,hsb;
    ofxCvGrayscaleImage hue,sat,bri,filtered;
    ofxCvContourFinder contours;
    ofColor targetColor;

    int w,h;
    int findHue;
    
    void makeLights();
    void makeShapes();
    
    float noiseIndex;
    float noiseStep;
    
    ofx::Light2D::List wanderingLights;
    
    ofx::Light2D::SharedPtr rotatingLight;
    
    ofx::LightSystem2D lightSystem;
};

#endif
