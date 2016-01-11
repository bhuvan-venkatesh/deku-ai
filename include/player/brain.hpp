#pragma once

#include <cstdint>
#include <unordered_map>

#include "pool.hpp"
#include "snes_controller.hpp"
#include "image_classifier.hpp"
#define timeout_constant 20;
using std::unordered_map;

struct Brain{
public:
	Brain();
	void initialize_run();
	void play();
	void send_signals(const vector<bool>& buttons);
	Snes_Controller controller;
private:
	Pool pool;
	Eye eye;
	Image_Classifier classifier;
	int rightmost;
	int timeout;
};
