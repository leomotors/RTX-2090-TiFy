#pragma once

#include <string>

class IProgressBar {
  public:
    virtual void InitDialog(std::string message, int total) = 0;
    virtual void UpdateDialog(std::string message, int current) = 0;
};
