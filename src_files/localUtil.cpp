#include "/src/headers/localUtil.hpp"

Mat localUtil::fillWithZeros(const Mat &image)
{
    Mat padded;
    int padding = 1;

    padded.create(image.rows + 2 * padding, image.cols + 2 * padding, image.type());
    padded.setTo(cv::Scalar::all(0));
    image.copyTo(padded(Rect(padding, padding, image.cols, image.rows)));
    return padded;
}

void localUtil::writeImageToTxt(const Mat &image, const std::string &filename)
{
    std::string outName = filename + ".txt";
    std::ofstream out(outName.c_str());
    out << image;
    out.close();
}
