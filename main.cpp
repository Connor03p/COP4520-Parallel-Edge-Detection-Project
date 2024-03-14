#include "headers/convolution.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include <string>

using namespace cv;

/*
 * Pad image it zeros on all borders
 * This is so we can convolve with 3x3 kernal
 */
Mat fillWithZeros(Mat image)
{
    Mat padded;
    int padding = 3;

    padded.create(image.rows + 2 * padding, image.cols + 2 * padding, image.type());
    padded.setTo(cv::Scalar::all(0));
    image.copyTo(padded(Rect(padding, padding, image.cols, image.rows)));
    return padded;
}

int main()
{
    // sets the dir where we search for photos using "samples::findFile()"
    samples::addSamplesDataSearchPath("/src/input_imgs");

    std::string inputImagePath = samples::findFile("sample.png");
    Mat inputImage = imread(inputImagePath, IMREAD_GRAYSCALE);

    Mat padded = fillWithZeros(inputImage);

    if (inputImage.empty())
    {
        std::cout << "Image was not loaded properly" << std::endl;
    }

    imshow("Display Window", padded);
    waitKey(0); // wait for a keystroke in the window

    // TODO: get to the multithreading sobel part after exam
    // TODO: implement counter class and thread class

    return 0;
}
