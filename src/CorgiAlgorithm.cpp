#include "CorgiAlgorithm.hpp"

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

cv::Mat Corgi::changeTone_HSV(cv::Mat NormalizedImage, std::tuple<uchar, uchar, uchar> Colors)
{
    std::vector<cv::Mat> inputChannels;
    cv::split(NormalizedImage, inputChannels);

    inputChannels[0] = std::get<0>(Colors);
    inputChannels[1] = std::get<1>(Colors);
    inputChannels[2] += cv::Scalar(std::get<2>(Colors)) - cv::mean(inputChannels[2]);

    cv::merge(inputChannels, NormalizedImage);
    cv::cvtColor(NormalizedImage, NormalizedImage, cv::COLOR_HSV2BGR);

    return NormalizedImage;
}
