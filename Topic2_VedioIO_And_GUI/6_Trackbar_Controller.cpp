/*
	Note:

	int cv::createTrackbar  (   const String &  trackbarname,
								const String &  winname,
								int *   value,
								int     count,
								TrackbarCallback    onChange = 0,
								void *  userdata = 0		)

	Parameters

	trackbarname : Name of the created trackbar.
	winname : Name of the window that will be used as a parent of the created trackbar.
	value : Optional pointer to an integer variable whose value reflects the position of the slider. Upon creation, the slider position is defined by this variable.
	count : Maximal position of the slider. The minimal position is always 0.
	onChange : Pointer to the function to be called every time the slider changes position. This function should be prototyped as void Foo(int,void*); , where the first parameter is the trackbar position and the second parameter is the user data (see the next parameter). If the callback is the NULL pointer, no callbacks are called, but only value is updated.
	userdata : User data that is passed as is to the callback. It can be used to handle trackbar events without using global variables.
*/


#include "VideoIO_GUI.h"

int maxScaleUp = 100;
int scaleFactor = 0;
int scaleType = 0;
int maxType = 1;

Mat im;

string windowName = "Resize Image";
string trackbarValue = "Scale";
string trackbarType = "Type: \n 0: Scale Up \n 1: Scale Down";

// Callback functions
void scaleImage(int, void*) {							 // TrackerbarCallback(int, void*): 

	if (scaleType == 1)	scaleFactor = -scaleFactor;
	
	double scaleFactorDouble = 1 + scaleFactor / 100.0;  // Scale up 1% in one trackbar value
	if (scaleFactorDouble == 0) {
		scaleFactorDouble = 1;
	}
	
	Mat scaledImage;
	resize(im, scaledImage, Size(), scaleFactorDouble, scaleFactorDouble, INTER_LINEAR);
	imshow(windowName, scaledImage);
}

int Trackbar_Controller() {
	// load an image
	im = imread("resources/truth.png");

	namedWindow(windowName, WINDOW_AUTOSIZE); // Create a window to display results
	createTrackbar(trackbarValue, windowName, &scaleFactor, maxScaleUp, scaleImage);  // trackbar value is ranged from 0 to maxScaleUp
	createTrackbar(trackbarType, windowName, &scaleType, maxType, scaleImage);

	scaleImage(25, 0);

	while (true) {
		int c;
		c = waitKey(20);
		if (static_cast<char>(c) == 27)
			break;
	}

	return 0;
}


