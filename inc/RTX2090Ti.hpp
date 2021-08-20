#pragma once

#include <opencv2/opencv.hpp>
#include <wx/wx.h>

#include "Configurations.hpp"

#define MYCODEC 'X', 'V', 'I', 'D'

class RTX2090Ti
{
  private:
    cv::Mat BaseImage;
    cv::Mat BaseImageGray;
    Configurations &Config;
    int fourcc;
    cv::VideoWriter OutVideo;
    int cols, rows;

    wxWindow *parent;

  public:
    RTX2090Ti(wxWindow *parent, cv::Mat BaseImage, Configurations &Config);
    // * Build Video, Configuration must be confirmed to be Ready for RTX
    bool buildVideo();

  private:
    void RayTracing(cv::Mat &canvas, std::pair<int, int> &Start, std::pair<int, int> &End);
    void linkAudio();
    std::string statusMessage(int loopsDone, int allLoops, int framesDone, int allFrames);
};
