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

### 7. Keypoints and their neighborhood size



|         |     |     |     |     |     |     |     |     |     |     |
|---------|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|
|SHITOMASI| 1370|	1301|	1361|	1358|	1333|	1284|	1322|	1366|	1389|	1339|
|HARRIS   |  115|	  98|	 113|	 121|	 160|	 383|	  85|	 210|	 171|	 281|
|FAST     | 5063|	4952|	4863|	4840|	4856|	4899|	4870|	4868|	4996|	4997|
|BRISK    | 2757|	2777|	2741|	2735|	2757|	2695|	2715|	2628|	2639|	2672|
|ORB      |  500|	 500|	 500|	 500|	 500|	 500|	 500|	 500|	 500|	 500|
|AKAZE    | 1351|	1327|	1311|	1351|	1360|	1347|	1363|	1331|	1357|	1331|
|BRISK    | 1437|	1371|	1381|	1336|	1303|	1370|	1396|	1382|	1462|	1422|

### 8. Matched keypoints

#### SHITOMASI detector
     BRISK	0	85	80	73	77	74	70	79	81	73
     BRIEF	0	97	93	92	89	94	93	86	92	86
       ORB	0	88	84	87	91	89	76	81	88	90
     FREAK	0	67	66	64	63	64	64	62	65	64
     AKAZE	0	0	0	0	0	0	0	0	0	0
      SIFT	0	115	109	104	103	101	101	97	107	99
   
#### HARRIS detector
     BRISK	0	11	9	10	11	16	14	12	21	17
     BRIEF	0	12	12	14	17	17	16	12	20	21
       ORB	0	11	11	14	17	19	19	13	21	20
     FREAK	0	11	9	13	14	13	18	10	17	18
     AKAZE	0	0	0	0	0	0	0	0	0	0
      SIFT	0	14	11	16	19	22	22	13	24	22
      
#### FAST detector
     BRISK	0	213	215	191	209	186	201	211	205	212
     BRIEF	0	229	256	237	248	226	245	256	263	241
       ORB	0	226	220	218	226	221	238	251	231	242
     FREAK	0	178	183	162	185	159	181	192	166	173
     AKAZE	0	0	0	0	0	0	0	0	0	0
      SIFT	0	315	326	301	316	294	328	319	304	302

#### BRISK detector
     BRISK	0	136	138	132	139	136	154	136	149	155
     BRIEF	0	136	160	129	140	147	153	157	157	145
       ORB	0	92	102	85	94	82	112	112	113	118
     FREAK	0	108	119	114	114	100	129	135	126	129
     AKAZE	0	0	0	0	0	0	0	0	0	0
      SIFT	0	177	187	171	177	168	190	193	173	181

#### ORB detector 
     BRISK	0	60	65	65	76	72	82	82	72	72
     BRIEF	0	37	38	37	53	42	60	57	61	60
       ORB	0	39	57	49	54	57	67	70	62	70
     FREAK	0	38	33	37	40	33	40	41	40	43
     AKAZE	0	0	0	0	0	0	0	0	0	0
      SIFT	0	66	79	78	79	82	93	94	93	92

#### AKAZE detector
     BRISK	0	123	111	120	117	114	119	134	138	125
     BRIEF	0	105	116	110	107	116	128	133	134	128
       ORB	0	100	95	96	83	90	117	103	112	117
     FREAK	0	100	104	94	97	97	115	126	116	113
     AKAZE	0	126	127	124	116	120	132	137	138	141
      SIFT	0	132	134	129	136	136	147	147	153	149

#### SIFT detector
     BRISK	0	56	61	57	61	55	53	54	63	73
     BRIEF	0	62	70	62	66	52	58	70	70	84
       ORB	0	0	0	0	0	0	0	0	0	0
     FREAK	0	58	61	53	64	51	51	48	53	65
     AKAZE	0	0	0	0	0	0	0	0	0	0
      SIFT	0	81	79	84	93	90	82	82	100	101
      
### 9. Time performance evaluation

![SHITOMASI detector](/images/evaluation/Shitomasi detector.png)

