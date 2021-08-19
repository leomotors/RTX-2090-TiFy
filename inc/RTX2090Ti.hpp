#pragma once

#include <opencv2/opencv.hpp>

#include "Configurations.hpp"

class RTX2090Ti
{
  private:
    cv::Mat BaseImage;
    Configurations &ConfigRef;

  public:
    RTX2090Ti(cv::Mat BaseImage, Configurations &ConfigRef);
};
