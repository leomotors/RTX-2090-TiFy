#include "MyFrame.hpp"

#include <wx/wx.h>
#include <wx/listctrl.h>

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    wxBoxSizer *mainColumn = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *dummyRow = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *dummyStTxt = new wxStaticText(this, wxID_ANY, "Enter Parameter: ");
    wxTextCtrl *dummyTxtBox = new wxTextCtrl(this, wxID_ANY, "Hello Test");
    dummyRow->Add(dummyStTxt, 1, wxFIXED | wxRight, 10);
    dummyRow->Add(dummyTxtBox, 1, wxFIXED | wxLeft, 10);
    mainColumn->Add(dummyRow, 1, wxFIXED | wxALL, 10);

    wxListView *ConfigList = new wxListView(this, wxID_ANY, wxDefaultPosition, wxSize(600, 300));

    ConfigList->InsertColumn(0, "Properties", wxLIST_FORMAT_CENTER, 200);
    ConfigList->InsertColumn(1, "Value", wxLIST_FORMAT_CENTER, 200);
    ConfigList->InsertItem(0, "Video Resolution");
    ConfigList->InsertItem(1, "Video FPS");
    ConfigList->InsertItem(2, "Video Length Per Loop");
    ConfigList->InsertItem(3, "Number of Loops");

    mainColumn->Add(ConfigList, 1, wxEXPAND | wxALL, 10);

    wxBoxSizer *buttonBar = new wxBoxSizer(wxHORIZONTAL);
    buttonBar->Add(new wxButton(this, wxID_ANY, "Advanced"), 0, wxFIXED | wxRight, 30);
    buttonBar->Add(new wxButton(this, wxID_ANY, "Generate!"), 0, wxFIXED | wxLeft, 30);

    mainColumn->Add(buttonBar, 0, wxFIXED | wxALL, 5);

    this->SetSizerAndFit(mainColumn);
}

void MyFrame::OnExit(wxCommandEvent &event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox("Turns you into RTX 2090 Ti",
                 "About RTX 2090 TiFy", wxOK | wxICON_INFORMATION);
}
