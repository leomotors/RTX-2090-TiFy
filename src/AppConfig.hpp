#pragma once

#define GITHUB_URL "https://github.com/Leomotors/RTX-2090-TiFy"
#define RICKROLL_URL "https://www.youtube.com/watch?v=dQw4w9WgXcQ"

#define APP_LOGO "./assets/linus256.jpg"
#define RTX_MUSIC "./assets/RTX.mp3"
#define RTX_LEN 60
#define DEFAULT_FILE_EXPLORER "explorer.exe"

#if __has_include("SPECIAL_CONFIG.h")
#include "SPECIAL_CONFIG.h"
#endif

#ifndef DEFAULT_POS
#define DEFAULT_POS 600, 200
#endif

namespace OS {
bool isWindows();
}
