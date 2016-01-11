#ifndef EMULATOR_WINDOW_H
#define EMULATOR_WINDOW_H

extern "C" {
#include <xdo.h>
}
extern "C"{
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
}
#include <X11/Xlib.h>
#include <memory>
#include <iostream>

struct Emulator_Window{
public:

  Display *disp;
	Window root;
	Screen* screen;
	unsigned int width;
	unsigned int height;
	int scr;
  xdo_t* xdo;
  char slop[4];

  static Emulator_Window* get_emulator();
  virtual ~Emulator_Window();
private:

  void get_window(const char* name);
  static Emulator_Window* window;
  Emulator_Window();
};
#endif
