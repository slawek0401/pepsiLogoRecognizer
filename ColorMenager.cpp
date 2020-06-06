#include "ColorMenager.h"

cv::Mat ColorMenager::findByColor(cv::Mat& image, int red, int green, int blue) {
    cv::Mat res(image.rows, image.cols, CV_8UC3);
    switch (image.channels()) {
    case 3:
        cv::Mat_<cv::Vec3b> _I = image;
        cv::Mat_<cv::Vec3b> _R = res;
        for (int i = 0; i < image.rows; ++i)
            for (int j = 0; j < image.cols; ++j) {
                if (_I(i, j)[2] == red &&
                    _I(i, j)[1] == green &&
                    _I(i, j)[0] == blue) {
                    _R(i, j)[0] = POSITIVE;
                    _R(i, j)[1] = POSITIVE;
                    _R(i, j)[2] = POSITIVE;
                }
                else {
                    _R(i, j)[0] = NEGATIVE;
                    _R(i, j)[1] = NEGATIVE;
                    _R(i, j)[2] = NEGATIVE;
                }
            }
        res = _R;
        break;
    }
    return res;
}

cv::Mat ColorMenager::findByColor(cv::Mat& image, int redFrom, int greenFrom, int blueFrom, int redTo, int greenTo, int blueTo) {
    cv::Mat res(image.rows, image.cols, CV_8UC3);
    switch (image.channels()) {
    case 3:
        cv::Mat_<cv::Vec3b> _I = image;
        cv::Mat_<cv::Vec3b> _R = res;
        for (int i = 0; i < image.rows; ++i)
            for (int j = 0; j < image.cols; ++j) {
                if (_I(i, j)[2] >= redFrom && _I(i, j)[2] <= redTo &&
                    _I(i, j)[1] >= greenFrom && _I(i, j)[1] <= greenTo &&
                    _I(i, j)[0] >= blueFrom && _I(i, j)[0] <= blueTo) {
                    _R(i, j)[0] = POSITIVE;
                    _R(i, j)[1] = POSITIVE;
                    _R(i, j)[2] = POSITIVE;
                }
                else {
                    _R(i, j)[0] = NEGATIVE;
                    _R(i, j)[1] = NEGATIVE;
                    _R(i, j)[2] = NEGATIVE;
                }
            }
        res = _R;
        break;
    }
    return res;
}

cv::Mat ColorMenager::findBlue(cv::Mat& image) {
    cv::Mat res(image.rows, image.cols, CV_8UC3);
    switch (image.channels()) {
    case 3:
        cv::Mat_<cv::Vec3b> _I = image;
        cv::Mat_<cv::Vec3b> _R = res;
        for (int i = 0; i < image.rows; ++i)
            for (int j = 0; j < image.cols; ++j) {
                uchar red = _I(i, j)[2];
                uchar green = _I(i, j)[1];
                uchar blue = _I(i, j)[0];
                if (blue > green + 20 && blue > red + 20 && blue > 80) {
                    _R(i, j)[0] = POSITIVE;
                    _R(i, j)[1] = POSITIVE;
                    _R(i, j)[2] = POSITIVE;
                }
                else {
                    _R(i, j)[0] = NEGATIVE;
                    _R(i, j)[1] = NEGATIVE;
                    _R(i, j)[2] = NEGATIVE;
                }
            }
        res = _R;
        break;
    }
    return res;
}

cv::Mat ColorMenager::findRed(cv::Mat& image) {
    cv::Mat res(image.rows, image.cols, CV_8UC3);
    switch (image.channels()) {
    case 3:
        cv::Mat_<cv::Vec3b> _I = image;
        cv::Mat_<cv::Vec3b> _R = res;
        for (int i = 0; i < image.rows; ++i)
            for (int j = 0; j < image.cols; ++j) {
                uchar red = _I(i, j)[2];
                uchar green = _I(i, j)[1];
                uchar blue = _I(i, j)[0];
                if (red > green + 20 && red > blue + 20 && red > 80) {
                    _R(i, j)[0] = POSITIVE;
                    _R(i, j)[1] = POSITIVE;
                    _R(i, j)[2] = POSITIVE;
                }
                else {
                    _R(i, j)[0] = NEGATIVE;
                    _R(i, j)[1] = NEGATIVE;
                    _R(i, j)[2] = NEGATIVE;
                }
            }
        res = _R;
        break;
    }
    return res;
}

cv::Mat ColorMenager::findWhite(cv::Mat& image) {
    cv::Mat res(image.rows, image.cols, CV_8UC3);
    switch (image.channels()) {
    case 3:
        cv::Mat_<cv::Vec3b> _I = image;
        cv::Mat_<cv::Vec3b> _R = res;
        for (int i = 0; i < image.rows; ++i)
            for (int j = 0; j < image.cols; ++j) {
                short red = _I(i, j)[2];
                short green = _I(i, j)[1];
                short blue = _I(i, j)[0];
                if (abs(red - green) < 50 && abs(red - blue) < 50 && abs(blue - green) < 50 && (red + green + blue) > 300) {
                    _R(i, j)[0] = POSITIVE;
                    _R(i, j)[1] = POSITIVE;
                    _R(i, j)[2] = POSITIVE;
                }
                else {
                    _R(i, j)[0] = NEGATIVE;
                    _R(i, j)[1] = NEGATIVE;
                    _R(i, j)[2] = NEGATIVE;
                }
            }
        res = _R;
        break;
    }
    return res;
}

cv::Mat ColorMenager::toHSV(cv::Mat& image) {
    cv::Mat res(image.rows, image.cols, CV_8UC3);
    switch (image.channels()) {
    case 3:
        cv::Mat_<cv::Vec3b> _I = image;
        cv::Mat_<cv::Vec3b> _R = res;
        for (int i = 0; i < image.rows; ++i)
            for (int j = 0; j < image.cols; ++j) {
                double r = (double)_I(i, j)[2] / 255;
                double g = (double)_I(i, j)[1] / 255;
                double b = (double)_I(i, j)[0] / 255;
                double cmax = std::max(r, std::max(g, b)); 
                double cmin = std::min(r, std::min(g, b)); 
                double diff = cmax - cmin; 
                double h, s;

                if (cmax == cmin)
                    h = 0;
                else if (cmax == r)
                    h = (int)(30 * ((g - b) / diff) + 180) % 180;
                else if (cmax == g)
                    h = (int)(30 * ((b - r) / diff) + 60) % 180;
                else if (cmax == b)
                    h = (int)(30 * ((r - g) / diff) + 120) % 180;

                if (cmax == 0)
                    s = 0;
                else
                    s = (diff / cmax) * 255;

                double v = cmax * 255;
                _R(i, j)[0] = h;
                _R(i, j)[1] = s;
                _R(i, j)[2] = v;
            }
        res = _R;
        break;
    }
    return res;
}