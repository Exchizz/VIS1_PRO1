#include <stdio.h>
#include <vector>
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

    Mat mag,phase;
    MagnitudePhase(image, mag, phase);
    //Mat imgout;
    //InverseMagnitudePhase(mag, phase, imgout);

    namedWindow("Mag", CV_WINDOW_NORMAL );
    imshow("Mag", mag);

    namedWindow("Input Image", CV_WINDOW_NORMAL );
    imshow("Input Image", image);

    namedWindow("Histogram", CV_WINDOW_NORMAL );
    imshow("Histogram", histImage );

    SaveImage(histImage, "hist_crop_im1");


    //namedWindow("Display Image", CV_WINDOW_AUTOSIZE );
    //imshow("Display Image", image);

    waitKey(0);

    return 0;
}
