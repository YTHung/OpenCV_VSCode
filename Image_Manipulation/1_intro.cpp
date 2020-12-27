/*
In this code, we will dive into the code and check out what are the functions available in OpenCV for manipulating images.

We will cover the following:

1. Image I/O - Read, Write & Display an image
2. Image Properties - color, channels, shape, image structure
3. Creating new images, accessing pixels and region of interest (ROI)
*/

#include "ImgManipulate.h"

string type2str(int type) {
    string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch (depth) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
    }

    r += "C";
    r += (chans + '0');

    return r;
}

int intro() {
    cout << "Hello, World!" << endl;
    cout << "OpenCV version is " << CV_VERSION << endl;


    // ------------------------------- Grey Image -------------------------------
    // Reading image as a Matrix
    string imagePath = "Img/number_zero.jpg";
    Mat testImage = imread(imagePath, 0);
    cout << testImage << endl;


    // Image Properties
    cout << "Data Type = " << type2str(testImage.type()) << endl;
    cout << "Data Dimensions = " << testImage.size() << endl;   // width x height
    cout << "Data Channel = " << testImage.channels() << endl;


    // Manipulate pixels
    cout << "Value at (0,0) = " << (int)testImage.at<uchar>(0, 0) << endl;  // access a single pixel (row, column). uchar: 8bits
    testImage.at<uchar>(0, 0) = 200;  // modify pixel value
    cout << testImage << endl;


    // Manipulate group of pixels
    Mat test_roi = testImage(Range(0, 2), Range(0, 4)); // roi: region of interest
    testImage(Range(0, 2), Range(0, 4)).setTo(111);     
    cout << "Modified Matrix\n" << testImage << endl;


    // Display an image
    /*
    Note :
        1. If the image is in float data type, then the range of values should be between 0 and 1.
        2. If the image is in int data type, then the range of values should be between 0 and 255.
    */
    imshow("number zero", testImage);
    waitKey(0);
    destroyAllWindows();


    // Save an image
    imwrite("../Image_Manipulation/Img/test.jpg", testImage);




    // ------------------------------- Colour Image -------------------------------
    /*
    Note:
        In OpenCV, the order of channels R, G and B is reverse. 
        i.e. In the image matrix, the Blue channel is indexed first, 
        followed by the Green Channel and finally the Red Channel.
    */
    imagePath = "img/musk.jpg";
    Mat img = imread(imagePath);
    cout << "Image size = " << img.size() << endl;
    cout << "Image channels = " << img.channels() << endl;  // 3 channels = RGB
    imshow("musk", img);
    waitKey(0);

    // Show Channels
    Mat imgChannels[3]; 
    split(img, imgChannels);
    imshow("B", imgChannels[0]);
    imshow("G", imgChannels[1]);
    imshow("R", imgChannels[2]);
    waitKey(0);
    destroyAllWindows();

    /*
    Observation:
        1. We can see in the original image that the background is blue in color. Thus, the blue channel is also having higher intensity values for the bakground, whereas the red channel is almost black for the background.
        2. The face is reddish in color and thus, the red channel has very high values in the face region, while the other channels are a bit lower.
        3. There is a greenish tint in the original image which is also reflected in the Green channel
    
    Conclusion:
        This kind of information extraction can be very useful if you want to build basic applications
        which take decisions based on color ( more so, using specific color channel )
    */


    // Manipulate colour pixels
    imagePath = "Img/number_zero.jpg";
    testImage = imread(imagePath, 1); // read image as colour image: 3 channels
    cout << "Data Value at (0,0) = " << testImage.at<Vec3b>(0, 0); // the intensity value now has 3 elements - one for each channel (3 bytes data at a pixel)
    // Modify a pixel
    testImage.at<Vec3b>(0, 0) = Vec3b(0, 255, 255);
    namedWindow("Yellow", WINDOW_NORMAL);
    imshow("Yellow", testImage);
    testImage.at<Vec3b>(1, 1) = Vec3b(255, 255, 0);
    namedWindow("Cyan", WINDOW_NORMAL);
    imshow("Cyan", testImage);
    testImage.at<Vec3b>(2, 2) = Vec3b(255, 0, 255);
    namedWindow("Magenta", WINDOW_NORMAL);
    imshow("Magenta", testImage);
    waitKey(0);
    destroyAllWindows();
    // Modify group of pixels
    testImage(Range(0, 3), Range(0, 3)).setTo(Scalar(255, 0, 0));
    testImage(Range(3, 6), Range(0, 3)).setTo(Scalar(0, 255, 0));
    testImage(Range(6, 9), Range(0, 3)).setTo(Scalar(0, 0, 255));
    namedWindow("Img", WINDOW_NORMAL);
    imshow("Img", testImage);
    waitKey(0);
    destroyAllWindows();



    // ------------------------------- Image with Alpha Channel -------------------------------
    imagePath = "Img/panther.png";
    Mat imgPNG = imread(imagePath, -1); // -1: unchanged image, read the image as is.
    cout << "image size = " << imgPNG.size() << endl;
    cout << "number of channels = " << imgPNG.channels() << endl;

    // split and merge the panther image
    Mat imgBGR;
    Mat imgPNGChannels[4];
    split(imgPNG, imgPNGChannels);     // split the data into 4 values - BGR and alpha channels, where alpha ch stores transparency information.
    merge(imgPNGChannels, 3, imgBGR);  // merge the data into one colour image
    Mat imgMask = imgPNGChannels[3];   
    
    imshow("imgBGR", imgBGR);    // show colour only image
    imshow("imgMask", imgMask);  // show alpah mask image
    waitKey(0);
    destroyAllWindows();
    /*
    Note:
    You can see the whiskers very clearly in the mask image. The alpha mask is basically a very accurate segmentation of the image.
    It is useful for creating overlays ( Augmented Reality type of applications ). If you don't have tha alpha mask, 
    then you have to separate out the whiskers from the white background ( see original image above ) which can be very difficult.
    */

    return 0;
}