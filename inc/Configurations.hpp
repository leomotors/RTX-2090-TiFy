#pragma once

#include <string>
#include <utility>
#include <wx/listctrl.h>
#include <wx/wx.h>

#include "ImageHandler.hpp"

#define DEFAULT_OUTVID_RESOLUTION 480, 480
#define DEFAULT_FPS 30
#define DEFAULT_LOOP_DURATION 5.5
#define DEFAULT_LOOPS_COUNT 6

class Configurations
{
  private:
    std::pair<int, int> Resolution{DEFAULT_OUTVID_RESOLUTION};
    int FPS{DEFAULT_FPS};
    double LoopDuration{DEFAULT_LOOP_DURATION};
    int nLoops{DEFAULT_LOOPS_COUNT};

    wxListView *ListView;
    ImageHandler &ImageHandlerRef;

  public:
    Configurations(wxWindow *parent, ImageHandler &ImageHandlerRef);
    wxListView *getListView();
    // * Update List with Current Data
    void updateList();
    void setOutputPath(std::string &outputPath);
};
