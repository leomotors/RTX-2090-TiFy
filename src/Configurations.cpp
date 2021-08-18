#include "Configurations.hpp"

#include <wx/listctrl.h>
#include <wx/wx.h>

Configurations::Configurations(wxWindow *parent)
{
    ListView = new wxListView(parent, wxID_ANY, wxDefaultPosition, wxSize(600, 300));

    ListView->InsertColumn(0, "Properties", wxLIST_FORMAT_CENTER, 200);
    ListView->InsertColumn(1, "Value", wxLIST_FORMAT_CENTER, 200);
    ListView->InsertItem(0, "Video Resolution");      // * Default 480x480
    ListView->InsertItem(1, "Video FPS");             // * Default 30
    ListView->InsertItem(2, "Video Length Per Loop"); // * Default 5.5s
    ListView->InsertItem(3, "Number of Loops");       // * Default 6
}

wxListView *Configurations::getListView()
{
    return ListView;
}
