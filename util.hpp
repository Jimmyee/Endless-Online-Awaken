// Endless Online Awaken v0.0.1

#ifndef UTIL_HPP_INCLUDED
#define UTIL_HPP_INCLUDED

#include <math.h>

static inline int path_length(int x1, int y1, int x2, int y2)
{
	int dx = std::abs(x1 - x2);
	int dy = std::abs(y1 - y2);

	return dx + dy;
}

#endif // UTIL_HPP_INCLUDED
