/*

	Blob: collection of white pixels connected to each other (white spots)

	Basic image processing :
	 1. erosion 
	 2. dilation
	 3. opening
	 4. closing
	 5. thresholding

	image processing: the input and output are image
	computer vision: the input is an image, and the output is some kind of the information.

	Computer vision algorithm -  Connected Component Analysis (CCA): label blobs in a binary image
	two pass algorithm
*/

#include "BinaryImageProcessing.h"

int CCA() {
	// Read image as grayscale
	Mat img = imread("resources/truth.png", IMREAD_GRAYSCALE);
	imshow("image", img);

	/*
	In the figure above, we see a binary image with five blobs corresponding to the letters T, R, U, T and H.
	Connected Component Analysis (CCA) allows us to apply a label to every blob. 
	In other words, CCA will create a mask where all pixels corresponding to the background are 0, 
	all pixels corresponding to the first blob, 
	(i.e. the letter T) are 1, those corresponding to R are 2 and so on and so forth.
	*/



	//--------------------------- CCA Analysis ---------------------------
	// Threshold Image
	Mat imThresh;
	threshold(img, imThresh, 127, 255, THRESH_BINARY);
	imshow("imThresh", imThresh);


	// Find connected components
	Mat imLabels;
	int nComponents = connectedComponents(imThresh, imLabels);	
	cout << "The number of connected components: " << nComponents << endl; // TRUTH and background = 6

	Mat imLabelsCopy = imLabels.clone();

	// Find the min and max values in imLabels
	Point minLoc, maxLoc;
	double minVal, maxVal;
	minMaxLoc(imLabels, &minVal, &maxVal, &minLoc, &maxLoc);	// every pixel in each blob is labeled by a number starting from 1
	cout << "Max value = " << maxVal << " @ " << maxLoc << endl;
	cout << "Min value = " << minVal << " @ " << minLoc << endl;

	// Normalize the image so the min value is 0 and max value is 255.
	imLabels = 255 * (imLabels - minVal) / (maxVal - minVal);

	// Convert image to 8-bits
	imLabels.convertTo(imLabels, CV_8U);	// the original type of imLabels is CV_32S
	imshow("imLabels", imLabels);
	waitKey(0);


	//--------------------------- Display each label ---------------------------
	imshow("c0", imLabelsCopy == 0);
	imshow("c1", imLabelsCopy == 1);
	imshow("c2", imLabelsCopy == 2);
	imshow("c3", imLabelsCopy == 3);
	imshow("c4", imLabelsCopy == 4);
	imshow("c5", imLabelsCopy == 5);
	waitKey(0);

	//--------------------------- Colour coding of components ---------------------------



	return 0;
}
