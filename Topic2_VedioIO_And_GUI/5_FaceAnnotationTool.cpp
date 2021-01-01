/*
	Note:
	This tool enables users to use mouse to

	1. create a bounding box around a face present in the input image 
	2. save the cropped face automatically.
*/

#include "VideoIO_GUI.h"

Point topLeftCorner, buttomRightCorner;  // two points of bounding box
Mat sourceHW, faceWithNaive, roiFace;	 // Source image


// function which will be called on mouse input
void drawBBox(int action, int x, int y, int flags, void* userdata)
{
	// Action to be taken when left mouse button is pressed
	if (action == EVENT_LBUTTONDOWN) {
		topLeftCorner = Point(x, y);
		circle(sourceHW, topLeftCorner, 1, Scalar(255, 255, 0), 2, LINE_AA);   // Mark the point
	}
	// Action to be taken when left mouse button is released
	else if (action == EVENT_LBUTTONUP) {
		buttomRightCorner = Point(x, y);
		// Draw the bounding box
		rectangle(sourceHW, topLeftCorner, buttomRightCorner, Scalar(255, 0, 255), 5, LINE_8);
		imshow("Window", sourceHW);

		faceWithNaive = sourceHW.clone();
		roiFace = faceWithNaive(Range(topLeftCorner.y, buttomRightCorner.y), Range(topLeftCorner.x, buttomRightCorner.x));
		imwrite("face.png", roiFace);
	}
}

int Face_Annotation_Tool() {
	int k = 0;

	sourceHW = imread("resources/sample.jpg", 1);
	Mat dummy = sourceHW.clone();            // Make a dummy image, will be useful to clear the drawing
	namedWindow("Window");					 // Create a window
	setMouseCallback("Window", drawBBox);    // Highgui function called when mouse events occur

	// loop until escape character is pressed
	while (k != 27) {		// 27: ASCII code for 'Esc'
		imshow("Window", sourceHW);
		putText(sourceHW, "Choose top-left corner, and drag, Press ESC to exit and c to clear", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 255), 2);

		k = waitKey(20) & 0xFF;
	}

	return 0;
}