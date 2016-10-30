#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "functions.hpp"

using namespace cv;

int main(int argc, char** argv )
{
    Mat image;
    image = imread( "ImagesForStudents/Image3.png", CV_LOAD_IMAGE_GRAYSCALE );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    namedWindow("Display Image", CV_WINDOW_NORMAL );
    imshow("Display Image", image);



    Mat image_noise_crop = image(Rect(Point(1000, 1300), Point(1100, 1400)));
    SaveImage(image_noise_crop, "image_noise_crop_before", false);


    medianBlur(image,image, 7);

    cv::namedWindow("Output image",CV_WINDOW_NORMAL);
    cv::imshow("Output image", image);


    // Generate kernel using:
    cv::Mat_<float> kernel(3,3);
    kernel << 0,  1, 0,
              1, -4, 1,
              0,  1, 0;

    SharpenFilter(image,kernel);
    cv::namedWindow("Output sharpened",CV_WINDOW_NORMAL);
    cv::imshow("Output sharpened", image);
    SaveImage(image,"result_im3", false);
    cv::waitKey(0);

    return 0;
}
