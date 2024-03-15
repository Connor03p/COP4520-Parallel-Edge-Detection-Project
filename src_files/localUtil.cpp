#include "/src/headers/localUtil.hpp"

Mat localUtil::padWithZeros(const Mat &image)
{
    Size size(256, 256);
    Mat padded(size, image.type());

    copyMakeBorder(image, padded, 1, 1, 1, 1, BORDER_CONSTANT, 0);

    return padded;
}

void localUtil::writeImageToTxt(const Mat &image, const std::string &filename)
{
    std::string outPath = "/src/output_txt/" + filename + ".txt";
    std::ofstream out(outPath.c_str());
    out << image;
    out.close();
}
