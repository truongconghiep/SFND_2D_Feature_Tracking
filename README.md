# SFND 2D Feature Tracking

<img src="images/keypoints.png" width="820" height="248" />

The idea of the camera course is to build a collision detection system - that's the overall goal for the Final Project. As a preparation for this, you will now build the feature tracking part and test various detector / descriptor combinations to see which ones perform best. This mid-term project consists of four parts:

* First, you will focus on loading images, setting up data structures and putting everything into a ring buffer to optimize memory load. 
* Then, you will integrate several keypoint detectors such as HARRIS, FAST, BRISK and SIFT and compare them with regard to number of keypoints and speed. 
* In the next part, you will then focus on descriptor extraction and matching using brute force and also the FLANN approach we discussed in the previous lesson. 
* In the last part, once the code framework is complete, you will test the various algorithms in different combinations and compare them with regard to some performance measures. 

See the classroom instruction and code comments for more details on each of these parts. Once you are finished with this project, the keypoint matching part will be set up and you can proceed to the next lesson, where the focus is on integrating Lidar points and on object detection using deep-learning. 

## [Data buffer optimization](https://github.com/truongconghiep/SFND_2D_Feature_Tracking/blob/bfc5777227cdbdf56d30a30c00a31d162d60c63a/src/MidTermProject_Camera_Student.cpp#L65)

To load images efficiently and prevent pushing the memory to its limits a ring buffer is implemented. This buffer is a FIFO (first in first out) buffer, which takes a new incoming image while drops the most old image out. The buffer size in this project is set to 2, that means the buffer can hold 2 images at the same time.

## [Keypoint detectors](https://github.com/truongconghiep/SFND_2D_Feature_Tracking/blob/bfc5777227cdbdf56d30a30c00a31d162d60c63a/src/matching2D_Student.cpp#L216)

Different keypoint detectors are implemented, such as SHITOMASI, HARRIS, FAST, BRISK, AKAZE and SIFT. Their performence will be evaluated and based on the performance measurement the best keypoint will be choosed.

## [Keypoint removal](https://github.com/truongconghiep/SFND_2D_Feature_Tracking/blob/bfc5777227cdbdf56d30a30c00a31d162d60c63a/src/MidTermProject_Camera_Student.cpp#L102)

The purpose of keypoint removal is to limit the detected keypoints only on the preceding vehicle. A rectangular window is used as bounding box. The keypoints which lie outside of this bounding box will be removed. 

## [Keypoint descriptors](https://github.com/truongconghiep/SFND_2D_Feature_Tracking/blob/bfc5777227cdbdf56d30a30c00a31d162d60c63a/src/matching2D_Student.cpp#L79)

•	A descriptor is a vector of values, which describes the image patch around a keypoint. There are various techniques ranging from comparing raw pixel values to much more sophisticated approaches such as histograms of gradient orientations. 
•	There are 2 types of keypoint descriptors: 
-	HOG-based descriptor: SIFT, SURF
-	Binary descriptor: BRIEF, BRISK, ORB, FREAK and KAZE

## [Descriptor matching](https://github.com/truongconghiep/SFND_2D_Feature_Tracking/blob/bfc5777227cdbdf56d30a30c00a31d162d60c63a/src/matching2D_Student.cpp#L49)

Two keypoints are matched to each other by evaluating their similarity, which is measured by the difference or distance between 2 descriptors.
The first distance function is “Sum of Absolute Differences (SAD)”, which takes as input two descriptor vectors and subtracting from every component in the first vector the corresponding component at the same position in the second vector. Then the absolute values will be summed up.
The second distance function is “Sum of Squared Differences”, which sums up squared differences between 2 keypoint descriptors.

## [Descriptor distance ratio test](https://github.com/truongconghiep/SFND_2D_Feature_Tracking/blob/bfc5777227cdbdf56d30a30c00a31d162d60c63a/src/matching2D_Student.cpp#L66)

With this method, a threshold is not applied directly to distances. Instead, for each keypoint in the source image, the two best matches are located in the reference image and the ratio between the descriptor distances is computed. Then, a threshold is applied to the ratio to sort out ambiguous matches.




## Dependencies for Running Locally
* cmake >= 2.8
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* OpenCV >= 4.1
  * This must be compiled from source using the `-D OPENCV_ENABLE_NONFREE=ON` cmake flag for testing the SIFT and SURF detectors.
  * The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./2D_feature_tracking`.
