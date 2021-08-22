#include "MyApp.hpp"

#include <cstdlib>
#include <ctime>
#include <wx/wx.h>

#include "AboutApp.hpp"
#include "MyFrame.hpp"

bool MyApp::OnInit()
{
    std::srand(std::time(nullptr));

    MyFrame *frame = new MyFrame(App::aboutAppSimple(), wxPoint(2500, 200), wxSize(800, 600));
    frame->SetIcon(wxIcon("assets/linus256.jpg"));
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(MyApp);
