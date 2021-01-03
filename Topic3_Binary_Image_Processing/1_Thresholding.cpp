#include "BinaryImageProcessing.h"

// create a thresholding algorithm from scratch
void thresholdingUsingForLoop(Mat src, Mat dst, int thresh, int maxValue) {
    int height = src.size().height;
    int width = src.size().width;

    // Loop over rows
    for (int i = 0; i < height; i++) {
        // Loop over columns
        for (int j = 0; j < width; j++) {
            if (src.at<uchar>(i, j) > thresh)
                dst.at<uchar>(i, j) = maxValue;
            else
                dst.at<uchar>(i, j) = 0;
        }
    }
}


int Thresholding() {
    // Read an image in grayscale
    string imagePath = "resources/threshold.png";
    Mat src = imread(imagePath, IMREAD_GRAYSCALE);
    Mat dst = src.clone();
    clock_t t;
    double cpu_time_used;

    // Set threshold and maximum value
    int thresh = 100;
    int maxValue = 255;
    imshow("image", src);
    waitKey(0);

    // Estimate the operating time
    t = clock();
    thresholdingUsingForLoop(src, dst, thresh, maxValue);
    t = clock() - t;
    cpu_time_used = ((double)t) / CLOCKS_PER_SEC;
    cout << "Time taken = " << cpu_time_used << endl;
    imshow("image", dst);           // The result of binary thresholding
    waitKey(0);

    // Estimate the operating time of OpenCV API
    t = clock();
    threshold(src, dst, thresh, maxValue, THRESH_BINARY);
    t = clock() - t;
    cpu_time_used = ((double)t) / CLOCKS_PER_SEC;
    cout << "Time taken = " << cpu_time_used << endl;

    imshow("image", dst);
    waitKey(0);

    //
    clock_t t_loop = 0, t_opencv = 0;
    double time_opencv = 0;
    double time_loops = 0;
    double n_samples = 10.0;

    for (int i = 0; i < (int)n_samples; i++) {
        t = clock();
        thresholdingUsingForLoop(src, dst, thresh, maxValue);
        t = clock() - t;
        t_loop += t;

        t = clock();
        threshold(src, dst, thresh, maxValue, THRESH_BINARY);
        t = clock() - t;
        t_opencv += t;
    }

    time_opencv = t_opencv / (n_samples * CLOCKS_PER_SEC);
    time_loops = t_loop / (n_samples * CLOCKS_PER_SEC);
    cout << "Average time taken by For Loop Code = " << time_loops << " seconds" << endl;
    cout << "Average time taken by OpenCV Code = " << time_opencv << " seconds" << endl;



    //  check all bianry image processing
    thresh = 100;
    maxValue = 150;
    Mat dst_bin, dst_bin_inv, dst_trunc, dst_to_zero, dst_to_zero_inv;


    threshold(src, dst_bin, thresh, maxValue, THRESH_BINARY);
    imshow("image", dst_bin);
    waitKey(0);

    threshold(src, dst_bin_inv, thresh, maxValue, THRESH_BINARY_INV);
    imshow("image", dst_bin_inv);
    waitKey(0);

    threshold(src, dst_trunc, thresh, maxValue, THRESH_TRUNC);
    imshow("image", dst_trunc);
    waitKey(0);

    threshold(src, dst_to_zero, thresh, maxValue, THRESH_TOZERO);
    imshow("image", dst_to_zero);
    waitKey(0);

    threshold(src, dst_to_zero_inv, thresh, maxValue, THRESH_TOZERO_INV);
    imshow("image", dst_to_zero_inv);
    waitKey(0);

    return 0;
}
