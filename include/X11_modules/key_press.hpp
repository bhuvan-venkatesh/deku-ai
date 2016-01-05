#ifndef KEY_PRESS_H
#define KEY_PRESS_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <ostream>
#include <iterator>
#include "emulator_window.hpp"

using std::vector;
using std::string;

string implode(const vector<string>& elements, const char* separator);

/* Xdotool Keykodes
* alphanumerics = "[A-Za-z0-9]" ex: "A"
* Directional = "Up" "Down" etc..
* Special Keys = "Alt" "Ctrl_L" "Shift_L"
*/
void press_keys(const vector<string>& stuff);

#endif
