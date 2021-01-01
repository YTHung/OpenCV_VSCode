#include "VideoIO_GUI.h"

int KB_Input() {

	VideoCapture cap(0);  // Open webcam
	Mat frame;
	int k = 0;			  // store the info. of KB input

	// Detect if webcam is working properly
	if (!cap.isOpened()) {
		cout << "Unable to detect webcam " << "\n";
		return 0;
	}
	else{
		while (1){
			cap >> frame;  	// Capture frame

			if (k == 27)	// ASCII code Esc
				break;
			// Identify if 'e' or 'E' is pressed
			if (k == 101 || k == 69)
				putText(frame, "E is pressed, press Z or ESC", Point(100, 180), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 3);
			// Identify if 'z' or 'Z' is pressed or not
			if (k == 90 || k == 122)
				putText(frame, "Z is pressed", Point(100, 180), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 3);
			imshow("Image", frame);
			// Waitkey is increased so that the display is shown
			k = waitKey(10000) & 0xFF;		// ASCII Code is 8-bit data
		}
	}
	cap.release();
	destroyAllWindows();

	return 0;
}
