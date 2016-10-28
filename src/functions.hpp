#include <stdio.h>
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void MakeHist(Mat & inputSrc, Mat &histImage);
void SaveImage(Mat & inputSrc, std::string file_name, bool inNormalized);
void dftshift(cv::Mat_<float> & magnitude);
void MagnitudePhase(cv::Mat & inputSrc, cv::Mat & magnitude, cv::Mat & magnitudeLog, cv::Mat & phase);
void InverseMagnitudePhase(cv::Mat & inputMagnitude, cv::Mat & inputPhase, cv::Mat & oldImage, cv::Mat & output_image);

#endif
