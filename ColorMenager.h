#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>
#include "simpleMethods.h"
#define POSITIVE 255
#define NEGATIVE 0
#define CHECKED 127

class ColorMenager
{
public:
    static cv::Mat findByColor(cv::Mat& image, int red, int green, int blue);
    static cv::Mat findByColor(cv::Mat& image, int redFrom, int greenFrom, int blueFrom, int redTo, int greenTo, int blueTo);
    static cv::Mat findBlue(cv::Mat& image);
    static cv::Mat findRed(cv::Mat& image);
    static cv::Mat findWhite(cv::Mat& image);
    static cv::Mat toHSV(cv::Mat& image);
};

