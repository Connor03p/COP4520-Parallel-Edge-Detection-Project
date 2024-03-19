#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include <iostream>
#include <fstream>
#include "headers/localUtil.hpp"
#include "headers/MultithreadedSobel_V1.hpp"
#include "headers/MultithreadedSobel_V2.hpp"

using namespace cv;

int main()
{

    Mat image = localUtil::loadImageFromFile("sample.png", cv::ImreadModes::IMREAD_GRAYSCALE);
    MultithreadedSobel_V2 sobel(1, 90);

    // TODO: this is used to time the function
    auto start = std::chrono::high_resolution_clock::now();
    Mat result = sobel.performSobelEdgeDetection(image);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    double executionTime = duration.count();

    std::cout << "Execution Time: " << executionTime << " seconds" << std::endl;

    imwrite("/src/output_imgs/yooo.png", result);
    // imshow("Image", retval);
    waitKey(0); // wait for a keystroke in the window

    // TODO: implement counter class and thread class

    return 0;
}
