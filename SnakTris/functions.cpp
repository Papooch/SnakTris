#include <windows.h>
#include <vector>
#include <string>

#include "global.h"

// clamps value inside bounds
int clamp(int value, int min, int max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

// if value exceeds lower returns the higher bound and vice versa
int rollover(int value, int min, int max) {
	if (value < min) return max;
	if (value > max) return min;
	return value;
}

// rerutns the length of a longest string in a vector of strings
int longestLine(std::vector<std::string> lines) {
	int max = 0;
	for (auto& line : lines) {
		if (line.length() > max) {
			max = line.length();
		}
	}
	return max;
}

// returns the signum of a number
int sgn(int val) {
	return (0 < val) - (val < 0);
}