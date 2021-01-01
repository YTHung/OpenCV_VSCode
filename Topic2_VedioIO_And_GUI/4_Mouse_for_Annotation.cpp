#include "VideoIO_GUI.h"

Point center, circumference;  // Points to store the center of the circle and a point on the circumference
Mat source;                   // Source image


// function which will be called on mouse input
void drawCircle(int action, int x, int y, int flags, void* userdata)
{
	// Action to be taken when left mouse button is pressed
	if (action == EVENT_LBUTTONDOWN) {
		center = Point(x, y);
		circle(source, center, 1, Scalar(255, 255, 0), 2, LINE_AA);   // Mark the center
	}
	// Action to be taken when left mouse button is released
	else if (action == EVENT_LBUTTONUP) {
		circumference = Point(x, y);
		// Calculate radius of the circle
		float radius = sqrt(pow(center.x - circumference.x, 2) + pow(center.y - circumference.y, 2));
		// Draw the circle
		circle(source, center, radius, Scalar(0, 255, 0), 2, LINE_AA);
		imshow("Window", source);
	}
}

int Mouse_Annotation() {
	int k = 0;

	source = imread("resources/sample.jpg", 1);
	Mat dummy = source.clone();                // Make a dummy image, will be useful to clear the drawing
	namedWindow("Window");
	setMouseCallback("Window", drawCircle);    // Highgui function called when mouse events occur

	// loop until escape character is pressed
	while (k != 27) {	// ASCII code Esc
		imshow("Window", source);
		putText(source, "Choose center, and drag, Press ESC to exit and c to clear", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 255), 2);

		k = waitKey(20) & 0xFF;
		if (k == 99)	// 'z'
		// Another way of cloning
			dummy.copyTo(source);
	}

	return 0;
}
