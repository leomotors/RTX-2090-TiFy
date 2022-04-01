#pragma once

#include <wx/listctrl.h>
#include <wx/wx.h>

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Configurations.hpp"
#include "ImageHandler.hpp"

class ConfigAdapter final : public Configurations {
  private:
    wxWindow *parent;
    wxListView *InputListView;
    wxListView *OutputListView;

    int SelectedInputRow{-1};
    int SelectedOutputRow{-1};

  public:
    ConfigAdapter(wxWindow *parent, ImageHandler &ImageHandlerRef);
    wxListView *getInputListView();
    wxListView *getOutputListView();

    // * Update List with Current Data
    void updateInputList();
    void updateOutputList();

  private:
    void OnOutputListActivated(int selected);
};
