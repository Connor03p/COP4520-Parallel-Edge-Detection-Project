#include "/src/headers/Convolution.hpp"

int Convolution::calcConvolution(std::vector<int> vector1, std::vector<int> vector2)
{

    int convolution = 0;

    // I use vector pointers here for a speed up instead of using .at()
    const int *vector1Ptr = vector1.data();
    const int *vector2Ptr = vector2.data();

    for (int i = 0; i < 9; i++)
    {
        convolution += *(vector1Ptr++) * *(vector2Ptr++);
    }

    return convolution;
}

std::vector<uchar> Convolution::formVectorOutOf3by3(cv::Mat &image, int x, int y)
{
    std::vector<uchar> retval;
    int i, j;

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

int Convolution::performSobelOnPatch(cv::Mat &image, int x, int y, int threshold)
{
    // this is called patch for consistent reasons, but values are contained within a vector
    std::vector<uchar> patchFromImage;
    int gxPatchVal, gyPatchVal, magnitudePatchVal, patchValueAfterThreshold;

    patchFromImage = formVectorOutOf3by3(image, x, y);

    // converts vector from uchar to int
    std::vector<int> patch_int(patchFromImage.begin(), patchFromImage.end());

    // the change in x and y are also just the patches and not the entire gx and gy
    gxPatchVal = calcConvolution(patch_int, Convolution::x_kernal_vector);
    gyPatchVal = calcConvolution(patch_int, Convolution::y_kernal_vector);

    // now we find the patches magnitude
    magnitudePatchVal = std::sqrt((gxPatchVal * gxPatchVal) + (gyPatchVal * gyPatchVal));

    // sets the final value to and edge depending on it exceeding the threshold value
    // TODO: Maybe implement auto thresholds
    patchValueAfterThreshold = (magnitudePatchVal > threshold) ? 255 : 0;

    // ?Maybe do this in a wrapper function

    return patchValueAfterThreshold;
}
