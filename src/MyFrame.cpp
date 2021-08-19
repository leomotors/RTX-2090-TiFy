#include "MyFrame.hpp"

#include <wx/listctrl.h>
#include <wx/wx.h>

#include "AppProps.h"
#include "Events.hpp"
#include "ImageHandler.hpp"

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size), ConfigList(Configurations(this, this->Image))
{
    // * Menu bar
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    // * Main Column
    wxBoxSizer *mainColumn = new wxBoxSizer(wxVERTICAL);

    // * Sub-Row: File Input Row
    wxBoxSizer *FileInputRow = new wxBoxSizer(wxHORIZONTAL);
    // wxStaticText *FileInputMsg =
    //    new wxStaticText(this, wxID_ANY, "Click here to select Input File -> ");
    wxButton *OpenFileButton = new wxButton(this, OPEN_FILE_BUTTON, "Browse Input File");
    wxButton *SaveFileButton = new wxButton(this, SAVE_FILE_BUTTON, "Select Output File Location");

    // FileInputRow->Add(FileInputMsg, 0, wxCENTER | wxRIGHT);
    FileInputRow->Add(OpenFileButton, 0, wxCENTER | wxRIGHT);
    FileInputRow->Add(SaveFileButton, 0, wxCENTER | wxLEFT);

    mainColumn->Add(FileInputRow, 0, wxCENTER | wxLEFT | wxRIGHT | wxUP, 10);

    // * Sub-Row: Configuration List
    mainColumn->Add(ConfigList.getListView(), 4, wxEXPAND | wxALL, 10);

    // * Sub-Row: Action Buttons
    wxBoxSizer *buttonBar = new wxBoxSizer(wxHORIZONTAL);
    buttonBar->Add(new wxButton(this, wxID_ANY, "Advanced"), 0, wxSTRETCH_NOT | wxALIGN_LEFT, 30);
    buttonBar->Add(new wxButton(this, wxID_ANY, "Generate!"), 0, wxSTRETCH_NOT | wxALIGN_RIGHT, 30);

    mainColumn->Add(buttonBar, 0, wxEXPAND | wxALL, 5);

    // * Set Sizer And Fit
    this->SetSizerAndFit(mainColumn);

    // * Configure About App
    AboutApp = "Turns you into RTX 2090 Ti\nApp Version: ";
    AboutApp += APP_VERSION;
    AboutApp += "\nCreated by Leomotors";
}

void MyFrame::OnExit(wxCommandEvent &event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox(AboutApp, "About RTX 2090 TiFy", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnOpenFile(wxCommandEvent &event)
{
    wxFileDialog openFileDialog(this, "Open Image file", ".", "", "Image Files (*.jpg)|*.jpg",
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        std::cout << "Opening File: Cancelled\n";
        return;
    }

    std::string tmpFilePath = openFileDialog.GetPath().mb_str(wxConvUTF8).data();

    std::cout << "Opening File at " << tmpFilePath << "\n";
    Image.loadFile(tmpFilePath);

    ConfigList.updateList();
}

void MyFrame::OnSaveFile(wxCommandEvent &event)
{
    wxFileDialog saveFileDialog(this, "Save Video File", ".", "", "MP4 File (*.mp4)|*.mp4",
                                wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
    {
        std::cout << "Saving File: Cancelled\n";
        return;
    }

    wxString tmpFilePathWX = saveFileDialog.GetPath();
    std::string tmpFilePath = tmpFilePathWX.mb_str(wxConvUTF8).data();

    std::cout << "Configged Output Path: " << tmpFilePath << "\n";

    ConfigList.setOutputPath(tmpFilePath);
}
