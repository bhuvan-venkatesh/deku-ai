#ifndef SNES_CONTROLLER_H
#define SNES_CONTROLLER_H
#include <unordered_map>
#include "key_press.hpp"
#define buffer_size 16

class Snes_Controller{
public:
  snes_controller(const std::unordered_map<string, string>& bindings_);
  snes_controller();
  snes_controller& a();
  snes_controller& b();
  snes_controller& x();
  snes_controller& y();

  snes_controller& up();
  snes_controller& down();
  snes_controller& right();
  snes_controller& left();

  snes_controller& right_trigger();
  snes_controller& left_trigger();

  snes_controller& start();
  snes_controller& select();

  void execute();

private:
  /*
    {
    "a":"z" //In the Emulator_Window
    "b":"x" //All commands are uppercase
  }
  */
  std::unordered_map<string, const char*> bindings;
  vector<string> buffer;
  snes_controller& press(const string& stuff);
};

#endif
