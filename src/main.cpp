// * main.cpp, I have no idea what to do rn

#include <wx/wx.h>

#include "MyFrame.hpp"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MyFrame::OnExit)
        EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
            wxEND_EVENT_TABLE()
                wxIMPLEMENT_APP(MyApp);

enum Events_ID
{

};

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("RTX 2090 TiFy", wxPoint(2500, 200), wxSize(800, 600));
    frame->SetIcon(wxIcon("assets/linus256.jpg"));
    frame->Show(true);
    return true;
}
