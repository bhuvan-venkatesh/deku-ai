#include "key_press.hpp"
extern "C" {
#include <xdo.h>
}

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

/* Xdotool Keykodes
* alphanumerics = "[A-Za-z0-9]" ex: "A"
* Directional = "Up" "Down" etc..
* Special Keys = "Alt" "Ctrl_L" "Shift_L"
*/
void press_keys(const vector<string>& stuff){
	static Emulator_Window* tool = NULL;
	if( !tool )
			tool = Emulator_Window::get_emulator();
  string command = implode(stuff, "+");
	int id = xdo_send_keysequence_window(tool->xdo, tool->root, command.c_str(), 0 );
}

void press_keys(const string& command){
	static Emulator_Window* tool = NULL;
	if( !tool )
			tool = Emulator_Window::get_emulator();
	xdo_send_keysequence_window(tool->xdo, tool->root, command.c_str(), 0 );
}
