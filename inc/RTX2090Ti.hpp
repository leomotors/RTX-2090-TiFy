#pragma once

#include <opencv2/opencv.hpp>
#include <wx/wx.h>

#include "Configurations.hpp"

#define MYCODEC 'X', 'V', 'I', 'D'

class RTX2090Ti
{
  private:
    cv::Mat BaseImage;
    Configurations &Config;
    int fourcc;
    cv::VideoWriter OutVideo;

    wxWindow *parent;

  public:
    RTX2090Ti(wxWindow *parent, cv::Mat BaseImage, Configurations Config);
    // * Build Video, Configuration must be confirmed to be Ready for RTX
    bool buildVideo();

  private:
    void linkAudio();
    std::string statusMessage(int loopsDone, int allLoops, int framesDone, int allFrames);
};
