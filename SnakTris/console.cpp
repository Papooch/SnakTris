#include <windows.h>
#include <vector>
#include <string>

#include "global.h"

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x >= 0 ? x : 0;
	coord.Y = y >= 0 ? y : 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void showConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

void fontSize(int a) {

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = a;
	cfi.dwFontSize.Y = a;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

}

void resizeConsole(int x, int y) {
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, x, y, TRUE); // 800 width, 100 height
}

void setColor(int text, int bg) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	int col = text + 14 * bg;

	FlushConsoleInputBuffer(out);
	SetConsoleTextAttribute(out, col);
}

void resetColor() {
	setColor(COLOR_WHITE, 0);
}

void gotoxyColor(int x, int y, int text, int bg) {
	gotoxy(x, y);
	setColor(text, bg);
}