#ifndef BRAIN_H
#define BRAIN_H
#include <cstdint>
#include <unordered_map>

#include "pool.hpp"
#include "snes_controller.hpp"
#include "image_classifier.hpp"

using std::unordered_map;

struct Brain{
public:
	Brain();
	void play();
	vector<int32_t> demonize_blocks(const vector<int32_t>& block_classes);
	void send_signals(const vector<bool>& buttons);
private:
	Pool pool;
	Eye eye;
	Snes_Controller controller;
	Image_Classifier classifier;
	time_t previous;
	unordered_map<uint32_t,int32_t> memory;
};



#endif
