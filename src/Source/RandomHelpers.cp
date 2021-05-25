#include "RandomHelpers.h"
#include <stdlib.h>

using namespace RandomHelpers;

SInt32
RandomHelpers::RandomRange(SInt32 min, SInt32 max) {
	SInt32 random = (rand() * (max - min + 1) / (RAND_MAX + 1)) + min;
	
	if(random < min || random > max) {
		Throw_(-1);
	}
	
	return random;
}