/*
	Ref:
	 https://blog.christianperone.com/2014/06/simple-and-effective-coin-segmentation-using-python-and-opencv/
	 https://tremaineconsultinggroup.com/opencv-coin-detection-project/

	Procedures:
	1. find out the best input image
	2. apply the threshold to get the binary image
	3. perform the morphological operation to reduce the noise
	4. Three apporach is utilised to detect the number of coins present in the image
		4-1. simple blob detection
		4-2. contour detection
		4-3. CCA
*/

#include "BinaryImageProcessing.h"


void displayImage(string title, Mat image, bool isDestroy) {
	imshow(title, image);
	waitKey(0);
	if(isDestroy) destroyAllWindows();
}

Mat displayConnectedComponents(Mat& im)
{
	Mat imLabels = im.clone();  // Make a copy of the image
	Mat imColorMap;				// Apply a color map
	Point minLoc, maxLoc;
	double min, max;

	minMaxLoc(imLabels, &min, &max, &minLoc, &maxLoc);  // finds the min and max pixel values and their locations in an image.
	imLabels = 255 * (imLabels - min) / (max - min); 	// Normalize the image so the min value is 0 and max value is 255.
	imLabels.convertTo(imLabels, CV_8U);  				// Convert image to 8-bits

	applyColorMap(imLabels, imColorMap, COLORMAP_JET);

	return imColorMap;
}

int Coin_Detection() {

	
	string imagePath = "resources/CoinsA.png";  // Image path
	Mat image = imread(imagePath);				// Read image
	Mat imageCopy = image.clone();
	displayImage( "image", image, false);

	// Convert image to grayscale
	Mat imageGray;
	cvtColor(image, imageGray, COLOR_BGR2GRAY);
	displayImage( "image in gray", imageGray, false);

	// Split cell into channels
	Mat imgRGBChannels[3], imageB, imageG, imageR;
	split(image, imgRGBChannels);  
	imageB = imgRGBChannels[0];
	imageG = imgRGBChannels[1];
	imageR = imgRGBChannels[2];
	displayImage("image in blue channel", imageB, false);
	displayImage("image in green channel", imageG, false);
	displayImage("image in red channel", imageR, true);

	// Perform thresholding - convert the image to binary image
	// from the image above we can see that the image in green channel has the highest contrast between background and coins.
	Mat dst;
	threshold(imageG, dst, 15, 255, THRESH_BINARY_INV);  // threshold at 15 can get the best reuslt
	displayImage("binary image", dst, true);



	//------------- Perform morphological operations -------------
	// remove the noise (black pixels) on the background
	int kernelSize = 10;
	Mat imageDilated, imageEroded;
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(2 * kernelSize + 1, 2 * kernelSize + 1),
					Point(kernelSize, kernelSize));  // anchor position: the pixel position of the kernel which map on the top of the input image
	
	dilate(dst, imageDilated, element, Point(-1, -1), 1);			// remove the noise on background
	displayImage("Dilated image", imageDilated, false);
	erode(imageDilated, imageEroded, element, Point(-1, -1), 1);	// restore the image
	displayImage("Eroded image", imageEroded, true);


	//------------- Perform simple blob detection -------------
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

	// Detect blobs
	std::vector<KeyPoint> keypoints;
	detector->detect(imageEroded, keypoints);	// pass the image to the detector and ask for the keypoints (interesting points of the image)
	cout << "Number of coins detected = " << keypoints.size() << endl;

	// Mark coins using image annotation concepts we have studied so far
	int x, y;
	int radius;
	double diameter;

	for (int i = 0; i < keypoints.size(); i++) {
		KeyPoint k = keypoints[i];
		Point keyPt;
		keyPt = k.pt;
		x = (int)keyPt.x;
		y = (int)keyPt.y;
		circle(image, Point(x, y), 5, Scalar(255, 0, 0), -1);  		// Mark center in BLUE
		// Get radius of coin
		diameter = k.size;
		radius = (int)diameter / 2.0;
		circle(image, Point(x, y), radius, Scalar(0, 255, 0), 2);   // Mark blob in GREEN
	}
	displayImage("detected coins", image, true);


	//------------- Find connected components (CCA) -------------
	// Use displayConnectedComponents function provided above
	Mat imLabels;
	int nComponents = connectedComponents(1-imageEroded, imLabels);
	cout << "The number of connected components: " << nComponents << endl; // Coins and background = 10
	Mat colorMap = displayConnectedComponents(imLabels);
	displayImage("CAA image", colorMap, true);
	
	

	//------------- Contour Detection -------------
	// Find all contours in the image
	vector<vector<Point>> contoursList, ContourExternal; // delcare a vector which contains vecotrs
	vector<Vec4i> hierarchy;							 // declare a 4-dim vector where data type is integer
	findContours(imageEroded, contoursList, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);  // Note that both the inner and outer contours have been detected.
	cout << "Number of contoursList found = " << contoursList.size() << endl;
	image = imageCopy.clone();  // reset the image
	drawContours(image, contoursList, -1, Scalar(0, 255, 0), 6);  	// Draw contours
	displayImage("Contour", image, true);

	// Find out the outter contour
	findContours(imageEroded, ContourExternal, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);  // only detect the outer contours
	cout << "Number of contours found = " << ContourExternal.size() << endl;
	image = imageCopy.clone();
	drawContours(image, ContourExternal, -1, Scalar(0, 255, 0), 20);
	displayImage("External Contour", image, true);
	
	// Print area and perimeter of all contours
	double area, biggestArea = 0;
	double perimeter;
	int biggestElement = 0;
	for (size_t i = 0; i < contoursList.size(); i++) {
		area = contourArea(contoursList[i]);
		perimeter = arcLength(contoursList[i], true);
		cout << "Contour #" << i + 1 << " has area = " << area << " and perimeter = " << perimeter << endl;
		if (area > biggestArea) {
			biggestArea = area;
			biggestElement = i;
		}
	}
	cout << "Maximum area of contour = " << contourArea(contoursList[biggestElement]) << endl;


	// Remove this contour and plot others
	contoursList.pop_back();    // remove the last element in the vector
	cout << "Number of contours found = " << contoursList.size() << endl;
	image = imageCopy.clone();	// reset the image
	drawContours(image, contoursList, -1, Scalar(0, 255, 0), 3);
	displayImage("Contour", image, true);


	// Fit circles on coins
	image = imageCopy.clone();
	Point2f center;
	float radius_2;
	for (size_t i = 0; i < contoursList.size(); i++) {
		// Fit a circle
		minEnclosingCircle(contoursList[i], center, radius_2);  // finds a circle of minimum area enclosing a 2D point set.
		circle(image, center, radius_2, Scalar(255, 0, 0), 5);  // Mark circle in blue
		circle(image, center, 5, Scalar(0, 0, 255), -1);        // Mark center in red
	}
	cout << "Number of coins detected using contour = " << contoursList.size() << endl;
	displayImage("Fit the circle", image, true);


