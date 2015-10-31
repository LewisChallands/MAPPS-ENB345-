//
//  main.cpp
//  CVTEST
//
//  Created by Lewis on 24/08/2015.
//
//  Original code written by  Kyle Hounslow 2013

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
//, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
//and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//IN THE SOFTWARE.


#include <iostream>
#include <sstream>
#include <math.h>
#include <string>
#include <vector>
#include "Object.h"
#include "Camera.h"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "wiringPi.h"
#include <stdio.h>
#include <stdint.h>
#include <softPwm.h>

#define LED 2
#define RED 3

using namespace std;
using namespace cv;


//Options to run code...
//trackobjects to enable object tracking
bool trackObjects = true;
//useMorph0ps to apply Morphological filter to binary image
bool useMorphOps = true;
//calibration mode activates the trackbars. Set to 'false' if values have been hard coded.
bool calibrationMode = true;

bool runmode = false;

//initial min and max HSV filter values.
//these will be changed using trackbars
int H_MIN = 0;
int H_MAX = 255;
int S_MIN = 0;
int S_MAX = 255;
int V_MIN = 0;
int V_MAX = 255;

bool ledflag = 0;
//limits of trigger square
const int X_MIN = -40;
const int X_MAX = 40;
const int Y_MIN = -40;
const int Y_MAX = 40;
//default capture width and height
const int FRAME_WIDTH =160;//640;
const int FRAME_HEIGHT =120;//480;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 25*25;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;
//names that will appear at the top of each window
const string windowName = "Original Image1";
const string windowName1 = "HSV Image1";
const string windowName2 = "Thresholded Image1";
const string windowName3 = "After Morphological Operations1";
//names that will appear at the top of each window
const string windowName4 = "Original Image2";
const string windowName5 = "HSV Image2";
const string windowName6 = "Thresholded Image2";
const string windowName7 = "After Morphological Operations2";
const string trackbarWindowName = "Trackbars";
void on_trackbar( int, void* )
{//This function gets called whenever a
    // trackbar position is changed
    
    //Refresh the console
    cout << string( 100, '\n' );
    
    //this will print the HSV values to console as they are changed
    cout << "H_MIN: " << H_MIN << endl;
    cout << "H_MAX: " << H_MAX << endl;
    cout << "S_MIN: " << S_MIN << endl;
    cout << "S_MAX: " << S_MAX << endl;
    cout << "V_MIN: " << V_MIN << endl;
    cout << "V_MAX: " << V_MAX << endl;
}
string intToString(int number){
    
    
    std::stringstream ss;
    ss << number;
    return ss.str();
}
void createTrackbars(){
    //create window for trackbars
    
    
    namedWindow(trackbarWindowName,0);
    //create memory to store trackbar name on window

    char TrackbarName[50];
    
    sprintf( TrackbarName, "H_MIN", H_MIN);
    sprintf( TrackbarName, "H_MAX", H_MAX);
    sprintf( TrackbarName, "S_MIN", S_MIN);
    sprintf( TrackbarName, "S_MAX", S_MAX);
    sprintf( TrackbarName, "V_MIN", V_MIN);
    sprintf( TrackbarName, "V_MAX", V_MAX);
    //create trackbars and insert them into window
    //3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
    //the max value the trackbar can move (eg. H_HIGH),
    //and the function that is called whenever the trackbar is moved(eg. on_trackbar)
    //                                  ---->    ---->     ---->
    createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
    createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
    createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
    createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
    createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
    createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );
    
    
}
void drawObject(vector <Object> objects,Mat &frame){
    
    //use some of the openCV drawing functions to draw crosshairs
    //on your tracked image!
    
    //UPDATE:JUNE 18TH, 2013
    //added 'if' and 'else' statements to prevent
    //memory errors from writing off the screen (ie. (-25,-25) is not within the window!)
    
    for (int i = 0; i < objects.size(); i++){
        
    circle(frame,Point(objects.at(i).getxPos(),objects.at(i).getyPos()),20,Scalar(0,255,0),2);
    if(objects.at(i).getyPos()-25>0)
        line(frame,Point(objects.at(i).getxPos(),objects.at(i).getyPos()),Point(objects.at(i).getxPos(),objects.at(i).getyPos()-25),Scalar(0,255,0),2);
    else line(frame,Point(objects.at(i).getxPos(),objects.at(i).getyPos()),Point(objects.at(i).getxPos(),0),Scalar(0,255,0),2);
    if(objects.at(i).getyPos()+25<FRAME_HEIGHT)
        line(frame,Point(objects.at(i).getxPos(),objects.at(i).getyPos()),Point(objects.at(i).getxPos(),objects.at(i).getyPos()+25),Scalar(0,255,0),2);
    else line(frame,Point(objects.at(i).getxPos(),objects.at(i).getyPos()),Point(objects.at(i).getxPos(),FRAME_HEIGHT),Scalar(0,255,0),2);
    if(objects.at(i).getxPos()-25>0)
        line(frame,Point(objects.at(i).getxPos(),objects.at(i).getyPos()),Point(objects.at(i).getxPos()-25,objects.at(i).getyPos()),Scalar(0,255,0),2);
    else line(frame,Point(objects.at(i).getxPos(),objects.at(i).getyPos()),Point(0,objects.at(i).getyPos()),Scalar(0,255,0),2);
    if(objects.at(i).getxPos()+25<FRAME_WIDTH)
        line(frame,Point(objects.at(i).getxPos(),objects.at(i).getyPos()),Point(objects.at(i).getxPos()+25,objects.at(i).getyPos()),Scalar(0,255,0),2);
    else line(frame,Point(objects.at(i).getxPos(),objects.at(i).getyPos()),Point(FRAME_WIDTH,objects.at(i).getyPos()),Scalar(0,255,0),2);
    
    putText(frame,intToString(objects.at(i).getxPos())+","+intToString(objects.at(i).getyPos()),Point(objects.at(i).getxPos(),objects.at(i).getyPos()+30),1,1,Scalar(0,255,0),2);
        
    }
}
void morphOps(Mat &thresh){
    
    //create structuring element that will be used to "dilate" and "erode" image.
    //the element chosen here is a 3px by 3px rectangle
    
    Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
    //dilate with larger element so make sure object is nicely visible
    Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));
    
    erode(thresh,thresh,erodeElement);
    erode(thresh,thresh,erodeElement);
    
    
    dilate(thresh,thresh,dilateElement);
    dilate(thresh,thresh,dilateElement);
    
    
    
}
void trackFilteredObject(vector <Object> objects, Mat threshold, Mat &cameraFeed){
    
    
    Mat temp;
    threshold.copyTo(temp);
    //these two vectors needed for output of findContours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //find contours of filtered image using openCV findContours function
    findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
    
    double refArea = 0;
    bool objectFound = false;
    ledflag = 0;
    if (hierarchy.size() > 0) {
        int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects<MAX_NUM_OBJECTS){
            for (int index = 0; index >= 0; index = hierarchy[index][0]) {
                //digitalWrite(RED,0);
                Moments moment = moments((Mat)contours[index]);
                double area = moment.m00;
                // Set up the detector with default parameters.
                
                
                // Draw detected blobs as red circles.
                // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
                Mat frame_with_keypoints;
                //if the area is less than 20 px by 20px then it is probably just noise
                //if the area is the same as the 3/2 of the image size, probably just a bad filter
                //we only want the object with the largest area so we safe a reference area each
                //iteration and compare it to the area in the next iteration.
                
                if(area>MIN_OBJECT_AREA){//) && area<MAX_OBJECT_AREA && area>refArea){
                  
                    Object object1;
                    object1.setxPos(moment.m10/area);
                    object1.setyPos(moment.m01/area);
                   // drawContours(cameraFeed,contours,0,1,3,8,hierarchy);
 
                  
                    
                    if (( object1.getxPos() >= ((FRAME_WIDTH/2)+X_MIN))&&(object1.getxPos() <=((FRAME_WIDTH/2)+X_MAX))&&( object1.getyPos() >= ((FRAME_HEIGHT/2)+Y_MIN))&&(object1.getyPos() <= ((FRAME_HEIGHT/2)+Y_MAX)))
                    {ledflag = 1;}
                    
                    else{ledflag = 0;}
                    
                    objects.push_back(object1);
                    
                    objectFound = true;
                    refArea = area;
                 
                    
                }else {
					objectFound = false;
					ledflag = 0;
				}
                
 
            }
            //let user know you found an object
            if(objectFound ==true){
                //draw object location on screen

                RNG rng(12345);
                Mat threshold_output;
                /// Find the rotated rectangles and ellipses for each contour
                vector<RotatedRect> minRect( contours.size() );
                vector<RotatedRect> minEllipse( contours.size() );
                
                for( int i = 0; i < contours.size(); i++ ){
                    if( contours[i].size() > 10 ){
                     minRect[i] = minAreaRect( Mat(contours[i]) );
                    
                     minEllipse[i] = fitEllipse( Mat(contours[i]) ); }
                }
                
                Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
                for( int i = 0; i< contours.size(); i++ )
                {

                    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
                    Point2f rect_points[4];
                    Point2f midrect_points[4];
                    minRect[i].points( rect_points );
                    ledflag = 0;
                    for( int j = 0; j < 4; j++ ){
                        
                       
                        midrect_points[j].x = floor((rect_points[j].x + rect_points[(j+1)%4].x)/2);
                        midrect_points[j].y = floor((rect_points[j].y + rect_points[(j+1)%4].y)/2);
                        
                        
                        line( cameraFeed, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );

                
                        circle(cameraFeed,midrect_points[j],1,Scalar(0,255,0),2);
                        circle(cameraFeed,rect_points[j],1,Scalar(0,255,0),2);
                        if ((( rect_points[j].x >= ((FRAME_WIDTH/2)+X_MIN))&&(rect_points[j].x <=((FRAME_WIDTH/2)+X_MAX))&&( rect_points[j].y >= ((FRAME_HEIGHT/2)+Y_MIN))&&(rect_points[j].y <= ((FRAME_HEIGHT/2)+Y_MAX))) || (( midrect_points[j].x >= ((FRAME_WIDTH/2)+X_MIN))&&(midrect_points[j].x <=((FRAME_WIDTH/2)+X_MAX))&&( midrect_points[j].y >= ((FRAME_HEIGHT/2)+Y_MIN))&&(midrect_points[j].y <= ((FRAME_HEIGHT/2)+Y_MAX))))
						{
                        ledflag = 1;}
                        
                      
                    }
                    


                //use moments method to find our filtered object
               
                
                               drawObject(objects,cameraFeed);
                               ledflag = 1;
                               //digitalWrite(RED,1);
                }
            
            }
            
        }//else if (objectFound == false)
        //{putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);}
    }
    
}



