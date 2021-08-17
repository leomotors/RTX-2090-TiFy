#include "Events.hpp"

#include <wx/wx.h>

#include "MyApp.hpp"
#include "MyFrame.hpp"

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MyFrame::OnExit)
        EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
    //EVT_MENU(OPEN_FILE_BUTTON, MyFrame::OnOpenFile)
    wxEND_EVENT_TABLE()
