#include "ImageHandler.hpp"

#include <opencv2/opencv.hpp>
#include <string>
#include <utility>

ImageHandler::ImageHandler() : status(false) {}

bool ImageHandler::loadFile(std::string &filename) {
    ImagePath = filename;
    Image = cv::imread(filename);
    ImageGray = cv::imread(filename, cv::IMREAD_GRAYSCALE);
    ImgRes.first = Image.cols;
    ImgRes.second = Image.rows;
    status = ImgRes.first > 0 && ImgRes.second > 0;
    return status;
}

std::pair<int, int> ImageHandler::getImageRes() {
    return ImgRes;
}

std::string ImageHandler::getImageResStr(bool withRatio) {
    std::string toReturn =
        std::to_string(ImgRes.first) + "x" + std::to_string(ImgRes.second);
    if (withRatio && ImgRes.first && ImgRes.second)
        toReturn +=
            " (" + std::to_string((double)ImgRes.first / ImgRes.second) + ":1)";

    return toReturn;
}

std::string ImageHandler::getImagePath() {
    return ImagePath;
}

cv::Mat ImageHandler::getImage() {
    return Image;
}
