#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int x_kernal[3][3] = {{-1, 0, 1},
                      {-2, 0, 2},
                      {-1, 0, 1}};

int y_kernal[3][3] = {{1, 2, 1},
                      {0, 0, 0},
                      {-1, -2, -1}};

Mat grayscale(const Mat &image)
{
    int numRows = image.rows;
    int numCols = image.cols;
    Mat grayImage = Mat::zeros(numRows, numCols, CV_8UC1);

    for (int i = 0; i < numRows; ++i)
    {
        for (int j = 0; j < numCols; ++j)
        {
            Vec3b intensity = image.at<Vec3b>(i, j);
            grayImage.at<uchar>(i, j) = 0.299 * intensity.val[2] + 0.587 * intensity.val[1] + 0.114 * intensity.val[0];
        }
    }

    return grayImage;
}

Mat sobelEdgeDetection(const Mat &image)
{
    int numRows = image.rows;
    int numCols = image.cols;
    Mat edgeImage = Mat::zeros(numRows, numCols, CV_8UC1);

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
                    gx += image.at<uchar>(i + k, j + l) * x_kernal[k + 1][l + 1];
                    gy += image.at<uchar>(i + k, j + l) * y_kernal[k + 1][l + 1];
                }
            }

            // Compute gradient magnitude
            edgeImage.at<uchar>(i, j) = std::sqrt(gx * gx + gy * gy);
        }
    }

    return edgeImage;
}

Mat threshold(const Mat &image, int minValue, int maxValue)
{
    int numRows = image.rows;
    int numCols = image.cols;
    Mat thresholdedImage = Mat::zeros(numRows, numCols, CV_8UC1);

    for (int i = 0; i < numRows; ++i)
    {
        for (int j = 0; j < numCols; ++j)
        {
            thresholdedImage.at<uchar>(i, j) = (image.at<uchar>(i, j) > minValue) ? 255 : 0;
        }
    }

    return thresholdedImage;
}

template <typename Func>
double timeFunction(Func function, Mat image)
{
    auto start = std::chrono::high_resolution_clock::now();
    function(image);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    return duration.count();
}

int main()
{
    Mat image = cv::imread("/src/input_imgs/goku.png");
    String output_path = "/src/realOutputMag.png";

    if (image.empty())
    {
        cout << "Failed to load image" << endl;
        return 1;
    }

    Mat grayImage = grayscale(image);
    // Mat edgeImage = sobelEdgeDetection(grayImage);

    double timeSeconds = timeFunction(sobelEdgeDetection, grayImage);
    std::cout << timeSeconds << "\n";

    // Mat thresholdedImage = threshold(edgeImage, 90, 255);

    // cv::imwrite(output_path, edgeImage);
    // cout << "Image saved to '" << output_path << "'" << endl;
}