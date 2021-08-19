#include "Configurations.hpp"

#include <wx/listctrl.h>
#include <wx/wx.h>

Configurations::Configurations(wxWindow *parent, ImageHandler &ImageHandlerRef)
    : ImageHandlerRef(ImageHandlerRef)
{
    ListView = new wxListView(parent, wxID_ANY, wxDefaultPosition, wxSize(800, 300));

    ListView->InsertColumn(0, "Properties", wxLIST_FORMAT_CENTER, 300);
    ListView->InsertColumn(1, "Value", wxLIST_FORMAT_CENTER, 500);
    ListView->InsertItem(0, "Input Image Path");
    ListView->InsertItem(1, "Input Image Resolution");
    ListView->InsertItem(2, wxEmptyString);
    ListView->InsertItem(3, "Output File Path");
    ListView->InsertItem(4, "Output Video Resolution");
    ListView->InsertItem(5, "Output Video FPS");
    ListView->InsertItem(6, "Output Video Length Per Loop");
    ListView->InsertItem(7, "Output Number of Loops");
}

wxListView *Configurations::getListView()
{
    return ListView;
}

void Configurations::updateList()
{
    ListView->SetItem(0, 1, ImageHandlerRef.getImagePath());
    ListView->SetItem(1, 1, ImageHandlerRef.getImageResStr());
    ListView->SetItem(4, 1,
                      std::to_string(Resolution.first) + "x" + std::to_string(Resolution.second));
    ListView->SetItem(5, 1, std::to_string(FPS));          // * Default 30
    ListView->SetItem(6, 1, std::to_string(LoopDuration)); // * Default 5.5s
    ListView->SetItem(7, 1, std::to_string(nLoops));       // * Default 6
}

void Configurations::setOutputPath(std::string &outputPath)
{
    if (!outputPath.ends_with(".mp4"))
        outputPath += ".mp4";

    ListView->SetItem(3, 1, outputPath);
}
