//
//  videocap.cpp
//  cv3Sec
//
//  Created by ChenMorgan on 17/5/12.
//  Copyright © 2017年 ChenMorgan. All rights reserved.
//

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "videocap.hpp"


using namespace cv;
using namespace std;


int getvideostream(char devIdx)
{
    int fps, height, width;
    Mat frame, graypic, edgpic;
    VideoCapture cap(0);
    
    cap.open(devIdx);
    
    string const mywords = "Are you OK ?";

    Point org;
    Scalar txtColor(255, 255, 255);
    //int i;
    //frame.ptr<uchar>(i);
    
  
    if(!cap.isOpened()){
        std::cout << "camera open error !" << std::endl;
        return -1;
    }

    cap.set(CAP_PROP_FPS, 30);
    fps = cap.get(CAP_PROP_FPS);
    width = cap.get(CAP_PROP_FRAME_WIDTH);
    height = cap.get(CAP_PROP_FRAME_HEIGHT);
    cout << "video property" << endl << "heigh: " << height << endl << "width: " << width << endl;
        
    while(true){
       
        cap >> frame;
        cap.read(frame);
        if(frame.empty()){
            cerr << "The video stream is empty!" << std::endl;
            return -1;
        }
        cvtColor(frame, graypic, COLOR_BGR2GRAY);
        GaussianBlur(graypic, graypic, Size(3, 5), 1.2, 1.2);
        Canny(graypic, edgpic, 30, 50);

        org.x = frame.rows / 5;
        org.y = frame.cols / 7;
        putText(edgpic, mywords, org, FONT_HERSHEY_SCRIPT_SIMPLEX, 2, txtColor);

        //namedWindow("cap video");
        imshow("Morgan's View", edgpic);

        if(waitKey(10) > 0){
            cout << "break out !" << endl;
            break;
        }
        
    }

    destroyAllWindows();
    return 0;
}

int createVideoStream(char devIdx)
{
    Mat src, logoPic, selROI;
    int width, height;
    double fps;
    int codec;
    double alpha = 0.5, beta = 1.0 - alpha;
    double timeStart, timeEnd;
    int timeout = 240;
    
    // use default camera as video source
    VideoCapture cap(devIdx);
    // check if we succeeded
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    
    logoPic = imread("/Users/chenxianqin/MyProject/testpic/opencv-logo-small.png");
    if(logoPic.empty()){
        cerr << "picture read fail !" << endl;
        return -1;
    }
    else{
        cout << "picture size: " << logoPic.rows << " * " << logoPic.cols << endl;
    }
    // get one frame from camera to know frame size and type
    cap >> src;
    // check if we succeeded
    if (src.empty()) {
        cerr << "ERROR! blank frame grabbed\n";
        return -1;
    }

    width = cap.get(CAP_PROP_FRAME_WIDTH);
    height = cap.get(CAP_PROP_FRAME_HEIGHT);
    codec = cap.get(CAP_PROP_FOURCC);
    bool isColor = (src.type() == CV_8UC3);
    
    //--- INITIALIZE VIDEOWRITER
    VideoWriter recoder;
    codec = CV_FOURCC('H', '2', '6', '4');  // select desired codec (must be available at runtime)
    fps = 30.0;       // framerate of the created video stream
    const string filename = "/Users/chenxianqin/MyProject/testpic/target/capstream.avi";      // name of the output video file
    
    recoder.open(filename, codec, fps, src.size(), isColor);
    // check if we succeeded
    if (!recoder.isOpened()) {
        cerr << "Could not open the output video file for write\n";
        return -1;
    }
    
    //--- GRAB AND WRITE LOOP
    cout << "Writing videofile: " << filename << endl;
    timeStart = static_cast<double>(getTickCount());
    for (;;)
    {
        // check if we succeeded
        if (!cap.read(src)) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        selROI = src(Rect(100, 120, logoPic.cols, logoPic.rows));
        addWeighted(selROI, alpha, logoPic, beta, 0.0, selROI);
        recoder << src; // overload write() function/method

        // show live and wait for a key with timeout long enough to show images
        namedWindow("cap video");
        imshow("Live", src);
        
        --timeout;
        
        if(timeout <= 0)
            break;
        
    }
    timeEnd = ((double)getTickCount() - timeStart) / getTickFrequency();
    cout << "time cost: " << timeEnd << endl;
    // the videofile will be closed and released automatically in VideoWriter destructor
    return 0;
}

