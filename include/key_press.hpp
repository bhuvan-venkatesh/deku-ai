#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <ostream>
#include <iterator>

extern "C" {
#include <xdo.h>
}
#include "emulator_window.hpp"

using std::vector;
using std::string;

string implode(const vector<string>& elements, const char* separator){
	switch (elements.size())
  {
      case 0:
          return "";
      case 1:
          return elements[0];
      default:
          std::ostringstream os;
          std::copy(elements.begin(), elements.end()-1, std::ostream_iterator<std::string>(os, separator));
          os << *elements.rbegin();
          return os.str();
  }
}

void press_key(const vector<string>& stuff){
	static Emulator_Window* tool = NULL;
	if( !tool )
			tool = Emulator_Window::get_emulator();
  string command = implode(stuff, "+");
	int id = xdo_send_keysequence_window(tool->xdo, tool->root, command.c_str(), 0 );
}
