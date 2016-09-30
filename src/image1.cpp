#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;


int main(int argc, char** argv )
{
    Mat image, dst;
    image = cv::imread("ImagesForStudents/Image2.png", CV_LOAD_IMAGE_GRAYSCALE);


    medianBlur ( image, dst, 11 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    namedWindow("Display Image", CV_WINDOW_NORMAL );
    imshow("Display Image", dst);

    waitKey(0);

    return 0;
}
