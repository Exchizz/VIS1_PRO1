#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "functions.hpp"

using namespace cv;

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image;
    image = imread( argv[1], 1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    cvtColor(image, image, CV_BGR2GRAY); // Ensure image is grayscale with one channel

    // rectangle(image, Point(1000, 1300), Point(1100, 1400), 0); // Draw crop rectangle on input image (in white)
    Mat image_noise_crop = image(Rect(Point(1000, 1300), Point(1100, 1400)));
 
    Mat histImage;
    MakeHist(image_noise_crop, histImage);
    // Histogram code - START - inspired from http://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/histogram_calculation/histogram_calculation.html
 //   int histSize = 256; //from 0 to 255
    /// Set the ranges
 //   float range[] = { 0, 256 } ; //the upper boundary is exclusive
 //   const float* histRange = { range };
 //   bool uniform = true; bool accumulate = false;
 //   Mat gray_hist;
 //   calcHist( &image_noise_crop, 1, 0, Mat(), gray_hist, 1, &histSize, &histRange, uniform, accumulate ); // Inout image goes here

  //  int hist_w = 512; int hist_h = 400;
 //   int bin_w = cvRound( (double) hist_w/histSize );

 //   Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 255,255,255) );
 //   normalize(gray_hist, gray_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    /// Draw for each channel
 //   for( int i = 0; i < histSize; i++ )
//    {
//      rectangle( histImage,
//           cv::Point(bin_w*(i),hist_h),
//           cv::Point(bin_w*(i+1)-1,hist_h - cvRound(gray_hist.at<float>(i))),
//           Scalar( 0,saturate_cast<uchar>(gray_hist.at<float>(i)*5), 0),
//           -1);
        //std::cout << "Bin no. " << i << " " << cvRound(gray_hist.at<float>(i)) << " and " << gray_hist.at<float>(i) << std::endl;
//    }
    // Histogram code - END

    namedWindow("Input Image", CV_WINDOW_NORMAL );
    imshow("Input Image", image);

    namedWindow("Histogram", CV_WINDOW_NORMAL );
    imshow("Histogram", histImage );


    //namedWindow("Display Image", CV_WINDOW_AUTOSIZE );
    //imshow("Display Image", image);

    waitKey(0);

    return 0;
}
