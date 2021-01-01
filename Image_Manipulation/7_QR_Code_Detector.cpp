/*
Ref:
	https://www.learnopencv.com/opencv-qr-code-scanner-c-and-python/
	https://www.learnopencv.com/barcode-and-qr-code-scanner-using-zbar-and-opencv/
*/

#include "ImgManipulate.h"

void display(Mat& im, vector<Point>& bbox)
{
	for (int i = 0; i < bbox.size(); i++){
		line(im, bbox[i], bbox[(i + 1) % bbox.size()], Scalar(255, 0, 0), 3);
	}
}

int QR_Code_Detector() {
	

	//-------------------- Step 1. Read image and store it in variable img --------------------
	string imgPath = "img/QR_Code_Detector/IDCard-Satya.png";
	Mat img = imread(imgPath);
	cout << img.size().height << " " << img.size().width << endl;    // output: 204 324
	imshow("img", img);
	waitKey(0);
	destroyAllWindows();

	// Create a QRCodeDetector Object
	QRCodeDetector qrDecoder = QRCodeDetector::QRCodeDetector();
	vector<Point> bbox;  // positions of qr code
	Mat	rectifiedImage;  // qr code image

	//-------------------- Detect QR Code in the Image --------------------
	string opencvData = qrDecoder.detectAndDecode(img, bbox, rectifiedImage);

	// Check if a QR Code has been detected
	if (opencvData.length() > 0) {
		cout << "QR Code Detected" << endl;
		cout << opencvData << endl;
	}
	else
		cout << "QR Code NOT Detected" << endl;

	//-------------------- Step 3. draw bounding box around the detected QR code --------------------
	display(img, bbox);
	rectifiedImage.convertTo(rectifiedImage, CV_8UC3);
	imshow("Rectified QRCode", rectifiedImage);
	waitKey(0);

	//-------------------- Step 4. print the decoded text --------------------
	// Write the result image
	string resultImagePath = "img/QR_Code_Detector/QRCode-Output.png";
	imwrite(resultImagePath, img);
	imshow("Result Image", img);
	waitKey(0);
	destroyAllWindows();

	return 0;
}