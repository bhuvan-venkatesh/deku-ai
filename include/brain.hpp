#ifndef BRAIN_H
#define BRAIN_H
#include <cstdint>
#include <unordered_map>

#include "eye.hpp"
#include "pool.hpp"
#include "snes_controller.hpp"

using std::unordered_map;

struct Brain{
public:
	Brain();
	void play();
	void group();
	vector<int32_t> analyze_screen();
	void randomize_memory();
private:
	Pool pool;
	Eye eye;
	Snes_Controller controller;

	unordered_map<uint32_t,int32_t> memory;
	vector<Point2f> prev;
	int cluster_centers;
};



#endif
