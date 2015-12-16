#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"

#include "ofxOpenCv.h"
#include "ofxImGui.h"

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
        bool show_test_window;
        bool show_another_window;

        float floatValue;
        GLuint tex_button;

};
