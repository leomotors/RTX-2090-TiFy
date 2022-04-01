#include "MyApp.hpp"

#include <cstdlib>
#include <ctime>
#include <wx/wx.h>

#include "AboutApp.hpp"
#include "AppConfig.hpp"
#include "MyFrame.hpp"

bool MyApp::OnInit() {
    std::srand(std::time(nullptr));

    MyFrame *frame = new MyFrame(App::aboutAppSimple(), wxPoint(DEFAULT_POS),
                                 wxSize(800, 600));
    frame->SetIcon(wxIcon(APP_LOGO));
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(MyApp);
