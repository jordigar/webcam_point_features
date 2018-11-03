
//OpenCV
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"

//std
#include <iostream>
#include <cstdlib>
#include <vector>

//consts
const unsigned int MIN_NUM_FEATURES = 300; //minimum number of point fetaures

int main(int argc, char *argv[])
{
    cv::VideoCapture camera; //OpenCV video capture object
    cv::Mat image; //OpenCV image object
	int cam_id; //camera id . Associated to device number in /dev/videoX
    cv::Ptr<cv::ORB> orb_detector = cv::ORB::create(); //ORB point feature detector
    orb_detector->setMaxFeatures(MIN_NUM_FEATURES);
    std::vector<cv::KeyPoint> point_set; //set of point features
    cv::Mat descriptor_set; //set of descriptors, for each feature there is an associated descriptor
    //cv::Mat mask; //mask
    //unsigned char * px_value1;

	//check user args
	switch(argc)
	{
		case 1: //no argument provided, so try /dev/video0
			cam_id = 0;
			break;
		case 2: //an argument is provided. Get it and set cam_id
			cam_id = atoi(argv[1]);
			break;
		default:
			std::cout << "Invalid number of arguments. Call program as: webcam_capture [video_device_id]. " << std::endl;
			std::cout << "EXIT program." << std::endl;
			break;
	}

	//advertising to the user
	std::cout << "Opening video device " << cam_id << std::endl;

    //open the video stream and make sure it's opened
    if( !camera.open(cam_id) )
	{
        std::cout << "Error opening the camera. May be invalid device id. EXIT program." << std::endl;
        return -1;
    }

    //Process loop. Capture and point feature extraction. User can quit pressing a key
    while(1)
	{
		//Read image and check it. Blocking call up to a new image arrives from camera.
        if(!camera.read(image))
		{
            std::cout << "No image" << std::endl;
            cv::waitKey();
        }

    //**************** Find ORB point fetaures and descriptors ****************************

        //clear previous points
        point_set.clear();

        //detect and compute(extract) features
        //orb_detector->detectAndCompute(image, cv::noArray(), point_set, descriptor_set);
        //use 4 mask for detect features points
        for(int i=0; i<4; i++)
        {
          cv::Mat mask = cv::Mat::zeros(image.size(), CV_8U);
          if (i==0)
          {
            cv::Mat roi(mask, cv::Rect(0, 0, image.cols/2, image.rows/2));
            roi = cv::Scalar(255);
            orb_detector->detectAndCompute(image, mask, point_set, descriptor_set);
            cv::drawKeypoints( image, point_set, image, 255, cv::DrawMatchesFlags::DEFAULT );
            cv::imshow("Output Window Mask 0", mask); //show mask
          }
          if (i==1)
          {
            cv::Mat roi(mask, cv::Rect(image.cols/2, 0, image.cols/2, image.rows/2));
            roi = cv::Scalar(255);
            orb_detector->detectAndCompute(image, mask, point_set, descriptor_set);
            cv::drawKeypoints( image, point_set, image, 255, cv::DrawMatchesFlags::DEFAULT );
            cv::imshow("Output Window Mask 1", mask); //show mask
          }
          if (i==2)
          {
            cv::Mat roi(mask, cv::Rect(0, image.rows/2, image.cols/2, image.rows/2));
            roi = cv::Scalar(255);
            orb_detector->detectAndCompute(image, mask, point_set, descriptor_set);
            cv::drawKeypoints( image, point_set, image, 255, cv::DrawMatchesFlags::DEFAULT );
            cv::imshow("Output Window Mask 2", mask); //show mask
          }
          if (i==3)
          {
            cv::Mat roi(mask, cv::Rect(image.cols/2, image.rows/2, image.cols/2, image.rows/2));
            roi = cv::Scalar(255);
            cv::drawKeypoints( image, point_set, image, 255, cv::DrawMatchesFlags::DEFAULT );
            cv::imshow("Output Window Mask 3", mask); //show mask
          }
        }

        //creat mask
        //mask=image;
        //mask.rows=image.rows/4;
        //mask.cols=image.cols/4;
        //mask.resize(image.rows/4, image.cols/4);
        //mask.resize(200, 200);
        //cv::Mat mask(image.rows/2, image.cols/2, CV_8UC3); //mask
        //cv::Mat mask(100, 100, CV_8UC1); //mask
        //for(int i = 0; i < image.rows/2; i++)
       //{
        //for(int j = 0; j < image1.cols; j++)
       //for(int j = 0; j < image.cols/2; j++)
       //{
        //       px_value1 = mask.ptr(i,j);
        //        px_value1[0] = 255;
        //        px_value1[1] = 255;
        //        px_value1[2] = 255;
       //}
      //}

        //mask.zeros(mask.rows, mask.cols, CV_32F);
        //mask.ones(mask.rows/4, mask.cols/4, CV_8UC1);
        //orb_detector->detectAndCompute(image, cv::noArray(), point_set, descriptor_set);

        //draw points on the image
        cv::drawKeypoints( image, point_set, image, 255, cv::DrawMatchesFlags::DEFAULT );

    //********************************************************************

        //show image
        cv::imshow("Output Window", image);

		//Waits 30 millisecond to check if 'q' key has been pressed. If so, breaks the loop. Otherwise continues.
    	if( (unsigned char)(cv::waitKey(30) & 0xff) == 'q' ) break;
    }
}
