#include "ofApp.h"
#include "ofAppGlutWindow.h"
#include <string>


//========================================================================
int main(int argc, char *argv[]){
	// ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// // this kicks off the running of my app
	// // can be OF_WINDOW or OF_FULLSCREEN
	// // pass in width and height too:
	// ofRunApp(new ofApp());

	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);

    ofApp *app = new ofApp();

    app->arguments = vector<string>(argv, argv + argc);

	ofSetFrameRate(60);
	ofRunApp(app); // start the app

}
