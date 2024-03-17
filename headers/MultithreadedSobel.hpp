#pragma once
#include "Counter.hpp"
#include "Convolution.hpp"
#include <string>
#include <opencv2/core/mat.hpp>
#include "localUtil.hpp"
#include <thread>
#include <vector>

class MultithreadedSobel
{
public:
    MultithreadedSobel();
    MultithreadedSobel(int numberOfThreads);
    MultithreadedSobel(int numberOfThreads, int threshold);

    cv::Mat performSobelEdgeDetection(cv::Mat &image);

private:
    Counter counter;
    int numberOfThreads;
    int threshold;
    cv::Mat inputImage;
};