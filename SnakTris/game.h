#pragma once

#include <windows.h>
#include <vector>
#include <cstdlib>

#include "assets.h"
//#include "controls.h"

enum GameState { SNAKE, BRICK, PAUSE };

class Game {
public:
	int xOrigin = 0;
	int yOrigin = 0;
	int xSize = BOARDW;
	int ySize = BOARDH;
	int allowedBrickXoffset = 4; // Horizontal offset from the right side of board
	int allowedBrickYoffset = 1; // Vertical offset from the top of board

	int noOfAllowedBricks; // number of allowed bricks

	int score;
	bool gameOver = false;

	bool isSnake = true; // true if player is snake, false if player is brick

	int lastDirection;
	
	Board* board;
	Snake* snake;
	Food* food;
	Brick* brick;
	vector<Brick*> allowedBricks;

	void init(int xO = 0, int yO = 0, int xS = BOARDW, int yS = BOARDH, int allowBircks = 2);

	int collideSnakeBoard(int dir); // returns 1 is snake would collide with board in the given direciton, otherwise returns 0
	int collideBrickBoard(int dir); // return 1 if brick would collide with board on left or right, return 2 if it would collide on bottom, otherwise returns 0
	int snakeGotFood(); // returns 1 if head of snake overlaps food
	void placeFood(); // finds the best place to put food so it is not blocked by bricks (TODO: FIX)
	int checkAllowedBricks(); // returns the number of allowed bricks that matches the snake's shape, returns -1 if none matches
	void replaceAllowedBrick(int val); // replaces allowed brick on position [val] by a new random brick

	void input(int dir); // reacts on input from outside
	void nextFrame();  // draws next frame

	void drawScore(); // draws score
	void clearScore(); // hides score
	void draw(); // draws everything connected to the game
	void clear(); // hides --||--
	

	Game();
	~Game();

};