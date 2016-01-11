#pragma once

#include <unordered_map>
#include "key_press.hpp"
#define buffer_size 16

class Snes_Controller{
public:
  Snes_Controller(const std::unordered_map<string, string>& bindings_);
  Snes_Controller();
  Snes_Controller& a();
  Snes_Controller& b();
  Snes_Controller& x();
  Snes_Controller& y();

  Snes_Controller& up();
  Snes_Controller& down();
  Snes_Controller& right();
  Snes_Controller& left();

  Snes_Controller& right_trigger();
  Snes_Controller& left_trigger();

  Snes_Controller& start();
  Snes_Controller& select();
  void load_state();

  void execute();

private:
  /*
    {
    "a":"z" //In the Emulator_Window
    "b":"x" //All commands are uppercase
  }
  */
  std::unordered_map<string, string> bindings;
  vector<string> buffer;
  Snes_Controller& press(const string& stuff);
};
