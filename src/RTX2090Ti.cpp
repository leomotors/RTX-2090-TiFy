#include "RTX2090Ti.hpp"

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <tuple>
#include <utility>
#include <wx/progdlg.h>
#include <wx/wx.h>

#include "AppConstants.h"
#include "Configurations.hpp"
#include "CorgiAlgorithm.hpp"
#include "Video.hpp"

RTX2090Ti::RTX2090Ti(wxWindow *parent, cv::Mat BaseImage, Configurations &Config)
    : parent(parent), Config(Config), fourcc(cv::VideoWriter::fourcc(MYCODEC))
{
    const auto &[x, y] = Config.Resolution;
    cols = x;
    rows = y;

    cv::resize(BaseImage, this->BaseImage, cv::Size(x, y));
    cv::cvtColor(this->BaseImage, BaseImageGray, cv::COLOR_BGR2GRAY);

    OutVideo =
        cv::VideoWriter(Config.OutVideoPath + ".avi", fourcc, Config.FPS, cv::Size(cols, rows));
    OutVideo.write(this->BaseImage);
}

bool RTX2090Ti::buildVideo()
{
    auto start{std::chrono::steady_clock::now()};

    int totalFrames = Config.FPS * Config.LoopDuration;

    wxProgressDialog BuildProgress("Building Video...",
                                   statusMessage(0, Config.nLoops, 0, totalFrames),
                                   totalFrames * Config.nLoops, parent);

    std::cout << "Building Video with these properties\n"
              << "Resolution: " << cols << "x" << rows << "\n"
              << "FPS: " << Config.FPS << "\n"
              << "Duration Per Loop: " << Config.LoopDuration << "\n"
              << "Number of Loops: " << Config.nLoops << "\n"
              << "Saving as: " << Config.OutVideoPath << "\n"
              << "Total of: " << totalFrames * Config.nLoops << " frames\n";

    int loopDone{0};
    cv::Mat temp(cv::Mat::zeros(cols, rows, CV_8UC3));

    for (std::pair<int, int> &point : Config.WarpPosition)
    {
        int framesDone = loopDone * totalFrames;

        std::pair<int, int> Start{point.first * cols, point.second * rows};
        std::pair<int, int> End{(point.first + 1) * cols, (point.second + 1) * rows + 1};

        int left = cols * point.first;
        int up = rows * point.second;
        int right = cols * (cols - point.first - 1);
        int down = rows * (rows - point.second - 1);

        for (int f = 1; f <= totalFrames; f++)
        {
            double expansionRate = (double)f / totalFrames;
            expansionRate = std::pow(expansionRate, 2.75);

            std::pair<int, int> LocalStart{Start.first - left * expansionRate,
                                           Start.second - up * expansionRate};
            std::pair<int, int> LocalEnd{End.first + right * expansionRate,
                                         End.second + down * expansionRate};

            RayTracing(OutVideo, LocalStart, LocalEnd, point);

            BuildProgress.Update(framesDone + f,
                                 statusMessage(loopDone, Config.nLoops, f, totalFrames));
        }

        loopDone++;
    }

    cv::destroyAllWindows();
    OutVideo.release();
    std::cout << "Video Build Success\n";

    Video::linkAudio(Config.OutVideoPath);

    auto end{std::chrono::steady_clock::now()};

    std::chrono::duration<double> time_took = end - start;
    std::cout << "Building Video took " << time_took.count() << " secs.\n";

    wxMessageDialog doneMessage(parent,
                                "Build Success! Took " + std::to_string(time_took.count()) +
                                    " seconds.\nYour Video is Ready, would you like to open the "
                                    "app directory in file explorer?",
                                "Build Success", wxOK | wxCANCEL);

    if (doneMessage.ShowModal() == wxID_OK)
    {
        std::string path_to_open = ".";

        std::string toExec = DEFAULT_FILE_EXPLORER;
        toExec += " " + path_to_open;

        std::cout << "Executing: " << toExec << "\n";
        std::system(toExec.c_str());
    }

    return true;
}

