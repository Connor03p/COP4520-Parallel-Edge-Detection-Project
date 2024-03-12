#pragma once

#include <vector>
#include <iostream>

class convolution
{

public:
    convolution()
    {
        std::cout << "test" << std::endl;
    }

    /*
     * Takes two flattened 3x3 matrices and performs a convolution
     */
    int calcConvolution(std::vector<int> vector1, std::vector<int> vector2);

    /*
     * Flattens a 3x3 Matrix into a 1d vector of 9 values
     */
    std::vector<int> convertFromMatrixToVector(int matrix[3][3]);

private:
};