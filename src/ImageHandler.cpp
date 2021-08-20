#include "ImageHandler.hpp"

#include <opencv2/opencv.hpp>
#include <string>
#include <utility>

ImageHandler::ImageHandler() {}

bool ImageHandler::loadFile(std::string &filename)
{
    ImagePath = filename;
    Image = cv::imread(filename);
    ImageGray = cv::imread(filename, cv::IMREAD_GRAYSCALE);
    ImgRes.first = Image.cols;
    ImgRes.second = Image.rows;
    return true;
}

std::pair<int, int> ImageHandler::getImageRes()
{
    return ImgRes;
}

std::string ImageHandler::getImageResStr()
{
    return std::to_string(ImgRes.first) + "x" + std::to_string(ImgRes.second);
}

std::string ImageHandler::getImagePath()
{
    return ImagePath;
}

cv::Mat ImageHandler::getImage()
{
    return Image;
}
