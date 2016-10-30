#include <stdio.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void MakeHist(Mat & inputSrc, Mat &histImage);
void SaveImage(Mat & inputSrc, std::string file_name, bool inNormalized);
void dftshift(cv::Mat_<float> & magnitude);
void MagnitudePhase(cv::Mat & inputSrc, cv::Mat & magnitude, cv::Mat & magnitudeLog, cv::Mat & phase);
void InverseMagnitudePhase(cv::Mat & inputMagnitude, cv::Mat & inputPhase, cv::Mat & oldImage, cv::Mat & output_image);
void FilterContraharmonicMean(cv::Mat & inputImage, cv::Mat & outputImage, int kernel_size, int Q);
void ContrastStretching(cv::Mat & inputImage, cv::Mat & outputImage);
void FilterAdaptiveMedian(cv::Mat & inputImage, cv::Mat & outputImage, int start_kernel_size, int max_kernel_size, int z_min, int z_max);

struct Center {
        int x;
        int y;
	Center(int x, int y){
		this->x = x;
		this->y = y;
	}
};
void ApplyNotchFilter(Mat & magnitudeInput, Mat & phaseInput, std::vector<Center> notch_centers, int d0k, unsigned int n);
void SharpenFilter(Mat & InputSrc, Mat & kernel);

// We probably also need the following functions (DO NOT DELETE)
// - Sharpen
// - Butterworth (notch highpass) G&W section 4.10 - for the frequency filtering - could be nice if it takes points in a vec as input and then a width for the filter and filters the points away.
// - Bilateral filter - see pp. lec 05 part 2 slide 28 : OpenCV has a func called bilateralFilter, see http://docs.opencv.org/2.4/doc/tutorials/imgproc/gausian_median_blur_bilateral_filter/gausian_median_blur_bilateral_filter.html
// The following pictures have been treated and output images saved to sharelatex: 1,2,4

#endif
