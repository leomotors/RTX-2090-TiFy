#include "MyFrame.hpp"

#include <wx/listctrl.h>
#include <wx/wx.h>

#include "Events.hpp"

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size), ConfigList(Configurations(this))
{
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    wxBoxSizer *mainColumn = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *FileInputRow = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *FileInputMsg = new wxStaticText(this, wxID_ANY, "Input File Path: ");
    wxTextCtrl *FileNameTextBox =
        new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(300, 30));
    wxButton *OpenFileButton = new wxButton(this, OPEN_FILE_BUTTON, "Browse Input File");

    FileInputRow->Add(FileInputMsg, 0, wxCENTER | wxRight);
    FileInputRow->Add(FileNameTextBox, 1, wxEXPAND | wxLeft | wxRight);
    FileInputRow->Add(OpenFileButton, 0, wxCENTER | wxLeft);
    mainColumn->Add(FileInputRow, 0, wxEXPAND | wxALL, 10);

    mainColumn->Add(ConfigList.getListView(), 4, wxEXPAND | wxALL, 10);

    wxBoxSizer *buttonBar = new wxBoxSizer(wxHORIZONTAL);
    buttonBar->Add(new wxButton(this, wxID_ANY, "Advanced"), 0, wxALIGN_LEFT, 30);
    buttonBar->Add(new wxButton(this, wxID_ANY, "Generate!"), 0, wxALIGN_RIGHT, 30);

    mainColumn->Add(buttonBar, 0, wxFIXED | wxALL, 5);

    this->SetSizerAndFit(mainColumn);
}

void MyFrame::OnExit(wxCommandEvent &event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox("Turns you into RTX 2090 Ti", "About RTX 2090 TiFy", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnOpenFile(wxCommandEvent &event)
{
    // ! Temp
    std::cout << "Why u click meh\n";
    ConfigList.getListView()->InsertItem(4, "KUAY");
}
