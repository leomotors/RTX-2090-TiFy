#include "RTX2090Ti.hpp"

#include <ctime>
#include <opencv2/opencv.hpp>
#include <wx/progdlg.h>
#include <wx/wx.h>

#include "Configurations.hpp"

RTX2090Ti::RTX2090Ti(wxWindow *parent, cv::Mat BaseImage, Configurations Config)
    : parent(parent), Config(Config), fourcc(cv::VideoWriter::fourcc(MYCODEC))
{
    const auto &[x, y] = Config.Resolution;

    cv::resize(BaseImage, this->BaseImage, cv::Size(x, y));
    OutVideo = cv::VideoWriter(Config.OutVideoPath + ".avi", fourcc, Config.FPS, cv::Size(x, y));
}

bool RTX2090Ti::buildVideo()
{
    auto start{std::clock()};

    const auto &[cols, rows] = Config.Resolution;
    const int bigcols = cols * cols;
    const int bigrows = rows * rows;

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

    for (std::pair<int, int> &point : Config.WarpPosition)
    {
        int framesDone = loopDone * totalFrames;

        std::pair<int, int> Start{point.first * cols, point.second * rows};
        std::pair<int, int> End{point.first * (cols + 1), point.second * (rows + 1)};

        int left = cols * point.first;
        int up = rows * point.second;
        int right = cols * (cols - point.first - 1);
        int down = rows * (rows - point.second - 1);

        for (int f = 0; f < totalFrames; f++)
        {
            double expansionRate = f / totalFrames;
            std::pair<int, int> LocalStart{Start.first - left * expansionRate,
                                           Start.second - up * expansionRate};
            std::pair<int, int> LocalEnd{End.first + right * expansionRate,
                                         End.second + down * expansionRate};
            OutVideo.write(BaseImage);
            BuildProgress.Update(framesDone + f,
                                 statusMessage(loopDone, Config.nLoops, f, totalFrames));
        }

        loopDone++;
    }

    OutVideo.release();
    std::cout << "Build Success (ทิพย์)\n";

    auto end{std::clock()};

    std::cout << "da Built took " << (double)(end - start) / CLOCKS_PER_SEC << " secs.\n";

    return true;
}

std::string RTX2090Ti::statusMessage(int loopsDone, int allLoops, int framesDone, int allFrames)
{
    std::string result;
    result += "Loops Done: " + std::to_string(loopsDone) + " of " + std::to_string(allLoops);
    result += "\n";
    result += "Frames Done: " + std::to_string(framesDone) + " of " + std::to_string(allFrames);
    return result;
}
