#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>

class ShowMenager
{
public:
    static cv::Mat drawSqueres(std::vector<std::vector<cv::Point>>& squeres, cv::Mat& image);
};

