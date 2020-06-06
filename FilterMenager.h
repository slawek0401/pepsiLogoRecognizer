#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>
#include "simpleMethods.h"

class FilterMenager
{
#include "FilterMenager.h"

    static int calculateNewValue(int i, int j, cv::Mat_<cv::Vec3b> I, int filter[3][3], int color);
    static cv::Mat executeFilter(cv::Mat& I); 
};

