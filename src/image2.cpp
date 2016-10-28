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

    // *** Histogram ex ***
    //rectangle(image, Point(1000, 1300), Point(1100, 1400), 0); // Draw crop rectangle on input image (in white)
    // Mat image_noise_crop = image(Rect(Point(1000, 1300), Point(1100, 1400)));
    // Mat histImage;
    // MakeHist(image_noise_crop, histImage);
    // namedWindow("Histogram", CV_WINDOW_NORMAL );
    // imshow("Histogram", histImage );
    //SaveImage(histImage, "hist_crop_im1", false); // Last parameter is weather the image is normalized

    // *** Freq and phase ex ***
    // Mat mag,magLog,phase;
    // MagnitudePhase(image, mag, magLog, phase); // 1st argument is input and the other 3 are outputs
    // cv::normalize(magLog, magLog, 0.0, 1.0, CV_MINMAX); // When showing float images the Mat should be normalized first! IMPORTANT !!
    // namedWindow("Mag", CV_WINDOW_NORMAL );
    // imshow("Mag", magLog);
    //SaveImage(magLog, "magLog_im1", true); // Last parameter is weather the image is normalized

    // *** Freq and phase ex ***
    // Mat imgout;
    // InverseMagnitudePhase(mag, phase, image, imgout);
    // cv::normalize(imgout, imgout, 0.0, 1.0, CV_MINMAX); // When showing float images the Mat should be normalized first! IMPORTANT !!
    // namedWindow("Output Image", CV_WINDOW_NORMAL );
    // imshow("Output Image", imgout);


    namedWindow("Input Image", CV_WINDOW_NORMAL );
    imshow("Input Image", image);

    Mat filteredImage, csImage, equalHist; //cs=ContrastStretching
    image.copyTo(filteredImage);
    image.copyTo(csImage);
    image.copyTo(equalHist);

    FilterAdaptiveMedian(image, filteredImage, 3, 5, 0, 255); // A max_kernel of 15 gives the optimal (only small noise reduction from 13 to 15 but visually none from 15 to 17). If time is a criteria it ca be lowered to 11 without major loss.
    namedWindow("filteredImage", CV_WINDOW_NORMAL );
    imshow("filteredImage", filteredImage);
    SaveImage(filteredImage, "im2-input-to-AdaptiveMedian", false); // Last parameter is weather the image is normalized


    ContrastStretching(filteredImage, csImage);
    namedWindow("csImage", CV_WINDOW_NORMAL );
    imshow("csImage", csImage);
    SaveImage(csImage, "im2-AdaptiveMean-to-ContrastStretching", false); // Last parameter is weather the image is normalized

    equalizeHist(csImage, equalHist); //equalize the histogram
    namedWindow("equalHist", CV_WINDOW_NORMAL );
    imshow("equalHist", equalHist);
    SaveImage(equalHist, "im2-ContrastStretching-to-equalHist", false); // Last parameter is weather the image is normalized

    waitKey(0);
    return 0;
}