int main(int argc, char* argv[])
{
	//Set up pi output pins for broadcom numbering
	wiringPiSetupGpio();
	//Setup LED as output pin
	pinMode(LED, OUTPUT);
	pinMode(RED,OUTPUT);
	digitalWrite(RED,1);

    //create vector of objects being tracked for multiple objects
  
	
    vector <Object> objects;
    
    //create vector of camera objects for multiple cameras
    vector <Camera> Cameras;
    
    //Create 2 camera objects for cameras 1 and 2 (change this if fewer are connected, macbook camera is 1 by default)
    Camera Cam1(0, FRAME_WIDTH,FRAME_HEIGHT);
    Cameras.push_back(Cam1);
   // Camera Cam2(1, FRAME_WIDTH,FRAME_HEIGHT);//2
    //Cameras.push_back(Cam2);
    
    
    if (calibrationMode == true){
    //create slider bars for HSV filtering
        createTrackbars();
			}


    //start an infinite loop where webcam feed is copied to cameraFeed matrix
    //all of our operations will be performed within this loop
    while(1){
		//digitalWrite(RED,0);
        for (int cam = 0; cam < Cameras.size(); cam++)
        {
			if (ledflag == 0)
			{digitalWrite (LED, 0);}
			//cout << "light goes on" << endl;
			//ledflag = 1;}
			else if(ledflag == 1){
				digitalWrite (LED, 1);}
				//cout << "light goes off" << endl;
			//ledflag = 0;}
				
        //store image to matrix
             Cameras.at(cam).Readcamfeed();

        //convert frame from BGR to HSV colorspace
             Cameras.at(cam).toHSV();

        //filter HSV image between values and store filtered image to
        //threshold matrix
            Cameras.at(cam).CaminRange(H_MIN,S_MIN,V_MIN,H_MAX,S_MAX,V_MAX);

            //copy image matricies from camera object for this frame
            Mat threshold = Cameras.at(cam).Gethreshold();

            Mat cameraFeed = Cameras.at(cam).GetCameraFeed();

            Mat  HSV = Cameras.at(cam).GetHSV();

            
            //perform morphological operations on thresholded image to eliminate noise
            //and emphasize the filtered object(s)
            if(useMorphOps){

                    morphOps(threshold);}
            
        //pass in thresholded frame to our object tracking function
        //this function will return the x and y coordinates of the
        //filtered object
            if(trackObjects){

                trackFilteredObject(objects,threshold,cameraFeed);}
            
            
        //mark the exclusion zone on the display
        rectangle(cameraFeed,Point((FRAME_WIDTH/2)+X_MIN,(FRAME_HEIGHT/2)+Y_MIN),Point((FRAME_WIDTH/2)+X_MAX,(FRAME_HEIGHT/2)+Y_MAX),Scalar(0,255,0),2);
 
          
            
            
            //Show frames
            if (runmode == false){
            if (cam == 0){

        imshow(windowName2,threshold);
        imshow(windowName,cameraFeed);
                imshow(windowName1,HSV);

                }
            
            else{
                imshow(windowName4,cameraFeed);
            }
		  }
            
            
            
        //delay 30ms so that screen can refresh.
        //image will not appear without this waitKey() command
        waitKey(1);
            
        //clear one line of the console
        cout << endl;
        
        
	}	
      
    }
    
    return 0;//why are you here??!?!
}
