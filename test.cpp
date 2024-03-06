#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    cout << "fuck\n";
    Mat image = cv::imread("/src/sample.png");
    if (image.empty())
    {
        cout << "Failed to load image" << endl;
        return 1;
    }

    imshow("Image", image);
    waitKey(0);
    return 0;
}