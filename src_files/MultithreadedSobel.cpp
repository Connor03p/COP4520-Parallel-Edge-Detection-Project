#include "/src/headers/MultithreadedSobel.hpp"

MultithreadedSobel::MultithreadedSobel()
{
    this->numberOfThreads = 1;
    this->threshold = 50;
}

MultithreadedSobel::MultithreadedSobel(int numberOfThreads)
{
    this->numberOfThreads = numberOfThreads;
    this->threshold = 50;
}

MultithreadedSobel::MultithreadedSobel(int numberOfThreads, int threshold)
{
    this->numberOfThreads = numberOfThreads;
    this->threshold = threshold;
}

// TODO: moved this here may get rid of it in the other class as its not needed
int performSobelOnPatch(cv::Mat &image, int x, int y, int threshold)
{
    const int x_kernal_vector[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

    const int y_kernal_vector[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

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
        gx += int(image.at<uchar>(x + i, y + j)) * x_kernal_vector[c];
        gy += int(image.at<uchar>(x + i, y + j)) * y_kernal_vector[c];
        j++;
    }

    magnitude = std::sqrt((gx * gx) + (gy * gy));

    // sets the final value to and edge depending on it exceeding the threshold value
    // TODO: Maybe implement auto thresholds
    // patchValueAfterThreshold is the pixel value that goes into the final image
    patchValueAfterThreshold = (magnitude > threshold) ? 255 : 0;

    // ?Maybe do this in a wrapper function

    std::cout << patchValueAfterThreshold << "to test diff threads " << std::endl;

    return patchValueAfterThreshold;
}

// TODO: WIP STILL NEED TO ADD COUNTER CLASS AND OTHER THINGS
cv::Mat MultithreadedSobel::performSobelEdgeDetection(cv::Mat &image)
{
    cv::Mat m1(image.rows, image.cols, CV_8UC1);
    int x = 0;
    int y = 0;
    std::vector<std::thread> threads;
    int thresh = 0;

    // TODO: still need to get indicies here properly
    // TODO need to use std::ref when passing cv::Mat !!!!!

    for (int i = 0; i < this->numberOfThreads; i++)
    {
        threads.push_back(std::thread([&m1, &image, &x, &y, &thresh]()
                                      {
                                        int result = performSobelOnPatch(image, x, y, thresh);
                                        m1.at<uchar>(x, y) = uchar(result); }));
    }

    for (int j = 0; j < this->numberOfThreads; j++)
    {
        threads.at(j).join();
    }

    localUtil::writeImageToTxt(m1, "yoooo");

    return m1;
}
