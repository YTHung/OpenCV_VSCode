/*
	Structuring Element ref: https://www.youtube.com/watch?v=9lqH5XLI-V4
*/

#include "BinaryImageProcessing.h"

int Assignment_writeFromScratch() {

	//----------------------- Create a source image -----------------------
	Mat demoImage = Mat::zeros(Size(10, 10), CV_8U);	// Create a 10 by 10 image
	Mat resizedDemoImg;	   // to store the resized demoImage
	resize(demoImage, resizedDemoImg, Size(100, 100), 0, 0, INTER_NEAREST);
	cout << "Original intensity of image: " << endl << demoImage << endl;
	imshow("intensity of the image", resizedDemoImg);
	waitKey(0);

	// Manipulate the intensity of interest pixels
	demoImage.at<uchar>(0, 1) = 1;
	demoImage.at<uchar>(9, 0) = 1;
	demoImage.at<uchar>(8, 9) = 1;
	demoImage.at<uchar>(2, 2) = 1;
	demoImage(Range(5, 8), Range(5, 8)).setTo(1);
	cout << demoImage << endl;
	resize(demoImage, resizedDemoImg, Size(100, 100), 0, 0, INTER_NEAREST);
	imshow("intensity of the image", resizedDemoImg * 255);
	waitKey(0);

	// Create a cross structuring element
	Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3));	// create a structuring element
	cout << element;
	int ksize = element.size().height;
	int	height = demoImage.size().height;
	int width = demoImage.size().width;
	cout << height << endl;
	cout << width << endl;
	waitKey(0);


	////----------------------- Wrtie code for Dilation from scratch with method 2 -----------------------
	// Check expected output using cv::dilate
	Mat dilatedCrossKernel;
	dilate(demoImage, dilatedCrossKernel, element);
	cout << "Expected output using cv::dilate : " << dilatedCrossKernel << endl;

	//
	int border = ksize / 2;
	Mat paddedDemoImage = Mat::zeros(Size(height + border * 2, width + border * 2), CV_8UC1);
	copyMakeBorder(demoImage, paddedDemoImage, border, border, border, border, BORDER_CONSTANT, 0);

	Mat paddedDilatedImage = paddedDemoImage.clone();
	Mat mask;
	Mat resizedFrame;

	double minVal, maxVal;

	// Create a VideoWriter object
	VideoWriter outavi("output.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(50, 50));
	Mat bitAnd;
	Point minLoc, maxLoc;

	uchar neighborhoodIntensity = 0;  // store the intensity of neighborhood around the kernel.
	for (int h_i = border; h_i < height + border; h_i++) {
		for (int w_i = border; w_i < width + border; w_i++) {

			neighborhoodIntensity = paddedDemoImage.at<uchar>(h_i - border, w_i) ||  // upper
				paddedDemoImage.at<uchar>(h_i, w_i - border) ||  // left
				paddedDemoImage.at<uchar>(h_i, w_i + border) ||  // right
				paddedDemoImage.at<uchar>(h_i + border, w_i);    // lower

			if (neighborhoodIntensity != 0)  // there exist value in neighborhood
			{
				bitwise_and(paddedDemoImage(Range(h_i - border, h_i + border + 1), Range(w_i - border, w_i + border + 1)), element, bitAnd);
				minMaxLoc(bitAnd, &minVal, &maxVal, &minLoc, &maxLoc);  // find the max and min intensity within the region
				paddedDilatedImage.at<uchar>(h_i, w_i) = maxVal;        // store the result

				cout << "Neighborhood White Pixel Found @ kernel position" << h_i << "," << w_i << endl;
				cout << "Result of bitwise and:" << endl << bitAnd << endl;
				cout << "Max Value: " << maxVal << endl;
				cout << "Result of padded and dilated image: " << endl << paddedDilatedImage << endl << endl;

				// video write
				resize(paddedDilatedImage, resizedFrame, Size(50, 50), 0, 0, INTER_NEAREST);
				cvtColor(resizedFrame * 255, resizedFrame, COLOR_GRAY2BGR);
				outavi.write(resizedFrame);
			}
		}
	}

	// Release the VideoWriter object
	outavi.release();

	// Display final image (cropped)
	Mat dilatedImage = paddedDilatedImage(Range(border, border + height), Range(border, border + width));
	imshow("dilated image",dilatedImage * 255);
	waitKey(0);



	////----------------------- Wrtie code for Erosion from scratch with method 2 -----------------------
	// Check expected output using cv::erode
	Mat ErodedCrossKernel;
	erode(demoImage, ErodedCrossKernel, element);
	cout << "Expected output using cv::erode : " << ErodedCrossKernel << endl;

	//
	border = ksize / 2;
	paddedDemoImage = Mat::zeros(Size(height + border * 2, width + border * 2), CV_8UC1);
	copyMakeBorder(demoImage, paddedDemoImage, border, border, border, border, BORDER_CONSTANT, 0);

	Mat paddedErodedImage = paddedDemoImage.clone();

	// Create a VideoWriter object
	VideoWriter outavi_erosion("output_erodedImage.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(50, 50));
	bitwise_not(element * 255, mask);
	cout << "mask: " << endl << mask << endl;

	for (int h_i = border; h_i < height + border; h_i++) {
		for (int w_i = border; w_i < width + border; w_i++) {

			bitwise_and(paddedDemoImage(Range(h_i - border, h_i + border + 1), Range(w_i - border, w_i + border + 1)),
				element, bitAnd);
			bitwise_or(bitAnd, mask, bitAnd);                         // focus on the interest Neighborhood
			minMaxLoc(bitAnd, &minVal, &maxVal, &minLoc, &maxLoc);  // find the max and min intensity within the region
			paddedErodedImage.at<uchar>(h_i, w_i) = minVal;         // store the result

			cout << "Min Value: " << minVal << endl;
			cout << "Result of padded and dilated image: " << endl << paddedErodedImage << endl << endl;

			// video write
			resize(paddedErodedImage, resizedFrame, Size(50, 50), 0, 0, INTER_NEAREST);
			cvtColor(resizedFrame * 255, resizedFrame, COLOR_GRAY2BGR);
			outavi_erosion.write(resizedFrame);
		}
	}

	// Release the VideoWriter object
	outavi_erosion.release();

	// Display final image (cropped)
	Mat erodedImage = paddedErodedImage(Range(border, border + height), Range(border, border + width));
	imshow("eroded image", erodedImage * 255);
	waitKey(0);


	return 0;
}