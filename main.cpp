#include "headers/localUtil.hpp"
#include "headers/Convolution.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include <iostream>
#include <fstream>

using namespace cv;

// TODO: remove later, this was just for testing
// Each thread will do an interation of this for loop
// save this for reference later, won't be used in the final
Mat testWrapper(Mat &image)
{
    Convolution conv;
    Mat magnitude(image.rows, image.cols, CV_8UC1);

    // TODO: figure out how to remove this border from magnitude
    // other then that, threads should be able to run this code individually and fill in magnitude
    // consider doing the threshold part as well within the cycle of a thread
    // the way i get indicies here anyway will change when multithreading since some things need
    // to be shared

    for (int i = 1; i < image.rows - 1; i++)
    {
        for (int j = 1; j < image.cols - 1; j++)
        {
            uchar mag = conv.performSobelOnPatch(image, i, j);
            magnitude.at<uchar>(i, j) = mag;
        }
    }

    return magnitude;
}

int main()
{
    Mat image = localUtil::loadImageFromFile("sample.png", cv::ImreadModes::IMREAD_GRAYSCALE);

    imshow("Image", image);
    waitKey(0); // wait for a keystroke in the window

    // TODO: implement counter class and thread class

    return 0;
}
