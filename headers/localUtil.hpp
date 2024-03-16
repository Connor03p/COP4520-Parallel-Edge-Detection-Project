#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>

/**
 * @brief localUtil will contain any useful helper function that don't really
 *        have a place in a separate class
 */
class localUtil
{

public:
    /**
     * @param  image the input image
     * @returns an image where the borders of the input image are 0's
     */
    static cv::Mat padWithZeros(const cv::Mat &image);

    /**
     * Write the pixel values of an image to a *.txt file
     *
     * @param image the input image to be written to a *.txt file
     * @param filename The name of the text file to write the pixel values to. (without extension)
     */
    static void writeImageToTxt(const cv::Mat &image, const std::string &filename);

    /**
     * @param filename the filename of the image you want to load, must be in /src/input_imgs
     * @param mode the ImreadModes that you want to load the image in with (i.e. color, grayscale)
     *
     * @return a Mat with the loaded image
     */
    static cv::Mat loadImageFromFile(const std::string &filename, cv::ImreadModes mode);

private:
};