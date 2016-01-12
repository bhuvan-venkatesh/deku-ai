#include "emulator_window.hpp"
extern "C" {
#include <xdo.h>
}
#define emulator_name "ZSNES"
#define screen_default NULL
#include <cstring>

Emulator_Window* Emulator_Window::window = NULL;

Emulator_Window::Emulator_Window(){
    disp = XOpenDisplay(screen_default);
    scr = DefaultScreen(disp);
    get_window(emulator_name);
}

void Emulator_Window::get_window(const char* name){
    xdo = xdo_new(screen_default);
    if(!xdo){
        std::cerr<<"Xdo Allocation Failed"<<std::endl;
        throw; //Okay in constructor
    }
    xdo_search_t search;
    memset(&search, 0, sizeof(xdo_search_t));

    search.max_depth = -1;
    search.require = xdo_search::SEARCH_ANY;
    search.searchmask |= SEARCH_NAME;
    search.winname = name;

    Window *ret;
    unsigned int len;
    xdo_search_windows(xdo, &search, &ret, &len );
    if(len != 1){
        std::cerr<<"More than one match, be more specific"<<std::endl;
        throw;
    }
    //Assignment happens at the end to provide strong exception guarentee
    xdo_get_window_size(xdo, ret[0], &width, &height);
    root = ret[0];
}

Emulator_Window* Emulator_Window::get_emulator(){
    if(! window ){
        window = new Emulator_Window();
    }
    return window;
}

Emulator_Window::~Emulator_Window(){
    if(xdo)
      xdo_free(xdo);
}
