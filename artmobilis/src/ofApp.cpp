#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetOrientation(OF_ORIENTATION_90_LEFT);
	// this makes the camera directly return grayscale image, faster!
	grabber.setPixelFormat(OF_PIXELS_MONO);
	grabber.setup(320,240);
	gray.allocate(grabber.getWidth(),grabber.getHeight());
	bg.allocate(grabber.getWidth(),grabber.getHeight());
	//diff.allocate(grabber.getWidth(),grabber.getHeight());
	//contourFinder.allocate(grabber.getWidth(),grabber.getHeight());
	one_second_time = ofGetSystemTime();
	camera_fps = 0;
	frames_one_sec = 0;
	captureBg = true;
	// imgui
	ofSetLogLevel(OF_LOG_VERBOSE);

	//required call
	gui.setup();


	ImGui::GetIO().MouseDrawCursor = true;
    ImGui::GetIO().FontGlobalScale = 4.0f;
	clear_color = ImColor(114, 144, 154);
    showGlobal = true;
	floatValue = 0.0f;

	tex_button = gui.loadImage("of.png");
    // imgui
    margin = 3;
    inBetween = 3;
    // mPreviewFboWidth 80 mPreviewFboHeight 60 margin 10 inBetween 15 mPreviewWidth = 160;mPreviewHeight = 120;
    w = 80 + margin;
    h = 60 * 2.3;
    largeW = (80 + margin) * 4;
    largeH = (60 + margin) * 5;
    largePreviewW = 80 + margin;
    largePreviewH = (60 + margin) * 2.4;
    displayHeight = 900 - 50;

    static float f = 0.0f;
}

//--------------------------------------------------------------
void ofApp::update(){
	grabber.update();
	if(grabber.isFrameNew()){
		frames_one_sec++;
		if( ofGetSystemTime() - one_second_time >= 1000){
			camera_fps = frames_one_sec;
			frames_one_sec = 0;
			one_second_time = ofGetSystemTime();
		}
		gray.setFromPixels(grabber.getPixels(),grabber.getWidth(),grabber.getHeight());
		if(captureBg){
			bg = gray;
			captureBg = false;
		}

		//gray.absDiff(bg);
		gray.threshold(80);
		contourFinder.findContours(gray,10,grabber.getWidth()*grabber.getHeight()/3,10,true,true);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetHexColor(0xFFFFFF);
	gray.draw(5,5);
	contourFinder.draw(5,5);
	ofSetColor(0x000000);

    //required to call this at beginning
    gui.begin();
    // 2. Show another simple window, this time using an explicit Begin/End pair
    if (showGlobal)
    {
        //note: ofVec2f and ImVec2f are interchangeable
        ImGui::SetNextWindowSize(ofVec2f(1200,700), ImGuiSetCond_FirstUseEver);
        sprintf(buf, "Fps %c %d###fps", "|/-\\"[(int)(ImGui::GetTime() / 0.25f) & 3], (int)ofGetFrameRate());
        ImGui::Begin(buf, &showGlobal);

        // fps
        static ImVector<float> values; if (values.empty()) { values.resize(100); memset(&values.front(), 0, values.size()*sizeof(float)); }
        static int values_offset = 0;
        static float refresh_time = -1.0f;
        if (ui::GetTime() > refresh_time + 1.0f / 6.0f)
        {
            refresh_time = ui::GetTime();
            values[values_offset] = ofGetFrameRate();
            values_offset = (values_offset + 1) % values.size();
        }
        sprintf(buf, "%d", (int)ofGetFrameRate());
        ui::PlotLines("FPS", &values.front(), (int)values.size(), values_offset, buf, 0.0f, 80.0f, ImVec2(400, 200));

        sprintf(buf, "Camera fps %c %d", "|/-\\"[(int)(ImGui::GetTime() / 0.25f) & 3], camera_fps);
        ImGui::Text(buf);

        ImGui::SliderFloat("float", &floatValue, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&clear_color);

        bool pressed = ImGui::ImageButton((ImTextureID)(uintptr_t)tex_button, ImVec2(200, 141));
        ImGui::End();
    }
    //required to call this at end
    gui.end();
    /*
     ui::SetNextWindowSize(ImVec2(largePreviewW + 20, largePreviewH), ImGuiSetCond_Once);
	ui::SetNextWindowPos(ImVec2(xPos, yPos), ImGuiSetCond_Once);
	sprintf_s(buf, "Fps %c %d###fps", "|/-\\"[(int)(ImGui::GetTime() / 0.25f) & 3], (int)mParameterBag->iFps);
	ui::Begin(buf);
	{
		ImGui::PushItemWidth(mParameterBag->mPreviewFboWidth);
		// fps
		static ImVector<float> values; if (values.empty()) { values.resize(100); memset(&values.front(), 0, values.size()*sizeof(float)); }
		static int values_offset = 0;
		static float refresh_time = -1.0f;
		if (ui::GetTime() > refresh_time + 1.0f / 6.0f)
		{
			refresh_time = ui::GetTime();
			values[values_offset] = mParameterBag->iFps;
			values_offset = (values_offset + 1) % values.size();
		}
		if (mParameterBag->iFps < 12.0) ui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
		ui::PlotLines("FPS", &values.front(), (int)values.size(), values_offset, mParameterBag->sFps.c_str(), 0.0f, 300.0f, ImVec2(0, 30));
		if (mParameterBag->iFps < 12.0) ui::PopStyleColor();

		// Checkbox
		ui::Checkbox("Tex", &showTextures);
		ui::SameLine();
		ui::Checkbox("Fbos", &showFbos);
		ui::SameLine();
		ui::Checkbox("Shada", &showShaders);

		ui::Checkbox("Audio", &showAudio);
		ui::SameLine();
		ui::Checkbox("Cmd", &showConsole);
		ui::SameLine();
		ui::Checkbox("OSC", &showOSC);

		ui::Checkbox("MIDI", &showMidi);
		ui::SameLine();
		ui::Checkbox("Test", &showTest);
		if (ui::Button("Save Params"))
		{
			// save warp settings
			mBatchass->getWarpsRef()->save("warps1.xml");
			// save params
			mParameterBag->save();
		}

		mParameterBag->iDebug ^= ui::Button("Debug");
		ui::SameLine();
		mParameterBag->mRenderThumbs ^= ui::Button("Thumbs");
		ui::PopItemWidth();
		if (ui::Button("Stop Loading")) mBatchass->stopLoading();
	}
	ui::End();
     * */
}
void ofApp::mouseScrolled(float x, float y)
{
    ofLogVerbose(__FUNCTION__) << "x: " << x << " y: " << y;
}
//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void ofApp::pause(){

}

//--------------------------------------------------------------
void ofApp::stop(){

}

//--------------------------------------------------------------
void ofApp::resume(){

}

//--------------------------------------------------------------
void ofApp::reloadTextures(){

}

//--------------------------------------------------------------
bool ofApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void ofApp::okPressed(){

}

//--------------------------------------------------------------
void ofApp::cancelPressed(){

}
