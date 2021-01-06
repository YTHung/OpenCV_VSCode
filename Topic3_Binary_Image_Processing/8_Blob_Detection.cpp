/*
	if the goal is dectect the blob, the easier way is to threshold the image, and then apply CCA or Contour to the image for finding blobs.
	While the CAA and Contour can finds the general blobs, blob detector helps us to find "particular blobs".


    **** How does Blob detection work?
    SimpleBlobDetector, as the name implies, is based on a rather simple algorithm described below. The algorithm is controlled by parameters ( shown in bold below ) and has the following steps. Scroll down to know how the parameters are set.

    1. Thresholding : Convert the source images to several binary images by thresholding the source image with thresholds starting at minThreshold. These thresholds are incremented by thresholdStep until maxThreshold. So the first threshold is minThreshold, the second is minThreshold + thresholdStep, the third is minThreshold + 2 x thresholdStep, and so on.
    2. Grouping : In each binary image, connected white pixels are grouped together. Let¡¦s call these binary blobs.
    3. Merging : The centers of the binary blobs in the binary images are computed, and blobs located closer than minDistBetweenBlobs are merged.
    4. Center & Radius Calculation : The centers and radii of the new merged blobs are computed and returned.


    Ref:
    https://www.learnopencv.com/blob-detection-using-opencv-python-c/

*/

#include"BinaryImageProcessing.h"

int BlobDetection() {

	Mat img = imread("resources/blob_detection.jpg", IMREAD_GRAYSCALE);
    imshow("image", img);
    waitKey(0);

	// Set up detector with default parameters
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create();  // create an object
	std::vector<KeyPoint> keypoints;
	detector->detect(img, keypoints);	// pass the image to the detector and ask for the keypoints (interesting points of the image)

    // Mark blobs using image annotation concepts we have studied so far
    int x, y;
    int radius;
    double diameter;
    cvtColor(img, img, COLOR_GRAY2BGR);         // convert the image back to the colour image in order to draw using red coolour
    for (int i = 0; i < keypoints.size(); i++) {
        KeyPoint k = keypoints[i];
        Point keyPt;
        keyPt = k.pt;
        x = (int)keyPt.x;
        y = (int)keyPt.y;
        // Mark center in BLACK
        circle(img, Point(x, y), 5, Scalar(255, 0, 0), -1);
        // Get radius of coin
        diameter = k.size;
        radius = (int)diameter / 2.0;
        // Mark blob in GREEN
        circle(img, Point(x, y), radius, Scalar(0, 255, 0), 2);
    }
    imshow("image", img);
    waitKey(0);


    //------------------- Find blobs by colour, size, and shape -------------------
    // Setup SimpleBlobDetector parameters.
    SimpleBlobDetector::Params params;

    // Change thresholds
    params.minThreshold = 10;
    params.maxThreshold = 200;

    // Filter by Area.
    params.filterByArea = true;
    params.minArea = 1500;

    // Filter by Circularity
    params.filterByCircularity = true;
    params.minCircularity = 0.1;

    // Filter by Convexity
    params.filterByConvexity = true;
    params.minConvexity = 0.87;

    // Filter by Inertia
    params.filterByInertia = true;
    params.minInertiaRatio = 0.01;

    detector = SimpleBlobDetector::create(params);
    










	return 0;
}