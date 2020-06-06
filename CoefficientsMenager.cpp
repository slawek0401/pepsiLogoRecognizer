#include "CoefficientsMenager.h"

double CoefficientsMenager::wspMalinowskiej(int s, int l) {
    return l / (2 * sqrt(s * M_PI)) - 1;
}

double CoefficientsMenager::maleM(cv::Mat& I, int p, int q) {
    double sum = 0;
    CV_Assert(I.depth() != sizeof(uchar));
    switch (I.channels()) {
    case 3:
        cv::Mat_<cv::Vec3b> _I = I;
        for (int i = 0; i < I.rows; ++i)
            for (int j = 0; j < I.cols; ++j) {
                double value = (255 - (_I(i, j)[0] + _I(i, j)[1] + _I(i, j)[2]) / 3) / 255;
                sum += pow(i + 1, p) * pow(j + 1, q) * value;
            }
        break;
    }
    return sum;
}

double CoefficientsMenager::maleM(std::vector<cv::Point> vec, int p, int q) {
    double sum = 0;
    for (auto point : vec)
        sum += pow(point.x + 1, p) * pow(point.y + 1, q);
    return sum;
}

cv::Point CoefficientsMenager::findMiddle(cv::Mat image) {
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
    cv::Point p;
    p.x = (minX + maxX) / 2;
    p.y = (minY + maxY) / 2;
    return p;
}

double CoefficientsMenager::M1(std::vector<cv::Point> image) {
    double m00 = maleM(image, 0, 0);
    double M20 = maleM(image, 2, 0) - pow(maleM(image, 1, 0), 2) / m00;
    double M02 = maleM(image, 0, 2) - pow(maleM(image, 0, 1), 2) / m00;
    double M1 = (M20 + M02) / pow(m00, 2);
    return M1;
}

double CoefficientsMenager::M7(std::vector<cv::Point> image) {
    double m00 = maleM(image, 0, 0);
    double M20 = maleM(image, 2, 0) - pow(maleM(image, 1, 0), 2) / m00;
    double M02 = maleM(image, 0, 2) - pow(maleM(image, 0, 1), 2) / m00;
    double M11 = (double)maleM(image, 1, 1) - (double)(maleM(image, 1, 0) * maleM(image, 0, 1)) / m00;
    double M7 = (M20 * M02 - M11 * M11) / pow(m00, 4);
    return M7;
}

//void CoefficientsMenager::countCoef(std::vector<std::string> names, int filter[3][3]) {
//    for (auto name : names) {
//        cv::Mat original = cv::imread(name);
//
//        auto vec = seperateArrows(original);
//        std::cout << name << std::endl;
//        for (int i = 0; i < vec.size(); ++i) {
//            auto image = vec[i];
//            //cv::imshow("Oryginal" + i, image);
//
//            cv::Mat image1 = executeFilter(image, filter);
//            int s = countPixels(image, 0);
//            int l = countPixels(image1, 255);
//
//            double m00 = maleM(image, 0, 0);
//            double iFalka = (double)maleM(image, 1, 0) / m00;
//            double jFalka = (double)maleM(image, 0, 1) / m00;
//            cv::Point p1 = findMiddle(image);
//            cv::Point p2;
//            p2.x = jFalka;
//            p2.y = 512 - iFalka;
//            p1.y = 512 - p1.y;
//            std::cout << "Strzalka R=" << 45 * i;
//            std::cout << " Nachylenie " << liczNachylenie(p1, p2);
//            std::cout << " S=" << s;
//            std::cout << ", L=" << l;
//            std::cout << ", W3=" << wspMalinowskiej(s, l);
//
//
//            double M20 = maleM(image, 2, 0) - pow(maleM(image, 1, 0), 2) / m00;
//            double M02 = maleM(image, 0, 2) - pow(maleM(image, 0, 1), 2) / m00;
//            double M1 = (M20 + M02) / pow(m00, 2);
//
//            double M11 = (double)maleM(image, 1, 1) - (double)(maleM(image, 1, 0) * maleM(image, 0, 1)) / m00;
//            double M7 = (M20 * M02 - M11 * M11) / pow(m00, 4);
//            std::cout << ", M1=" << M1;
//            std::cout << ", M7=" << M7 << std::endl;
//        }
//    }
//}