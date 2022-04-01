#include "ConfigAdapter.hpp"

#include <wx/listctrl.h>
#include <wx/wx.h>

#include <cstdio>
#include <cstdlib>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Configurations.hpp"
#include "ImageHandler.hpp"
#include "MyFrame.hpp"

ConfigAdapter::ConfigAdapter(wxWindow *parent, ImageHandler &ImageHandlerRef)
    : Configurations(ImageHandlerRef) {
    auto init = [](wxListView *toInit) -> void {
        toInit->InsertColumn(0, "Properties", wxLIST_FORMAT_CENTER, 250);
        toInit->InsertColumn(1, "Value", wxLIST_FORMAT_CENTER, 530);
    };

    // * Input Part Listview
    InputListView =
        new wxListView(parent, wxID_ANY, wxDefaultPosition, wxSize(780, 75));
    init(InputListView);

    InputListView->InsertItem(0, "Input Image Path");
    InputListView->InsertItem(1, "Input Image Resolution");

    InputListView->Bind(wxEVT_LIST_ITEM_SELECTED, [this](wxListEvent &event) {
        int selected = this->InputListView->GetNextItem(-1, wxLIST_NEXT_ALL,
                                                        wxLIST_STATE_SELECTED);
        if (SelectedOutputRow != -1) {
            this->OutputListView->SetItemState(SelectedOutputRow, 0,
                                               wxLIST_STATE_SELECTED);
            SelectedOutputRow = -1;
        }
        SelectedInputRow = selected;
    });
    InputListView->Bind(wxEVT_LIST_ITEM_ACTIVATED, [this](wxListEvent &event) {
        ((MyFrame *)(this->parent))->OnOpenFile();
    });

    // * Output Part Listview (Configable)
    OutputListView =
        new wxListView(parent, wxID_ANY, wxDefaultPosition, wxSize(780, 180));
    init(OutputListView);

    for (int i = 0; i < ItemsName.size(); i++)
        OutputListView->InsertItem(i, ItemsName[i]);

    OutputListView->Bind(wxEVT_LIST_ITEM_SELECTED, [this](wxListEvent &event) {
        int selected = this->OutputListView->GetNextItem(-1, wxLIST_NEXT_ALL,
                                                         wxLIST_STATE_SELECTED);
        if (SelectedInputRow != -1) {
            this->InputListView->SetItemState(SelectedInputRow, 0,
                                              wxLIST_STATE_SELECTED);
            SelectedInputRow = -1;
        }
        SelectedOutputRow = selected;
    });
    OutputListView->Bind(wxEVT_LIST_ITEM_ACTIVATED, [this](wxListEvent &event) {
        this->OnOutputListActivated(this->OutputListView->GetNextItem(
            -1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED));
    });
}

wxListView *ConfigAdapter::getInputListView() {
    updateInputList();
    return InputListView;
}

wxListView *ConfigAdapter::getOutputListView() {
    updateOutputList();
    return OutputListView;
}

void ConfigAdapter::updateInputList() {
    InputListView->SetItem(0, 1, ImageHandlerRef.getImagePath());
    InputListView->SetItem(1, 1, ImageHandlerRef.getImageResStr(true));
}

void ConfigAdapter::updateOutputList() {
    OutputListView->SetItem(0, 1,
                            OutVideoPath.empty() ? "" : OutVideoPath + ".mp4");
    OutputListView->SetItem(1, 1,
                            std::to_string(Resolution.first) + "x" +
                                std::to_string(Resolution.second));
    OutputListView->SetItem(2, 1, std::to_string(FPS));
    OutputListView->SetItem(3, 1, std::to_string(LoopDuration));
    OutputListView->SetItem(4, 1, std::to_string(nLoops));
    OutputListView->SetItem(5, 1, Algorithms[chosenAlgorithm]);
}

void ConfigAdapter::OnOutputListActivated(int selected) {
    if (selected == 0) {
        ((MyFrame *)parent)->OnSaveFile();
        return;
    }

    if (selected >= ItemsName.size()) throw "What did ya just selected!?";

    wxTextEntryDialog ConfigDialog(parent, ItemsGuide[selected],
                                   "Setting \"" + ItemsName[selected] + "\"");

    if (ConfigDialog.ShowModal() == wxID_OK) {
        std::string traceback =
            validateConfig(selected, ConfigDialog.GetValue().ToStdString());
        if (!traceback.empty())
            wxMessageBox(traceback, "Validation Failed: Value not set",
                         wxOK | wxICON_ERROR);
        else
            std::cout << "Config #" << selected << " Success\n";

        updateOutputList();
    } else {
        std::cout << "Config Settings cancelled by User\n";
    }
}
