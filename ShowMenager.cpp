#include "ShowMenager.h"

cv::Mat ShowMenager::drawSqueres(std::vector<std::vector<cv::Point>>& squeres, cv::Mat& image) {
    cv::Mat_<cv::Vec3b> _I = image;
    for (auto squere : squeres) {
        cv::Point min = squere[0];
        cv::Point max = squere[1];
        for (int i = min.x; i <= max.x; ++i) {
            _I(min.y, i)[2] = 0;
            _I(min.y, i)[1] = 255;
            _I(min.y, i)[0] = 0;
            _I(max.y, i)[2] = 0;
            _I(max.y, i)[1] = 255;
            _I(max.y, i)[0] = 0;
        }
        for (int i = min.y; i <= max.y; ++i) {
            _I(i, min.x)[2] = 0;
            _I(i, min.x)[1] = 255;
            _I(i, min.x)[0] = 0;
            _I(i, max.x)[2] = 0;
            _I(i, max.x)[1] = 255;
            _I(i, max.x)[0] = 0;
        }
    }
    image = _I;
    return image;
}