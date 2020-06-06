#include "simpleMethods.h"

bool isInDivision(int a, int down, int up) {
    return a >= down && a <= up;
}

int inDivision(int a, int b, int c) {
    return std::max(std::min(a, c), b);
}

int inDivision(int a) {
    return std::max(std::min(a, 255), 0);
}

int countPixels(cv::Mat& I, int value) {
    int sum = 0;
    CV_Assert(I.depth() != sizeof(uchar));
    switch (I.channels()) {
    case 3:
        cv::Mat_<cv::Vec3b> _I = I;
        for (int i = 0; i < I.rows; ++i)
            for (int j = 0; j < I.cols; ++j) {
                if ((_I(i, j)[0] + _I(i, j)[1] + _I(i, j)[2]) / 3 == value)
                    ++sum;
            }
        break;
    }
    return sum;
}

int countPixels(cv::Mat& I, int value, int fromX, int fromY, int toX, int toY) {
    int sum = 0;
    CV_Assert(I.depth() != sizeof(uchar));
    switch (I.channels()) {
    case 3:
        cv::Mat_<cv::Vec3b> _I = I;
        for (int i = fromY; i < toY; ++i)
            for (int j = fromX; j < toX; ++j) {
                if ((_I(i, j)[0] + _I(i, j)[1] + _I(i, j)[2]) / 3 == value)
                    ++sum;
            }
        break;
    }
    return sum;
}

void printHistogram(const cv::Mat& I) {
    CV_Assert(I.depth() != sizeof(uchar));
    cv::Mat  res(I.rows, I.cols, CV_8UC3);
    int his[] = { 0,0,0,0,0,0,0,0 };
    switch (I.channels()) {
    case 3:
        cv::Mat_<cv::Vec3b> _I = I;
        for (int i = 0; i < I.rows; ++i)
            for (int j = 0; j < I.cols; ++j) {
                int brigthness = (_I(i, j)[0] + _I(i, j)[1] + _I(i, j)[2]) / 3;
                his[brigthness / 32] += 1;
            }
        break;
    }

    for (int i = 0; i < 8; ++i) {
        std::cout << "Pikseli o jasnosci " << 32 * i << "-" << 32 * i + 31 << " jest: " << his[i] << std::endl;
    }
    int sum = 0;
    for (int i = 0; i < 8; ++i) {
        sum += his[i];
    }
    std::cout << "Suma pikseli: " << sum << std::endl;
}

int mean(cv::Mat_<cv::Vec3b>& image, int fromX, int fromY, int toX, int toY, int channel) {
    unsigned sum = 0;
    unsigned n = 0;
    for (int i = fromX; i < toX; ++i)
        for (int j = fromY; j < toY; ++j) {
            sum += image(i, j)[channel];
            ++n;
        }
    return sum / n;
}

cv::Mat and (cv::Mat& im1, cv::Mat& im2) {
    if (im1.rows != im2.rows || im1.cols != im2.cols)
        throw std::exception("obrazy w iloczynie boolowskim musza miec taki sam rozmiar");
    cv::Mat res(im1.rows, im1.cols, CV_8UC3);
        
    cv::Mat_<cv::Vec3b> i1 = im1;
    cv::Mat_<cv::Vec3b> i2 = im2;
    cv::Mat_<cv::Vec3b> r = res;

    for (int i = 0; i < i1.rows; ++i)
        for (int j = 0; j < i1.cols; ++j) {
            if (i1(i, j)[0] == 255 || i2(i, j)[0] == 255) {
                r(i, j)[0] = 255;
                r(i, j)[1] = 255;
                r(i, j)[2] = 255;
            }
            else {
                r(i, j)[0] = 0;
                r(i, j)[1] = 0;
                r(i, j)[2] = 0;
            }
        }
    res = r;
    return res;
    
}