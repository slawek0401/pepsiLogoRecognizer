#include "SegmentsMenager.h"


void SegmentsMenager::addToSegment(cv::Mat_<cv::Vec3b>& image, std::vector<cv::Point>& segment, int i, int j) {
    image(i, j)[2] = CHECKED;
    image(i, j)[1] = CHECKED;
    image(i, j)[0] = CHECKED;
    cv::Point p;
    p.x = j;
    p.y = i;
    segment.push_back(p);
}

void SegmentsMenager::calcSegment(cv::Mat_<cv::Vec3b>& image, std::vector<cv::Point>& segment, int i, int j, int rows, int cols) {
    addToSegment(image, segment, i, j);
    for (size_t a = 0; a < segment.size(); ++a) {
        cv::Point q = segment[a];
        if (isInDivision(q.y - 1, 0, rows - 1) && image(q.y - 1, q.x)[1] == POSITIVE)
            addToSegment(image, segment, q.y - 1, q.x);
        if (isInDivision(q.y + 1, 0, rows - 1) && image(q.y + 1, q.x)[1] == POSITIVE)
            addToSegment(image, segment, q.y + 1, q.x);

        if (isInDivision(q.x - 1, 0, cols - 1) && image(q.y, q.x - 1)[1] == POSITIVE)
            addToSegment(image, segment, q.y, q.x - 1);
        if (isInDivision(q.x + 1, 0, cols - 1) && image(q.y, q.x + 1)[1] == POSITIVE)
            addToSegment(image, segment, q.y, q.x + 1);
    }

}

std::vector<std::vector<cv::Point>> SegmentsMenager::segmentize(cv::Mat& image, int fromX, int fromY, int toX, int toY) {
    cv::Mat res(image.rows, image.cols, CV_8UC3);
    std::vector<std::vector<cv::Point>> result;

    toX = toX == 0 ? image.cols : toX;
    toY = toY == 0 ? image.rows : toY;
    switch (image.channels()) {
    case 3:
        cv::Mat_<cv::Vec3b> _I = image;
        for (int i = fromY; i < toY; ++i)
            for (int j = fromX; j < toX; ++j) {
                if (_I(i, j)[2] == POSITIVE) {
                    std::vector<cv::Point> segment;
                    calcSegment(_I, segment, i, j, image.rows, image.cols);
                    result.push_back(segment);
                }
            }
        for (int i = 0; i < image.rows; ++i)
            for (int j = 0; j < image.cols; ++j)
                if (_I(i, j)[2] == CHECKED) {
                    _I(i, j)[2] = POSITIVE;
                    _I(i, j)[1] = POSITIVE;
                    _I(i, j)[0] = POSITIVE;
                }

        break;
    }
    return result;
}

std::vector<std::vector<cv::Point>> SegmentsMenager::selectize(std::vector<std::vector<cv::Point>>& vec) {
    std::vector<std::vector<cv::Point>> result;
    for (auto area : vec) {
        double m1 = CoefficientsMenager::M1(area);
        double m7 = CoefficientsMenager::M7(area);
        if (m1 > 0.15 && m1 < 0.3 &&
            m7 > 0.006 && m7 < 0.0075)
            result.push_back(area);
    }
    return result;
}

std::vector<std::vector<cv::Point>> SegmentsMenager::selectizeBlue(std::vector<std::vector<cv::Point>>& vec) {
    std::vector<std::vector<cv::Point>> result;
    for (auto area : vec) {
        double m1 = CoefficientsMenager::M1(area);
        double m7 = CoefficientsMenager::M7(area);
        if (m1 > 0.25 && m1 < 0.4 &&
            m7 > 0.009 && m7 < 0.015)
            result.push_back(area);
    }
    return result;
}

std::vector<std::vector<cv::Point>> SegmentsMenager::selectizeWhite(std::vector<std::vector<cv::Point>>& vec) {
    std::vector<std::vector<cv::Point>> result;
    for (auto area : vec) {
        double m1 = CoefficientsMenager::M1(area);
        double m7 = CoefficientsMenager::M7(area);
        if (m1 > 0.3 && m1 < 0.45 &&
            m7 > 0.009 && m7 < 0.015)
            result.push_back(area);
    }
    return result;
}

