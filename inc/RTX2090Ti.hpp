#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <utility>
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
    // * For each Frames
    void RayTracing(cv::VideoWriter &OutVideo, std::pair<int, int> &Start, std::pair<int, int> &End,
                    std::pair<int, int> &OriginalLoc);
    void renderPixel(int c, int r, std::pair<int, int> &Start, std::pair<int, int> &End,
                     cv::Mat &normalizedPic, cv::Mat &RenderOn, std::pair<int, int> &OriginalLoc);
    void safeCopyTo(cv::Mat&src, cv::Mat &dest, cv::Rect& roi);
    void linkAudio();
    std::string statusMessage(int loopsDone, int allLoops, int framesDone, int allFrames);
};
