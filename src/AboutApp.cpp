#include "AboutApp.hpp"

#include <string>

#include "AppProps.h"

std::string App::aboutApp()
{
    std::string AboutApp = "Turns you into RTX 2090 Ti\nApp Version: ";
    AboutApp += APP_VERSION;
    AboutApp += "\nCreated by Leomotors";
    return AboutApp;
}
