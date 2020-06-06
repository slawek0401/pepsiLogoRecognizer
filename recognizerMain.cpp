#define _USE_MATH_DEFINES
#define POSITIVE 255
#define NEGATIVE 0
#define CHECKED 127

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>

#include "ColorMenager.h"
#include "SegmentsMenager.h"
#include "ColorMenager.h"
#include "ShowMenager.h"

using namespace std;
using namespace cv;

void print(std::vector<std::vector<Point>> vec) {
    for (auto i : vec) {
        for (auto j : i)
            std::cout << "\t" << j.x << "  " << j.y << std::endl;
        std::cout << "koniec" << std::endl;
     }
}

cv::Mat myResize(cv::Mat &image, int value) {
    cv::Mat res(image.rows / value, image.cols / value, CV_8UC3);
    switch (image.channels()) {
    case 3:
        cv::Mat_<cv::Vec3b> _I = image;
        cv::Mat_<cv::Vec3b> _R = res;
        for (int i = 0; i < _R.rows - 1; ++i)
            for (int j = 0; j < _R.cols - 1; ++j) {
                int fromX = i * value;
                int fromY = j * value;
                int toX = fromX + value;
                int toY = fromY + value;
                _R(i, j)[0] = mean(_I, fromX, fromY, toX, toY, 0);
                _R(i, j)[1] = mean(_I, fromX, fromY, toX, toY, 1);
                _R(i, j)[2] = mean(_I, fromX, fromY, toX, toY, 2);
            }
        res = _R;
        break;
    }
    return res;
}

int main(int, char *[]) {
    std::cout << "Start ..." << std::endl;

    for (int i = 1; i <= 14; ++i) {
        std::cout << "start " << i << std::endl;
        cv::Mat image = cv::imread("./dataSet/" + std::to_string(i) + ".jpg");
        cv::Mat dest;// = myResize(image, 4);
        
        cv::Size size(1152, 576);
        if (image.cols > 1500)
            resize(image, dest, size);
        else
            dest = image;
        //executeFilter(dest);
        //cv::imshow("Oryginal", image);
        /*auto red = findByColor(dest, 150, 0, 0, 255, 120, 120);
        auto blue = findByColor(dest, 0, 0, 80, 100, 100, 255);
        auto white = findByColor(dest, 150, 150, 150, 255, 255, 255);*/

        auto imageHSV = ColorMenager::toHSV(dest);

        //auto red = ColorMenager::findRed(dest);
        //auto blue = ColorMenager::findBlue(dest);
        //auto white = ColorMenager::findWhite(dest);

        auto red = and(ColorMenager::findByColor(imageHSV, 0, 50, 160, 255, 255, 180), ColorMenager::findByColor(imageHSV, 0, 50, 0, 255, 255, 15));
        auto blue = ColorMenager::findByColor(imageHSV, 0, 50, 100, 255, 255, 140);
        auto white = ColorMenager::findByColor(imageHSV, 150, 0, 0, 255, 100, 180);

        auto vec = SegmentsMenager::segmentize(red);
        vec = SegmentsMenager::selectize(vec);
        auto boundingSqueres = SegmentsMenager::countBoundingSqueres(vec);
        auto logos = SegmentsMenager::findLogos(boundingSqueres, red, blue, white);
        auto imageWithSqueres = ShowMenager::drawSqueres(logos, dest);
        cv::imshow("result" + std::to_string(i), imageWithSqueres);
        cv::imwrite("./hsv/" + std::to_string(i) + ".jpg", imageWithSqueres);
       // print(vec);

        cv::imshow("red", red);
        cv::imshow("blue", blue);
        cv::imshow("White", white);
        std::cout << "end " << i << std::endl;
    }
    cv::waitKey(-1); 
    
    return 0;
}
