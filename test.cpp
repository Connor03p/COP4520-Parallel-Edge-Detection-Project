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
    Timer timer(" Grayscale");

    int numRows = image.rows;
    int numCols = image.cols;
    Mat grayImage = Mat_<uchar>::zeros(numRows, numCols);

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
    Timer timer(" Grayscale (" + std::to_string(numThreads) + " threads)");

    int numRows = image.rows;
    int numCols = image.cols;
    Mat grayImage = Mat_<uchar>::zeros(numRows, numCols);

    // Create threads
    std::vector<std::thread> threads;
    int threadRows = numRows / numThreads;

    for (int i = 0; i < numThreads; ++i)
    {
        threads.push_back(std::thread([=, &image, &grayImage]()
        {
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

double gaussian(int x, int y, double sigma)
{
    return exp(-(x * x + y * y) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
}

// Function to generate a Gaussian kernel for blurring
vector<vector<double>> guassianKernel(int size, double sigma)
{
    vector<vector<double>> kernel(size, vector<double>(size));
    int center = size / 2;

    double total = 0.0;
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            kernel[i][j] = gaussian(i - center, j - center, sigma);
            total += kernel[i][j];
        }
    }

    // Normalize the kernel
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            kernel[i][j] /= total;
        }
    }

    return kernel;
}

Mat gaussianBlur(const Mat &image, int blurSize = 5, double sigma = 1)
{
    Timer timer(" Blur");

    int numRows = image.rows;
    int numCols = image.cols;
    Mat blurredImage = Mat_<uchar>::zeros(numRows, numCols);

    // Create the kernel
    vector<vector<double>> kernel = guassianKernel(blurSize, sigma);

    // Apply the kernel to the image
    int halfBlurSize = blurSize / 2;
    for (int y = halfBlurSize; y < image.rows - halfBlurSize; ++y)
    {
        for (int x = halfBlurSize; x < image.cols - halfBlurSize; ++x)
        {
            double sum = 0.0;
            for (int ky = -halfBlurSize; ky <= halfBlurSize; ++ky)
            {
                for (int kx = -halfBlurSize; kx <= halfBlurSize; ++kx)
                {
                    sum += kernel[ky + halfBlurSize][kx + halfBlurSize] * image.at<uchar>(y + ky, x + kx);
                }
            }
            blurredImage.at<uchar>(y, x) = sum;
        }
    }

    timer.timerEnd();
    return blurredImage;
}

Mat gaussianBlur_mt(const Mat &image, int blurSize = 5, double sigma = 1, int numThreads = 4)
{
    Timer timer(" Blur (" + std::to_string(numThreads) + " threads)");

    int numRows = image.rows;
    int numCols = image.cols;
    Mat blurredImage = Mat_<uchar>::zeros(numRows, numCols);

    // Create the kernel
    vector<vector<double>> kernel = guassianKernel(blurSize, sigma);

    // Create threads
    std::vector<std::thread> threads;
    int threadRows = numRows / numThreads;

    for (int i = 0; i < numThreads; ++i)
    {
        threads.push_back(std::thread([=, &image, &blurredImage]()
        {
            int startRow = i * threadRows;
            int endRow = startRow + threadRows;

            // Apply the kernel to the image
            int halfBlurSize = blurSize / 2;
            for (int y = startRow; y < endRow; ++y)
            {
                for (int x = halfBlurSize; x < image.cols - halfBlurSize; ++x)
                {
                    double sum = 0.0;
                    for (int ky = -halfBlurSize; ky <= halfBlurSize; ++ky)
                    {
                        for (int kx = -halfBlurSize; kx <= halfBlurSize; ++kx)
                        {
                            sum += kernel[ky + halfBlurSize][kx + halfBlurSize] * image.at<uchar>(y + ky, x + kx);
                        }
                    }
                    blurredImage.at<uchar>(y, x) = sum;
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
    Timer timer(" Sobel");
    int numRows = image.rows;
    int numCols = image.cols;
    Mat edgeImage = Mat_<uchar>::zeros(numRows, numCols);

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
    Timer timer(" Sobel (" + std::to_string(numThreads) + " threads)");
    int numRows = image.rows;
    int numCols = image.cols;
    Mat verticalGradient = Mat_<int>::zeros(numRows, numCols);
    Mat horizontalGradient = Mat_<int>::zeros(numRows, numCols);
    Mat edgeImage = Mat_<uchar>::zeros(numRows, numCols);

    // Create threads
    std::vector<std::thread> threads;
    int threadRows = numRows / numThreads;

    // Thread to calculate the vertical gradient
    for (int i = 0; i < numThreads; ++i)
    {
        threads.push_back(std::thread([=, &image, &verticalGradient]()
        {
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
        threads.push_back(std::thread([=, &image, &horizontalGradient]()
        {
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
    threads.clear();

    // Threads to calculate gradient magnitude
    for (int i = 0; i < numThreads; ++i)
    {
        threads.push_back(std::thread([=, &edgeImage, &verticalGradient, &horizontalGradient]()
                                      {
            int startRow = i * threadRows;
            int endRow = startRow + threadRows;

            for (int j = startRow; j < endRow; ++j)
            {
                for (int k = 1; k < numCols - 1; ++k)
                {
                    edgeImage.at<uchar>(j, k) = std::sqrt(verticalGradient.at<int>(j, k) * verticalGradient.at<int>(j, k) + horizontalGradient.at<int>(j, k) * horizontalGradient.at<int>(j, k));
                }
            } }));
    }

    // Join threads
    for (thread &t : threads)
    {
        t.join();
    }

    timer.timerEnd();
    return edgeImage;
}

Mat threshold(const Mat &image, int minValue, int maxValue)
{
    Timer timer(" Threshold");
    int numRows = image.rows;
    int numCols = image.cols;
    Mat thresholdedImage = Mat_<uchar>::zeros(numRows, numCols);

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
    Timer timer(" Threshold (" + std::to_string(numThreads) + " threads)");
    int numRows = image.rows;
    int numCols = image.cols;
    Mat thresholdedImage = Mat_<uchar>::zeros(numRows, numCols);

    // Create threads
    std::vector<std::thread> threads;
    int threadRows = numRows / numThreads;

    for (int i = 0; i < numThreads; ++i)
    {
        threads.push_back(std::thread([=, &image, &thresholdedImage]()
                                      {
            int startRow = i * threadRows;
            int endRow = startRow + threadRows;

            for (int j = startRow; j < endRow; ++j)
            {
                for (int k = 0; k < numCols; ++k)
                {
                    thresholdedImage.at<uchar>(j, k) = (image.at<uchar>(j, k) > minValue) ? 255 : 0;
                }
            } }));
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
    String input_path = "/src/sample2.png";
    String output1_path = "/src/output1.png";
    String output2_path = "/src/output2.png";

    Mat image = cv::imread(input_path);
    if (image.empty())
    {
        cout << "Failed to load image" << endl;
        return 1;
    }

    cout << endl << "Loaded '" << input_path << "'" << endl;
    cout << " Width: " << image.cols << endl;
    cout << " Height: " << image.rows << endl;

    cout << endl << "Performing single-threaded edge detection:" << endl;
    Timer timer("Total time");
    Mat grayImage = grayscale(image);
    Mat blurredImage = gaussianBlur(grayImage, 15, 5);
    Mat edgeImage = sobelEdgeDetection(blurredImage);
    Mat thresholdedImage = threshold(edgeImage, 25, 255);
    timer.timerEnd();
    cv::imwrite(output1_path, thresholdedImage);
    cout << "Image saved to '" << output1_path << "'" << endl;

    cout << endl << "Performing multi-threaded edge detection:" << endl;
    Timer timer2("Total time");
    Mat grayImage_mt = grayscale_mt(image, 4);
    Mat blurredImage_mt = gaussianBlur_mt(grayImage_mt, 15, 5, 8);
    Mat edgeImage_mt = sobelEdgeDetection_mt(blurredImage_mt, 4);
    Mat thresholdedImage_mt = threshold_mt(edgeImage_mt, 25, 255, 4);
    timer2.timerEnd();
    cv::imwrite(output2_path, thresholdedImage_mt);
    cout << "Image saved to '" << output2_path << "'" << endl << endl;

    return 0;
}