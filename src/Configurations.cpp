#include "Configurations.hpp"

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>
#include <wx/listctrl.h>
#include <wx/wx.h>

#include "MyFrame.hpp"

Configurations::Configurations(wxWindow *parent, ImageHandler &ImageHandlerRef)
    : parent(parent), ImageHandlerRef(ImageHandlerRef)
{
    auto init = [](wxListView *toInit) -> void
    {
        toInit->InsertColumn(0, "Properties", wxLIST_FORMAT_CENTER, 250);
        toInit->InsertColumn(1, "Value", wxLIST_FORMAT_CENTER, 530);
    };

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

    OutputListView = new wxListView(parent, wxID_ANY, wxDefaultPosition, wxSize(780, 150));
    init(OutputListView);

    OutputListView->InsertItem(0, "Output Video Path");
    OutputListView->InsertItem(1, "Output Video Resolution");
    OutputListView->InsertItem(2, "Output Video FPS");
    OutputListView->InsertItem(3, "Output Video Length Per Loop");
    OutputListView->InsertItem(4, "Number of Loops");

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

    for (int i = 0; i < DEFAULT_LOOPS_COUNT; i++)
    {
        WarpPosition.push_back(
            {std::rand() % DEFAULT_OUTVID_RESOLUTION_X, std::rand() % DEFAULT_OUTVID_RESOLUTION_Y});
    }
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
    InputListView->SetItem(1, 1, ImageHandlerRef.getImageResStr());
}

void Configurations::updateOutputList()
{
    OutputListView->SetItem(0, 1, OutVideoPath.empty() ? "" : OutVideoPath + ".mp4");
    OutputListView->SetItem(
        1, 1, std::to_string(Resolution.first) + "x" + std::to_string(Resolution.second));
    OutputListView->SetItem(2, 1, std::to_string(FPS));
    OutputListView->SetItem(3, 1, std::to_string(LoopDuration));
    OutputListView->SetItem(4, 1, std::to_string(nLoops));
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

std::string Configurations::validateWarp(std::string ToValidate)
{
    std::stringstream ss(ToValidate);

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
    if (ImageHandlerRef.getImagePath().empty())
    {
        return "Input Path is Empty";
    }

    if (OutVideoPath.empty())
    {
        return "Output Path is Empty";
    }

    return "";
}

void Configurations::setWarpPosition(std::vector<std::pair<int, int>> &newWarp)
{
    while (newWarp.size() < nLoops)
        newWarp.push_back(
            {std::rand() % DEFAULT_OUTVID_RESOLUTION_X, std::rand() % DEFAULT_OUTVID_RESOLUTION_Y});

    WarpPosition = newWarp;
}

void Configurations::OnOutputListActivated(int selected)
{
    switch (selected)
    {
    case 0:
        ((MyFrame *)parent)->OnSaveFile();
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    default:
        throw "What did you selected???";
    }
}
