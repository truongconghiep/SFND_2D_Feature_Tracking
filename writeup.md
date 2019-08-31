1. Data buffer optimization

To load images efficiently and prevent pushing the memory to its limits a ring buffer is implemented. This buffer is a FIFO (first in first out) buffer, which takes a new incoming image while drops the most old image out. The buffer size in this project is set to 2, that means the buffer can hold 2 images at the same time.

2. Keypoint detectors
Different keypoint detectors are implemented, such as SHITOMASI, HARRIS, FAST, BRISK, AKAZE and SIFT. Their performence will be evaluated and based on the performance measurement the best keypoint will be choosed. 
