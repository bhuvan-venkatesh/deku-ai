#include "snes_controller.hpp"
#include <algorithm>
#include <string>
#include <fstream>      // std::ifstream
#include <cctype>
#define file_name "bindings.txt"

inline string string_upper(const string& refe){
  string ref(refe);
  std::transform(ref.begin(), ref.end(), ref.begin(), ::toupper);
  return ref;
}
Snes_Controller::Snes_Controller(const std::unordered_map<string, string>& bindings_){
  for(auto i = bindings_.begin(); i != bindings_.end(); ++i)
    bindings[string_upper(i->first)] = i->second;
}

Snes_Controller::Snes_Controller(){
  char name[32];
  std::ifstream bindings_txt(file_name);

  while(bindings_txt.good()){
    bindings_txt.getline(name, 32);
    string line(name);
    auto space = line.find(" ");
    string lhs = line.substr(0, space);
    line.erase(line.begin(), line.begin()+(space+1));
    bindings[lhs] = line;
  }
}
Snes_Controller& Snes_Controller::a(){
    return press("A");
}

Snes_Controller& Snes_Controller::b(){
    return press("B");
}

Snes_Controller& Snes_Controller::x(){
    return press("X");
}

Snes_Controller& Snes_Controller::y(){
    return press("Y");
}

Snes_Controller& Snes_Controller::up(){
    return press("UP");
}

Snes_Controller& Snes_Controller::down(){
    return press("DOWN");
}

Snes_Controller& Snes_Controller::right(){
    return press("RIGHT");
}

Snes_Controller& Snes_Controller::left(){
    return press("LEFT");
}

Snes_Controller& Snes_Controller::right_trigger(){
    return press("RIGHT_TRIGGER");
}

Snes_Controller& Snes_Controller::left_trigger(){
    return press("LEFT_TRIGGER");
}

Snes_Controller& Snes_Controller::start(){
    return press("START");
}

Snes_Controller& Snes_Controller::select(){
    return press("SELECT");
}

void load_state(){
  press("LOAD");
  execute();
}

void Snes_Controller::execute(){
    press_keys(buffer);
    buffer.clear();
}

Snes_Controller& Snes_Controller::press(const string& stuff){
  buffer.push_back(bindings[stuff]);
  return *this;
}
