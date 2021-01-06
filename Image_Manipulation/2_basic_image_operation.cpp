/*
In this code, we will learn about some basic operations on images which are used very extensively for any computer vision project.
In particular, we will see the following:

1. Copying and creating new images
2. Cropping an image section
3. Resizing an Image
4. Creating binary masks for images

*/

#include "ImgManipulate.h"

int Basic_Image_Operation() {
	
	// clone the image
	Mat image = imread("img/boy.jpg");
	Mat imageCopy = image.clone();
	imshow("boy", image);
	imshow("boyCopy", imageCopy);
	waitKey(0);
	destroyAllWindows();

	// Create an empty matrix
	Mat emptyMatrix = Mat(100, 200, CV_8UC3, Scalar(0, 0, 0));  // create a 100x200, 8bits, 3 channels, black image
	imshow("emptyMatrix", emptyMatrix);
	waitKey(0);
	destroyAllWindows();

	// set empty matrix as white image
	emptyMatrix.setTo(Scalar(255, 255, 255));
	imshow("emptyMatrix", emptyMatrix);
	waitKey(0);
	destroyAllWindows();

	// create an empty matrix which is same as original image
	Mat emptyOriginal = Mat(emptyMatrix.size(), emptyMatrix.type(), Scalar(100, 100, 100));
	imshow("emptyOriginal", emptyOriginal);
	waitKey(0);
	destroyAllWindows();


	// Extracting the roi by specifying the range
	// Crop out a rectangle. X coordinates = 170 to 320, Y coordinate = 40 to 200
	Mat cropFace = image(Range(40, 200), Range(170, 320));
	imshow("FaceImg", cropFace);
	waitKey(0);
	destroyAllWindows();

	// Copy the facial region to another place
	int roiHeight = cropFace.size().height;
	int roiWidth = cropFace.size().width;
	cropFace.copyTo(imageCopy(Range(40, 40 + roiHeight), Range(10, 10 + roiWidth)));
	cropFace.copyTo(imageCopy(Range(40, 40 + roiHeight), Range(330, 330 + roiWidth)));
	imshow("imageCopy", imageCopy);
	waitKey(0);
	destroyAllWindows();


	// Resize an image
	// method 1: specify width and height
	int resizeDownWidth = 300;
	int resizeDownHeight = 200;
	int resizeUpWidth = 600;
	int resizeUpHeight = 900;
	Mat resizeDown, resizeUp;
	resize(image, resizeDown, Size(resizeDownWidth, resizeDownHeight), 0, 0, INTER_LINEAR);
	resize(image, resizeUp, Size(resizeUpWidth, resizeUpHeight), 0, 0, INTER_LINEAR);
	imshow("image", image);
	imshow("image scaled up", resizeUp);
	imshow("image scaled down", resizeDown);
	waitKey(0);
	destroyAllWindows();
	// method 2: specify scaling fac
	double scaleUpX = 1.5; 	// Scaling Down the image 1.5 times by specifying both scaling factors
	double scaleUpY = 1.5;  // Scaling Down the image 1.5 times by specifying both scaling factors
	double scaleDown = 0.6; // Scaling Down the image 0.6 times specifying a single scale factor.
	Mat scaledUp, scaledDown;
	resize(image, scaledDown, Size(), scaleDown, scaleDown, INTER_LINEAR);  // Size() : null
	resize(image, scaledUp, Size(), scaleUpX, scaleUpY, INTER_LINEAR);
	imshow("image scaled up", scaledUp);
	imshow("image scaled down", scaledDown);
	waitKey(0);
	destroyAllWindows();



	//------------------- Create an image mask -------------------
	/*
		Note:
		In many computer image applications, we're not processing entire image but only parts of image. 
		Sometimes it could be rectanglar, but most of the time it's not. So we use image mask to specify 
		the region of interest that we're going to precess. 
		The goal is to segment out the area of interest and apply your algorithm to a specific part of the image.
		Usually, binary mask is used, where white pixcels are the region that we're interested in processing, 
		and black represent the pixels we are not.
	*/

	// Create a mask using coordinates
	Mat mask1 = Mat::zeros(image.size(), image.type());  // Create an empty image of same size as the original
	imshow("mask1", mask1);
	waitKey(0);
	mask1(Range(50, 200), Range(170, 320)).setTo(255);   // Set roi
	imshow("mask1", mask1);
	waitKey(0);
	destroyAllWindows();

	// Create a mask using pixel intensity or colour
	/*
		Another way of creating masks is by using some logic. One example is using a simple color information. 
		For example, Let us try to find out the pixels which are approximately red in color.

		If we want to focus on red pixels, the simplest logic that does the trick is:

		* The red channel should have high intensity ( keep the range of pixel values from 150 to 255 )
		* The other 2 channels should have low intensity ( keep the range of pixel values in Blue and Green channels between 0 to 100)
	*/
	Mat mask2;
	inRange(image, Scalar(0, 0, 150), Scalar(100, 100, 255), mask2); // lower boundary, upper boundary
	imshow("mask2", mask2);
	waitKey(0);
	destroyAllWindows();

	// As you can see the white portion in the output mask corressponds to the red pixels in the original image.



	return 0;
}