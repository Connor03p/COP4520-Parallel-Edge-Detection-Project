#pragma once
#include "Counter.hpp"
#include "Convolution.hpp"
#include <string>
#include <opencv2/core/mat.hpp>
#include "localUtil.hpp"
#include <thread>
#include <vector>
#include <mutex>

class MultithreadedSobel
{
public:
    MultithreadedSobel(int numberOfThreads, int threshold);

    cv::Mat performSobelEdgeDetection(cv::Mat &image);

    int performSobelOnPatch(cv::Mat &image, int x, int y, int threshold);

private:
    int numberOfThreads;
    int threshold;
    cv::Mat inputImage;
    std::mutex mutex;
};