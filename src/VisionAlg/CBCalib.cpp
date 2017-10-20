#include "VisionAlg/CBCalib.h"
#include <vector>

bool findCBC(const cv::Mat& image, cv::Size pattern_size_wh, std::vector<cv::Point2f>& corners) {

    int flags_cbc = CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS;

    bool found = cv::findChessboardCorners(
        image,
        pattern_size_wh,
        corners,
        flags_cbc
    );

    cv::Size searchwin_size(5, 5);
    cv::Size zero_zone_size(-1, -1);
    cv::TermCriteria term_criteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1);
    if (found) {

        cornerSubPix(image, corners, searchwin_size, zero_zone_size, term_criteria);

    }

    return found;

}

