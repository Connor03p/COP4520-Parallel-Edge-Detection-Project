#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include "headers/convolution.hpp"

using namespace cv;
using namespace std;

int x_kernal[3][3] = {{-1, 0, 1},
                      {-2, 0, 2},
                      {-1, 0, 1}};

int y_kernal[3][3] = {{1, 2, 1}, 
                      {0, 0, 0}, 
                      {-1, -2, -1}};

// Timer
class Timer
{
    public:
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        std::string name;

        Timer(string name = "Timer") : name(name)
        {
            start = std::chrono::high_resolution_clock::now();
        }

        void timerStart()
        {
            start = std::chrono::high_resolution_clock::now();
        }

        void timerEnd()
        {
            end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            std::cout << name << ": " << elapsed.count() << " s\n";
        }
};

Mat grayscale(const Mat &image)
{
    Timer timer("Grayscale");

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

    timer.timerEnd();
    return grayImage;
}

// Multithreaded Grayscale
Mat grayscale_mt(const Mat &image, int numThreads = 4)
{
    Timer timer("Grayscale_mt");

    int numRows = image.rows;
    int numCols = image.cols;
    Mat grayImage = Mat::zeros(numRows, numCols, CV_8UC1);

    // Create threads
    std::vector<std::thread> threads;
    int threadRows = numRows / numThreads;

    for (int i = 0; i < numThreads; ++i)
    {
        threads.push_back(std::thread([=, &image, &grayImage]() {
            int startRow = i * threadRows;
            int endRow = startRow + threadRows;

            for (int j = startRow; j < endRow; ++j)
            {
                for (int k = 0; k < numCols; ++k)
                {
                    Vec3b intensity = image.at<Vec3b>(j, k);
                    grayImage.at<uchar>(j, k) = 0.299 * intensity.val[2] + 0.587 * intensity.val[1] + 0.114 * intensity.val[0];
                }
            }
        }));
    }

    // Join threads
    for (thread &t : threads)
    {
        t.join();
    }

    timer.timerEnd();

    return grayImage;
}


Mat gaussianBlur(const Mat &image)
{
    Timer timer("Blur");

    int numRows = image.rows;
    int numCols = image.cols;
    Mat blurredImage = Mat::zeros(numRows, numCols, CV_8UC1);

    for (int i = 1; i < numRows - 1; ++i)
    {
        for (int j = 1; j < numCols - 1; ++j)
        {
            int sum = 0;

            for (int k = -1; k <= 1; ++k)
            {
                for (int l = -1; l <= 1; ++l)
                {
                    sum += image.at<uchar>(i + k, j + l);
                }
            }

            blurredImage.at<uchar>(i, j) = sum / 9;
        }
    }

    timer.timerEnd();
    return blurredImage;
}

Mat gaussianBlur_mt(const Mat &image, int numThreads = 4)
{
    Timer timer("Blur_mt");

    int numRows = image.rows;
    int numCols = image.cols;
    Mat blurredImage = Mat::zeros(numRows, numCols, CV_8UC1);

    // Create threads
    std::vector<std::thread> threads;
    int threadRows = numRows / numThreads;

    for (int i = 0; i < numThreads; ++i)
    {
        threads.push_back(std::thread([=, &image, &blurredImage]() {
            int startRow = i * threadRows;
            int endRow = startRow + threadRows;

            for (int j = startRow; j < endRow; ++j)
            {
                for (int k = 1; k < numCols - 1; ++k)
                {
                    int sum = 0;

                    for (int l = -1; l <= 1; ++l)
                    {
                        for (int m = -1; m <= 1; ++m)
                        {
                            sum += image.at<uchar>(j + l, k + m);
                        }
                    }

                    blurredImage.at<uchar>(j, k) = sum / 9;
                }
            }
        }));
    }

    // Join threads
    for (thread &t : threads)
    {
        t.join();
    }

    timer.timerEnd();
    return blurredImage;
}


Mat sobelEdgeDetection(const Mat &image)
{
    Timer timer("Sobel");
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

    timer.timerEnd();
    return edgeImage;
}

