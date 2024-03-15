#include "/src/headers/Convolution.hpp"

int Convolution::calcConvolution(std::vector<int> vector1, std::vector<int> vector2)
{
    int convolution = 0;
    int vector1Size = vector1.size();
    int vector2Size = vector2.size();

    if (vector1Size != vector2Size)
    {
        std::cout << "Can't convolve, vectors have different sizes" << std::endl;
        return -1;
    }

    for (int i = 0; i < vector1Size; i++)
    {
        convolution += vector1.at(i) * vector2.at(i);
    }

    return convolution;
}

std::vector<uchar> Convolution::formVectorOutOf3by3(cv::Mat &image, int x, int y)
{
    std::vector<uchar> retval;
    int i, j;

    // this is to make sure that the indicies passed in are over the original image and not the add padding
    // we cam still collect those values with the following algorithm, we just don't want to
    // center the pixel anywhere in the padding.
    // if (x < 1 || x > 256 || y < 1 || y > 256)
    // {
    //     std::cout << "X or Y index out of bounds of un padded image" << std::endl;
    //     return retval;
    // }

    // reserve space in memory for 9 values, faster than adding values only with push back
    retval.reserve(9);
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
        retval.push_back(image.at<uchar>(x + i, y + j));
        j++;
    }

    return retval;
}

int Convolution::performConvolutionOnPatch(cv::Mat &image, int x, int y)
{
    // this is called patch for consistent reasons, but values are contained within a vector
    std::vector<uchar> patch;
    int gx_patch, gy_patch, magnitude_patch;

    patch = formVectorOutOf3by3(image, x, y);

    std::vector<int> patch_int(patch.begin(), patch.end());

    // the change in x and y are also just the patches and not the entire gx and gy
    gx_patch = calcConvolution(patch_int, Convolution::x_kernal_vector);
    gy_patch = calcConvolution(patch_int, Convolution::y_kernal_vector);

    // now we find the patches magnitude
    magnitude_patch = std::sqrt((gx_patch * gx_patch) + (gy_patch * gy_patch));

    // TODO: should scale magnitude image before taking threshold
    // ?Maybe do this in a wrapper function

    return magnitude_patch;
}
