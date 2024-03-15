#include "/src/headers/convolution.hpp"

int convolution::calcConvolution(std::vector<int> vector1, std::vector<int> vector2)
{
    int convolution = 0;
    int vector1Size = vector1.size();
    int vector2Size = vector2.size();

    if (vector1Size != vector2Size)
    {
        std::cout << "Can't convolve, matrices have different sizes" << std::endl;
        return -1;
    }

    for (int i = 0; i < vector1Size; i++)
    {
        convolution += vector1.at(i) * vector2.at(i);
    }

    return convolution;
}

std::vector<int> convolution::convertFromMatrixToVector(int matrix[3][3])
{
    std::vector<int> retval;
    retval.reserve(9); // Reserve space for 9 elements

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            retval.push_back(matrix[i][j]);
        }
    }

    return retval;
}