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

	return 0;
}