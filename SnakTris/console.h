#pragma once
#include <vector>
#include <string>

#include "global.h"

//windows console stuff
void gotoxy(int x, int y); // sets the console cursor position to x and y
void showConsoleCursor(bool showFlag); // shows and hides blinking console cursor
void fontSize(int a); // sets the console font size
void resizeConsole(int x, int y); // sets the console window dimensions
void setColor(int text, int bg = COLOR_BLACK); // sets the console text coloe
void resetColor(); // resets the console text color to original
void gotoxyColor(int x, int y, int text = COLOR_WHITE, int bg = COLOR_BLACK); // sets the console cursor position and text color