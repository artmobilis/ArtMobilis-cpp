#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"

#include "ofxOpenCv.h"
#include "ofxImGui.h"
// #define IM_ARRAYSIZE(_ARR)			((int)(sizeof(_ARR)/sizeof(*_ARR)))
class ofApp : public ofxAndroidApp{
	
	public:
		
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void windowResized(int w, int h);

		void touchDown(int x, int y, int id);
		void touchMoved(int x, int y, int id);
		void touchUp(int x, int y, int id);
		void touchDoubleTap(int x, int y, int id);
		void touchCancelled(int x, int y, int id);
		void swipe(ofxAndroidSwipeDir swipeDir, int id);

        void mouseScrolled(float x, float y);

		void pause();
		void stop();
		void resume();
		void reloadTextures();

		bool backPressed();
		void okPressed();
		void cancelPressed();

		ofVideoGrabber grabber;
		ofxCvGrayscaleImage gray, bg;//, diff;
		ofxCvContourFinder contourFinder;

		bool captureBg;

		int one_second_time;
		int camera_fps;
		int frames_one_sec;
        // imgui
        ofxImGui gui;
        ImVec4 clear_color;

        float floatValue;
        GLuint tex_button;
    float						color[4];
    float						backcolor[4];
    int							playheadPositions[12];
    float						speeds[12];
    // mPreviewFboWidth 80 mPreviewFboHeight 60 margin 10 inBetween 15
    int							w;
    int							h;
    int							displayHeight;
    int							xPos;
    int							yPos;
    int							largeW;
    int							largeH;
    int							largePreviewW;
    int							largePreviewH;
    int							margin;
    int							inBetween;

    float						f = 0.0f;
    char						buf[64];

    bool						showGlobal ;

};
