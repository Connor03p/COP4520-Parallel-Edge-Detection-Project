#include "/src/headers/MultithreadedSobel_V2.hpp"
#include <fstream>
#include <omp.h>

// Array forms of the 3x3 x and y kernal
// makes doing math easier in this format
const int x_kernal_vector[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

const int y_kernal_vector[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

MultithreadedSobel_V2::MultithreadedSobel_V2(int numberOfThreads, int threshold)
{
    this->numberOfThreads = numberOfThreads;
    this->threshold = threshold;
}

// TODO: moved this here may get rid of it in the other class as its not needed
int MultithreadedSobel_V2::performSobelOnPatch(cv::Mat &image, int x, int y, int threshold)
{

    // std::cout << "x " << x << "y: " << y << std::endl;

    int i, j, gx = 0, gy = 0, magnitude = 0, patchValueAfterThreshold = 0;

    i = j = -1;

    // Think of c as the indices of a 3x3 matrix, but in a sequential sense
    // first row start at 0, then 3, then 6
    for (int c = 0; c < 9; c++)
    {
        // at the start of every row, increase i and reset j
        // we use i and j here to help select the original values from the image in a 3x3 fashion
        if (c != 0 && c % 3 == 0)
        {
            i++;
            j = -1;
        }

        // For whatever reason, if you read your image in as grayscale
        // then the values are considered char when you want to read the out with Mat.at<>
        // so casting the chars to int is needed to get the values

        // TODO: print statement for debugging
        // std::cout << int(image.at<char>(x + i, y + j)) << " ";
        if (x + i >= 0 && x + i < image.rows && y + j >= 0 && y + j < image.cols)
        {
            gx += int(image.at<uchar>(x + i, y + j)) * x_kernal_vector[c];
            gy += int(image.at<uchar>(x + i, y + j)) * y_kernal_vector[c];
        }
        else
        {
            gx += 0;
            gy += 0;
        }
        j++;
    }

    // magnitude = std::sqrt((gx * gx) + (gy * gy));

    // using approximation for possible speed up
    magnitude = std::abs(gx) + std::abs(gy);

    // sets the final value to and edge depending on it exceeding the threshold value
    // patchValueAfterThreshold is the pixel value that goes into the final image
    patchValueAfterThreshold = (magnitude > threshold) ? 255 : 0;

    return patchValueAfterThreshold;
}

cv::Mat MultithreadedSobel_V2::performSobelEdgeDetection(cv::Mat &image)
{
    int imageRows = image.rows;
    int imageCols = image.cols;
    cv::Mat retval(imageRows, imageCols, CV_8UC1);

    std::vector<std::thread> threads;
    threads.reserve(this->numberOfThreads);

    // loop until all columns have been completed
    for (int colNum = imageCols - 1; colNum >= 0; colNum--)
    {

        // for each column create a thread and run the sobel operation for all pixels in that column
        threads.emplace_back([this, &imageRows, &image, &retval, colNum]()
                             {
            for (int i = 0; i < imageRows; i++)
            {
                retval.at<uchar>(i, colNum) = performSobelOnPatch(image, i, colNum, this->threshold);
            } });

        // this limits the amount of threads to whats passed in to the constructor
        if (threads.size() >= this->numberOfThreads)
        {
            for (auto &thread : threads)
            {
                thread.join();
            }
            threads.clear();
        }
    }

    // finally join the remaining threads before returning the final image
    for (auto &thread : threads)
    {
        thread.join();
    }

    return retval;
}
