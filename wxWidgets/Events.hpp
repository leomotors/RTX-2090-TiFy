#pragma once

#include <wx/wx.h>

enum EventsID {
    GITHUB_MENU = 44,
    OPEN_FILE_BUTTON = 69,
    SAVE_FILE_BUTTON = 420,
    WARPSET_BUTTON = 1337,
    GENERATE_BUTTON = -69420,
    LMAO_BUTTON = -177013
};

namespace Events {
static wxCommandEvent dummyEvent;
}
