#include "headers/localUtil.hpp"
#include "headers/convolution.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include <iostream>
#include <fstream>

using namespace cv;

int main()
{
    // sets the dir where we search for photos using "samples::findFile()"
    samples::addSamplesDataSearchPath("/src/input_imgs");

    std::string inputImagePath = samples::findFile("sample_256x256.png");
    Mat inputImage = imread(inputImagePath, IMREAD_GRAYSCALE);

    if (inputImage.empty())
    {
        std::cout << "Image was not loaded properly" << std::endl;
    }

    Mat padded = localUtil::fillWithZeros(inputImage);

    localUtil::writeImageToTxt(inputImage, "Regular");
    localUtil::writeImageToTxt(padded, "padded");

    imshow("Display Window", padded);
    waitKey(0); // wait for a keystroke in the window

    // TODO: get to the multithreading sobel part after exam
    // TODO: implement counter class and thread class

    return 0;
}
