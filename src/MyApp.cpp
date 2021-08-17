#include "MyApp.hpp"

#include <wx/wx.h>

#include "MyFrame.hpp"

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("RTX 2090 TiFy", wxPoint(2500, 200), wxSize(800, 600));
    frame->SetIcon(wxIcon("assets/linus256.jpg"));
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(MyApp);
