#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>
#include <wx/listctrl.h>
#include <wx/wx.h>

#include "ImageHandler.hpp"

enum Algorithms {
    CORGI_LEGACY = 69,
    BRIGHTNESS_COMPENSATE = 420,
    CORGI_HSV = 1337,
    BLEND_S = 177013
};

class Configurations {
    friend class RTX2090Ti;

  private:
    std::vector<std::string> ItemsName;
    std::vector<std::string> ItemsGuide;
    std::map<int, std::string> Algorithms;
    int chosenAlgorithm;

    std::pair<int, int> Resolution;
    int FPS;
    double LoopDuration;
    int nLoops;
    std::vector<std::pair<int, int>> WarpPosition;
    std::string OutVideoPath;

    wxWindow *parent;
    wxListView *InputListView;
    wxListView *OutputListView;

    ImageHandler &ImageHandlerRef;

    int SelectedInputRow{-1};
    int SelectedOutputRow{-1};

  public:
    Configurations(wxWindow *parent, ImageHandler &ImageHandlerRef);
    wxListView *getInputListView();
    wxListView *getOutputListView();

    // * Update List with Current Data
    void updateInputList();
    void updateOutputList();
    void setOutputPath(std::string &outputPath);

    std::string getPositionsAsString();
    // * Validate and If Pass, Set it. Return empty string if pass, else traceback
    std::string validateWarp(std::string toValidate);
    // * Returns Empty String if is Ready, otherwise return error message
    std::string isRTXReady();

  private:
    void setWarpPosition(std::vector<std::pair<int, int>> newWarp =
                             std::vector<std::pair<int, int>>());

    void OnOutputListActivated(int selected);

    std::string validateConfig(int itemID, std::string toValidate);
};
