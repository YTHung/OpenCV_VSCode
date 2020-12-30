#include "ImgManipulate.h"

void display(Mat& im, vector<Point>& bbox)
{
	for (int i = 0; i < bbox.size(); i++){
		line(im, bbox[i], bbox[(i + 1) % bbox.size()], Scalar(255, 0, 0), 3);
	}
	imshow("Result of Bounding Box", im);
}

int QR_Code_Detector() {
	
	// Image Path
	string imgPath = "img/QR_Code_Detector/IDCard-Satya.png";

	// Read image and store it in variable img
	///
	/// YOUR CODE HERE
	///
	Mat img = imread(imgPath);
	imshow("img", img);
	waitKey(0);
	destroyAllWindows();

	cout << img.size().height << " " << img.size().width << endl;    // output: 204 324


	vector<Point> bbox;
	Mat	rectifiedImage;  // positions of qr code, qr code image

	// Create a QRCodeDetector Object
	// Variable name should be qrDecoder
	///
	/// YOUR CODE HERE
	///
	QRCodeDetector qrDecoder = QRCodeDetector::QRCodeDetector();

	// Detect QR Code in the Image
	// Output should be stored in opencvData
	///
	/// YOUR CODE HERE
	///
	string opencvData = qrDecoder.detectAndDecode(img, bbox, rectifiedImage);

	// Check if a QR Code has been detected
	if (opencvData.length() > 0) {
		cout << "QR Code Detected" << endl;
		cout << opencvData << endl;
	}
	else
		cout << "QR Code NOT Detected" << endl;

	//-------------------- Step 3. draw bounding box around the detected QR code --------------------

	// Draw the bounding box
	///
	/// YOUR CODE HERE
	///
	display(img, bbox);
	rectifiedImage.convertTo(rectifiedImage, CV_8UC3);
	imshow("Rectified QRCode", rectifiedImage);
	waitKey(0);


	//-------------------- Step 4. print the decoded text --------------------
	// Since we have already detected and decoded the QR Code
	// using qrDecoder.detectAndDecode, we will directly
	// use the decoded text we obtained at that step (opencvData)

	cout << "QR Code Detected!" << endl;
	///
	/// YOUR CODE HERE
	///

	// Write the result image
	string resultImagePath = "img/QR_Code_Detector/QRCode-Output.png";

	///
	/// YOUR CODE HERE
	///





	return 0;
}