#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <ostream>
#include <iterator>
#include "emulator_window.hpp"
#define xdo_sep "+"

using std::vector;
using std::string;

string implode(const vector<string> &elements, const char *separator = xdo_sep);

/* Xdotool Keykodes
* alphanumerics = "[A-Za-z0-9]" ex: "A"
* Directional = "Up" "Down" etc..
* Special Keys = "Alt" "Ctrl_L" "Shift_L"
*/
void press_keys(const vector<string> &stuff);

void press_keys(const char *command);
