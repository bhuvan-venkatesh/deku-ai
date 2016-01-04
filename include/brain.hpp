#ifndef BRAIN_H
#define BRAIN_H
#include "pool.hpp"
#include <cstdint>
#include <unordered_map>
#include "eye.hpp"

using std::unordered_map;

struct Brain{
public:
	Brain(std::int32_t inputs, std::int32_t outputs): pool(Pool(inputs, outputs)){};
	void play();
private:
	Pool pool;
	Eye eye;
	unordered_map<uint32_t,int32_t> memory;
};



#endif
