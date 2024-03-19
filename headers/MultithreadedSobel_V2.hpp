#pragma once
#include <opencv2/core/mat.hpp>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include "localUtil.hpp"
#include "Counter.hpp"

class MultithreadedSobel_V2
{
public:
    MultithreadedSobel_V2(int numberOfThreads, int threshold);
    cv::Mat performSobelEdgeDetection(cv::Mat &image);

    /**
     * @brief this will take the image and the coordinates given, perform the sobel
     * algorithm and return the magnitude for that 3x3
     *
     * @param image the input image that holds the pixels you want to grab
     * @param x the x index of the center pixel of some 3x3 you want to grab
     * @param y the y index of the center pixel of some 3x3 you want to grab
     * @param threshold any value from the magnitude image greater than this value will be kept as an edge
     *
     * @return the final pixel value of a single patch after performing sobel
     */
    int performSobelOnPatch(cv::Mat &image, int x, int y, int threshold);

    std::mutex mutex;

private:
    int numberOfThreads;
    int threshold;
    cv::Mat inputImage;
};