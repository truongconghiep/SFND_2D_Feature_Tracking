/* INCLUDES FOR THIS PROJECT */
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

#include "dataStructures.h"
#include "matching2D.hpp"

using namespace std;

double FeatureTracking(string detectorType, 
                       string descriptorType, 
                       string matcherType, 
                       string desType, 
                       string selectorType, 
                       bool VisualizeEnable,
                       double * ProcessTime,
                       int * DetectedKeypoint,
                       int * MatchedKeypoint)
{
/* INIT VARIABLES AND DATA STRUCTURES */

    // data location
    string dataPath = "../";

    // camera
    string imgBasePath = dataPath + "images/";
    string imgPrefix = "KITTI/2011_09_26/image_00/data/000000"; // left camera, color
    string imgFileType = ".png";
    int imgStartIndex = 0; // first file index to load (assumes Lidar and camera names have identical naming convention)
    int imgEndIndex = 9;   // last file index to load
    int imgFillWidth = 4;  // no. of digits which make up the file index (e.g. img-0001.png)

    // misc
    int dataBufferSize = 2;       // no. of images which are held in memory (ring buffer) at the same time
    vector<DataFrame> dataBuffer; // list of data frames which are held in memory at the same time
    bool bVis = false;            // visualize results
    double t = 0;

    /* MAIN LOOP OVER ALL IMAGES */

    cout << "/************   Detector " << detectorType << " *********** Descriptor " << descriptorType << " ************/" << endl;
    for (size_t imgIndex = 0; imgIndex <= imgEndIndex - imgStartIndex; imgIndex++)
    {
        t = 0;
        cout << "/----------- Frame " << (imgIndex + 1) << "------------/" << endl;
        /* LOAD IMAGE INTO BUFFER */

        // assemble filenames for current index
        ostringstream imgNumber;
        imgNumber << setfill('0') << setw(imgFillWidth) << imgStartIndex + imgIndex;
        string imgFullFilename = imgBasePath + imgPrefix + imgNumber.str() + imgFileType;

        // load image from file and convert to grayscale
        cv::Mat img, imgGray;
        img = cv::imread(imgFullFilename);
        cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

        //// STUDENT ASSIGNMENT
        //// TASK MP.1 -> replace the following code with ring buffer of size dataBufferSize

        // push image into data frame buffer
        DataFrame frame;
        frame.cameraImg = imgGray;
        
        // rotate the vector and pop the last element up
        if(dataBuffer.size() >= dataBufferSize)
        {
            std::rotate(dataBuffer.begin(), dataBuffer.begin()+1, dataBuffer.end());
            dataBuffer.pop_back();
        }
        dataBuffer.push_back(frame);  

        //// EOF STUDENT ASSIGNMENT
        cout << "#1 : LOAD IMAGE INTO BUFFER done" << endl;

        /* DETECT IMAGE KEYPOINTS */ 

        // extract 2D keypoints from current image
        vector<cv::KeyPoint> keypoints; // create empty feature list for current image
        

        //// STUDENT ASSIGNMENT
        //// TASK MP.2 -> add the following keypoint detectors in file matching2D.cpp and enable string-based selection based on detectorType
        //// -> HARRIS, FAST, BRISK, ORB, AKAZE, SIFT

        t = detKeypointsModern(keypoints, imgGray, detectorType);
        DetectedKeypoint[imgIndex] = keypoints.size();
                       
        //// EOF STUDENT ASSIGNMENT

        //// STUDENT ASSIGNMENT
        //// TASK MP.3 -> only keep keypoints on the preceding vehicle

        // only keep keypoints on the preceding vehicle
        bool bFocusOnVehicle = true;
        cv::Rect vehicleRect(535, 180, 180, 150);
        //cv::rectangle(imgGray,vehicleRect, cv::Scalar(255,0,0));
       
        if (bFocusOnVehicle)
        {
            for (int i = 0; i < keypoints.size(); i++)
            {
                //cout << "keypoint before" << keypoints.size()<<endl;
                if((keypoints[i].pt.x < 535) || (keypoints[i].pt.x > 715) || 
                    (keypoints[i].pt.y < 180) || (keypoints[i].pt.y > 330))
                {
                    keypoints.erase(keypoints.begin() + i);
                    i--;
                }
            }
        }
        
        //// EOF STUDENT ASSIGNMENT

        // optional : limit number of keypoints (helpful for debugging and learning)
        bool bLimitKpts = false;
        if (bLimitKpts)
        {
            int maxKeypoints = 50;

            if (detectorType.compare("SHITOMASI") == 0)
            { // there is no response info, so keep the first 50 as they are sorted in descending quality order
                keypoints.erase(keypoints.begin() + maxKeypoints, keypoints.end());
            }
            cv::KeyPointsFilter::retainBest(keypoints, maxKeypoints);
            cout << " NOTE: Keypoints have been limited!" << endl;
        }

        // push keypoints and descriptor for current frame to end of data buffer
        (dataBuffer.end() - 1)->keypoints = keypoints;
        cout << "#2 : DETECT KEYPOINTS done" << endl;

        /* EXTRACT KEYPOINT DESCRIPTORS */

        //// STUDENT ASSIGNMENT
        //// TASK MP.4 -> add the following descriptors in file matching2D.cpp and enable string-based selection based on descriptorType
        //// -> BRIEF, ORB, FREAK, AKAZE, SIFT

        cv::Mat descriptors;
        t += descKeypoints((dataBuffer.end() - 1)->keypoints, (dataBuffer.end() - 1)->cameraImg, descriptors, descriptorType);
        //// EOF STUDENT ASSIGNMENT

        // push descriptors for current frame to end of data buffer
        (dataBuffer.end() - 1)->descriptors = descriptors;

        cout << "#3 : EXTRACT DESCRIPTORS done" << endl;

        cout << "Detection and descriptor extraction time "<< t << " ms" << endl;

        ProcessTime[imgIndex] = t;

        if (dataBuffer.size() > 1) // wait until at least two images have been processed
        {

            /* MATCH KEYPOINT DESCRIPTORS */

            vector<cv::DMatch> matches;

            //// STUDENT ASSIGNMENT
            //// TASK MP.5 -> add FLANN matching in file matching2D.cpp
            //// TASK MP.6 -> add KNN match selection and perform descriptor distance ratio filtering with t=0.8 in file matching2D.cpp

            MatchedKeypoint[imgIndex] = matchDescriptors((dataBuffer.end() - 2)->keypoints, (dataBuffer.end() - 1)->keypoints,
                                                         (dataBuffer.end() - 2)->descriptors, (dataBuffer.end() - 1)->descriptors,
                                                            matches, descriptorType, matcherType, selectorType, desType);

            //// EOF STUDENT ASSIGNMENT

            // store matches in current data frame
            (dataBuffer.end() - 1)->kptMatches = matches;

            cout << "#4 : MATCH KEYPOINT DESCRIPTORS done" << endl;

            // visualize matches between current and previous image
            bVis = true;
            if (bVis && VisualizeEnable)
            {
                cv::Mat matchImg = ((dataBuffer.end() - 1)->cameraImg).clone();
                cv::drawMatches((dataBuffer.end() - 2)->cameraImg, (dataBuffer.end() - 2)->keypoints,
                                (dataBuffer.end() - 1)->cameraImg, (dataBuffer.end() - 1)->keypoints,
                                matches, matchImg,
                                cv::Scalar::all(-1), cv::Scalar::all(-1),
                                vector<char>(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

                string windowName = "Matching keypoints between two camera images";
                cv::namedWindow(windowName, 7);
                cv::imshow(windowName, matchImg);
                string ImgName = detectorType + ".jpg";
                cv::imwrite(ImgName, matchImg);
                cout << "Press key to continue to next image" << endl;
                cv::waitKey(0); // wait for key to be pressed
            }
            bVis = false;
        }
        else
        {
            MatchedKeypoint[imgIndex] = 0;
        }
    } // eof loop over all images

    return t;
}

/* MAIN PROGRAM */
int main(int argc, const char *argv[])
{
    vector<string> detectorTypes{"SHITOMASI", "HARRIS", "FAST", "BRISK", "ORB", "AKAZE", "SIFT"};
    vector<string> descriptorTypes{"BRISK", "BRIEF", "ORB", "FREAK", "AKAZE", "SIFT"};
    string matcherType = "MAT_BF";        // MAT_BF, MAT_FLANN
    string descriptorType = "DES_BINARY"; // DES_BINARY, DES_HOG
    string selectorType = "SEL_KNN";       // SEL_NN, SEL_KNN

    bool PerformanceEstimationEnable = false;

    double Time[6][10];
    int DetectedKeypoint[6][10];
    int MatchedKeypoint[6][10];
    int desIdx = 0;

    ofstream TimeLog;
    TimeLog.open("InfoLog.csv");

    if (!PerformanceEstimationEnable)
    {
        FeatureTracking(detectorTypes[6], descriptorTypes[0], matcherType, descriptorType, selectorType, true,  &Time[0][0], &DetectedKeypoint[0][0], &MatchedKeypoint[0][0]);
        return 0;
    }

    for (std::string det : detectorTypes)
    {
        desIdx = 0;
        memset(&Time, 0, sizeof(Time));
        memset(&DetectedKeypoint, 0, sizeof(DetectedKeypoint));
        memset(&MatchedKeypoint, 0, sizeof(MatchedKeypoint));

        for (std::string des : descriptorTypes)
        {
            if ((det.compare("AKAZE") == 0) && (des.compare("AKAZE") == 0))
            {
                FeatureTracking(det, des, matcherType, descriptorType, selectorType, false, &Time[desIdx][0], &DetectedKeypoint[desIdx][0], &MatchedKeypoint[desIdx][0]);
            }
            else if ((det.compare("SIFT") == 0))
            {
                if((des.compare("ORB") != 0) && (des.compare("AKAZE") != 0))
                {
                    FeatureTracking(det, des, matcherType, descriptorType, selectorType, false, &Time[desIdx][0], &DetectedKeypoint[desIdx][0], &MatchedKeypoint[desIdx][0]);
                }
            }
            else if (des.compare("AKAZE") != 0)
            {
                FeatureTracking(det, des, matcherType, descriptorType, selectorType, false, &Time[desIdx][0], &DetectedKeypoint[desIdx][0], &MatchedKeypoint[desIdx][0]);
            }
            desIdx++;
        }

        TimeLog << "/----------------------------------------Detector: " << det << "  -----------------------------------------/\n";
        /* Output processing time */
        TimeLog << "Processing time: \n";
        for(int i = 0; i < 6; i++)
        {
            TimeLog << std::setw(10) << descriptorTypes[i] << ",";
            
            for (int j = 0; j < 10; j++)
            {
                TimeLog << std::setw(10) << Time[i][j] << ",";
            }
            TimeLog << "\n";
        }
        /* Output number of detected keypoints*/
        TimeLog << "number of detected keypoints: \n";
        for(int i = 0; i < 6; i++)
        {
            TimeLog << std::setw(10) << descriptorTypes[i] << ",";
            
            for (int j = 0; j < 10; j++)
            {
                TimeLog << std::setw(10) << DetectedKeypoint[i][j] << ",";
            }
            TimeLog << "\n";
        }
        /* Output number of detected keypoints*/
        TimeLog << "number of matched keypoints: \n";
        for(int i = 0; i < 6; i++)
        {
            TimeLog << std::setw(10) << descriptorTypes[i] << ",";
            
            for (int j = 0; j < 10; j++)
            {
                TimeLog << std::setw(10) << MatchedKeypoint[i][j] << ",";
            }
            TimeLog << "\n";
        }
    }
    TimeLog.close();
 
    return 0;
}