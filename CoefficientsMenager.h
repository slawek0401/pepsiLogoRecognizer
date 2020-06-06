#pragma once
#define _USE_MATH_DEFINES
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>

class CoefficientsMenager
{
public:
    static double wspMalinowskiej(int s, int l);
    static double maleM(cv::Mat& I, int p, int q);
    static double maleM(std::vector<cv::Point> vec, int p, int q);
    static cv::Point findMiddle(cv::Mat image);
    static double M1(std::vector<cv::Point> image);
    static double M7(std::vector<cv::Point> image);
    //static void countCoef(std::vector<std::string> names, int filter[3][3]);
};

