// #include <iostream>
// #include <cmath>
// #include <vector>

// // Kernel for Sobel operator
// const int sobelX[3][3] = {{-1, 0, 1}, {2, 0, -2}, {1, 0, -1}};
// const int sobelY[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

// // Function to perform Sobel edge detection
// void sobelEdgeDetection(const std::vector<std::vector<int>> &image, std::vector<std::vector<int>> &edgeImage)
// {
//     int numRows = image.size();
//     int numCols = image[0].size();

//     // Apply Sobel operator to each pixel in the image
//     for (int i = 1; i < numRows - 1; ++i)
//     {
//         for (int j = 1; j < numCols - 1; ++j)
//         {
//             int gx = 0, gy = 0;

//             // Convolution with Sobel kernels
//             for (int k = -1; k <= 1; ++k)
//             {
//                 for (int l = -1; l <= 1; ++l)
//                 {
//                     gx += image[i + k][j + l] * sobelX[k + 1][l + 1];
//                     gy += image[i + k][j + l] * sobelY[k + 1][l + 1];
//                 }
//             }

//             // Compute gradient magnitude
//             edgeImage[i][j] = std::sqrt(gx * gx + gy * gy);
//         }
//     }
// }

// int main()
// {
//     // Example image (grayscale)
//     std::vector<std::vector<int>> image = {
//         {100, 100, 100, 100, 100},
//         {100, 120, 120, 120, 100},
//         {100, 120, 200, 120, 100},
//         {100, 120, 120, 120, 100},
//         {100, 100, 100, 100, 100}};

//     // Resulting edge image
//     std::vector<std::vector<int>> edgeImage(image.size(), std::vector<int>(image[0].size(), 0));

//     // Perform Sobel edge detection
//     sobelEdgeDetection(image, edgeImage);

//     // Output edge image
//     for (const auto &row : edgeImage)
//     {
//         for (int pixel : row)
//         {
//             std::cout << pixel << " ";
//         }
//         std::cout << std::endl;
//     }

//     return 0;
// }
