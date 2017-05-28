//
//  main.cpp
//  cv3Sec
//
//  Created by 沈贤勤 on 17/5/5.
//  Copyright © 2017年 沈贤勤. All rights reserved.
//

//
//  main.cpp
//  Camera_Capture
//
//  Created by zhoufan on 15/5/24.
//  Copyright (c) 2015年 zhoufan. All rights reserved.
//

/**
 @file videocapture_basic.cpp
 @brief A very basic sample for using VideoCapture and VideoWriter
 @author PkLab.net
 @date Aug 24, 2016
 */


#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
//#include <vector>
//#include <imageproc.hpp>
//#include <videocap.hpp>

using namespace cv;
using namespace std;

int dft_demo(int, char**)
{
    Mat srcImg, tmpImg, dstImg, padded;
    
    srcImg = imread("/Users/chenxianqin/MyProject/testpic/Baboon.tiff", IMREAD_GRAYSCALE);
    if(srcImg.empty())
    {
        cerr << "Cannot read image file !" << endl;
        return -1;
    }
    int m = getOptimalDFTSize(srcImg.rows);
    int n = getOptimalDFTSize(srcImg.cols);
    copyMakeBorder(srcImg, padded, 0, m - srcImg.rows, 0, n - srcImg.cols, BORDER_CONSTANT);
    // merge planes to muti channel matrix complexI
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexImg;
    merge(planes, 2, complexImg);
    
    dft(complexImg, complexImg);
    
    // compute log(1 + sqrt(Re(DFT(img))**2 + Im(DFT(img))**2))
    split(complexImg, planes);
    magnitude(planes[0], planes[1], planes[0]);
    Mat mag = planes[0];
    mag += Scalar::all(1);
    log(mag, mag);
    
    // crop the spectrum, if it has an odd number of rows or columns
    mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));
    
    int cx = mag.cols/2;
    int cy = mag.rows/2;
    
    // rearrange the quadrants of Fourier image
    // so that the origin is at the image center
    Mat tmp;
    Mat q0(mag, Rect(0, 0, cx, cy));
    Mat q1(mag, Rect(cx, 0, cx, cy));
    Mat q2(mag, Rect(0, cy, cx, cy));
    Mat q3(mag, Rect(cx, cy, cx, cy));
    
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
    
    normalize(mag, mag, 0, 1, NORM_MINMAX);
    imshow("original", srcImg);
    imshow("DFT", mag);
    
    waitKey(0);
    destroyAllWindows();
    
    return 0;
}

