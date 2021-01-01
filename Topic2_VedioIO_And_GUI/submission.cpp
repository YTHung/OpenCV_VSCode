#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;


Point topLeftCorner, buttomRightCorner;  // two points of bounding box
Mat sourceImg, faceNaive, roiFace;	 // Source image


// function which will be called on mouse input
void drawBBox(int action, int x, int y, int flags, void* userdata)
{
	// Action to be taken when left mouse button is pressed
	if (action == EVENT_LBUTTONDOWN) {
		topLeftCorner = Point(x, y);
		circle(sourceImg, topLeftCorner, 1, Scalar(255, 255, 0), 2, LINE_AA);   // Mark the point
	}
	// Action to be taken when left mouse button is released
	else if (action == EVENT_LBUTTONUP) {
		buttomRightCorner = Point(x, y);
		// Draw the bounding box
		rectangle(sourceImg, topLeftCorner, buttomRightCorner, Scalar(255, 0, 255), 5, LINE_8);
		imshow("Window", sourceImg);

		faceNaive = sourceImg.clone();
		roiFace = faceNaive(Range(topLeftCorner.y, buttomRightCorner.y), Range(topLeftCorner.x, buttomRightCorner.x));
		imwrite("face.png", roiFace);
	}
}

int main() {
	int k = 0;
	
	sourceImg = imread("sample.jpg", 1);
	namedWindow("Window");					 // Create a window
	setMouseCallback("Window", drawBBox);    // Highgui function called when mouse events occur

	// loop until escape character is pressed
	while (k != 27) {		// 27: ASCII code for 'Esc'
		imshow("Window", sourceImg);
		putText(sourceImg, "Choose top-left corner, and drag, Press ESC to exit and c to clear", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 255), 2);

		k = waitKey(20) & 0xFF;
	}

	return 0;
}