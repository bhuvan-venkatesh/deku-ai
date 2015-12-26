#ifndef BRAIN_H
#define BRAIN_H
#include "pool.h"
#include <cstdint>

struct Brain{
public:
	Brain(std::uint32_t inputs, std::uint32_t outputs): pool(Pool(inputs, outputs)){};
private:
	Pool pool;
};



#endif