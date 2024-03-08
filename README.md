# COP4520 Parallel Edge Detection Project
The problem that we are trying to address is that of optimizing the process of edge detection (a common technique within the world of image processing). We want to use multithreading to investigate whether the process of edge detection can be improved on compared to the traditional sequential way of performing edge detection. Overall, we want to highlight differences in performance between parallel edge detection and traditional sequential edge detection.

## Research
Before edge detection can be used, the image used should be grayscale to simplify calculations to a single color channel. It should also be blurred to reduce noise caused by small details. Doing these will reduce the amount of calculations needed and reduce errors.

Each of these transformations typically make use of a kernel, which is a matrix that is used for calculations at each pixel of an image. The order these kernels are applied matters, which introduces some challenges in making this a multithreaded process. Kernels are calculated in order of each pixel left-to-right, top-to-bottom.

We will be using the Sobel filter to highlight edges in our image. This filter uses a set of two 3x3 kernels:
 - The horizontal kernel (typically called SobelX)
    
        [+1,  0,  -1]
        [+2,  0,  -2]
        [+1,  0,  -1]
	

 -  The vertical kernel (typically called SobelY)
    
        [ +1, +2, +1]
        [  0,  0,  0]
        [ -1, -2, -1]

These kernels are applied individually to create gradients, this results in two images being produced, one representing the gradient in the horizontal collection, Gx, and another representing the gradient in the vertical direction, Gy and then combined using the following formula at each pixel in the image:  G=Gx2+Gy2. This is where edge detection is performed, a previously defined threshold value will serve to define which pixels make up the edges of the original image. Comparing each calculated gradient magnitude, it is verified whether they are higher or lower in value to the threshold, if they are higher they make up an edge, if they are lower they are not part of an edge. Finally, we output grayscale image of edges.



## Current Progress
To help manage dependencies like opencv2, we have shared a Docker container that includes all the necessary files and compile instructions, so that we do not have to set all of this up individually. We can use VS Code extensions to connect to the container to easily edit and test the program.

We have completed all the necessary research for edge detection and the Sobel algorithm, as well as for multithreading in C++.

Our current C++ program is a simple Sobel edge detection function that is single-threaded. We started with the single-threaded version so that we have a better understanding of the edge detection process.

A grayscale image is input as a 2-dimensional array, one for each row of pixels, and one to store the brightness values of each pixel in that row. The program gets the width and height of the image by taking the number of rows and the number of pixels in the first row. 

We use a nested for loop to apply the Sobel kernels to each pixel in the image and calculate the strength of the edge at each point.
We are currently discussing possible implementations for a multithreaded version of this program. Some ideas we have discussed:
 - Breaking the image into n chunks, where each thread will take one chunk of the image and perform the full sobel algorithm on
 - One thread for the vertical kernel, one for horizontal kernel, and one or more for combining the results

As for the input that we are going to use for testing, we need to decide on a dataset, either a collection of 5-10 images of 256x256 size in color or in grayscale format. The color of the images still is a topic that needs to be discussed with further work done with implementation of the multi-threaded Sobel code.

The stretch goals for this project have been defined and agreed upon

The minimum deliverable/result has also been discussed and set

## Implementation
We will be comparing the performance of two C++ programs that run the Sobel Algorithm. One will be multithreaded, while the other will not. For the program that is multithreaded:  
 - If image is in color, create a grayscale operation, otherwise process the image as received 
 - Parallelize select parts of Sobel, using locks to ensure thread safety
 - Split up the work between N threads
 - Split up the work either evenly or unevenly between threads, depending on the number of threads 

To compare the efficiency and results of both sets of code, we will select anywhere from 5-10 images to make up our dataset, these images will be the standard size of 256x256, and we have yet to decide on whether these images will be in grayscale or in color.

When comparing results of each program, we will observe runtime, possibly using eustis to ensure consistency. 
We might attempt to compare edge detection results, but we are still yet to decide on how to go about doing that
The simple Sobel program will be used to collect the standard or benchmark results, and the multi-threaded Sobel program will be tested against these results

Once successful testing and and improvement on the base Sobel code using multithreading is achieved, stretch goals will commence

## What’s Left
So far we have programmed a single-threaded version of the Sobel algorithm and have come up with some ways of implementing a multithreaded version. From here, we will work towards a multithreaded version by comparing different implementations to see which is most efficient. With successful implementation of a multithreaded Sobel method we need to investigate and determine a proper way of comparing results and judging possible improvements made on the base Sobel implementation. The dataset used for testing still needs to be determined. A structure of final document will be discussed in the following weeks. Finally, different priorities of stretch goals need to be defined. 

### Minimum Goal
 - Parallelize Sobel Edge Detection and obtain superior efficiency results to regular Sobel Edge Detection
### Stretch Goals
 - Parallelize other Edge Detection techniques like Canny or Laplacian
 - Perform Parallel Sobel Edge Detection with larger input images 

<br><nr>

# How to compile and run
### If you have clang compiler (Mac)
    clang++ -std=c++11 sobel.cpp -o sobel
### G++ Compiler 
    g++ -std=c++11 sobel.cpp -o sobel