Mat sobelEdgeDetection_mt(const Mat &image, int numThreads = 4)
{
    Timer timer("Sobel_mt");
    int numRows = image.rows;
    int numCols = image.cols;
    Mat verticalGradient = Mat::zeros(numRows, numCols, CV_32SC1);
    Mat horizontalGradient = Mat::zeros(numRows, numCols, CV_32SC1);
    Mat edgeImage = Mat::zeros(numRows, numCols, CV_8UC1);

    // Create threads
    std::vector<std::thread> threads;
    int threadRows = numRows / numThreads;
    
    // Thread to calculate the vertical gradient
    for (int i = 0; i < numThreads; ++i)
    {
        threads.push_back(std::thread([=, &image, &verticalGradient]() {
            int startRow = i * threadRows;
            int endRow = startRow + threadRows;

            for (int j = startRow; j < endRow; ++j)
            {
                for (int k = 1; k < numCols - 1; ++k)
                {
                    int sum = 0;

                    for (int l = -1; l <= 1; ++l)
                    {
                        for (int m = -1; m <= 1; ++m)
                        {
                            sum += image.at<uchar>(j + l, k + m) * y_kernal[l + 1][m + 1];
                        }
                    }

                    verticalGradient.at<int>(j, k) = sum;
                }
            }
        }));
    }

    // Threads to calculate the horizontal gradient
    for (int i = 0; i < numThreads; ++i)
    {
        threads.push_back(std::thread([=, &image, &horizontalGradient]() {
            int startRow = i * threadRows;
            int endRow = startRow + threadRows;

            for (int j = startRow; j < endRow; ++j)
            {
                for (int k = 1; k < numCols - 1; ++k)
                {
                    int sum = 0;

                    for (int l = -1; l <= 1; ++l)
                    {
                        for (int m = -1; m <= 1; ++m)
                        {
                            sum += image.at<uchar>(j + l, k + m) * x_kernal[l + 1][m + 1];
                        }
                    }

                    horizontalGradient.at<int>(j, k) = sum;
                }
            }
        }));
    }

    // Join threads
    for (thread &t : threads)
    {
        t.join();
    }

    // Calculate gradient magnitude
    for (int i = 1; i < numRows - 1; ++i)
    {
        for (int j = 1; j < numCols - 1; ++j)
        {
            edgeImage.at<uchar>(i, j) = std::sqrt(verticalGradient.at<int>(i, j) * verticalGradient.at<int>(i, j) + horizontalGradient.at<int>(i, j) * horizontalGradient.at<int>(i, j));
        }
    }

    timer.timerEnd();
    return edgeImage;
}

Mat threshold(const Mat &image, int minValue, int maxValue)
{
    Timer timer("Threshold");
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

    timer.timerEnd();
    return thresholdedImage;
}

Mat threshold_mt(const Mat &image, int minValue, int maxValue, int numThreads = 4)
{
    Timer timer("Threshold_mt");
    int numRows = image.rows;
    int numCols = image.cols;
    Mat thresholdedImage = Mat::zeros(numRows, numCols, CV_8UC1);

    // Create threads
    std::vector<std::thread> threads;
    int threadRows = numRows / numThreads;

    for (int i = 0; i < numThreads; ++i)
    {
        threads.push_back(std::thread([=, &image, &thresholdedImage]() {
            int startRow = i * threadRows;
            int endRow = startRow + threadRows;

            for (int j = startRow; j < endRow; ++j)
            {
                for (int k = 0; k < numCols; ++k)
                {
                    thresholdedImage.at<uchar>(j, k) = (image.at<uchar>(j, k) > minValue) ? 255 : 0;
                }
            }
        }));
    }

    // Join threads
    for (thread &t : threads)
    {
        t.join();
    }

    timer.timerEnd();
    return thresholdedImage;
}

int main()
{
    Mat image = cv::imread("/src/sample2.png");
    String output1_path = "/src/output1.png";
    String output2_path = "/src/output2.png";

    if (image.empty())
    {
        cout << "Failed to load image" << endl;
        return 1;
    }

    cout << endl << "Performing single-threaded edge detection:" << endl;
    Timer timer("Total time");
    Mat grayImage = grayscale(image);
    Mat blurredImage = gaussianBlur(grayImage);
    Mat edgeImage = sobelEdgeDetection(blurredImage);
    Mat thresholdedImage = threshold(edgeImage, 90, 255);
    cv::imwrite(output1_path, thresholdedImage);
    timer.timerEnd();
    cout << "Image saved to '" << output1_path << "'" << endl;

    cout << endl << "Performing multi-threaded edge detection:" << endl;
    Timer timer2("Total time");
    Mat grayImage_mt = grayscale_mt(image, 4);
    Mat blurredImage_mt = gaussianBlur_mt(grayImage_mt);
    Mat edgeImage_mt = sobelEdgeDetection_mt(blurredImage_mt);
    Mat thresholdedImage_mt = threshold_mt(edgeImage_mt, 90, 255, 4);    
    cv::imwrite(output2_path, thresholdedImage_mt);
    timer2.timerEnd();
    cout << "Image saved to '" << output2_path << "'" << endl << endl;

    return 0;
}