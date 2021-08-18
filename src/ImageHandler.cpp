#include "ImageHandler.hpp"

#include <opencv2/opencv.hpp>
#include <string>
#include <utility>

ImageHandler::ImageHandler(std::string &filename)
{
    Image = cv::imread(filename);
    ImgRes.first = Image.cols;
    ImgRes.second = Image.rows;
}

std::pair<int, int> ImageHandler::getImageRes()
{
    return ImgRes;
}
