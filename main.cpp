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

    int numThreads = 1;

    std::string filename = "sample3.png";
    std::string path = "/src/output_imgs/" + filename;
    Mat image = localUtil::loadImageFromFile(filename, cv::ImreadModes::IMREAD_GRAYSCALE);
    MultithreadedSobel_V1 sobel(numThreads, 130);
    Mat result;
    int numTest = 25;
    double avgTime = 0;
    // TODO: this is used to time the function

    for (; numThreads <= 8;)
    {
        for (int test = 0; test < numTest; test++)
        {
            auto start = std::chrono::high_resolution_clock::now();
            result = sobel.performSobelEdgeDetection(image);
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> duration = end - start;
            double executionTime = duration.count();
            avgTime += executionTime;
        }

        std::cout << "Result size: " << result.size() << std::endl;
        std::cout << "Number Threads: " << numThreads << std::endl;
        std::cout << "Average Time: " << (avgTime / numTest) << "s" << std::endl;

        imwrite(path, result);
        numThreads *= 2;
    }

    // localUtil::writeImageToTxt(result, "mountain");
    // imshow("Image", retval);
    waitKey(0); // wait for a keystroke in the window

    // TODO: implement counter class and thread class

    return 0;
}
