#include "MyFrame.hpp"

#include <wx/listctrl.h>
#include <wx/wx.h>

#include <cstdlib>
#include <exception>
#include <opencv2/opencv.hpp>

#include "AboutApp.hpp"
#include "AppConfig.hpp"
#include "ConfigAdapter.hpp"
#include "Events.hpp"
#include "ImageHandler.hpp"
#include "ProgressBar.hpp"
#include "RTX2090Ti.hpp"

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size),
      ConfigList(ConfigAdapter(this, Image)) {
    // * Menu bar
    wxMenu *menuAbout = new wxMenu;
    menuAbout->Append(GITHUB_MENU, "Visit GitHub Page");
    menuAbout->Append(wxID_ABOUT, "About App");

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuAbout, "&Menu");

    SetMenuBar(menuBar);

    // * Main Column
    wxBoxSizer *mainColumn = new wxBoxSizer(wxVERTICAL);

    // * Sub-Row: File Input Row
    wxBoxSizer *FileInputRow = new wxBoxSizer(wxHORIZONTAL);
    wxButton *OpenFileButton =
        new wxButton(this, OPEN_FILE_BUTTON, "Browse Input File");
    wxButton *SaveFileButton =
        new wxButton(this, SAVE_FILE_BUTTON, "Select Output File Location");

    FileInputRow->Add(OpenFileButton, 0, wxCENTER | wxRIGHT);
    FileInputRow->Add(SaveFileButton, 0, wxCENTER | wxLEFT);

    mainColumn->Add(FileInputRow, 0, wxCENTER | wxLEFT | wxRIGHT | wxUP, 10);

    // * Sub-Row: Configuration List
    mainColumn->Add(
        new wxStaticText(this, wxID_ANY, "  Input Image Informations: "), 0,
        wxLEFT | wxUP, 10);
    mainColumn->Add(ConfigList.getInputListView(), 2, wxEXPAND | wxALL, 10);
    mainColumn->Add(new wxStaticText(this, wxID_ANY,
                                     "  Your Output Video Configurations: "),
                    0, wxLEFT | wxUP, 10);
    mainColumn->Add(ConfigList.getOutputListView(), 4, wxEXPAND | wxALL, 10);

    // * Sub-Row: Action Buttons
    wxBoxSizer *buttonBar = new wxBoxSizer(wxHORIZONTAL);
    buttonBar->Add(
        new wxButton(this, WARPSET_BUTTON, "Set Warp Points (Advanced)"), 0,
        wxALIGN_CENTER, 30);
    buttonBar->Add(new wxButton(this, LMAO_BUTTON, "Use GPU Acceleration"), 0,
                   wxALIGN_CENTER, 30);
    buttonBar->Add(new wxButton(this, GENERATE_BUTTON, "Generate!"), 0,
                   wxALIGN_CENTER, 30);

    mainColumn->Add(buttonBar, 0, wxCENTER | wxALL, 10);

    // * Set Sizer And Fit
    this->SetSizerAndFit(mainColumn);
}

// * App Menu Events

void MyFrame::OnExit(wxCommandEvent &event) {
    std::cout << "Program exited by Users\n";
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent &event) {
    wxMessageBox(App::aboutAppLong(), "About RTX 2090 TiFy",
                 wxOK | wxICON_INFORMATION);
}

void MyFrame::OnGitHub(wxCommandEvent &event) {
    std::cout << "Opening RTX 2090 TiFy's GitHub Page\n";
    wxLaunchDefaultBrowser(GITHUB_URL);
}

// * Application Events

void MyFrame::OnOpenFile(wxCommandEvent &event) {
    wxFileDialog openFileDialog(
        this, "Open Image file", ".", "",
        "Image Files (*.jpg *.jpeg *.png)|*.jpg;*.jpeg;*.png",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        std::cout << "Opening File: Cancelled\n";
        return;
    }

    std::string tmpFilePath =
        openFileDialog.GetPath().mb_str(wxConvUTF8).data();

    std::cout << "Opening File at " << tmpFilePath << "\n";

    if (!Image.loadFile(tmpFilePath)) {
        std::cout << "Error Opening File\n";
        wxMessageBox("There is some issue and image wasn't open properly",
                     "Error Opening File", wxOK | wxICON_ERROR);
    }

    ConfigList.updateInputList();
}

void MyFrame::OnSaveFile(wxCommandEvent &event) {
    wxFileDialog saveFileDialog(this, "Save Video File", "./exports", "",
                                "MP4 File (*.mp4)|*.mp4",
                                wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL) {
        std::cout << "Saving File: Cancelled\n";
        return;
    }

    wxString tmpFilePathWX = saveFileDialog.GetPath();
    std::string tmpFilePath = tmpFilePathWX.mb_str(wxConvUTF8).data();

    std::cout << "Configged Output Path: " << tmpFilePath << "\n";

    ConfigList.setOutputPath(tmpFilePath);
}

void MyFrame::OnAdvanced(wxCommandEvent &event) {
    const char *tmpMsg{
        "These are the warp positions that will style your "
        "video. If one is set "
        "empty, it will be randomly generated again."};
    wxTextEntryDialog WarpSetDialog(this, tmpMsg, "Warp Points Settings",
                                    ConfigList.getPositionsAsString(),
                                    wxTextEntryDialogStyle, wxDefaultPosition);

    if (WarpSetDialog.ShowModal() == wxID_OK) {
        std::string traceback =
            ConfigList.validateWarp(WarpSetDialog.GetValue().ToStdString());
        if (!traceback.empty()) {
            wxMessageBox(traceback, "Validation Failed: Value not set",
                         wxOK | wxICON_ERROR);
        }
    } else
        std::cout << "Warp Points Settings Cancelled by User\n";
}

void MyFrame::OnGenerate(wxCommandEvent &event) {
    std::string traceback = ConfigList.isRTXReady();
    ProgressBar progBar;

    if (traceback.empty()) {
        try {
            RTX2090Ti RTX(progBar, Image.getImage(), ConfigList);
            RTX.buildVideo();
        } catch (const std::exception &ex) {
            wxMessageBox(ex.what(), "std::exception thrown!",
                         wxOK | wxICON_ERROR);
        } catch (const std::string &ex) {
            wxMessageBox(ex, "Exception thrown!", wxOK | wxICON_ERROR);
        } catch (...) {
            wxMessageBox("Unknown Exception has been thrown!",
                         "Unknown Exception thrown!", wxOK | wxICON_ERROR);
        }

        cv::destroyAllWindows();
    } else {
        wxMessageBox(traceback, "Build Aborted: Not RTX Ready",
                     wxOK | wxICON_ERROR);
    }

    return;
}

void MyFrame::OnRickRoll(wxCommandEvent &event) {
    std::cout << "User is getting Rick Rolled (*/ω＼*)\n";
    wxLaunchDefaultBrowser(RICKROLL_URL);
}
