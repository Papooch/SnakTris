#pragma once
#include <windows.h>
#include <vector>
#include <cstdlib>

#include "controls.h"


/*****CONTROLS*****/

Button Controls::get() {
	if (read == false) {
		read = true;
		return lastButton;
	}
	else return BUTTON_NONE;
}


void Controls::scan() {
	unsigned int i = 0;
	Button newButton = BUTTON_NONE;
	for (i = 0; i < buttonConstants.size(); i++) {

		if (GetAsyncKeyState(buttonConstants[i])) {
			newButton = (Button) i ;
		}
	}
	if (newButton != lastButton) {
		lastButton = newButton;
		read = false;
	}
	//cout << lastButton;
}
