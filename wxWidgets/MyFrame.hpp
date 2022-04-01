#pragma once

#include <wx/listctrl.h>
#include <wx/wx.h>

#include <string>

#include "ConfigAdapter.hpp"
#include "Events.hpp"
#include "ImageHandler.hpp"

class MyFrame : public wxFrame {
    friend class ConfigAdapter;

  private:
    ConfigAdapter ConfigList;
    ImageHandler Image;

    std::string AboutApp;

  public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

  private:
    void OnExit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnGitHub(wxCommandEvent &event);

    void OnOpenFile(wxCommandEvent &event = Events::dummyEvent);
    void OnSaveFile(wxCommandEvent &event = Events::dummyEvent);
    void OnAdvanced(wxCommandEvent &event);
    void OnGenerate(wxCommandEvent &event);
    void OnRickRoll(wxCommandEvent &event);

    wxDECLARE_EVENT_TABLE();
};
