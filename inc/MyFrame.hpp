#pragma once

#include <wx/wx.h>

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    void OnExit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);

    wxDECLARE_EVENT_TABLE();
};
