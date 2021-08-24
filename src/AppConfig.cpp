#include "AppConfig.hpp"

bool OS::isWindows()
{
#if defined(WIN32)
    return true;
#else
    return false;
#endif
}