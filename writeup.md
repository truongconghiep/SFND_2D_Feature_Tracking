### 1. Data buffer optimization

To load images efficiently and prevent pushing the memory to its limits a ring buffer is implemented. This buffer is a FIFO (first in first out) buffer, which takes a new incoming image while drops the most old image out. The buffer size in this project is set to 2, that means the buffer can hold 2 images at the same time. [Implementation code](https://github.com/truongconghiep/SFND_2D_Feature_Tracking/blob/bfc5777227cdbdf56d30a30c00a31d162d60c63a/src/MidTermProject_Camera_Student.cpp#L65)

### 2. Keypoint detectors

Different keypoint detectors are implemented, such as SHITOMASI, HARRIS, FAST, BRISK, AKAZE and SIFT. Their performence will be evaluated and based on the performance measurement the best keypoint will be choosed. [Implementation code](https://github.com/truongconghiep/SFND_2D_Feature_Tracking/blob/bfc5777227cdbdf56d30a30c00a31d162d60c63a/src/matching2D_Student.cpp#L216)


### 3. Keypoint removal

The purpose of keypoint removal is to limit the detected keypoints only on the preceding vehicle. A rectangular window is used as bounding box. The keypoints which lie outside of this bounding box will be removed. [Implementation code](https://github.com/truongconghiep/SFND_2D_Feature_Tracking/blob/bfc5777227cdbdf56d30a30c00a31d162d60c63a/src/MidTermProject_Camera_Student.cpp#L102)


### 4. Keypoint descriptors

•	A descriptor is a vector of values, which describes the image patch around a keypoint. There are various techniques ranging from comparing raw pixel values to much more sophisticated approaches such as histograms of gradient orientations. 
•	There are 2 types of keypoint descriptors: 
-	HOG-based descriptor: SIFT, SURF
-	Binary descriptor: BRIEF, BRISK, ORB, FREAK and KAZE

[Implementation code](https://github.com/truongconghiep/SFND_2D_Feature_Tracking/blob/bfc5777227cdbdf56d30a30c00a31d162d60c63a/src/matching2D_Student.cpp#L79)

### 5. Descriptor matching

Two keypoints are matched to each other by evaluating their similarity, which is measured by the difference or distance between 2 descriptors.
The first distance function is “Sum of Absolute Differences (SAD)”, which takes as input two descriptor vectors and subtracting from every component in the first vector the corresponding component at the same position in the second vector. Then the absolute values will be summed up.
The second distance function is “Sum of Squared Differences”, which sums up squared differences between 2 keypoint descriptors.
[Implementation code](https://github.com/truongconghiep/SFND_2D_Feature_Tracking/blob/bfc5777227cdbdf56d30a30c00a31d162d60c63a/src/matching2D_Student.cpp#L49)

### 6. Descriptor distance ratio test

With this method, a threshold is not applied directly to distances. Instead, for each keypoint in the source image, the two best matches are located in the reference image and the ratio between the descriptor distances is computed. Then, a threshold is applied to the ratio to sort out ambiguous matches. [Implementation code](https://github.com/truongconghiep/SFND_2D_Feature_Tracking/blob/bfc5777227cdbdf56d30a30c00a31d162d60c63a/src/matching2D_Student.cpp#L66)
