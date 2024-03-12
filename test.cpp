#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include "headers/convolution.hpp"

using namespace cv;
using namespace std;

int x_kernal[3][3] = {{-1, 0, 1},
                      {-2, 0, 2},
                      {-1, 0, 1}};

int main()
{
    // Mat image = cv::imread("/src/sample.png");
    // if (image.empty())
    // {
    //     cout << "Failed to load image" << endl;
    //     return 1;
    // }

    // imshow("Image", image);
    // waitKey(0);

    convolution conv;

    std::vector<int> one(9, 1);
    std::vector<int> x_kernal_flattened = conv.convertFromMatrixToVector(x_kernal);

    // int val = conv.calcConvolution(one, x_kernal_flattened);

    return 0;
}