#include "snes_controller.hpp"
#include <algorithm>
#include <string>
#include <fstream>      // std::ifstream
#define file_name "bindings.txt"

inline void tolower(string& ref){
  std::transform(ref.begin(), ref.end(), ref.begin(), ::tolower);
}
Snes_Controller::snes_controller(const std::unordered_map<string, string>& bindings_){
  for(auto i = bindings_.begin(); i != bindings.end(); ++i)
    bindings[tolower(i->first)] = i->second;
}

Snes_Controller::snes_controller(){
  char name[32];
  ifstream bindings_txt(file_name);

  while(bindings_txt.good()){
    std::getline(name, 32);
    string line(name);
    auto space = line.find(" ");
    string lhs = line.substr(0, space);
    line.erase(line.begin(), line.begin()+(space+1));
    bindings[lhs] = rhs;
  }
}
Snes_Controller& Snes_Controller::a(){
    return press("A");
}

Snes_Controller& Snes_Controller::b(){
    return press("B");
}

Snes_Controller& Snes_Controller::x(){
    return press("X")
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

void Snes_Controller::execute(){
    press_keys(buffer);
    buffer.clear();
}

Snes_Controller& Snes_Controller::press(const string& stuff){
  buffer.push_back(bindings[stuff]);
  return *this;
}