//-----------------------------//
//			Part B			   //
//-----------------------------//

	imagePath = "resources/CoinsB.png";
	image = imread(imagePath);
	imageCopy = image.clone();
	cvtColor(image, imageGray, COLOR_BGR2GRAY);
	displayImage("image in gray", imageGray, false);

	// Split cell into channels
	split(image, imgRGBChannels);
	imageB = imgRGBChannels[0];
	imageG = imgRGBChannels[1];
	imageR = imgRGBChannels[2];
	displayImage("image in blue channel", imageB, false);
	displayImage("image in green channel", imageG, false);
	displayImage("image in red channel", imageR, true);

	// Perform thresholding
	threshold(imageB, dst, 130, 255, THRESH_BINARY);  
	displayImage("binary image", dst, true);

	// Perform morphological operations
	// firstly, we have to fill up the coins with black pixels, so we hvae to perform the openning.
	erode(dst, imageEroded, element, Point(-1, -1), 1);	// restore the image
	dilate(imageEroded, imageDilated, element, Point(-1, -1), 1);
	displayImage("Step1. Openning", imageDilated, true);
	// From the result of the image, we can see that if we use closing on the image the noise could be removed.
	// Than we can do the openning to enlose the coins.
	dilate(imageDilated, imageDilated, element, Point(-1, -1), 1);
	erode(imageDilated, imageEroded, element, Point(-1, -1), 1);
	displayImage("Step1. Closing", imageEroded, true);

/*
	// Setup SimpleBlobDetector parameters.

	params.blobColor = 0;

	params.minDistBetweenBlobs = 2;

	// Filter by Area
	params.filterByArea = false;

	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.8;

	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.8;

	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.8;

	// Set up detector with params
	detector = SimpleBlobDetector::create(params);

	// Detect blobs
	///
	/// YOUR CODE HERE
	///

	// Print number of coins detected
	///
	/// YOUR CODE HERE
	///

	// Mark coins using image annotation concepts we have studied so far
	///
	/// YOUR CODE HERE
	///

	// Find connected components
	///
	/// YOUR CODE HERE
	///

	// Find all contours in the image
	///
	/// YOUR CODE HERE
	///

	// Print the number of contours found
	///
	/// YOUR CODE HERE
	///

	// Draw all contours
	///
	/// YOUR CODE HERE
	///

	// Remove the inner contours
	// Display the result
	///
	/// YOUR CODE HERE
	///

	displayImage(image);

	// Print area and perimeter of all contours
	///
	/// YOUR CODE HERE
	///

	// Print maximum area of contour
	// This will be the box that we want to remove
	///
	/// YOUR CODE HERE
	///

	// Remove this contour and plot others
	///
	/// YOUR CODE HERE
	///

	// Print area and perimeter of all contours
	///
	/// YOUR CODE HERE
	///

	// Remove contours
	///
	/// YOUR CODE HERE
	///

	// Draw revised contours
	///
	/// YOUR CODE HERE
	///

	// Fit circles on coins
	///
	/// YOUR CODE HERE
	///

	displayImage(image);
*/
	return 0;
}