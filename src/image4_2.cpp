#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "functions.hpp"

using namespace cv;

int main(int argc, char** argv )
{
    Mat image;
    image = imread( "ImagesForStudents/Image4_2.png", CV_LOAD_IMAGE_GRAYSCALE );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    namedWindow("Display Image", CV_WINDOW_NORMAL );
    imshow("Display Image", image);



    Mat image_noise_crop = image(Rect(Point(1000, 1300), Point(1100, 1400)));
    SaveImage(image_noise_crop, "image_noise_crop_before", false);

    //*** Freq and phase ex ***
    Mat mag,magLog,phase;
    MagnitudePhase(image, mag, magLog, phase); // 1st argument is input and the other 3 are outputs
    cv::normalize(magLog, magLog, 0.0, 1.0, CV_MINMAX); // When showing float images the Mat should be normalized first! IMPORTANT !!
    namedWindow("Mag", CV_WINDOW_NORMAL );
    imshow("Mag", magLog);


   std::vector<Center> listOfNotchPoints;

   //Locations of the four frequency peaks
   listOfNotchPoints.push_back(Center(2105,1814));
   listOfNotchPoints.push_back(Center(2342,2401));
   listOfNotchPoints.push_back(Center(1523,1573));
   listOfNotchPoints.push_back(Center(968,1815));

   ApplyNotchFilter(mag, phase, listOfNotchPoints, 26, 5); // order = 5, diameter of notch = 26

    // Used for visualization only
    Mat magnitude = mag + 1.0f;
    cv::log(magnitude, magnitude);
    cv::normalize(magnitude, magnitude, 0, 1, CV_MINMAX);
    namedWindow("img_dft", CV_WINDOW_NORMAL );
    cv::imshow("img_dft", magnitude);
    cv::imwrite("img_dft.png", magnitude * 255);


    // *** Freq and phase ex ***
    Mat imgout;
    InverseMagnitudePhase(mag, phase, image, imgout);
    cv::normalize(imgout, imgout, 0.0, 1.0, CV_MINMAX); // When showing float images the Mat should be normalized first! IMPORTANT !!
    namedWindow("Output Image", CV_WINDOW_NORMAL );
    imshow("Output Image", imgout);
    SaveImage(imgout, "result_im4_2", true);

    image_noise_crop = magnitude(Rect(Point(1000, 1300), Point(1100, 1400)));
    SaveImage(image_noise_crop, "image_noise_crop_after", true);


    waitKey(0);

    return 0;
}
