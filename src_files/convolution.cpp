#include "/src/headers/convolution.hpp"

int convolution::calcConvolution(std::vector<int> vector1, std::vector<int> vector2)
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

std::vector<int> convolution::formVectorOutOf3by3(cv::Mat &image, int x, int y)
{
    std::vector<int> retval;
    int i, j;

    // this is to make sure that the indicies passed in are over the original image and not the add padding
    // we cam still collect those values with the following algorithm, we just don't want to
    // center the pixel anywhere in the padding.
    if (x < 1 || x > 255 || y < 1 || y > 255)
    {
        std::cout << "X or Y index out of bounds of un padded image" << std::endl;
        return retval;
    }

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
        retval.push_back(int(image.at<char>(x + i, y + j)));
        j++;
    }

    return retval;
}