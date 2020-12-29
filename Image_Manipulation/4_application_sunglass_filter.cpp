#include "ImgManipulate.h"

int Sunglass_filter() {

	// load images
	string faceImagePath = "img/musk.jpg";
	string glassImagePath = "img/sunglass.png";
	Mat faceImage = imread(faceImagePath);
	Mat glassPNG = imread(glassImagePath, -1);  // load with transparent parameter
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

	// find the eye region
	/*
	the eye region lies approximately from row #150 to row #250 and col #140 to col #440. 
	Thus, this will be region where the sunglass is to be placed.
	*/

	// using naive replace
	// Make a copy
	Mat faceWithGlassesNaive = faceImage.clone();
	Mat roiFace = faceWithGlassesNaive(Range(150, 250), Range(140, 440));
	// Replace the eye region with the sunglass image
	glassBGR.copyTo(roiFace);
	imwrite("img/results/faceWithGlassesNaive.png", faceWithGlassesNaive);
	imshow("faceWithGlassesNaive", faceWithGlassesNaive);
	waitKey(0);
	destroyAllWindows();


	// Using Arithmetic Operations and Alpha Mask
	/*
	In order to put the sunglass on top of the eye region, we need to follow these steps:

	1. Create a binary mask with 3-channels using the single channel mask.
	2. Extract the eye region from the face image
	3. Multiply the Mask with the sunglass to get the masked sunglass
	4. Multiply the negative of Mask with the eye region to create a hole in the eye region for the sunglass to be placed.
	5. Add the masked sunglass and eye regions to get the combined eye region with the sunglass.
	6. Replace the eye region in the original image with that of the output we got in the previous step. This is the final output
	*/
	
	// Make the dimensions of the mask same as the input image.
	// Since Face Image is a 3-channel image, we create a 3 channel image for the mask
	Mat glassMask;
	Mat glassMaskChannels[] = { glassMask1,glassMask1,glassMask1 };
	merge(glassMaskChannels, 3, glassMask);
	// Make the values [0,1] since we are using arithmetic operations
	glassMask = glassMask / 255;
	// Make a copy
	Mat faceWithGlassesArithmetic = faceImage.clone();
	// Get the eye region from the face image
	Mat eyeROI = faceWithGlassesArithmetic(Range(150, 250), Range(140, 440));

	Mat eyeROIChannels[3];     // used for containing the RGE value of ROI
	Mat maskedEyeChannels[3];  // used for containing RGB value applied the masks.
	Mat maskedEye;             // a masked eye image

	split(eyeROI, eyeROIChannels);
	for (int i = 0; i < 3; i++){
		// Use the mask to create the masked eye region
		multiply(eyeROIChannels[i], (1-glassMaskChannels[i]), maskedEyeChannels[i]);
	}
	merge(maskedEyeChannels, 3, maskedEye);

	Mat maskedGlass;
	Mat eyeRoiFinal;
	multiply(glassBGR, glassMask, maskedGlass); // Use the mask to create the masked sunglass region
	add(maskedEye, maskedGlass, eyeRoiFinal); 	// Combine the Sunglass in the Eye Region to get the augmented image
	imwrite("img/results/maskedEyeRegion.png", maskedEye);
	imwrite("img/results/maskedSunglassRegion.png", maskedGlass);
	imwrite("img/results/augmentedEyeAndSunglass.png", eyeRoiFinal);
	imshow("maskedEye", maskedEye);
	imshow("maskedGlass", maskedGlass);
	imshow("eyeRoiFinal", eyeRoiFinal);
	waitKey(0);
	destroyAllWindows();

	// Replace the eye ROI with the output from the previous section
	eyeRoiFinal.copyTo(eyeROI);
	imwrite("ing/results/withSunglasses.png", faceWithGlassesArithmetic);
	imshow("eyeRoiFinal", faceWithGlassesArithmetic);
	waitKey(0);
	destroyAllWindows();

	return 0;
}
