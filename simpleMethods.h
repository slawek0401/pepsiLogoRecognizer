#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>

bool isInDivision(int a, int down, int up);
int inDivision(int a, int b, int c);
int inDivision(int a);
int countPixels(cv::Mat& I, int value);
int countPixels(cv::Mat& I, int value, int fromX, int fromY, int toX, int toY);
void printHistogram(const cv::Mat& I);
int mean(cv::Mat_<cv::Vec3b>& image, int fromX, int fromY, int toX, int toY, int channel);
cv::Mat and(cv::Mat& im1, cv::Mat& im2);