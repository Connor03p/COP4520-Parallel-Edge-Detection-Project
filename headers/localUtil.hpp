#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>

using namespace cv;

/*
 * localUtil will contain any usefull helper function that don't really
 * have a place in a sepearate class
 */
class localUtil
{

public:
    /**
     * @param  image the input image
     * @returns an image where the borders of the input image are 0's
     */
    static Mat fillWithZeros(const Mat &image);

    /**
     * Write the pixel values of an image to a *.txt file
     *
     * @param image the input image to be written to a *.txt file
     * @param filename The name of the text file to write the pixel values to. (without extension)
     */
    static void writeImageToTxt(const Mat &image, const std::string &filename);

private:
};