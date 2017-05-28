//
//  edgedetect.cpp
//  cv3Sec
//
//  Created by ChenMorgan on 17/5/12.
//  Copyright © 2017年 ChenMorgan. All rights reserved.
//

#include <opencv2/opencv.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "imageproc.hpp"

using namespace cv;
using namespace std;

int selROI()
{
    Mat srcImg, logoImg, targetImg;
    double alpha = 0.4, beta;
    
    srcImg = imread("/Users/chenxianqin/MyProject/testpic/IMG3.jpeg");
    logoImg = imread("/Users/chenxianqin/MyProject/testpic/Lena.tiff");
    targetImg = srcImg(Rect(200, 250, logoImg.rows, logoImg.cols));
    beta = 1.0 - alpha;
    addWeighted(targetImg, alpha, logoImg, beta, 0.0, targetImg);
    namedWindow("maskpic", WINDOW_AUTOSIZE);
    imshow("blending picture", srcImg);
    imwrite("/Users/chenxianqin/MyProject/testpic/taget/target.png", srcImg);
    
    if(waitKey(7000) > 0)
        return -1;
    
    return 0;
}

int linearBlending()
{
    Mat srcImg, logoImg, targetImg;
    double alpha = 0.5, beta;
    
    srcImg = imread("/Users/chenxianqin/MyProject/testpic/Baboon.tiff");
    logoImg = imread("/Users/chenxianqin/MyProject/testpic/Lena.tiff");
    
    if(srcImg.empty() && logoImg.empty()){
        cerr << "picture read fail !!!" << endl;
        return -1;
    }
    
    cout << "lena size: " << srcImg.size << endl << "Baboon size: " << logoImg.size << endl;
    
    beta = 1.0 - alpha;
    addWeighted(srcImg, alpha, logoImg, beta, 0.0, targetImg);
    
    
    namedWindow("maskpic", WINDOW_AUTOSIZE);
    imshow("copyddddd", targetImg);
    imwrite("/Users/chenxianqin/MyProject/testpic/taget.png", targetImg);
    
    if(waitKey(5000) > 0)
        return -1;
    
    return 0;
}

int edgedetection()
{
   
    Mat srcimg;
    Mat edgePic;
    Mat img1, img2;
    //char response;
    double sum = 0;
    
    
    srcimg = imread("/Users/chenxianqin/MyProject/testpic/Lena.tiff");
    //  namedWindow("test", CV_WINDOW_NORMAL);
    if(srcimg.empty()){
        std::cout << "read image error !!!" << std::endl;
        return 0;
    }
    else{
        cout << "read image success !" << endl;
    }
    std::cout << "Data type of picture: " << srcimg.type() << " Data depth of img: " << srcimg.depth() << std::endl;
    cvtColor(srcimg, edgePic, COLOR_BGR2GRAY);
    blur(srcimg, edgePic, Size(3, 3));
    Canny(edgePic, img1, 50, 100);
    Sobel(edgePic, img2, srcimg.depth(), 1, 1);
    //Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));
    //Mat erodeimg;
    //erode(frame, erodeimg, element);
    
   // img1 = edgePic.clone();
    Mat img = Mat(768, 1366, CV_8UC1, 128);
    MatConstIterator_<double> img_start = img.begin<double>(), img_end = img.end<double>();
    
    for(; img_start != img_end; ++img_start)
        ++sum;//sum += std::max(*img_start, 0.);
    img.row(128) = 0;
    img.col(128) = 0;
    
    cout << "sum of img1 is " << sum << "\n"<< endl;
    
    
    imwrite("/Users/chenxianqin/MyProject/testpic/target/canny_detect.png", img);
    imwrite("/Users/chenxianqin/MyProject/testpic/target/sobel_detect.png", img2);
    
    while (true){
        imshow("edage", img2);
        if(waitKey(0) > 0){
            cout << "break out" << endl;
            break;
        }
    }
    return 0;

}

