#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    ofBackground(0,0,0);
    
    w = 640;
    h = 480;
    
    movie.setup(w, h, true);
    
    //reserve memory for cv images
    rgb.allocate(w, h);
    hsb.allocate(w, h);
    hue.allocate(w, h);
    sat.allocate(w, h);
    bri.allocate(w, h);
    filtered.allocate(w, h);
    
    noiseIndex = 0;
    noiseStep = 0.1;
    
    makeLights();
    makeShapes();
    
    rotatingLight = std::make_shared<ofx::Light2D>();
    rotatingLight->setPosition(ofVec3f(2.0f * ofGetWidth() / 3, 2.0f * ofGetHeight() / 3));
    rotatingLight->setViewAngle(ofDegToRad(360));
    lightSystem.add(rotatingLight);
    
    //creates additional triangular lights. 
    
//    for (int i = 0; i < 3; ++i)
//    {
//        ofx::Light2D::SharedPtr light = std::make_shared<ofx::Light2D>();
//
//        ofVec3f position(ofRandomWidth(), ofRandomHeight(), 0);
//        ofFloatColor color(ofRandomuf(), ofRandomuf(), ofRandomuf(), 1);
//
//        float radius = ofRandom(300, 1000);
//
//        float viewAngle = (ofRandom(1) > 0.5) ? TWO_PI : (ofRandom(PI/4, PI/3));
//
//        float angle = ofRandom(TWO_PI);
//
//        light->setAngle(angle);
//        light->setViewAngle(viewAngle);
//        light->setPosition(position);
//        light->setRadius(radius);
//        light->setColor(color);
//        light->setPosition(position);
//        lightSystem.add(light);
//
//        wanderingLights.push_back(light);
//    }
}

//--------------------------------------------------------------
void testApp::update(){
    
    movie.update();
    
    if (movie.isFrameNew()) {
        
        //copy webcam pixels to rgb image
        rgb.setFromPixels(movie.getPixels().getData(), w, h);
        
        //mirror horizontal
        rgb.mirror(false, true);
        
        //duplicate rgb
        hsb = rgb;
        
        //convert to hsb
        hsb.convertRgbToHsv();
        
        //store the three channels as grayscale images
        hsb.convertToGrayscalePlanarImages(hue, sat, bri);
        
        //filter image based on the hue value were looking for
        for (int i=0; i<w*h; i++) {
            filtered.getPixels()[i] = ofInRange(hue.getPixels()[i],findHue + 5,findHue + 5) ? 255 : 0;
        }
        filtered.flagImageChanged();
        
        //run the contour finder on the filtered image to find blobs with a certain hue
        contours.findContours(filtered, 50, w*h/2, 1, false);
    }
    
    noiseIndex += noiseStep;
    
    rotatingLight->setAngle(ofWrapRadians(rotatingLight->getAngle() + (PI / 360.0f)));
    
    rotatingLight->setPosition(ofVec3f(ofGetMouseX(),
                                       ofGetMouseY(),
                                       rotatingLight->getPosition().z));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor(10), ofColor(0));

    ofSetColor(255,255,255);
    
    //draw all cv images
    rgb.draw(0,0);
    //    hsb.draw(640,0);
    //    hue.draw(0,240);
    //    sat.draw(320,240);
    //    bri.draw(640,240);
    //    filtered.draw(0,480);
    //    contours.draw(0,480);
    lightSystem.clearShapes();

    makeShapes();

    ofSetColor(255, 0, 0);
    ofFill();
    
    //draw red circles for found blobs commented out to be replacedby rect shadow
    for (int i=0; i<contours.nBlobs; i++) {
        ofDrawCircle(contours.blobs[i].centroid.x, contours.blobs[i].centroid.y, 10);
    }
    
//    ofTranslate(8, 75);
//    ofFill();
//    ofSetColor(0);
//    ofDrawRectangle(-3, -3, 64+6, 64+6);
//    ofSetColor(targetColor);
//    ofDrawRectangle(0, 0, 64, 64);
    

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
    
    //calculate local mouse x,y in image
    int mx = x % w;
    int my = y % h;
    
    //get hue value on mouse position
    findHue = hue.getPixels()[my*w+mx];
}

void testApp::makeLights()
{
    for (int i = 0; i < 0; ++i)
    {
        ofx::Light2D::SharedPtr light = std::make_shared<ofx::Light2D>();
        
        ofVec3f position(ofRandomWidth(), ofRandomHeight(), 0);
        ofFloatColor color(ofRandomuf(), ofRandomuf(), ofRandomuf(), 1);
        
        float radius = ofRandom(300, 1000);
        
        light->setPosition(position);
        light->setRadius(radius);
        light->setColor(color);
        
        
        lightSystem.add(light);
    }
}


void testApp::makeShapes()
{
//    for (int i = 0; i < 4; ++i)
//    {
    
//        rect.setFromCenter(ofRandomWidth(),
//                           ofRandomHeight(),
//                           ofRandom(50, 70),
//                           ofRandom(50, 70));
        
        
        for (int i=0; i<contours.nBlobs; i++) {
            ofx::Shape2D::SharedPtr shape = std::make_shared<ofx::Shape2D>();
            ofRectangle rect;
            rect.setFromCenter(contours.blobs[i].centroid.x, contours.blobs[i].centroid.y, 20, 20);
            shape->setShape(ofPolyline::fromRectangle(rect));
            lightSystem.add(shape);
    }
}

