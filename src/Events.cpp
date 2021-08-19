#include "Events.hpp"

#include <wx/wx.h>

#include "MyApp.hpp"
#include "MyFrame.hpp"

// clang-format off
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame) 
    EVT_MENU(wxID_EXIT, MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
    EVT_BUTTON(OPEN_FILE_BUTTON, MyFrame::OnOpenFile)
    EVT_BUTTON(SAVE_FILE_BUTTON, MyFrame::OnSaveFile)
wxEND_EVENT_TABLE();
// clang-format on
