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
    image = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
    //image = cv::imread("ImagesForStudents/Image2.png", CV_LOAD_IMAGE_GRAYSCALE);

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    //rectangle(image, Point(1000, 1300), Point(1100, 1400), 0); // Draw crop rectangle on input image (in white)
    Mat image_noise_crop = image(Rect(Point(1000, 1300), Point(1100, 1400)));
    Mat histImage;
    MakeHist(image_noise_crop, histImage);
    namedWindow("Histogram", CV_WINDOW_NORMAL );
    imshow("Histogram", histImage );
    //SaveImage(histImage, "hist_crop_im1", false); // Last parameter is weather the image is normalized

    Mat mag,magLog,phase;
    MagnitudePhase(image, mag, magLog, phase); // 1st argument is input and the other 3 are outputs
    Mat imgout;
    InverseMagnitudePhase(mag, phase, image, imgout);

    cv::normalize(magLog, magLog, 0.0, 1.0, CV_MINMAX); // When showing float images the Mat should be normalized first! IMPORTANT !!
    namedWindow("Mag", CV_WINDOW_NORMAL );
    imshow("Mag", magLog);
    //SaveImage(magLog, "magLog_im1", true); // Last parameter is weather the image is normalized

    cv::normalize(imgout, imgout, 0.0, 1.0, CV_MINMAX); // When showing float images the Mat should be normalized first! IMPORTANT !!
    namedWindow("Output Image", CV_WINDOW_NORMAL );
    imshow("Output Image", imgout);


    namedWindow("Input Image", CV_WINDOW_NORMAL );
    imshow("Input Image", image);

    Mat filteredImage, csImage; //cs=ContrastStretching
    image.copyTo(filteredImage);
    image.copyTo(csImage);

    FilterContraharmonicMean(image, filteredImage, 5, 1);
    namedWindow("filteredImage", CV_WINDOW_NORMAL );
    imshow("filteredImage", filteredImage);
    ContrastStretching(filteredImage, csImage);
    namedWindow("csImage", CV_WINDOW_NORMAL );
    imshow("csImage", csImage);


    waitKey(0);
    return 0;
}
