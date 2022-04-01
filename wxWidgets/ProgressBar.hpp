#pragma once

#include "IProgressBar.hpp"

class ProgressBar : public IProgressBar {
  public:
    void InitDialog(std::string message, int total);
    void UpdateDialog(std::string message, int current);
};