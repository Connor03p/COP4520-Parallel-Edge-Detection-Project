#include "headers/convolution.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include <string>

using namespace cv;

int main()
{
    // sets the dir where we search for photos using "samples::findFile()"
    samples::addSamplesDataSearchPath("/src/input_imgs");

    std::string inputImagePath = samples::findFile("sample.png");
    Mat inputImage = imread(inputImagePath, IMREAD_GRAYSCALE);

    if (inputImage.empty())
    {
        std::cout << "Image was not loaded properly" << std::endl;
    }

    imshow("Display Window", inputImage);
    waitKey(0); // wait for a keystroke in the window

    // TODO: get to the multithreading sobel part after exam
    // TODO: implement counter class and thread class

    return 0;
}