/*
	In this code, we will dive into some basic mathemathical operations which are havily used 
	in all image processing and computer vision applications. Moreover, a few pitfalls in OpenCV
	will be discover as well.

	Contrast:   Io = £\I
	Brightness: Io = I+b
*/

#include "ImgManipulate.h"

int Mathematical_Operation() {

	//------------ Datatype Conversion ------------
	/*
	an image is represented as a numpy array once it's read using cv2.imread function. 
	The default datatype of this object is unsigned integer (8 bits). 
	For most Arithmentic operations like multiplication or division,
	we need to convert the array type to floating point with each pixel having 16, 32 or 64 bits of data. 
	This helps prevent overflow while working with the images.

	Most of the time we dont need to work using float64 and float32 
	is enough for most image processing operations ( even float16 )
	*/
	double scalingFactor = 1 / 255.0;  // normalization
	double shift = 0;
	Mat image = imread("img/boy.jpg");

	image.convertTo(image, CV_32FC3, scalingFactor, shift);		   //Converting from unsigned char to float(32bit)
	cout << "Value at (0,0) = " << image.at<Vec3f>(0, 0) << endl;  // access a single pixel (row, column).
	image.convertTo(image, CV_8UC3, 1.0 / scalingFactor, -shift / scalingFactor); 	//Converting from float to unsigned char
	cout << "Value at (0,0) = " << image.at<Vec3b>(0, 0) << endl;  // access a single pixel (row, column).


	//------------ Contrast Enhancement ------------
	/*
	Many definitions of contrast exist in literature. Without going into the gory details, roughly speaking it is the difference in intensity between the brightest and darkest regions of a given image. 
	Higher the difference, higher is the contrast. The maximum contrast of an image is also known as Dynamic Range. In an image with good contrast, objects are easily discernible.

	One of the easiest ways to improve contrast of an image is Intensity Scaling.
	In this approach, a scale factor (  £\  ) is multiplied with intensity values of all the pixels. Given below is the code snippet to do the same.
	
	Intensity scaling is represented by the following equation
	Io=£\I
	*/
	double contrastPercentage = 30.0;
	Mat contrastHigh = image.clone();
	contrastHigh.convertTo(contrastHigh, CV_64F, scalingFactor, shift);
	contrastHigh = contrastHigh * (1 + (contrastPercentage / 100.0)); // increase the level of intensity
	imshow("image", image);
	imshow("contrastHigh", contrastHigh);
	waitKey(0);
	destroyAllWindows();


	//------------ Brightness Enhancement ------------
	int brightnessOffest = 50;
	Mat brightHigh = image.clone();
	Mat brightHighChannels[3];
	split(brightHigh, brightHighChannels);

	for (int i = 0; i < 3; i++) {
		add(brightHighChannels[i], brightnessOffest, brightHighChannels[i]);
	}

	merge(brightHighChannels, 3, brightHigh);
	imshow("image", image);
	imshow("brightHigh", brightHigh);
	waitKey(0);
	destroyAllWindows();

	double minVal, maxVal;
	cout << "Original Image Datatype : " << type2str(image.type()) << endl;
	cout << "Brightness Image Datatype : " << type2str(brightHigh.type()) << endl;

	minMaxLoc(image, &minVal, &maxVal);
	cout << "Original Image Highest Pixel Intensity : " << maxVal << endl;
	cout << "Original Image Lowest Pixel Intensity : " << minVal << endl;
	minMaxLoc(brightHigh, &minVal, &maxVal);
	cout << "Brightness Image Highest Pixel Intensity : " << maxVal << endl;
	cout << "Original Image Loweset Pixel Intensity : " << minVal << endl;



	//------------ Pitfalls of using unit8 ------------
	uint8_t data[] = { 100, 110, 120, 130 };
	Mat a(2, 2, CV_8UC1, data);
	cout << a << endl;
	cout << a + 130 << endl;     // the output is clippied at 255.
	cout << a - 130 << endl;     // the output is clippied at 0.  
	// solution 1: use OpenCV to handle unit8 arithmetic.
	Mat addition;
	add(a, Scalar(130), addition);
	cout << addition << endl;
	// solution 2a: convert to int
	int dataInt[] = { 100,110,120,130 };
	Mat b(2, 2, CV_32SC1, dataInt);
	b = b + 130;
	cout << b << endl;
	b.convertTo(b, CV_8UC1);
	cout << b << endl;
	// solution 2b: convert to normalized float32/float64
	/*
	It is a good practice to convert the uint8 to float and normalize the range to [0,1] 
	and change it back to [0,255] after doing all arithmetic operations
	*/
	Mat a_float32;
	a.convertTo(a_float32, CV_32F, 1.0 / 255.0, 0);
	a_float32 = a_float32 + 130.0 / 255.0;
	cout << a_float32 << endl;
	Mat c;
	a_float32.convertTo(c, CV_32F, 255.0, 0.0);
	cout << c << endl;
	// Clipped output
	Mat b_uint8;
	c.convertTo(b_uint8, CV_8UC1);
	cout << b_uint8;

	return 0;
}