#include <stdio.h>
#include <string>
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
    // Histogram code - END
}

void SaveImage(Mat & inputSrc, std::string file_name, bool inNormalized) {
  // Image save code taken from http://docs.opencv.org/2.4/modules/highgui/doc/reading_and_writing_images_and_video.html
  std::vector<int> compression_params;
  compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
  compression_params.push_back(9); // CV_IMWRITE_PNG_COMPRESSION
  if (inNormalized)
    imwrite(file_name+".png", inputSrc* 255, compression_params);
  else
    imwrite(file_name+".png", inputSrc, compression_params);

}

void dftshift(cv::Mat_<float> & magnitude) {
  const int cx = magnitude.cols/2;
  const int cy = magnitude.rows/2;

  cv::Mat_<float> tmp;
  cv::Mat_<float> topLeft(magnitude, cv::Rect(0, 0, cx, cy));
  cv::Mat_<float> topRight(magnitude, cv::Rect(cx, 0, cx, cy));
  cv::Mat_<float> bottomLeft(magnitude, cv::Rect(0, cy, cx, cy));
  cv::Mat_<float> bottomRight(magnitude, cv::Rect(cx, cy, cx, cy));

  topLeft.copyTo(tmp);
  bottomRight.copyTo(topLeft);
  tmp.copyTo(bottomRight);

  topRight.copyTo(tmp);
  bottomLeft.copyTo(topRight);
  tmp.copyTo(bottomLeft);
}

void MagnitudePhase(cv::Mat & inputSrc, cv::Mat & magnitude, cv::Mat & magnitudeLog, cv::Mat & phase)
// http://docs.opencv.org/2.4/doc/tutorials/core/discrete_fourier_transform/discrete_fourier_transform.html
{
    // Make an internal copy to avoid Mat type errors
    cv::Mat_<float> tempInputSrc;
    inputSrc.convertTo(tempInputSrc, CV_32F);
    //inputSrc.copyTo(tempInputSrc);

    //Pad the image with borders using copyMakeBorders. Use getOptimalDFTSize(A+B-1). See G&W page 251,252 and 263 and dft tutorial. (Typicly A+B-1 ~ 2A is used)
    int rows = cv::getOptimalDFTSize(2*tempInputSrc.rows);
    int cols = cv::getOptimalDFTSize(2*tempInputSrc.cols);
    cv::Mat_<float> img_padded;
    copyMakeBorder(tempInputSrc, img_padded, 0, rows - tempInputSrc.rows, 0, cols - tempInputSrc.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    //The second channel should be all zeros.
    cv::Mat_<float> imgs[] = {img_padded.clone(), cv::Mat_<float>(img_padded.rows, img_padded.cols, 0.0f)};
    cv::Mat_<cv::Vec2f> img_dft;
    cv::merge(imgs,2,img_dft);

    cv::dft(img_dft, img_dft); // Compute DFT using img_dft as input
    cv::split(img_dft, imgs); // Split img_dft, you can save result into imgs

    cv::Mat_<float> tempMagnitude, tempPhase; // Make temp Mats for phase and mag
    cv::cartToPolar(imgs[0],imgs[1],tempMagnitude,tempPhase);

    // Shift magnitude quadrants for viewability, use dftshift
    dftshift(tempMagnitude);

    // Take logarithm of magnitude  (log())
    cv::Mat_<float> magnitudel;
    magnitudel = tempMagnitude.clone();
    magnitudel += cv::Scalar::all(1); // switch to logarithmic scale
    cv::log(magnitudel, magnitudel);

    // Copy internal images to input arguments
    tempMagnitude.copyTo(magnitude);
    magnitudel.copyTo(magnitudeLog);
    tempPhase.copyTo(phase);
}

void InverseMagnitudePhase(cv::Mat & inputMagnitude, cv::Mat & inputPhase, cv::Mat & oldImage, cv::Mat & output_image)
// http://docs.opencv.org/2.4/doc/tutorials/core/discrete_fourier_transform/discrete_fourier_transform.html
{
    int imgRows = oldImage.rows;
    int imgCols = oldImage.cols;
    cv::Mat_<float> tempMagnitude = inputMagnitude.clone(); // Do not mess with external passed image
    dftshift(tempMagnitude); // Shift back magnitude quadrants of the spectrum, use dftshift();

    cv::Mat_<float> imgs[2];
    polarToCart(tempMagnitude,inputPhase,imgs[0],imgs[1]); // Compute complex DFT output from magnitude/phase (polarToCart()), store result in imgs

    cv::merge(imgs,2,output_image); // Merge DFT (imgs) into one image

    //Restore, use dft with DFT_INVERSE flag, save result in imgout
    cv::dft(output_image, output_image, cv::DFT_INVERSE| cv::DFT_REAL_OUTPUT);

    output_image = output_image(cv::Rect(0,0,imgCols,imgRows));
}

void FilterContraharmonicMean(cv::Mat & inputImage, cv::Mat & outputImage, int kernel_size, int Q)
// Function: Contraharmonic mean filter.
// Description: For positive values of Q, the filter eliminates pepper noise. For negative values of Q it eliminates salt noise.
// NOTE: the kernel size is given as the height or width of the kernel and should be odd, not the area of the kernel
// Made from eq 5.3-6 in GW book p. 323
{
    int im_max_col = inputImage.cols;
    int im_max_row = inputImage.rows;
    int x = 0;
    int y = 0;
    int internal_kernel = (kernel_size-1)/2;

    for (int row = 0; row < im_max_row; row++) {
        for (int col = 0; col < im_max_col; col++) {
            int num = 0;
            int den = 0;
            for (int s = -internal_kernel; s <= internal_kernel; s++) { // NOTE: you cannot use size_t here!!
              for (int t = -internal_kernel; t <= internal_kernel; t++) {
                  x = col + s;
                  y = row + t;
                  if ((x >= 0 and x<im_max_col) and (y>=0 and y<im_max_row)) {
                      num += pow(inputImage.at<uchar>(y,x),Q+1);
                      den += pow(inputImage.at<uchar>(y,x),Q);
                  }
              }
            }
            if (den != 0)
                outputImage.at<uchar>(row,col) = static_cast<uchar>(round(num/den));
            else
                outputImage.at<uchar>(row,col) = 255;
        }
    }
}

void ContrastStretching(cv::Mat & inputImage, cv::Mat & outputImage)
// https://www.bruzed.com/2009/10/contrast-stretching-and-histogram-equalization/
{
    int max_8u = 255;
    int im_max_col = inputImage.cols;
    int im_max_row = inputImage.rows;

    double min, max;
    cv::minMaxLoc(inputImage, &min, &max);
    std::cout << "Max: " << max << std::endl;
    std::cout << "Min: " << min << std::endl;

    for (int row = 0; row < im_max_row; row++) {
        for (int col = 0; col < im_max_col; col++) {
            outputImage.at<uchar>(row,col) = round(((inputImage.at<uchar>(row,col) - min) / (max - min)) * max_8u);
        }
    }
}
