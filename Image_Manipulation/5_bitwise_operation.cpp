/*
	Bitwise operation is highly useful while extracting any part of the image (as we will see in coming modules), 
	defining and working with non-rectangular ROI etc.

	In this code, We will follow the same steps of sunglass filter. But instead of arithmetic operations like MULTIPLY & ADD ,
	we will use bitwise operations like AND, NOT and OR.

	Bitwise operatoin and faster than arithmetic operation but less accurate than the arithmetic operation.
*/

#include "ImgManipulate.h"

int Bitwise_Operation() {

	// load images
	string faceImagePath = "img/musk.jpg";
	string glassImagePath = "img/sunglass.png";
	Mat faceImage = imread(faceImagePath);
	Mat glassPNG = imread(glassImagePath, -1);  // load with transparent parameter
	Mat faceWithGlassesBitwise = faceImage.clone(); 	// Make a copy

	resize(glassPNG, glassPNG, Size(300, 100));
	cout << "glassPNG Dimension = " << glassPNG.size() << endl;
	cout << "glassPNG Number of channels = " << glassPNG.channels();
	imshow("glassPNG", glassPNG);
	imshow("faceImage", faceImage);
	waitKey(0);
	destroyAllWindows();

	// separate the color and alpha channels
	Mat glassRGBAChannels[4];
	Mat glassRGBChannels[3];
	split(glassPNG, glassRGBAChannels);
	for (int i = 0; i < 3; i++) {
		glassRGBChannels[i] = glassRGBAChannels[i]; // Copy R,G,B channel from RGBA to RGB
	}

	Mat glassBGR, glassMask1;
	merge(glassRGBChannels, 3, glassBGR); // Prepare BGR Image
	glassMask1 = glassRGBAChannels[3]; 	  // Alpha channel is the 4th channel in RGBA Image
	imwrite("img/results/sunglassRGB.png", glassBGR);
	imwrite("img/results/sunglassAlpha.png", glassMask1);
	imshow("glassBGR", glassBGR);
	imshow("glassMask1", glassMask1);
	waitKey(0);
	destroyAllWindows();


	// --------------------- Get the masked outputs ---------------------
	// Get the eye region from the face image
	Mat eyeROI = faceWithGlassesBitwise(Range(150, 250), Range(140, 440));

	// Make the dimensions of the mask same as the input image.
	// Since Face Image is a 3-channel image, we create a 3 channel image for the mask
	Mat glassMask;
	Mat glassMaskChannels[] = { glassMask1,glassMask1,glassMask1 };
	merge(glassMaskChannels, 3, glassMask);

	// Use the mask to create the masked eye region
	Mat eye;
	Mat glassMaskNOT;
	bitwise_not(glassMask1, glassMaskNOT);
	imshow("glassMaskNOT", glassMaskNOT);
	imwrite("img/results/glassMaskNOT.png", glassMaskNOT);
	waitKey(0);
	destroyAllWindows();


	Mat eyeChannels[3];
	Mat eyeROIChannels[3];
	Mat maskedGlass;
	Mat eyeRoiFinal;

	split(eyeROI, eyeROIChannels);
	for (int i = 0; i < 3; i++){
		bitwise_and(eyeROIChannels[i], glassMaskNOT, eyeChannels[i]);
	}
	merge(eyeChannels, 3, eye);


	Mat glassMaskNOTChannels[] = { glassMaskNOT,glassMaskNOT,glassMaskNOT };
	Mat glassMaskNOTMerged;
	// The following code is an alternative way of merging the 3 channels
	// Another way has been shown in the code above (Mat glassMaskNOTChannels[] = ...)
	merge(glassMaskNOTChannels, 3, glassMaskNOTMerged);

	bitwise_and(eyeROI, glassMaskNOTMerged, eye);
	// Use the mask to create the masked sunglass region
	Mat sunglass;
	Mat sunglassChannels[3];
	Mat glassBGRChannels[3];

	split(glassBGR, glassBGRChannels);

	for (int i = 0; i < 3; i++)
		bitwise_and(glassBGRChannels[i], glassMask1, sunglassChannels[i]);

	merge(sunglassChannels, 3, sunglass);
	// The following code is an alternative to the bitwise_and
	// approach we have followed above
	multiply(glassBGR, glassMask, maskedGlass);

	// Combine the Sunglass in the Eye Region to get the augmented image
	bitwise_or(eye, sunglass, eyeRoiFinal);


	imshow("eye", eye);
	imshow("sunglass", sunglass);
	imshow("eyeRoiFinal", eyeRoiFinal);
	waitKey(0);
	destroyAllWindows();


	// Get the final result
	eyeRoiFinal.copyTo(eyeROI);
	imwrite("./results/withSunglassesBitwise.png", faceWithGlassesBitwise);
	imshow("eyeRoiFinal", faceWithGlassesBitwise);
	waitKey(0);
	destroyAllWindows();
	return 0;
}