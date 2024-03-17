#include "headers/localUtil.hpp"
#include "headers/Convolution.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include "headers/MultithreadedSobel.hpp"
#include <iostream>
#include <fstream>

using namespace cv;

// TODO: remove later, this was just for testing
// TODO: add multithreading to this tomorrow
// Each thread will do an interation of this for loop
// save this for reference later, won't be used in the final
// cv::Mat testWrapper(Mat &image)
// {
//     Convolution conv;
//     Mat magnitude(image.rows, image.cols, CV_8UC1);
//     int threshold = 95;

//     // other then that, threads should be able to run this code individually and fill in magnitude
//     // consider doing the threshold part as well within the cycle of a thread
//     // the way i get indicies here anyway will change when multithreading since some things need
//     // to be shared

//     for (int i = 1; i < image.rows - 1; i++)
//     {
//         for (int j = 1; j < image.cols - 1; j++)
//         {
//             magnitude.at<uchar>(i, j) = conv.performSobelOnPatch(image, i, j, threshold);
//         }
//     }

//     return magnitude;
// }

template <typename Func>
double timeFunction(Func function, cv::Mat image)
{
    auto start = std::chrono::high_resolution_clock::now();
    function(image);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    return duration.count();
}

int main()
{

    Mat image = localUtil::loadImageFromFile("sample_256x256.png", cv::ImreadModes::IMREAD_GRAYSCALE);
    MultithreadedSobel sobel(20);

    Mat m1 = sobel.performSobelEdgeDetection(image);

    imwrite("/src/output_imgs/yooo.png", m1);
    // imshow("Image", retval);
    waitKey(0); // wait for a keystroke in the window

    // TODO: implement counter class and thread class

    return 0;
}
