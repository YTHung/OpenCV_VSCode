/*
	Contours are boundaries of objects which are different from edges. 
	Contours = edges + connectivity

	1. use binary image as an input where white pixels assume to be foreground, and the black pixels assume to be background.
	2. use radial sweep algorithm to find the contour

	Contours are simply the boundaries of an object or part of object in an image. 
	They are useful in shape analysis and object Detection/Recognition using traditional Computer Vision algorithms.
	You can do a lot of analysis based on contours to arrive at a solution to many computer vision problems.

Function Syntax

void cv::findContours    (    InputArray     image,
OutputArrayOfArrays     contours,
OutputArray     hierarchy,
int     mode,
int     method,
Point     offset = Point()
)
Where,

image - input image (8-bit single-channel). Non-zero pixels are treated as 1's. Zero pixels remain 0's, so the image is treated as binary. 
		You can use compare, inRange, threshold , adaptiveThreshold, Canny, and others to create a binary image out of a grayscale or color one.
contours - Detected contours. Each contour is stored as a vector of points.
hierarchy - Optional output vector containing information about the image topology. 
			It has been described in detail in the video above.
mode - Contour retrieval mode, ( RETR_EXTERNAL, RETR_LIST, RETR_CCOMP, RETR_TREE )
method - Contour approximation method. ( CHAIN_APPROX_NONE, CHAIN_APPROX_SIMPLE, CHAIN_APPROX_TC89_L1 etc )
offset - Optional offset by which every contour point is shifted. 
		 This is useful if the contours are extracted from the image ROI and then they should be analyzed in the whole image context.


*/


#include "BinaryImageProcessing.h"

int Contour() {
	string imagePath = "resources/Contour.png";
	Mat image = imread(imagePath);
	Mat imageCopy = image.clone();
	Mat imageGray;

	// Convert to grayscale
	cvtColor(image, imageGray, COLOR_BGR2GRAY);
	imshow("src image", image);
	waitKey(0);

	//------ Find all contours in the image ------
	vector<vector<Point>> contoursList, ContourExternal; // delcare a vector which contains vecotrs 
	vector<Vec4i> hierarchy;							 // declare a 4-dim vector where data type is integer
	findContours(imageGray, contoursList, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);  // Note that both the inner and outer contours have been detected.
	cout << "Number of contoursList found = " << contoursList.size() << endl;
	drawContours(image, contoursList, -1, Scalar(0, 255, 0), 6);  	// Draw contours
	imshow("contour list", image);
	waitKey(0);

	//------ Find external contours in the image ------
	findContours(imageGray, ContourExternal, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);  // only detect the outer contours
	cout << "Number of contours found = " << ContourExternal.size();
	image = imageCopy.clone();
	drawContours(image, ContourExternal, -1, Scalar(0, 255, 0), 3);		
	imshow("contour external", image);
	waitKey(0);

	// draw the 3rd contour - not knowing the numbering of coutour in ternms of shapes at this point
	image = imageCopy.clone();
	drawContours(image, ContourExternal, 2, Scalar(0, 255, 0), 3);
	imshow("The 3rd Contour", image);
	waitKey(0);

	//------ Contour Properties ------
	// Centre of mass or centroid
	Moments M;	// image moment
	int x, y;
	for (size_t i = 0; i < contoursList.size(); i++) {
		// We will use the contour moments to find the centroid
		M = moments(contoursList[i]);	// fine the image moment of each contour
		x = int(M.m10 / double(M.m00));
		y = int(M.m01 / double(M.m00));

		// Mark the center
		circle(image, Point(x, y), 10, Scalar(255, 0, 0), -1);
	}
	imshow("Centre of Contours", image);  // every centroid of contours will be plotted, including inner and outter contours.
	waitKey(0);

	// Area and Perimeter
	double area;
	double perimeter;
	for (size_t i = 0; i < contoursList.size(); i++) {
		area = contourArea(contoursList[i]);
		perimeter = arcLength(contoursList[i], true);
		cout << "Contour #" << i + 1 << " has area = " << area << " and perimeter = " << perimeter << endl;
	}



	//------ Bounding Boxes ------
	/*
	There are 2 type of bounding boxes we can create around a contour:
		1. A vertical rectangle
		2. A rotated rectangle - This is the bounding box with the minimum area
	*/

	// Method 1.
	image = imageCopy.clone();
	Rect rect;
	for (size_t i = 0; i < contoursList.size(); i++) {
		// Vertical rectangle
		rect = boundingRect(contoursList[i]);
		rectangle(image, rect, Scalar(255, 0, 255), 2);
	}
	imshow("Bounding Boxes of Contours", image);  
	waitKey(0);

	// Method 2.
	image = imageCopy.clone();
	RotatedRect rotrect;
	Mat boxPoints2f, boxPointsCov;
	for (size_t i = 0; i < contoursList.size(); i++) {
		// Rotated rectangle
		rotrect = minAreaRect(contoursList[i]);			// finds a rotated rectangle of the minimum area 
		boxPoints(rotrect, boxPoints2f);				// finds four vertices of a rotated rectangle
		boxPoints2f.assignTo(boxPointsCov, CV_32S);		// convert the value to 32-bit signed (float)
		polylines(image, boxPointsCov, true, Scalar(0, 255, 255), 2);  // draws serveral polygonal curvers
	}
	imshow("rotated rectangle", image);
	waitKey(0);


	// Fit a circle and ellipse (enclosed is not needed)
	image = imageCopy.clone();
	Point2f center;
	float radius;
	for (size_t i = 0; i < contoursList.size(); i++) {
		// Fit a circle
		minEnclosingCircle(contoursList[i], center, radius);     // finds a circle of minimum area enclosing a 2D point set.
		circle(image, center, radius, Scalar(125, 125, 125), 2);
	}
	imshow("Circle", image);
	waitKey(0);

	image = imageCopy.clone();
	RotatedRect rellipse;
	for (size_t i = 0; i < contoursList.size(); i++) {
		// Fit an ellipse
		// We can fit an ellipse only when the contour has minimum 5 points
		if (contoursList[i].size() < 5)
			continue;
		rellipse = fitEllipse(contoursList[i]);
		ellipse(image, rellipse, Scalar(255, 0, 125), 2);
	}
	imshow("Ellipse", image);
	waitKey(0);

	return 0;
}