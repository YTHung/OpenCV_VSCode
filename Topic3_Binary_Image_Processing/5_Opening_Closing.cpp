/*
	Remove small black or white spots from a binary image

	1. Opening refers Erosion followed by Dilation and these operations is used for clearing white blobs
	2. Closing refers Dilation followed by Erosion and are used for clearing black holes
*/

#include "BinaryImageProcessing.h"

int OpeningClosing() {
	string imageName = "resources/opening.png";
	Mat image = imread(imageName, IMREAD_GRAYSCALE);

	imshow("image", image);
	cout << "image size: " << image.size() << endl;
	waitKey(0);

	//-------------------- Opening: method 1 - using combination of erosion and dilation --------------------
	// Specify Kernel Size
	int kernelSize = 10;
	
	// Create the kernel
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(2 * kernelSize + 1, 2 * kernelSize + 1),
		Point(kernelSize, kernelSize));  // anchor position: the pixel position of the kernel which map on the top of the input image
	imshow("Structuring Element : Ellipse, Kernel Size : 10", element * 255);
	waitKey(0);
	destroyAllWindows();

	// Perform erosion
	Mat imEroded;
	erode(image, imEroded, element, Point(-1, -1), 1);
	imshow("imEroded", imEroded);
	waitKey(0);
	destroyAllWindows();
	// Perform dilation
	Mat imOpen;
	dilate(imEroded, imOpen, element, Point(-1, -1), 1);
	imshow("imEroded", imOpen);
	waitKey(0);
	destroyAllWindows();

	//-------------------- Opening: method 2 - using combination of erosion and dilation --------------------
	// Get structuring element/kernel which will be used for opening operation
	int openingSize = 3;

	// Selecting a elliptical kernel 
	element = getStructuringElement(MORPH_ELLIPSE, Size(2 * openingSize + 1, 2 * openingSize + 1),
		Point(openingSize, openingSize));
	
	// apply erosion followed by dilation on image by using morphologyEX
	Mat imageMorphOpened;
	morphologyEx(image, imageMorphOpened, MORPH_OPEN, element, Point(-1, -1), 3);
	imshow("Opening image", imageMorphOpened);
	waitKey(0);
	destroyAllWindows();





	//-------------------- Closing: method 1 - using combination of erosion and dilation --------------------
	imageName = "resources/closing.png";
	image = imread(imageName, IMREAD_GRAYSCALE);
	imshow("image", image);

	// Specify kernel size
	kernelSize = 10;
	// Create kernel
	element = getStructuringElement(MORPH_ELLIPSE, Size(2 * kernelSize + 1, 2 * kernelSize + 1),
		Point(kernelSize, kernelSize));

	// Perform Dilation
	Mat imDilated;
	dilate(image, imDilated, element);
	imshow("imDilated", imDilated);
	waitKey(0);
	destroyAllWindows();

	// Perform erosion
	Mat imClose;
	erode(imDilated, imClose, element);
	imshow("imClose", imClose);
	waitKey(0);
	destroyAllWindows();

	//-------------------- Closing: method 2 - using combination of erosion and dilation --------------------
	// Create a structuring element
	int closingSize = 10;
	// Selecting a elliptical kernel and storing in Mat element
	element = getStructuringElement(MORPH_ELLIPSE, Size(2 * closingSize + 1, 2 * closingSize + 1),
		Point(closingSize, closingSize));
	
	// Apply dilation followed by closing on image by using morphologyEx
	Mat imageMorphClosed;
	morphologyEx(image, imageMorphClosed, MORPH_CLOSE, element);
	imshow("imageMorphClosed", imageMorphClosed);
	waitKey(0);
	destroyAllWindows();

	return 0;
}