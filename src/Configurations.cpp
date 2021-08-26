#include "Configurations.hpp"

#include <cstdio>
#include <cstdlib>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <wx/listctrl.h>
#include <wx/wx.h>

#include "ImageHandler.hpp"
#include "MyFrame.hpp"

#define DEFAULT_OUTVID_RESOLUTION 480, 480
#define DEFAULT_FPS 30
#define DEFAULT_LOOP_DURATION 5.5
#define DEFAULT_LOOPS_COUNT 6
#define DEFAULT_ALGORITHM CORGI_LEGACY

Configurations::Configurations(wxWindow *parent, ImageHandler &ImageHandlerRef)
    : parent(parent), ImageHandlerRef(ImageHandlerRef),
      ItemsName(std::vector<std::string>{"Output Video Path", "Output Video Resolution",
                                         "Output Video FPS", "Output Video Length Per Loop",
                                         "Number of Loops", "Algorithm"}),
      ItemsGuide(std::vector<std::string>{
          "", "Resolution of Output Video\nExample: 480x480", "FPS of Output Video",
          "Length of each Loop in the Video", "Number of Loops in the Video",
          "Algorithm used in rendering video\nPlease enter ID of your desire algorithm\n"}),
      Algorithms(std::map<int, std::string>{
          {CORGI_LEGACY, "Corgi Legacy"},
          {BRIGHTNESS_COMPENSATE, "Corgi Legacy with Brightness Compensate"},
          {CORGI_HSV, "Corgi HSV"}}),
      Resolution({DEFAULT_OUTVID_RESOLUTION}), FPS(DEFAULT_FPS),
      LoopDuration(DEFAULT_LOOP_DURATION), nLoops(DEFAULT_LOOPS_COUNT),
      chosenAlgorithm(DEFAULT_ALGORITHM)
{
    ItemsGuide[5] += [this]() -> std::string
    {
        std::string algodesc;
        for (auto algorithm : this->Algorithms)
            algodesc += algorithm.second + " : " + std::to_string(algorithm.first) + "\n";
        return algodesc;
    }();

    auto init = [](wxListView *toInit) -> void
    {
        toInit->InsertColumn(0, "Properties", wxLIST_FORMAT_CENTER, 250);
        toInit->InsertColumn(1, "Value", wxLIST_FORMAT_CENTER, 530);
    };

    // * Input Part Listview
    InputListView = new wxListView(parent, wxID_ANY, wxDefaultPosition, wxSize(780, 75));
    init(InputListView);

    InputListView->InsertItem(0, "Input Image Path");
    InputListView->InsertItem(1, "Input Image Resolution");

    InputListView->Bind(
        wxEVT_LIST_ITEM_SELECTED,
        [this](wxListEvent &event)
        {
            int selected =
                this->InputListView->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
            if (SelectedOutputRow != -1)
            {
                this->OutputListView->SetItemState(SelectedOutputRow, 0, wxLIST_STATE_SELECTED);
                SelectedOutputRow = -1;
            }
            SelectedInputRow = selected;
        });
    InputListView->Bind(wxEVT_LIST_ITEM_ACTIVATED,
                        [this](wxListEvent &event) { ((MyFrame *)(this->parent))->OnOpenFile(); });

    // * Output Part Listview (Configable)
    OutputListView = new wxListView(parent, wxID_ANY, wxDefaultPosition, wxSize(780, 180));
    init(OutputListView);

    for (int i = 0; i < ItemsName.size(); i++)
        OutputListView->InsertItem(i, ItemsName[i]);

    OutputListView->Bind(
        wxEVT_LIST_ITEM_SELECTED,
        [this](wxListEvent &event)
        {
            int selected =
                this->OutputListView->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
            if (SelectedInputRow != -1)
            {
                this->InputListView->SetItemState(SelectedInputRow, 0, wxLIST_STATE_SELECTED);
                SelectedInputRow = -1;
            }
            SelectedOutputRow = selected;
        });
    OutputListView->Bind(wxEVT_LIST_ITEM_ACTIVATED,
                         [this](wxListEvent &event)
                         {
                             this->OnOutputListActivated(this->OutputListView->GetNextItem(
                                 -1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED));
                         });

    setWarpPosition();
}

wxListView *Configurations::getInputListView()
{
    updateInputList();
    return InputListView;
}

wxListView *Configurations::getOutputListView()
{
    updateOutputList();
    return OutputListView;
}

void Configurations::updateInputList()
{
    InputListView->SetItem(0, 1, ImageHandlerRef.getImagePath());
    InputListView->SetItem(1, 1, ImageHandlerRef.getImageResStr(true));
}

void Configurations::updateOutputList()
{
    OutputListView->SetItem(0, 1, OutVideoPath.empty() ? "" : OutVideoPath + ".mp4");
    OutputListView->SetItem(
        1, 1, std::to_string(Resolution.first) + "x" + std::to_string(Resolution.second));
    OutputListView->SetItem(2, 1, std::to_string(FPS));
    OutputListView->SetItem(3, 1, std::to_string(LoopDuration));
    OutputListView->SetItem(4, 1, std::to_string(nLoops));
    OutputListView->SetItem(5, 1, Algorithms[chosenAlgorithm]);
}

