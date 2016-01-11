#pragma once

extern "C"{
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
}
#include <X11/Xlib.h>
#include <iostream>
#include <cstdlib>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "emulator_window.hpp"

using cv::Mat;

struct Eye{
public:
	Eye();
	cv::Mat analyze_screen();
	bool draw_keypoints;

private:
	Emulator_Window* window;

	cairo_surface_t* convert_xlib_to_image_surface(cairo_surface_t* x11_surf,const unsigned int& width, const unsigned int& height);
	void convert_image_surface_to_mat(cairo_surface_t* img_surf,const unsigned int& width, const unsigned int& height, Mat& ret);
};
