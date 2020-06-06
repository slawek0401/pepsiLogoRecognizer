#include "FilterMenager.h"

int FilterMenager::calculateNewValue(int i, int j, cv::Mat_<cv::Vec3b> I, int filter[3][3], int color) {
    int sum = 0;
    for (int p = 0; p < 3; ++p)
        for (int q = 0; q < 3; ++q)
            sum += filter[p][q] * I(i - 1 + p, j - 1 + q)[color];
    return sum;
}

cv::Mat FilterMenager::executeFilter(cv::Mat& I, int filter[3][3]) {
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