void RTX2090Ti::RayTracing(cv::VideoWriter &OutVideo, std::pair<int, int> &Start,
                           std::pair<int, int> &End, std::pair<int, int> &OriginalLoc)
{
    if ((End.first - Start.first) / cols > 0.2 * cols)
    {
        cv::Mat cropped_img = BaseImage(cv::Range(Start.second / rows, End.second / rows),
                                        cv::Range(Start.first / cols, End.first / cols));

        cv::resize(cropped_img, cropped_img, cv::Size(cols, rows));
        OutVideo.write(cropped_img);

        cv::imshow("Live Preview", cropped_img);
        return;
    }

    cv::Mat Canvas = cv::Mat::zeros(cv::Size(cols, rows), CV_8UC3);

    std::pair<int, int> Imsize{End.first - Start.first, End.second - Start.second};

    std::pair<int, int> BigTileStart{Start.first / cols, Start.second / rows};
    std::pair<int, int> BigTileEnd{std::ceil((double)End.first / cols),
                                   std::ceil((double)End.second / rows)};

    std::pair<int, int> PixelSize{cols * cols / Imsize.first + 1, rows * rows / Imsize.second + 1};

    cv::Mat SmolImage;
    cv::resize(BaseImageGray, SmolImage, cv::Size(PixelSize.first, PixelSize.second));

    for (int c = BigTileStart.first; c < BigTileEnd.first; c++)
    {
        for (int r = BigTileStart.second; r < BigTileEnd.second; r++)
        {
            renderPixel(c, r, Start, End, SmolImage, Canvas, OriginalLoc);
        }
    }

    OutVideo.write(Canvas);
    cv::imshow("Live Preview", Canvas);
}

void RTX2090Ti::renderPixel(int c, int r, std::pair<int, int> &Start, std::pair<int, int> &End,
                            cv::Mat &normalizedPic, cv::Mat &RenderOn,
                            std::pair<int, int> &OriginalLoc)
{
    cv::Vec3b color = BaseImage.at<cv::Vec3b>(cv::Point(c, r));
    cv::Mat ColoredImg =
        Corgi::changeTone(normalizedPic, std::tuple<int, int, int>(color[0], color[1], color[2]));

    cv::Point renderOnPos((c * cols - Start.first) * cols / (End.first - Start.first),
                          (r * rows - Start.second) * rows / (End.second - Start.second));

    cv::Point renderEnd(renderOnPos.x + normalizedPic.cols, renderOnPos.y + normalizedPic.rows);

    cv::Rect renderRange(renderOnPos, renderEnd);

    // * Case: Original Picture
    if (c == OriginalLoc.first && r == OriginalLoc.second)
    {
        cv::Mat toRender;
        cv::resize(BaseImage, toRender, cv::Size(normalizedPic.cols, normalizedPic.rows));

        safeCopyTo(toRender, RenderOn, renderRange);

        return;
    }

    // * General Case
    safeCopyTo(ColoredImg, RenderOn, renderRange);
}

void RTX2090Ti::safeCopyTo(cv::Mat &src, cv::Mat &dest, cv::Rect &roi)
{
    std::pair<int, int> newXRange{0, roi.width};
    std::pair<int, int> newYRange{0, roi.height};

    bool left{false};
    bool up{false};
    bool right{false};
    bool down{false};

    if (roi.x < 0)
    {
        newXRange.first = -roi.x;
        left = true;
    }
    if (roi.y < 0)
    {
        newYRange.first = -roi.y;
        up = true;
    }
    if (roi.x + roi.width > cols)
    {
        newXRange.second = cols - roi.x;
        right = true;
    }
    if (roi.y + roi.height > rows)
    {
        newYRange.second = rows - roi.y;
        down = true;
    }

    if (left || up || right || down)
    {
        src = src(cv::Range(newYRange.first, newYRange.second),
                  cv::Range(newXRange.first, newXRange.second));

        if (left)
        {
            roi.width += roi.x;
            roi.x = 0;
        }
        if (right)
        {
            roi.width = src.cols;
        }
        if (up)
        {
            roi.height += roi.y;
            roi.y = 0;
        }
        if (down)
        {
            roi.height = src.rows;
        }
    }

    src.copyTo(dest(roi));
}

std::string RTX2090Ti::statusMessage(int loopsDone, int allLoops, int framesDone, int allFrames)
{
    std::string result;
    result += "Loops Done: " + std::to_string(loopsDone) + " of " + std::to_string(allLoops);
    result += "\n";
    result += "Frames Done: " + std::to_string(framesDone) + " of " + std::to_string(allFrames);
    return result;
}
