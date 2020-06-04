#define _USE_MATH_DEFINES
#define POSITIVE 255
#define NEGATIVE 0
#define CHECKED 127

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

struct MyPoint{
    unsigned short x;
    unsigned short y;
};

bool isInDivision(int a, int down, int up) {
    return a >= down && a <= up;
}

int inDivision(int a, int b, int c) {
    return std::max(std::min(a, c), b);
}

int inDivision(int a) {
    return std::max(std::min(a, 255), 0);
}

int calculateNewValue(int i, int j, cv::Mat_<cv::Vec3b> I, int filter[3][3], int color) {
    int sum = 0;
    for (int p = 0; p < 3; ++p)
        for (int q = 0; q < 3; ++q) 
            sum += filter[p][q] * I(i-1+p, j-1+q)[color];
    return sum;
}

cv::Mat executeFilter(cv::Mat& I, int filter[3][3]) {
    int sum = 0;
    for (int p = 0; p < 3; ++p)
        for (int q = 0; q < 3; ++q)
            sum += filter[p][q];
    if (sum == 0)
        sum = 1;
    CV_Assert(I.depth() != sizeof(uchar));
    cv::Mat  res(I.rows, I.cols, CV_8UC3);
    switch (I.channels()) {
    case 3:
        cv::Mat_<cv::Vec3b> _I = I;
        cv::Mat_<cv::Vec3b> _R = res;
        for (int i = 1; i < I.rows - 1; ++i)
            for (int j = 1; j < I.cols - 1; ++j) {
                _R(i, j)[0] = inDivision(calculateNewValue(i, j, _I, filter, 0) / sum);
                _R(i, j)[1] = inDivision(calculateNewValue(i, j, _I, filter, 1) / sum);
                _R(i, j)[2] = inDivision(calculateNewValue(i, j, _I, filter, 2) / sum);
            }
        res = _R;
        break;
    }
    return res;
}