void Configurations::setOutputPath(std::string &outputPath)
{
    if (!outputPath.ends_with(".mp4"))
        outputPath += ".mp4";

    OutVideoPath = outputPath.substr(0, outputPath.size() - 4);

    updateOutputList();
}

std::string Configurations::getPositionsAsString()
{
    std::string result_str;
    for (std::pair<int, int> pos : WarpPosition)
    {
        result_str += std::to_string(pos.first) + "x" + std::to_string(pos.second) + " ";
    }
    return result_str;
}

std::string Configurations::validateWarp(std::string toValidate)
{
    std::stringstream ss(toValidate);

    std::vector<std::string> word_arr;
    std::string tmp;
    while (ss >> tmp)
    {
        word_arr.push_back(tmp);
    }

    if (word_arr.size() > nLoops)
    {
        return "Too Many Points Recieved";
    }

    std::vector<std::pair<int, int>> newWarp;
    for (std::string word : word_arr)
    {
        int x, y;
        std::sscanf(word.c_str(), "%dx%d", &x, &y);

        if (x < 0 || x >= Resolution.first)
        {
            std::string traceback = "The Value x of " + std::to_string(x) + " from " + word +
                                    " is not in the acceptable range.";
            return traceback;
        }

        if (y < 0 || y >= Resolution.second)
        {
            std::string traceback = "The Value y of " + std::to_string(y) + " from " + word +
                                    " is not in the acceptable range.";
            return traceback;
        }

        newWarp.push_back({x, y});
    }

    std::cout << "Warp Points Settings Validation Success and Set\n";
    setWarpPosition(newWarp);
    return "";
}

std::string Configurations::isRTXReady()
{
    if (ImageHandlerRef.getImagePath().empty() || !ImageHandlerRef.status)
    {
        return "Input Path is not set or Image isn't loaded properly";
    }

    if (OutVideoPath.empty())
    {
        return "Output Path is not set";
    }

    return "";
}

void Configurations::setWarpPosition(std::vector<std::pair<int, int>> newWarp)
{
    while (newWarp.size() < nLoops)
        newWarp.push_back({std::rand() % Resolution.first, std::rand() % Resolution.second});

    WarpPosition = newWarp;
}

void Configurations::OnOutputListActivated(int selected)
{
    if (selected == 0)
    {
        ((MyFrame *)parent)->OnSaveFile();
        return;
    }

    if (selected >= ItemsName.size())
        throw "What did ya just selected!?";

    wxTextEntryDialog ConfigDialog(parent, ItemsGuide[selected],
                                   "Setting \"" + ItemsName[selected] + "\"");

    if (ConfigDialog.ShowModal() == wxID_OK)
    {
        std::string traceback = validateConfig(selected, ConfigDialog.GetValue().ToStdString());
        if (!traceback.empty())
            wxMessageBox(traceback, "Validation Failed: Value not set", wxOK | wxICON_ERROR);
        else
            std::cout << "Config #" << selected << " Success\n";

        updateOutputList();
    }
    else
    {
        std::cout << "Config Settings cancelled by User\n";
    }
}

std::string Configurations::validateConfig(int itemID, std::string toValidate)
{
    switch (itemID)
    {
    case 1:
    {
        int w{0}, h{0};
        sscanf(toValidate.c_str(), "%dx%d", &w, &h);

        if (w < 144)
            return "Width of " + std::to_string(w) + " is too small! Minimum is 144";
        if (h < 144)
            return "Height of " + std::to_string(h) + " is too small! Minimum is 144";

        Resolution = {w, h};
        setWarpPosition();
        break;
    }
    case 2:
    {
        int fps{0};
        sscanf(toValidate.c_str(), "%d", &fps);

        if (fps < 1)
            return "FPS of " + std::to_string(fps) + " is too Small";

        FPS = fps;
        break;
    }
    case 3:
    {
        double vidlen{0.0};
        sscanf(toValidate.c_str(), "%lf", &vidlen);

        if (vidlen < 1.0)
            return "Video Length Per Loop of " + std::to_string(vidlen) +
                   " is too Small. Minimum is 1 second";

        LoopDuration = vidlen;
        break;
    }
    case 4:
    {
        int nLoopsNew{0};
        sscanf(toValidate.c_str(), "%d", &nLoopsNew);

        if (nLoopsNew < 1)
            return "Number of Loops of " + std::to_string(nLoopsNew) +
                   " will make video not exist!";

        nLoops = nLoopsNew;
        setWarpPosition();
        break;
    }
    case 5:
    {
        int newAlgorithm{0};
        sscanf(toValidate.c_str(), "%d", &newAlgorithm);

        if (Algorithms.find(newAlgorithm) == Algorithms.end())
            return "Algorithm with ID of " + std::to_string(newAlgorithm) + " does not exist";

        chosenAlgorithm = newAlgorithm;
        break;
    }
    default:
        throw "Some Member Function or Friend Class is SUS";
    }
    return "";
}
