#pragma once

#include "Configurations.hpp"

#include <wx/listctrl.h>
#include <wx/wx.h>

class MyFrame : public wxFrame
{
  private:
    Configurations ConfigList;

  public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

  private:
    void OnExit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);

    void OnOpenFile(wxCommandEvent &event);

    wxDECLARE_EVENT_TABLE();
};