int countPixels(cv::Mat& I, int value) {
    int sum = 0;
    CV_Assert(I.depth() != sizeof(uchar));
    switch (I.channels()) {
    case 3:
        cv::Mat_<cv::Vec3b> _I = I;
        for (int i = 0; i < I.rows ; ++i)
            for (int j = 0; j < I.cols ; ++j) {
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

double wspMalinowskiej(int s, int l) {
    return l / (2 * sqrt(s * M_PI)) - 1;
}

double maleM(cv::Mat& I, int p, int q) {
    double sum = 0;
    CV_Assert(I.depth() != sizeof(uchar));
    switch (I.channels()) {
    case 3:
        cv::Mat_<cv::Vec3b> _I = I;
        for (int i = 0; i < I.rows; ++i)
            for (int j = 0; j < I.cols; ++j) {
                double value = (255 - (_I(i, j)[0] + _I(i, j)[1] + _I(i, j)[2]) / 3)/255;
                sum += pow(i + 1, p) * pow(j + 1, q) * value;
            }
        break;
    }
    return sum;
}

double maleM(std::vector<Point> vec, int p, int q) {
    double sum = 0;
    for (auto point : vec) 
        sum += pow(point.x + 1, p) * pow(point.y + 1, q);
    return sum;
}

cv::Mat findByColor(cv::Mat& image, int red, int green, int blue) {
    cv::Mat res(image.rows, image.cols, CV_8UC3);
    switch (image.channels()) {
    case 3:
        cv::Mat_<cv::Vec3b> _I = image;
        cv::Mat_<cv::Vec3b> _R = res;
        for (int i = 0; i < image.rows; ++i)
            for (int j = 0; j < image.cols; ++j) {
                if (_I(i, j)[2] == red && 
                    _I(i, j)[1] == green && 
                    _I(i, j)[0] == blue ) {
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

cv::Mat findByColor(cv::Mat& image, int redFrom, int greenFrom, int blueFrom, int redTo, int greenTo, int blueTo) {
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

std::vector<cv::Mat> seperateArrows(cv::Mat& image) {
    std::vector<cv::Mat> res;
    for (int red = 0; red <= 180; red += 45) {
        int green = 180 - red;
        res.push_back(findByColor(image, red, green, 0));
    }
    return res;
}

MyPoint findMiddle(cv::Mat image) {
    int minX = image.cols;
    int minY = image.rows;
    int maxX = 0;
    int maxY = 0;
    switch (image.channels()) {
    case 3:
        cv::Mat_<cv::Vec3b> _I = image;
        for (int i = 0; i < image.rows; ++i)
            for (int j = 0; j < image.cols; ++j) {
                if (_I(i, j)[2] == 0 &&
                    _I(i, j)[1] == 0 &&
                    _I(i, j)[0] == 0) {
                    if (j < minX)
                        minX = j;
                    if (i < minY)
                        minY = i;
                    if (j > maxX)
                        maxX = j;
                    if (i > maxY)
                        maxY = i;
                }

            }
        break;
    }
    MyPoint p;
    p.x = (minX + maxX) / 2;
    p.y = (minY + maxY) / 2;
    return p;
}

double liczNachylenie(MyPoint p1, MyPoint p2) {
    double miara = atan((double)(p2.y - p1.y) / (p2.x - p1.x)) * 180 / M_PI;
    if (p2.x >= p1.x ) // I lub IV cwiartka
        return miara;
    if (p2.x < p1.x ) // II lub III cwiartka
        return 180 + miara;

}

double M1(std::vector<Point> image) {
    double m00 = maleM(image, 0, 0);
    double M20 = maleM(image, 2, 0) - pow(maleM(image, 1, 0), 2) / m00;
    double M02 = maleM(image, 0, 2) - pow(maleM(image, 0, 1), 2) / m00;
    double M1 = (M20 + M02) / pow(m00, 2);
    return M1;
}

double M7(std::vector<Point> image) {
    double m00 = maleM(image, 0, 0);
    double M20 = maleM(image, 2, 0) - pow(maleM(image, 1, 0), 2) / m00;
    double M02 = maleM(image, 0, 2) - pow(maleM(image, 0, 1), 2) / m00;
    double M11 = (double)maleM(image, 1, 1) - (double)(maleM(image, 1, 0) * maleM(image, 0, 1)) / m00;
    double M7 = (M20 * M02 - M11 * M11) / pow(m00, 4);
    return M7;
}

void countCoef(std::vector<std::string> names, int filter[3][3]) {
    for (auto name : names) {
        cv::Mat original = cv::imread(name);

        auto vec = seperateArrows(original);
        std::cout << name << std::endl;
        for (int i = 0; i < vec.size(); ++i) {
            auto image = vec[i];
            //cv::imshow("Oryginal" + i, image);

            cv::Mat image1 = executeFilter(image, filter);
            int s = countPixels(image, 0);
            int l = countPixels(image1, 255);

            double m00 = maleM(image, 0, 0);
            double iFalka = (double)maleM(image, 1, 0) / m00;
            double jFalka = (double)maleM(image, 0, 1) / m00;
            MyPoint p1 = findMiddle(image);
            MyPoint p2;
            p2.x = jFalka;
            p2.y = 512 - iFalka;
            p1.y = 512 - p1.y;
            std::cout << "Strzalka R=" << 45 * i;
            std::cout << " Nachylenie " << liczNachylenie(p1, p2);
            std::cout << " S=" << s;
            std::cout << ", L=" << l;
            std::cout << ", W3=" << wspMalinowskiej(s, l);


            double M20 = maleM(image, 2, 0) - pow(maleM(image, 1, 0), 2) / m00;
            double M02 = maleM(image, 0, 2) - pow(maleM(image, 0, 1), 2) / m00;
            double M1 = (M20 + M02) / pow(m00, 2);

            double M11 = (double)maleM(image, 1, 1) - (double)(maleM(image, 1, 0) * maleM(image, 0, 1)) / m00;
            double M7 = (M20 * M02 - M11 * M11) / pow(m00, 4);
            std::cout << ", M1=" << M1;
            std::cout << ", M7=" << M7 << std::endl;
        }
    }
}
void addToSegment(cv::Mat_<cv::Vec3b>& image, std::vector<Point>& segment, int i, int j){
image(i, j)[2] = CHECKED;
image(i, j)[1] = CHECKED;
image(i, j)[0] = CHECKED;
Point p;
p.x = j;
p.y = i;
segment.push_back(p);
}

void calcSegment(cv::Mat_<cv::Vec3b>& image, std::vector<Point>& segment, int i, int j, int rows, int cols) {
    addToSegment(image, segment, i, j);
    for (size_t a = 0; a < segment.size(); ++a) {
        Point q = segment[a];
        if (isInDivision(q.y - 1, 0, rows - 1) && image(q.y - 1, q.x)[1] == POSITIVE)
            addToSegment(image, segment, q.y - 1, q.x);
        if (isInDivision(q.y + 1, 0, rows - 1) && image(q.y + 1, q.x)[1] == POSITIVE)
            addToSegment(image, segment, q.y + 1, q.x);

        if (isInDivision(q.x - 1, 0, cols - 1) && image(q.y, q.x - 1)[1] == POSITIVE)
            addToSegment(image, segment, q.y, q.x  - 1);
        if (isInDivision(q.x + 1, 0, cols - 1) && image(q.y, q.x + 1)[1] == POSITIVE)
            addToSegment(image, segment, q.y, q.x + 1);
    }
    
}

std::vector<std::vector<Point>> segmentize(cv::Mat& image) {
    cv::Mat res(image.rows, image.cols, CV_8UC3);
    std::vector<std::vector<Point>> result;

    switch (image.channels()) {
    case 3:
        cv::Mat_<cv::Vec3b> _I = image;
        for (int i = 0; i < image.rows; ++i)
            for (int j = 0; j < image.cols; ++j) {
                if (_I(i, j)[2] == POSITIVE){
                    std::vector<Point> segment;
                    calcSegment(_I, segment,  i, j, image.rows, image.cols);
                    result.push_back(segment);
                    //return result;
                }
            }
        break;
    }
    return result;
}

void print(std::vector<std::vector<Point>> vec) {
    for (auto i : vec) {
        for (auto j : i)
            std::cout << "\t" << j.x << "  " << j.y << std::endl;
        std::cout << "koniec" << std::endl;
     }
}

std::vector<std::vector<Point>> countBoundingSqueres(std::vector<std::vector<Point>> vec) {
    std::vector<std::vector<Point>> result;
    for (auto area : vec) {
        int minX = USHRT_MAX;
        int minY = USHRT_MAX;
        int maxX = 0;
        int maxY = 0;
        for (auto point : area) {
            minX = point.x < minX ? point.x : minX;
            maxX = point.x > maxX ? point.x : maxX;
            minY = point.y < minY ? point.y : minY;
            maxY = point.y > maxY ? point.y : maxY;
        }
        Point min;
        min.x = minX;
        min.y = minY;
        Point max;
        max.x = maxX;
        max.y = maxY;
        std::vector<Point> vec;
        vec.push_back(min);
        vec.push_back(max);
        result.push_back(vec);
    }
    return result;
}

std::vector<std::vector<Point>> findLogos(std::vector<std::vector<Point>>& squeres, cv::Mat& red, cv::Mat& blue, cv::Mat& white) {
    std::vector<std::vector<Point>> res;
    int rows = red.rows;
    int cols = red.cols;
    for (auto squere : squeres) {
        Point min = squere[0];
        Point max = squere[1];
        max.x = inDivision(max.x + (max.x - min.x) / 2, 0, cols - 1);
        max.y = inDivision(max.y + (max.y - min.y) / 2, 0, rows - 1);
        int redPixels = countPixels(red, CHECKED, min.x, min.y, max.x, max.y);
        int bluePixels = countPixels(blue, POSITIVE, min.x, min.y, max.x, max.y);
        int whitePixels = countPixels(white, POSITIVE, min.x, min.y, max.x, max.y);
        //std::cout << redPixels << "  " << bluePixels << "  " << whitePixels << "  " << std::endl;
        if (redPixels > 10 && bluePixels > redPixels / 3 && whitePixels > redPixels / 3 && whitePixels < 2 * redPixels) {
            std::vector<Point> vec;
            vec.push_back(min);
            vec.push_back(max);
            res.push_back(vec);
        }
    }
    return res;
}

cv::Mat drawSqueres(std::vector<std::vector<Point>>& squeres, cv::Mat& image) {
    cv::Mat_<cv::Vec3b> _I = image;
    for (auto squere : squeres) {
        Point min = squere[0];
        Point max = squere[1];
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

std::vector<std::vector<Point>> selectize(std::vector<std::vector<Point>>& vec) {
    std::vector<std::vector<Point>> result;
    for (auto area : vec) {
        double m1 = M1(area);
        double m7 = M7(area);
        if (m1 > 0.15 && m1 < 0.3 &&
            m7 > 0.006 && m7 < 0.007)
            result.push_back(area);
    }
    return result;
}

int main(int, char *[]) {
    std::cout << "Start ..." << std::endl;

    int filter[3][3] = {
        {0,-1,0},
        {-1,4,-1},
        {0,-1,0},

    };
    for (int i = 1; i <= 10; ++i) {
        cv::Mat image = cv::imread("./dataSet/" + std::to_string(i) + ".jpg");
        cv::Mat dest;
        cv::Size size(1152, 576);
        resize(image, dest, size);
        
        //cv::imshow("Oryginal", image);
        auto red = findByColor(dest, 150, 0, 0, 255, 120, 120);
        auto blue = findByColor(dest, 0, 0, 80, 100, 100, 255);
        auto white = findByColor(dest, 150, 150, 150, 255, 255, 255);

        auto vec = segmentize(red);
        vec = selectize(vec);
        auto boundingSqueres = countBoundingSqueres(vec);
        auto logos = findLogos(boundingSqueres, red, blue, white);
        auto imageWithSqueres = drawSqueres(logos, dest);
        cv::imshow("result" + std::to_string(i), imageWithSqueres);
        cv::imwrite("./resultsSelectize/" + std::to_string(i) + ".jpg", imageWithSqueres);
       // print(vec);

        /*cv::imshow("red", red);
        cv::imshow("blue", blue);
        cv::imshow("White", white);*/
    }
    cv::waitKey(-1); 
    
    return 0;
}
