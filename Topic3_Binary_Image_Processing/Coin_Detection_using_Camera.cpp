#include "BinaryImageProcessing.h"



int CoinDetection_Camera() {
	
	VideoCapture cap(0);   // Create a video reader object
	if (!cap.isOpened()) {
		cout << "Error opening video stream or file" << endl;
		return -1;
	}



	//-------------- parameters declaration of coins detection
	Mat imageGray;
	Mat imgRGBChannels[3], imageB, imageG, imageR;
	Mat dst;

	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;
	params.blobColor = 0;
	params.minDistBetweenBlobs = 2;
	params.filterByArea = false;	    // Filter by Area
	params.filterByCircularity = true;  // Filter by Circularity
	params.minCircularity = 0.8;
	params.filterByConvexity = true;    // Filter by Convexity
	params.minConvexity = 0.8;
	params.filterByInertia = true;		// Filter by Inertia
	params.minInertiaRatio = 0.8;
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);  // Set up detector with params
	
	std::vector<KeyPoint> keypoints;
	// mark
	int x, y;
	int radius;
	double diameter;


	while (1) {

		Mat frame;
		cap >> frame;       // Capture frame-by-frame
		if (frame.empty())  // If the frame is empty, break immediately
			break;

	
		//-----------------------------------------------------------------
		
		cvtColor(frame, imageGray, COLOR_BGR2GRAY);
		// Split cell into channels
		split(frame, imgRGBChannels);
		imageB = imgRGBChannels[0];
		imageG = imgRGBChannels[1];
		imageR = imgRGBChannels[2];

		// Perform thresholding - convert the image to binary image
		threshold(imageG, dst, 15, 255, THRESH_BINARY_INV);  // threshold at 15 can get the best reuslt

		//------------- Perform morphological operations -------------
		// remove the noise (black pixels) on the background
		int kernelSize = 10;
		Mat imageDilated, imageEroded;
		Mat element = getStructuringElement(MORPH_ELLIPSE, Size(2 * kernelSize + 1, 2 * kernelSize + 1),
			Point(kernelSize, kernelSize));  // anchor position: the pixel position of the kernel which map on the top of the input image

		dilate(dst, imageDilated, element, Point(-1, -1), 1);			// remove the noise on background
		//displayImage("Dilated image", imageDilated, false);
		erode(imageDilated, imageEroded, element, Point(-1, -1), 1);	// restore the image
		//displayImage("Eroded image", imageEroded, true);


		//------------- Perform simple blob detection -------------
		// Detect blobs
		detector->detect(imageEroded, keypoints);	// pass the image to the detector and ask for the keypoints (interesting points of the image)
		cout << "Number of coins detected = " << keypoints.size() << endl;

		// Mark coins using image annotation concepts we have studied so far
		for (int i = 0; i < keypoints.size(); i++) {
			KeyPoint k = keypoints[i];
			Point keyPt;
			keyPt = k.pt;
			x = (int)keyPt.x;
			y = (int)keyPt.y;
			circle(frame, Point(x, y), 5, Scalar(255, 0, 0), -1);  		// Mark center in BLUE
			// Get radius of coin
			diameter = k.size;
			radius = (int)diameter / 2.0;
			circle(frame, Point(x, y), radius, Scalar(0, 255, 0), 2);   // Mark blob in GREEN
		}

		//----------------------------------------------------------------------

		imshow("Frame", frame);  // Display the resulting frame
		imshow("Gray", imageGray);  // Display the resulting frame
		imshow("Red", imageR);  // Display the resulting frame
		imshow("Green", imageG);  // Display the resulting frame
		imshow("Blue", imageB);  // Display the resulting frame
		imshow("Blue", imageB);  // Display the resulting frame
		imshow("imageEroded", imageEroded);  // Display the resulting frame

		char c = (char)waitKey(25);  // Press ESC on keyboard to exit
		if (c == 27)
			break;
	}

	cap.release();        // When everything done, release the video capture object
	destroyAllWindows();  // Closes all the frames


	return 0;
}