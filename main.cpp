#include "headers/localUtil.hpp"
#include "headers/Convolution.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include <iostream>
#include <fstream>

using namespace cv;

void testWrapper(Mat &image)
{
    Convolution conv;
    Mat magnitude(256, 256, CV_8UC1);

    for (int i = 1; i < 256; i++)
    {
        for (int j = 1; j < 256; j++)
        {
            int mag = conv.performConvolutionOnPatch(image, i, j);
            magnitude.at<char>(i, j) = char(mag);
        }
    }

    localUtil::writeImageToTxt(magnitude, "magnitude");
    imshow("display", magnitude);
    waitKey(0);
}

int main()
{
    // sets the dir where we search for photos using "samples::findFile()"
    samples::addSamplesDataSearchPath("/src/input_imgs");

    Convolution conv;

    std::string inputImagePath = samples::findFile("sample_256x256.png");
    Mat inputImage = imread(inputImagePath, IMREAD_GRAYSCALE);

    if (inputImage.empty())
    {
        std::cout << "Image was not loaded properly" << std::endl;
    }

    Mat padded = localUtil::padWithZeros(inputImage);

    testWrapper(padded);

    // imshow("Display Window", padded);
    // waitKey(0); // wait for a keystroke in the window

    // TODO: get to the multithreading sobel part after exam
    // TODO: implement counter class and thread class

    return 0;
}
