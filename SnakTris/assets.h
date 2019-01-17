#pragma once
#include <windows.h>
#include <cstdlib>
#include <vector>
#include <string>

#include "global.h"

using std::vector;

enum BrickType { BRICK_O, BRICK_I, BRICK_L1, BRICK_L2, BRICK_S1, BRICK_S2, BRICK_SNAKE };

/*** Available TETRIS blocks (T-block is ommited) ***/
const bool brick_o[16] = { 0, 0, 0, 0, \
0, 1, 1, 0, \
0, 1, 1, 0, \
0, 0, 0, 0 };

const bool brick_i[16] = { 0, 1, 0, 0, \
0, 1, 0, 0, \
0, 1, 0, 0, \
0, 1, 0, 0 };

const bool brick_l1[16] = { 0, 1, 0, 0, \
0, 1, 0, 0, \
0, 1, 1, 0, \
0, 0, 0, 0 };

const bool brick_l2[16] = { 0, 0, 1, 0, \
0, 0, 1, 0, \
0, 1, 1, 0, \
0, 0, 0, 0 };

const bool brick_s1[16] = { 0, 1, 0, 0, \
0, 1, 1, 0, \
0, 0, 1, 0, \
0, 0, 0, 0 };

const bool brick_s2[16] = { 0, 0, 1, 0, \
0, 1, 1, 0, \
0, 1, 0, 0, \
0, 0, 0, 0 };

const bool brick_list[] = { brick_o, brick_i, brick_l1, brick_l2, brick_s1, brick_s2 };

class Asset {

	//Common items
public:
	int xPos = 0;
	int yPos = 0;
	int xOrigin = 0;
	int yOrigin = 0;

	bool visible = 0;
	virtual void draw() = 0;  // Draws object on screen
	virtual void clear() = 0; // Hides the drawn object, keeping all it's properties
};

class Rect : public Asset {
public:
	int xSize;
	int ySize;
	int color;

	void draw();
	void draw(int col);
	void draw(int x, int y, int w, int h, int col = COLOR_WHITE);
	void clear();
	Rect(int x, int y, int w, int h, int col = COLOR_WHITE);
	Rect() {};
};

class Menu : public Asset{

public:
	int xSize;
	vector<std::string> menuItems;

	int textColor = COLOR_WHITE;
	int selectorColor = COLOR_RED;

	bool visible;
	int selectedItem = 0;

	Menu(vector<std::string> items);
	void draw();
	void draw(unsigned x, unsigned y); // Sets the position without drawing
	void setPosition(int x, int y);
	void update(bool nextItem);	// Moves the cursor up(0) or down(1) and redraws it on screen
	void clear();
};


class Food : public Asset {
public:
	int color;

	void draw();
	void draw(int x, int y);
	void clear();
	Food(int x, int y, int xO = 0, int yO = 0, int col = COLOR_LIGHTRED);
};

class SnakePiece {
public:
	int x, y;
	SnakePiece* front = nullptr;

	SnakePiece(int x_, int y_);
};


class Brick; // Forward declaration of Brick needed for cross-reference with Snake

class Snake : public Asset{ // Snake is implemented as a reverse linked list
public:
	SnakePiece* tail;
	SnakePiece* head;

	int direction;
	int bodyColor;

	void draw();
	void clear();
	void move(); // moves the snake 1 unit forward, redraws moved piece
	void move(int dir); // moves the snake 1 unit in the direction specified, redraws moved piece

	int verifyBrick(const Brick* brick); // returns rotation of brick or -1 if failed
	Brick* makeBrick(); // return a pointer to a new brick with the current shape of snake

	Snake(int x, int y, int dir = 0, int xO = 0, int yO = 0, int col = COLOR_LIGHTGREEN);
	~Snake();
};

class Brick : public Asset{

public:
	int rotation;
	BrickType brickType;
	int color;
	
	bool brick[4 * 4] = { 0 }; // Array that holds the shape of brick

	void setPosition(int x, int y);
	void draw();
	void draw(int x, int y);
	void clear();
	void rotate(); // in-place clockwise rotation of the brick array by 90°
	void move(int dir); // moves the whole brick 1 unit in given direction, redraws if visible

	void replaceBrick(const bool source[16]); // replaces the brick array with the source array

	Brick(BrickType type, int xO = 0, int yO = 0, int col = COLOR_PINK);
	~Brick();
};

class Board : public Asset{

public:
	int xSize;
	int ySize;
	int* board; // Array that holds the shape and color of every block on the board, including walls

	int wallColor;

	void draw();
	void draw(int x, int y);
	void clear();
	void imprintBrick(Brick* brick); // copies brick to the board array
	int checkLines(); // returns the number of line that is complete, returns -1 if none
	void popLine(int line); // clears the given line moving all blocks above it down by 1, redraws board

	Board(int w, int h, int x = 0, int y = 0, int col = COLOR_GRAY);
};
