#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "functions.hpp"

void MakeHist(Mat & inputSrc, Mat &histImage){
    int histSize = 256; //from 0 to 255
    /// Set the ranges
    float range[] = { 0, 256 } ; //the upper boundary is exclusive
    const float* histRange = { range };
    bool uniform = true; bool accumulate = false;
    Mat gray_hist;
    calcHist( &inputSrc, 1, 0, Mat(), gray_hist, 1, &histSize, &histRange, uniform, accumulate ); // Inout image goes here

    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );

    Mat tempHist( hist_h, hist_w, CV_8UC3, Scalar( 255,255,255) );

    tempHist.copyTo(histImage);

    normalize(gray_hist, gray_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    /// Draw for each channel
    for( int i = 0; i < histSize; i++ )
    {
      rectangle( histImage,
           cv::Point(bin_w*(i),hist_h),
           cv::Point(bin_w*(i+1)-1,hist_h - cvRound(gray_hist.at<float>(i))),
           Scalar( 0,saturate_cast<uchar>(gray_hist.at<float>(i)*5), 0),
           -1);
        //std::cout << "Bin no. " << i << " " << cvRound(gray_hist.at<float>(i)) << " and " << gray_hist.at<float>(i) << std::endl;
    }
    // Histogram code - EN
}
