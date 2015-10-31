//
//  Camera.h
//  cvtest
//
//  Created by Lewis on 15/09/2015.
//  Copyright (c) 2015 Lewis. All rights reserved.
//

#ifndef __cvtest__Camera__
#define __cvtest__Camera__

#include <stdio.h>
#include <string>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/videoio/videoio.hpp"

using namespace std;
using namespace cv;

class Camera{
    
public:
    
    Camera(int camNum,int FRAME_WIDTH, int FRAME_HEIGHT);
    ~Camera(void);
    
    VideoCapture capture1;
    VideoCapture capture2;
    
    void Readcamfeed();
    
    void toHSV();
    
    void CaminRange(int H_MIN, int V_MIN, int S_MIN, int H_MAX, int S_MAX, int V_MAX);
    
    Mat Gethreshold(void);
    
    Mat GetCameraFeed(void);
    
    Mat GetHSV(void);
        
private:
    //Matrix to store each frame of the webcam feed
    Mat cameraFeed;
    //matrix storage for HSV image
    Mat HSV;
    //matrix storage for binary threshold image
    Mat threshold;    //Matrix to store each frame of the webcam feed
    //camera number
    int camnNum;
    
    int FRAME_HEIGHT;
    int FRAME_WIDTH;
    
    
};




#endif /* defined(__cvtest__Camera__) */
