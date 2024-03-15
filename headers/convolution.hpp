#pragma once

#include <vector>
#include <iostream>
#include <opencv2/core/mat.hpp>

class convolution
{

public:
    /**
     * @brief Calculates the convolution of two flattened 3x3 matrices.
     *
     * @param vector1 The first flattened matrix.
     * @param vector2 The second flattened matrix.
     * @return The result of the convolution operation.
     */
    int calcConvolution(std::vector<int> vector1, std::vector<int> vector2);

    /**
     * @brief Given an image and indicies, takes a 3x3 block and creates a vector out of said values
     * assumes image has already be padded ny zeros to avoid out of bounds errors
     *
     *
     * @param image the input image that holds the pixels you want to grab
     * @param x the x index of the center pixel of some 3x3 you want to grab
     * @param y the y index of the center pixel of some 3x3 you want to grab
     * @return The resulting 1D vector. If x || y are not in bounds of the un padded image then zero vector is returned
     */
    std::vector<int> formVectorOutOf3by3(cv::Mat &image, int x, int y);

private:
    /**
     * Both kernals are pre flatten to vectors to save on computation
     * the vectors here are equivalent to their 3x3 matrix counter parts i.e.
     *
     *   gx = -1  0  1    gy = 1  2  1
     *        -2  0  2         0  0  0
     *        -1  0  1        -1 -2 -1
     *
     */
    const std::vector<int> gx_kernal_vector{1, 0, -1, 2, 0, -2, 1, 0, -1};
    const std::vector<int> gy_kernal_vector{1, 2, 1, 0, 0, 0, -1, -2, -1};
};