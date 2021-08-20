#include "ImageUtils.hpp"

#include <iostream>
#include <opencv2/opencv.hpp>
#include <tuple>
#include <vector>

cv::Mat Corgi::changeTone(cv::Mat &NormalizedImage, std::tuple<int, int, int> Colors)
{
    cv::Mat FinalImage;

    auto BlueChannel = NormalizedImage * ((double)std::get<0>(Colors) / 255);
    auto GreenChannel = NormalizedImage * ((double)std::get<1>(Colors) / 255);
    auto RedChannel = NormalizedImage * ((double)std::get<2>(Colors) / 255);

    std::vector<cv::Mat> channels;
    channels.push_back(BlueChannel);
    channels.push_back(GreenChannel);
    channels.push_back(RedChannel);

    cv::merge(channels, FinalImage);

    return FinalImage;
}
