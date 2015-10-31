//
//  Camera.cpp
//  cvtest
//
//  Created by Lewis on 15/09/2015.
//  Copyright (c) 2015 Lewis. All rights reserved.
//

#include "Camera.h"

Camera::Camera(int camNum, int FRAME_WIDTHin, int FRAME_HEIGHTin){
	FRAME_WIDTH = FRAME_WIDTHin;
	FRAME_HEIGHT = FRAME_HEIGHTin;
    if (capture1.isOpened()){
        capture2.open(camNum);
        }
    else{
    capture1.open(camNum);
    
    }
    //set height and width of capture frame
    
    
    }

Camera::~Camera(void){}

void Camera::Readcamfeed(){

    capture1.read(cameraFeed);
    resize(cameraFeed, cameraFeed, Size(FRAME_WIDTH,FRAME_HEIGHT), 0, 0, INTER_CUBIC);
}

void Camera::toHSV(){

    cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);

}

void Camera::CaminRange(int H_MIN, int V_MIN, int S_MIN, int H_MAX, int S_MAX, int V_MAX){

    inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);

}

Mat Camera::Gethreshold(void){

    return threshold;
}

Mat Camera::GetCameraFeed(void){

    return cameraFeed;

}

Mat Camera::GetHSV(void){

    return HSV;

}
