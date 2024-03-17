#include "/src/headers/Convolution.hpp"

int Convolution::performSobelOnPatch(cv::Mat &image, int x, int y, int threshold)
{
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
        gx += int(image.at<uchar>(x + i, y + j)) * Convolution::x_kernal_vector[c];
        gy += int(image.at<uchar>(x + i, y + j)) * Convolution::y_kernal_vector[c];
        j++;
    }

    magnitude = std::sqrt((gx * gx) + (gy * gy));

    // sets the final value to and edge depending on it exceeding the threshold value
    // TODO: Maybe implement auto thresholds
    // patchValueAfterThreshold is the pixel value that goes into the final image
    patchValueAfterThreshold = (magnitude > threshold) ? 255 : 0;

    // ?Maybe do this in a wrapper function

    return patchValueAfterThreshold;
}
