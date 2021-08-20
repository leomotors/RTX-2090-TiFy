#include "RTX2090Ti.hpp"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <opencv2/opencv.hpp>
#include <wx/progdlg.h>
#include <wx/wx.h>

#include "Configurations.hpp"
#include "ImageUtils.hpp"

RTX2090Ti::RTX2090Ti(wxWindow *parent, cv::Mat BaseImage, Configurations &Config)
    : parent(parent), Config(Config), fourcc(cv::VideoWriter::fourcc(MYCODEC))
{
    const auto &[x, y] = Config.Resolution;
    cols = x;
    rows = y;

    cv::resize(BaseImage, this->BaseImage, cv::Size(x, y));
    cv::cvtColor(BaseImage, BaseImageGray, cv::COLOR_BGR2GRAY);

    OutVideo =
        cv::VideoWriter(Config.OutVideoPath + ".avi", fourcc, Config.FPS, cv::Size(cols, rows));
}

bool RTX2090Ti::buildVideo()
{
    auto start{std::clock()};

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
    cv::Mat temp(cv::Mat::zeros(cols, rows, CV_8UC3));

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

            RayTracing(temp, LocalStart, LocalEnd);

            OutVideo.write(temp);
            cv::imshow("Live Preview", temp);
            BuildProgress.Update(framesDone + f,
                                 statusMessage(loopDone, Config.nLoops, f, totalFrames));
        }

        loopDone++;
    }

    cv::destroyAllWindows();
    OutVideo.release();
    std::cout << "Video Build Success (ทิพย์)\n";

    BuildProgress.Update(totalFrames * Config.nLoops, "Linking Audio...");
    linkAudio();

    auto end{std::clock()};

    double time_took = (double)(end - start) / CLOCKS_PER_SEC;
    std::cout << "da Built took " << time_took << " secs.\n";

    wxMessageDialog doneMessage(parent,
                                "Build Success! Took " + std::to_string(time_took) +
                                    " seconds.\nYour Video is Ready, wanna open it right away?",
                                "Build Success", wxOK | wxCANCEL);

    if (doneMessage.ShowModal() == wxID_OK)
    {
        wxLaunchDefaultBrowser("https://www.youtube.com/watch?v=dQw4w9WgXcQ");
        std::cout << "Going to Rick Roll the user... :)\n";
    }

    return true;
}

void RTX2090Ti::RayTracing(cv::Mat &canvas, std::pair<int, int> &Start, std::pair<int, int> &End)
{

    std::pair<int, int> BigTileStart{Start.first / cols, Start.second / rows};
    std::pair<int, int> BigTileEnd{std::ceil((double)End.first / cols),
                                   std::ceil((double)End.second / rows)};

    canvas = Corgi::changeTone(BaseImageGray, std::tuple<int, int, int>(0, 0, 255));
}

void RTX2090Ti::linkAudio()
{
    std::string toExec("ffmpeg -i ");
    toExec += Config.OutVideoPath +
              ".avi -i ./assets/RTX.mp3 -b:a 384k -map 0:v:0 -map 1:a:0 -c:v copy -shortest ";
    toExec += Config.OutVideoPath + ".mp4";

    std::cout << "Executing: " << toExec << "\n";
    std::system(toExec.c_str());
    std::cout << "FFmpeg: Linking Audio Success\n";
    return;
}

std::string RTX2090Ti::statusMessage(int loopsDone, int allLoops, int framesDone, int allFrames)
{
    std::string result;
    result += "Loops Done: " + std::to_string(loopsDone) + " of " + std::to_string(allLoops);
    result += "\n";
    result += "Frames Done: " + std::to_string(framesDone) + " of " + std::to_string(allFrames);
    return result;
}
