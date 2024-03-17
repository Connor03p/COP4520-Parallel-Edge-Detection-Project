#include "/src/headers/MultithreadedSobel.hpp"
#include <fstream>

const int x_kernal_vector[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

const int y_kernal_vector[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

MultithreadedSobel::MultithreadedSobel(int numberOfThreads, int threshold)
{
    this->numberOfThreads = numberOfThreads;
    this->threshold = threshold;
}

// TODO: moved this here may get rid of it in the other class as its not needed
int MultithreadedSobel::performSobelOnPatch(cv::Mat &image, int x, int y, int threshold)
{

    // std::cout << "x " << x << "y: " << y << std::endl;

    int i, j, gx = 0, gy = 0, magnitude = 0, patchValueAfterThreshold = 0;

    i = j = -1;
    // Think of c a the indices of a 3x3 matrix, but in a sequential sense
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
        this->mutex.lock();
        gx += int(image.at<uchar>(x + i, y + j)) * x_kernal_vector[c];
        gy += int(image.at<uchar>(x + i, y + j)) * y_kernal_vector[c];
        j++;
        this->mutex.unlock();
    }

    this->mutex.lock();
    magnitude = std::sqrt((gx * gx) + (gy * gy));

    // sets the final value to and edge depending on it exceeding the threshold value
    // TODO: Maybe implement auto thresholds
    // patchValueAfterThreshold is the pixel value that goes into the final image
    patchValueAfterThreshold = (magnitude > threshold) ? 255 : 0;
    this->mutex.unlock();

    // ?Maybe do this in a wrapper function

    return patchValueAfterThreshold;
}

// TODO: WIP STILL NEED TO ADD COUNTER CLASS AND OTHER THINGS
cv::Mat MultithreadedSobel::performSobelEdgeDetection(cv::Mat &image)
{

    int imageRows = image.rows;
    int imageCols = image.cols;
    int thresh = this->threshold;

    cv::Mat final(imageRows, imageCols, CV_8UC1);
    Counter counter(imageRows - 1, imageCols - 1);
    std::vector<std::thread> threads;

    // TODO: still need to get indicies here properly
    // TODO need to use std::ref when passing cv::Mat !!!!!

    for (int i = 0; i < this->numberOfThreads; i++)
    {
        threads.push_back(std::thread([this, &final, &image, &thresh, &counter]()
                                      {
                                        int result = 0;
                                        int index[2];
                                        while (!counter.getIsFinished())
                                        {
                                            index[0] = counter.getIndex()[0];
                                            index[1] = counter.getIndex()[1];
                                            result = performSobelOnPatch(image, index[0], index[1], thresh);
                                            final.at<uchar>(index[0], index[1]) = uchar(result);
                                            counter.getCoordinates();
                                        } }));
    }

    for (int j = 0; j < this->numberOfThreads; j++)
    {
        threads.at(j).join();
    }

    // localUtil::writeImageToTxt(final, "yoooo");

    return final;
}
