/*
	What is structuring element?

	Structuring elements ( a.k.a Kernels ) are used to modify the shape of the blobs. 
	These are used to scan the image and modify the pixels on the basis of some rule/algorithm 
	( which governs whether you are doing Erosion or Dilation or something else )

	There are 3 types of structuring elements supported by OpenCV.

	1. Ellipse/Circular shaped
	2. Rectangular shaped
	3. Cross shaped

	--------------------------------------------------------------------------------------------
	There are two methods 
	Method 1:
		step 1 Scan the kernel through the image
		step 2 when you find a white pixel overlapping with the center of the kernel, 
			   perform an OR operation of the Kernel and the pixel Neighborhood.

	Method 2:
		step 1 Scan through the image and superimpose the kernel on the neighborhood of each pixel.
		step 2 Perform an AND operation of the neighborhood with the kernel.
		step 3 Replace the pixel value with the maximum value in the neighborhood given by the kernel.

		This means that you check every pixel and its neighborhood with respect to the kernel and change the pixel to white if any of the pixel in this neighborhood is white.
		OpenCV implements an optimized version of this method. This will work even if the image is not a binary image.


	Note. In Range(_start, _end), start is an inclusive left boundary of the range and end is an exclusive right boundary of the range. 
		  Such a half-opened interval is usually denoted as [start,end).
*/


#include "BinaryImageProcessing.h"

int MorphologicalOperation() {
	
	//----------------------- Create a source image -----------------------
	Mat resizedImage;
	Mat demoImage = Mat::zeros(Size(10, 10), CV_8U);	// Create a 10 by 10 image
	cout << "Original intensity of image: " << endl << demoImage << endl;
	imshow("intensity of the image", demoImage);
	waitKey(0);

	// Manipulate the intensity of interest pixels
	demoImage.at<uchar>(0, 1) = 1;		
	demoImage.at<uchar>(9, 0) = 1;
	demoImage.at<uchar>(8, 9) = 1;
	demoImage.at<uchar>(2, 2) = 1;
	demoImage(Range(5, 8), Range(5, 8)).setTo(1);
	cout << demoImage << endl;
	imshow("intensity of the image", demoImage * 255);
	waitKey(0);

	// create a cross structuring element
	Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3));	// create a structuring element
	cout << element;
	int ksize = element.size().height;
	int	height = demoImage.size().height;
	int width = demoImage.size().width;
	cout << height << endl;
	cout << width << endl;
	waitKey(0);


	////----------------------- Wrtie code for Dilation from scratch with method 1 -----------------------
	int border = ksize / 2;
	Mat paddedDemoImage = Mat::zeros(Size(height + border * 2, width + border * 2), CV_8UC1);
	copyMakeBorder(demoImage, paddedDemoImage, border, border, border, border, BORDER_CONSTANT, 0);  // alternative approach to above line
	Mat bitOR;

	for (int h_i = border; h_i < height + border; h_i++) {		 // for-loop the height of demo image
		for (int w_i = border; w_i < width + border; w_i++) {	 // for-loop the width of the demo image
			if (demoImage.at<uchar>(h_i - border, w_i - border)) // check the intensity of the demo image
			{
				cout << "White Pixel Found @ " << h_i << "," << w_i << endl;
				cout << paddedDemoImage(Range(h_i - border, h_i + border + 1), Range(w_i - border, w_i + border + 1)) << endl; // show filtering region
				cout << element << endl;																					   // show the filter
				bitwise_or(paddedDemoImage(Range(h_i - border, h_i + border + 1), Range(w_i - border, w_i + border + 1)), element, bitOR); // bitwise operation
				cout << "The output of bitwiseOR" << endl << bitOR << endl;										

				bitOR.copyTo(paddedDemoImage(Range(h_i - border, h_i + border + 1), Range(w_i - border, w_i + border + 1)));  // copy to new image
				cout << "The result of padded image:" << endl << paddedDemoImage << endl;
				imshow("image", paddedDemoImage * 255);
				waitKey(0);

			}
		}
	}

	// Crop out the original dimension from the padded output image
	Mat dilatedImage = paddedDemoImage(Range(border, border + height), Range(border, border + width));
	imshow("dilatedImage from scratch", dilatedImage * 255);
	waitKey(0);

	Mat dilatedEllipseKernel;
	dilate(demoImage, dilatedEllipseKernel, element);
	cout << dilatedEllipseKernel << endl;

	imshow("image", dilatedEllipseKernel * 255);
	waitKey(0);

	return 0;


}