#pragma once

#include <string>
#include <utility>
#include <vector>
#include <wx/listctrl.h>
#include <wx/wx.h>

#include "ImageHandler.hpp"

#define DEFAULT_OUTVID_RESOLUTION_X 480
#define DEFAULT_OUTVID_RESOLUTION_Y 480
#define DEFAULT_FPS 30
#define DEFAULT_LOOP_DURATION 5.5
#define DEFAULT_LOOPS_COUNT 6

class Configurations
{
    friend class RTX2090Ti;

  private:
    std::pair<int, int> Resolution{DEFAULT_OUTVID_RESOLUTION_X, DEFAULT_OUTVID_RESOLUTION_Y};
    int FPS{DEFAULT_FPS};
    double LoopDuration{DEFAULT_LOOP_DURATION};
    int nLoops{DEFAULT_LOOPS_COUNT};
    std::vector<std::pair<int, int>> WarpPosition;
    std::string OutVideoPath;

    wxWindow *parent;
    wxListView *ListView;
    ImageHandler &ImageHandlerRef;

  public:
    Configurations(wxWindow *parent, ImageHandler &ImageHandlerRef);
    wxListView *getListView();
    // * Update List with Current Data
    void updateList();
    void setOutputPath(std::string &outputPath);

    std::string getPositionsAsString();
    // * Validate and If Pass, Set it. Return empty string if pass, else traceback
    std::string validateWarp(std::string ToValidate);
    bool isRTXReady();

  private:
    void setWarpPosition(std::vector<std::pair<int, int>> &newWarp);
};
