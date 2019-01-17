#pragma once
#include <vector>
#include <string>

#include "global.h"

//custom stuff
int clamp(int value, int min, int max);
int rollover(int value, int min, int max);
int longestLine(std::vector<std::string> lines);
void createbrick(bool destination[16], const bool source[16]);
int sgn(int val);

//