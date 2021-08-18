#pragma once

#include <utility>
#include <wx/listctrl.h>
#include <wx/wx.h>

#define DEFAULT_FPS 30
#define DEFAULT_LOOP_DURATION 5.5
#define DEFAULT_LOOPS_COUNT 6

class Configurations
{
  private:
    std::pair<int, int> Resolution;
    int FPS{DEFAULT_FPS};
    double LoopDuration{DEFAULT_LOOP_DURATION};
    int Loops{DEFAULT_LOOPS_COUNT};
    wxListView *ListView;

  public:
    Configurations(wxWindow *parent);
    wxListView *getListView();
};
