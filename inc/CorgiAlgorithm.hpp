#pragma once

#include <opencv2/opencv.hpp>
#include <tuple>

namespace Corgi
{
cv::Mat changeTone(cv::Mat &NormalizedImage, std::tuple<int, int, int> Colors,
                   double ImgBrightness = -1);
cv::Mat changeTone_HSV(cv::Mat NormalizedImage, std::tuple<int, int, int> Colors);
void fConvert(std::tuple<int, int, int> &Colors, double p);
} // namespace Corgi
