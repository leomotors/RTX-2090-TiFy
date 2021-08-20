#pragma once

#include <opencv2/opencv.hpp>
#include <tuple>

namespace Corgi
{
cv::Mat changeTone(cv::Mat &NormalizedImage, std::tuple<int, int, int> Colors);
}
