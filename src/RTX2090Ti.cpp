#include "RTX2090Ti.hpp"

#include <opencv2/opencv.hpp>

#include "Configurations.hpp"

RTX2090Ti::RTX2090Ti(cv::Mat BaseImage, Configurations &ConfigRef)
    : BaseImage(BaseImage), ConfigRef(ConfigRef)
{
}
