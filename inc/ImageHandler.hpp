#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <utility>

class ImageHandler
{
  private:
    cv::Mat Image;
    std::pair<int, int> ImgRes;

  public:
    ImageHandler(std::string &filename);
    std::pair<int, int> getImageRes();
};
