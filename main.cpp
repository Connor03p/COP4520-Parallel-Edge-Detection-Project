#include "headers/convolution.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>
// #include <string>
// #include <fstream>
// #include <iostream>

using namespace cv;

/**
 * Writes the pixel values of an image to a text file.
 *
 * @param image The input image.
 * @param filename The name of the text file to write the pixel values to.
 */
// void writeImageToTxtFile(const cv::Mat &image, const std::string &filename)
// {
//     std::ofstream file("mame.txt");

//     file << "wooords" << std::endl;

//     if (file.is_open())
//     {
//         for (int row = 0; row < image.rows; ++row)
//         {
//             for (int col = 0; col < image.cols; ++col)
//             {
//                 file << static_cast<int>(image.at<uchar>(row, col)) << " ";
//             }
//             file << std::endl;
//         }
//         file.close();
//         std::cout << "Image written to " << filename << std::endl;
//     }
//     else
//     {
//         std::cout << "Failed to open file for writing" << std::endl;
//     }
// }

Mat fillWithZeros(Mat image)
{
    Mat padded;
    int padding = 1;

    padded.create(image.rows + 2 * padding, image.cols + 2 * padding, image.type());
    padded.setTo(cv::Scalar::all(0));
    image.copyTo(padded(Rect(padding, padding, image.cols, image.rows)));
    return padded;
}

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

    // Mat padded = fillWithZeros(inputImage);
    // writeImageToTxtFile(padded, "padded");
    // printImage(padded);

    imshow("Display Window", inputImage);
    waitKey(0); // wait for a keystroke in the window

    // TODO: get to the multithreading sobel part after exam
    // TODO: implement counter class and thread class

    return 0;
}