std::vector<std::vector<cv::Point>> SegmentsMenager::countBoundingSqueres(std::vector<std::vector<cv::Point>> vec) {
    std::vector<std::vector<cv::Point>> result;
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
        cv::Point min;
        min.x = minX;
        min.y = minY;
        cv::Point max;
        max.x = maxX;
        max.y = maxY;
        std::vector<cv::Point> vec;
        vec.push_back(min);
        vec.push_back(max);
        result.push_back(vec);
    }
    return result;
}

std::vector<std::vector<cv::Point>> SegmentsMenager::findLogos(std::vector<std::vector<cv::Point>>& squeres, cv::Mat& red, cv::Mat& blue, cv::Mat& white) {
    std::vector<std::vector<cv::Point>> res;
    int rows = red.rows;
    int cols = red.cols;
    for (auto squere : squeres) {
        cv::Point min = squere[0];
        cv::Point max = squere[1];

        min.x = inDivision(min.x - (max.x - min.x) / 2, 0, cols - 1);
        min.y = inDivision(min.y - (max.y - min.y) / 2, 0, rows - 1);
        max.x = inDivision(max.x + (max.x - min.x) / 2, 0, cols - 1);
        max.y = inDivision(max.y + (max.y - min.y) / 2, 0, rows - 1);

        int redPixels = countPixels(red, POSITIVE, min.x, min.y, max.x, max.y);
        int bluePixels = countPixels(blue, POSITIVE, min.x, min.y, max.x, max.y);
        int whitePixels = countPixels(white, POSITIVE, min.x, min.y, max.x, max.y);
        //std::cout << redPixels << "  " << bluePixels << "  " << whitePixels << "  " << std::endl;
        if (redPixels > 10 && bluePixels > redPixels / 6 && whitePixels > redPixels / 9 /*&& whitePixels < 2 * redPixels*/) {
            std::vector<cv::Point> vec;
            vec.push_back(min);
            vec.push_back(max);
            res.push_back(vec);
        }
    }
    return res;
}

std::vector<std::vector<cv::Point>> SegmentsMenager::findLogos2(std::vector<std::vector<cv::Point>>& squeres, cv::Mat& red, cv::Mat& blue, cv::Mat& white) {
    std::vector<std::vector<cv::Point>> res;
    int rows = red.rows;
    int cols = red.cols;
    for (auto squere : squeres) {
        cv::Point min = squere[0];
        cv::Point max = squere[1];
        
        min.x = inDivision(min.x - (max.x - min.x) / 2, 0, cols - 1);
        min.y = inDivision(min.y - (max.y - min.y) / 2, 0, rows - 1);
        max.x = inDivision(max.x + (max.x - min.x) / 2, 0, cols - 1);
        max.y = inDivision(max.y + (max.y - min.y) / 2, 0, rows - 1);
               

        //auto redSegments = segmentize(red, min.x, min.y, max.x, max.y);
        auto blueSegments = segmentize(blue, min.x, min.y, max.x, max.y);
        //blueSegments = selectizeBlue(blueSegments);
        auto whiteSegments = segmentize(white, min.x, min.y, max.x, max.y);
        whiteSegments = selectizeWhite(whiteSegments);

        int redPixels = countPixels(red, POSITIVE, min.x, min.y, max.x, max.y);
        for (auto whiteSegment : whiteSegments) {
            int whitePixels = whiteSegment.size();
            for (auto blueSegment : blueSegments) {
                int bluePixels = blueSegment.size();
                //std::cout << redPixels << "  " << bluePixels << "  " << whitePixels << "  " << std::endl;
                if (redPixels > 10 && bluePixels > redPixels / 6 && whitePixels > redPixels / 9) {
                    std::vector<cv::Point> vec;
                    vec.push_back(min);
                    vec.push_back(max);
                    res.push_back(vec);
                }
            }
        }
        
    }
    return res;
}