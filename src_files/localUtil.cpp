#include "/src/headers/localUtil.hpp"

cv::Mat localUtil::padWithZeros(const cv::Mat &image)
{
    cv::Mat padded(image.rows, image.cols, image.type());
    copyMakeBorder(image, padded, 1, 1, 1, 1, cv::BORDER_CONSTANT, 0);

    return padded;
}

void localUtil::writeImageToTxt(const cv::Mat &image, const std::string &filename)
{
    std::string outPath = "/src/output_txt/" + filename + ".txt";
    std::ofstream out(outPath.c_str());
    out << image;
    out.close();
}

cv::Mat localUtil::loadImageFromFile(const std::string &filename, cv::ImreadModes mode)
{
    // sets the dir where we search for photos using "samples::findFile()"
    cv::samples::addSamplesDataSearchPath("/src/input_imgs");
    cv::Mat retval;

    std::string inputImagePath = cv::samples::findFile(filename);
    retval = imread(inputImagePath, mode);

    if (retval.empty())
    {
        std::cout << "Image was not loaded properly" << std::endl;
    }

    return retval;
}
