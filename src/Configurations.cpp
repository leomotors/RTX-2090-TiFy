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
    ListView->InsertItem(2, "Output Video Resolution");
    ListView->InsertItem(3, "Output Video FPS");
    ListView->InsertItem(4, "Output Video Length Per Loop");
    ListView->InsertItem(5, "Output Number of Loops");
}

wxListView *Configurations::getListView()
{
    return ListView;
}

void Configurations::updateList()
{
    ListView->SetItem(0, 1, ImageHandlerRef.getImagePath());
    ListView->SetItem(1, 1, ImageHandlerRef.getImageResStr());
    ListView->SetItem(2, 1, "480x480"); // * Default 480x480
    ListView->SetItem(3, 1, "30");      // * Default 30
    ListView->SetItem(4, 1, "5.5");     // * Default 5.5s
    ListView->SetItem(5, 1, "6");       // * Default 6
}
