#include "Configurations.hpp"

#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>
#include <wx/listctrl.h>
#include <wx/wx.h>

Configurations::Configurations(wxWindow *parent, ImageHandler &ImageHandlerRef)
    : parent(parent), ImageHandlerRef(ImageHandlerRef)
{
    ListView = new wxListView(parent, wxID_ANY, wxDefaultPosition, wxSize(780, 300));

    ListView->InsertColumn(0, "Properties", wxLIST_FORMAT_CENTER, 250);
    ListView->InsertColumn(1, "Value", wxLIST_FORMAT_CENTER, 530);
    ListView->InsertItem(0, "Input Image Path");
    ListView->InsertItem(1, "Input Image Resolution");
    ListView->InsertItem(2, wxEmptyString);
    ListView->InsertItem(3, "Output Video Path");
    ListView->InsertItem(4, "Output Video Resolution");
    ListView->InsertItem(5, "Output Video FPS");
    ListView->InsertItem(6, "Output Video Length Per Loop");
    ListView->InsertItem(7, "Number of Loops");

    for (int i = 0; i < DEFAULT_LOOPS_COUNT; i++)
    {
        WarpPosition.push_back(
            {std::rand() % DEFAULT_OUTVID_RESOLUTION_X, std::rand() % DEFAULT_OUTVID_RESOLUTION_Y});
    }
}

wxListView *Configurations::getListView()
{
    updateList();
    return ListView;
}

void Configurations::updateList()
{
    ListView->SetItem(0, 1, ImageHandlerRef.getImagePath());
    ListView->SetItem(1, 1, ImageHandlerRef.getImageResStr());

    ListView->SetItem(4, 1,
                      std::to_string(Resolution.first) + "x" + std::to_string(Resolution.second));
    ListView->SetItem(5, 1, std::to_string(FPS));
    ListView->SetItem(6, 1, std::to_string(LoopDuration));
    ListView->SetItem(7, 1, std::to_string(nLoops));
}

void Configurations::setOutputPath(std::string &outputPath)
{
    if (!outputPath.ends_with(".mp4"))
        outputPath += ".mp4";

    ListView->SetItem(3, 1, outputPath);
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

bool Configurations::validate(std::string ToValidate)
{
    std::stringstream ss(ToValidate);

    std::vector<std::string> word_arr;
    std::string tmp;
    while (ss >> tmp)
    {
        word_arr.push_back(tmp);
    }

    if (tmp.size() > nLoops)
        return false;

    return true;
}
