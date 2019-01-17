#pragma once
#include <windows.h>
#include <vector>
#include <cstdlib>

using std::vector;

enum Button {BUTTON_UP, BUTTON_RIGHT, BUTTON_DOWN, BUTTON_LEFT, BUTTON_ENTER, BUTTON_ESCAPE, BUTTON_SPACE, BUTTON_NONE};

class Controls {
private:
	Button lastButton;
	bool read;
	vector<int> buttonConstants = { VK_UP, VK_RIGHT, VK_DOWN, VK_LEFT, VK_RETURN, VK_ESCAPE, VK_SPACE };

public:
	Button get(); // returns the last button pressed, then sets it to BUTTON_NONE
	void scan();  // sets lastButton to the last pressed button
};