#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include <iostream>
#include <fstream>
#include "headers/localUtil.hpp"
#include "headers/MultithreadedSobel_V1.hpp"
#include "headers/MultithreadedSobel_V2.hpp"

using namespace cv;

// Kernel for Sobel operator
const int sobelX[3][3] = {{-1, 0, 1}, {2, 0, -2}, {1, 0, -1}};
const int sobelY[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

// Function to perform Sobel edge detection
void sobelEdgeDetection(Mat &image, Mat &output)
{
    int numRows = image.rows;
    int numCols = image.cols;
    int magnitude, threshold = 130;

    // Apply Sobel operator to each pixel in the image
    for (int i = 1; i < numRows - 1; ++i)
    {
        for (int j = 1; j < numCols - 1; ++j)
        {
            int gx = 0, gy = 0;

            // Convolution with Sobel kernels
            for (int k = -1; k <= 1; ++k)
            {
                for (int l = -1; l <= 1; ++l)
                {
                    // gx += image[i + k][j + l] * sobelX[k + 1][l + 1];
                    // gy += image[i + k][j + l] * sobelY[k + 1][l + 1];
                    gx += image.at<uchar>(i + k, j + l) * sobelX[k + 1][l + 1];
                    gy += image.at<uchar>(i + k, j + l) * sobelY[k + 1][l + 1];
                }
            }

            // Compute gradient magnitude
            magnitude = std::sqrt(gx * gx + gy * gy);

            // threshold
            if (magnitude > threshold)
            {
                output.at<uchar>(i, j) = 255;
            }
            else
            {
                output.at<uchar>(i, j) = 0;
            }
        }
    }
}

void testNormalSobel()
{

    std::vector<std::string> photos{"sample1.png", "sample2.png", "sample3.png"};

    for (std::string filename : photos)
    {
        std::string path = "/src/output_imgs/" + filename;
        Mat image = localUtil::loadImageFromFile(filename, cv::ImreadModes::IMREAD_GRAYSCALE);
        Mat output(image.size(), image.type());
        double avgTime = 0;

        for (int test = 0; test < 25; test++)
        {
            auto start = std::chrono::high_resolution_clock::now();
            sobelEdgeDetection(image, output);
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> duration = end - start;
            double runtime = duration.count();
            avgTime += runtime;
        }

        std::cout << "Result size: " << output.size() << std::endl;
        std::cout << "Threshold value: " << 130 << std::endl;
        std::cout << "Average Time: " << (avgTime / 25) << "s" << std::endl;
        std::cout << "\n";

        imwrite(path, output);
    }
}

int main()
{

    std::string filename = "sample3.png";
    std::vector<std::string> photos{"sample1.png", "sample2.png", "sample3.png"};

    for (std::string filename : photos)
    {
        int numThreads = 1;
        int threshold = 130;
        std::string path = "/src/output_imgs/" + filename;
        Mat image = localUtil::loadImageFromFile(filename, cv::ImreadModes::IMREAD_GRAYSCALE);
        MultithreadedSobel_V2 sobel(numThreads, threshold);
        Mat result;
        int numTest = 15;
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

            std::cout << "METHOD B" << std::endl;
            std::cout << "Result size: " << result.size() << std::endl;
            std::cout << "Number Of Test: " << numTest << std::endl;
            std::cout << "Threshold value: " << threshold << std::endl;
            std::cout << "Number Threads: " << numThreads << std::endl;
            std::cout << "Average Time: " << (avgTime / numTest) << "s" << std::endl;
            std::cout << "\n"
                      << std::endl;

            imwrite(path, result);
            numThreads *= 2;
        }

        std::cout << "\n"
                  << std::endl;
    }

    // localUtil::writeImageToTxt(result, "mountain");
    // imshow("Image", retval);

    waitKey(0); // wait for a keystroke in the window

    // TODO: implement counter class and thread class

    return 0;
}
