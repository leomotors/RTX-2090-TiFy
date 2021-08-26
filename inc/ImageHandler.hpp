#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <utility>

class ImageHandler
{
  private:
    std::string ImagePath;
    cv::Mat Image;
    cv::Mat ImageGray;
    std::pair<int, int> ImgRes;

  public:
    bool status;

  public:
    ImageHandler();
    bool loadFile(std::string &filename);
    std::pair<int, int> getImageRes();
    std::string getImageResStr(bool withRatio = false);
    std::string getImagePath();
    cv::Mat getImage();
};
