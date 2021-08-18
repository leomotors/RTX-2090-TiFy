#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <utility>

class ImageHandler
{
  private:
    std::string ImagePath;
    cv::Mat Image;
    std::pair<int, int> ImgRes;

  public:
    ImageHandler();
    bool loadFile(std::string &filename);
    std::pair<int, int> getImageRes();
    std::string getImageResStr();
    std::string getImagePath();
};
