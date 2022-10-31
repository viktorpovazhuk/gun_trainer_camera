//
// Created by vivi on 27.10.22.
//

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <vector>

#include "functions.h"

using namespace cv;
using namespace std;

Mat get_thr_val_mat(const Mat &mat, double thr_coef) {
    std::vector<cv::Mat> planes(3);
    cv::split(mat, planes);

    Mat val_mat = planes[2];

    double max_val;
    cv::minMaxLoc(val_mat, nullptr, &max_val);

    double thr_val = max_val * thr_coef;

    Mat thr_mat;
    threshold(val_mat, thr_mat, thr_val, 255, THRESH_BINARY);

    return thr_mat;
}

Ptr<SimpleBlobDetector> create_blob_detector() {
    SimpleBlobDetector::Params params;

    // Change thresholds
    params.minRepeatability = 1;
    params.thresholdStep = 10;
    params.minThreshold = 254;
    params.maxThreshold = 255;

    // Filter by Area
    params.filterByArea = false;
    params.minArea = 1;
    params.maxArea = 36;

    // Filter by Circularity
    params.filterByCircularity = false;
    params.minCircularity = 0.5;

    // Filter by Convexity
    params.filterByConvexity = false;
    params.minConvexity = 0.5;

    // Other filters
    params.filterByColor = false;
    params.filterByInertia = false;

    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

    return detector;
}

bool check_laser_area(const Mat &area) {
    Mat thr_mat = get_thr_val_mat(area, 0.9);
    int num_pixels_greater_thr = countNonZero(thr_mat);

    std::cout << "number of pixels greater than 0.9 thr in cropped area, total number: " <<  num_pixels_greater_thr << ", " << area.total() << '\n';
    std::cout << "proportion of greater than 0.9 thr in cropped area: " <<  num_pixels_greater_thr / (float)area.total() << '\n';

    return num_pixels_greater_thr / (float)area.total() < 0.6;
}