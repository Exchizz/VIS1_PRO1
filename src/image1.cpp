#include <stdio.h>
#include <vector>
#include <opencv2/opencv.hpp>
#include "functions.hpp"

using namespace cv;

int main(int argc, char** argv )
{
    Mat image, dst;
    image = cv::imread("ImagesForStudents/Image1.png", CV_LOAD_IMAGE_GRAYSCALE);

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    namedWindow("Input Image", CV_WINDOW_NORMAL );
    imshow("Input Image", image);

    //rectangle(image, Point(1000, 1300), Point(1100, 1400), 0); // Draw crop rectangle on input image (in white)
    Mat image_noise_crop = image(Rect(Point(1000, 1300), Point(1100, 1400)));
    Mat histImage;
    MakeHist(image_noise_crop, histImage);
    namedWindow("Histogram", CV_WINDOW_NORMAL );
    imshow("Histogram", histImage );

    Mat filteredImage, csImage, equalHist, filteredImage2nd; //cs=ContrastStretching
    image.copyTo(filteredImage);
    image.copyTo(csImage);
    image.copyTo(equalHist);
    image.copyTo(filteredImage2nd);

    FilterContraharmonicMean(image, filteredImage, 5, 1);
    namedWindow("filteredImage", CV_WINDOW_NORMAL );
    imshow("filteredImage", filteredImage);
    SaveImage(filteredImage, "im1-ContraharmonicMean-to-ContraharmonicMean", false); // Last parameter is weather the image is normalized

    ContrastStretching(filteredImage, csImage);
    namedWindow("csImage", CV_WINDOW_NORMAL );
    imshow("csImage", csImage);
    SaveImage(csImage, "im1-ContraharmonicMean-to-ContrastStretching", false); // Last parameter is weather the image is normalized

    equalizeHist(csImage, equalHist); // Actually increases the noise from the prevoius image
    namedWindow("equalHist", CV_WINDOW_NORMAL );
    imshow("equalHist", equalHist);
    SaveImage(equalHist, "im1-ContrastStretching-to-equalHist", false); // Last parameter is weather the image is normalized


    FilterContraharmonicMean(equalHist, filteredImage2nd, 3, 0); // Remove some of the last noise
    namedWindow("filteredImage2nd", CV_WINDOW_NORMAL );
    imshow("filteredImage2nd", filteredImage2nd);
    SaveImage(filteredImage2nd, "im1-equalHist-to-ContraharmonicMean", false); // Last parameter is weather the image is normalized

    // Can also be filtered using median filter instead of FilterContraharmonicMean
    //medianBlur(InputArray src, OutputArray dst, 11)

    waitKey(0);
    return 0;
}
