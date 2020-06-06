#pragma once
#define POSITIVE 255
#define NEGATIVE 0
#define CHECKED 127

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>
#include "simpleMethods.h"
#include "CoefficientsMenager.h"

class SegmentsMenager
{
public:
    static void addToSegment(cv::Mat_<cv::Vec3b>& image, std::vector<cv::Point>& segment, int i, int j);
    static void calcSegment(cv::Mat_<cv::Vec3b>& image, std::vector<cv::Point>& segment, int i, int j, int rows, int cols);
    static std::vector<std::vector<cv::Point>> segmentize(cv::Mat& image, int fromX = 0, int fromY = 0, int toX = 0, int toY = 0);
    static std::vector<std::vector<cv::Point>> selectize(std::vector<std::vector<cv::Point>>& vec);
    static std::vector<std::vector<cv::Point>> selectizeBlue(std::vector<std::vector<cv::Point>>& vec);
    static std::vector<std::vector<cv::Point>> selectizeWhite(std::vector<std::vector<cv::Point>>& vec);
    static std::vector<std::vector<cv::Point>> findLogos(std::vector<std::vector<cv::Point>>& squeres, cv::Mat& red, cv::Mat& blue, cv::Mat& white);
    static std::vector<std::vector<cv::Point>> findLogos2(std::vector<std::vector<cv::Point>>& squeres, cv::Mat& red, cv::Mat& blue, cv::Mat& white) ;
    static std::vector<std::vector<cv::Point>> countBoundingSqueres(std::vector<std::vector<cv::Point>> vec);
};

