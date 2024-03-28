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
    std::string filename = "sample1.png";
    std::string path = "/src/output_imgs/" + filename;

    Mat image = localUtil::loadImageFromFile(filename, cv::ImreadModes::IMREAD_GRAYSCALE);
    MultithreadedSobel_V2 sobel(1, 90);
    Mat result;
    // TODO: this is used to time the function

    for (int test = 0; test < 10; test++)
    {

        auto start = std::chrono::high_resolution_clock::now();
        result = sobel.performSobelEdgeDetection(image);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;
        double executionTime = duration.count();

        std::cout << "Execution Time: " << executionTime << " seconds" << std::endl;
    }

    imwrite(path, result);
    // localUtil::writeImageToTxt(result, "mountain");
    // imshow("Image", retval);
    waitKey(0); // wait for a keystroke in the window

    // TODO: implement counter class and thread class

    return 0;
}
