/*
	Note:
	  1. Dilation is used to merge or expand white regions which may be close to each other and
	  2. Erosion is used to separate or shrink white regions
*/
#include "BinaryImageProcessing.h"

int ErosionDilation() {
	string imageName = "resources/dilation_example.jpg";
	int kSize1 = 7, kSize2 = 3;			// the size of kernel
	Mat kernel1 = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kSize1, kSize1));
	Mat kernel2 = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kSize2, kSize2));

	//------------- Dilation -------------
	Mat image = imread(imageName, IMREAD_COLOR);
	Mat imageDilated;
	Mat scaledImage;
	imshow("image", image);				// show original image
	waitKey(0);

	// dilate by kernel 1
	resize(kernel1, scaledImage, Size(500, 500), INTER_LINEAR);
	dilate(image, imageDilated, kernel1);	
	imshow("image", imageDilated);						// show the dilated image
	imshow("structuring element", scaledImage * 255);	// show structuring element
	waitKey(0);
	destroyAllWindows();

	// dilate by kernel 2
	Mat imageDilated1, imageDilated2;
	resize(kernel2, scaledImage, Size(500, 500), INTER_LINEAR);
	dilate(image, imageDilated1, kernel2, Point(-1, -1), 1);	// dilate once
	dilate(image, imageDilated2, kernel2, Point(-1, -1), 2);	// dilate twice

	imshow("image(dilate once)", imageDilated1);
	imshow("image(dilate twice)", imageDilated2);
	imshow("structuring element", scaledImage * 255);   // convert the value from 0~1 to 0~255
	waitKey(0);
	destroyAllWindows();


	//------------- Erosion -------------
	imageName = "resources/erosion_example.jpg";
	image = imread(imageName, IMREAD_COLOR);
	imshow("image", image);
	waitKey(0);

	Mat imageEroded;
	// First parameter is the original image, second is the eroded image  
	erode(image, imageEroded, kernel1);
	imshow("image", imageEroded);
	waitKey(0);

	return 0;
}