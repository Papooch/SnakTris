
#include <windows.h>
#include <vector>
#include <cstdlib>

////
#include "console.h"
#include <iostream>
////

#include "functions.h"

#include "assets.h"
#include "controls.h"

#include "global.h"
#include "game.h"

Game::Game() {
	lastDirection = DIRECTION_DOWN;
	score = 0;
}

void Game::init(int xO, int yO, int xS, int yS, int allowBricks) {

	xOrigin = xO + 2;
	yOrigin = yO;
	xSize = xS;
	ySize = yS;
	noOfAllowedBricks = allowBricks;

	isSnake = true;

	board = new Board(xSize, ySize, xOrigin, yOrigin, COLOR_LIGHTWHITE);
	snake = new Snake(rand() % (xSize - 4) + 4, 0, 1, xOrigin, yOrigin, rand() % 15 + 1);
	food = new Food(xSize / 2, 5, xOrigin, yOrigin);

	int randBrick = rand() % 6;
	for (int i = 0; i < noOfAllowedBricks; i++) {
		Brick* temp = new Brick((BrickType)((randBrick + i) < 6 ? randBrick + i : randBrick + i - 6), xOrigin, yOrigin, rand() % 15 + 1);
		temp->visible = false;
		allowedBricks.push_back(temp);
	}

}

Game::~Game() {
	if (isSnake) {
		delete snake;
		delete food;
	}
	else {
		delete brick;
	}
	delete board;
	
	for (int i = 0; i < noOfAllowedBricks; i++) {
		delete allowedBricks[i];
	}
}

int Game::collideSnakeBoard(int dir) {
	switch (dir) {
	case DIRECTION_RIGHT:
		if (board->board[snake->head->x + 1 + board->xSize*(snake->head->y)] > 0) return 1;
		break;
	case DIRECTION_DOWN:
		if (board->board[snake->head->x + board->xSize*(snake->head->y + 1)] > 0) return 1;
		break;
	case DIRECTION_LEFT:
		if (board->board[snake->head->x - 1 + board->xSize * (snake->head->y)] > 0) return 1;
		break;
	case DIRECTION_UP:
		if (board->board[snake->head->x + board->xSize* (snake->head->y - 1)] > 0) return 1;
		break;
	}
	return 0;
}


int Game::collideBrickBoard(int dir) {
	switch (dir) {
	case DIRECTION_RIGHT:
		for (int i = 0; i < 4; i++) {
			for (int u = 0; u < 4; u++) {
				if (brick->brick[u + 4 * i] == true && board->board[(brick->xPos + u + 1) + board->xSize * (brick->yPos + i)] > 0) {
					return 1;
				}
			}
		}
		break;
	case DIRECTION_LEFT:
		for (int i = 0; i < 4; i++) {
			for (int u = 0; u < 4; u++) {
				if (brick->brick[u + 4 * i] == true && board->board[(brick->xPos + u - 1) + board->xSize * (brick->yPos + i)] > 0) {
					return 1;
				}
			}
		}
		break;
	case DIRECTION_DOWN:
		for (int i = 0; i < 4; i++) {
			for (int u = 0; u < 4; u++) {
				if (brick->brick[u + 4 * i] == true && board->board[(brick->xPos + u) + board->xSize * (brick->yPos + i + 1)] > 0) {
					return 2;
				}
			}
		}
		break;
	}
	return 0;

}

int Game::snakeGotFood() {
	return (food->xPos == snake->head->x && food->yPos == snake->head->y);
}

void Game::placeFood() {
	food->xPos = rand() % (xSize - 1) + 1;
	food->yPos = rand() % ((ySize - 4) / 3) + 4;
	
	while (food->yPos >= 1) {
		for (int i = 1; i < board->xSize - 1; i++) {
			if (board->board[food->xPos + board->xSize*food->yPos] != 0) {
				food->xPos = rollover(food->xPos + 1, 1, board->xSize - 1);
			}
			else {
				return;
			}
		}
		food->yPos -= 1;
	}

}

int Game::checkAllowedBricks() {
		for (int i = 0; i < noOfAllowedBricks; i++) {
			if (int isOk = (snake->verifyBrick(allowedBricks[i]) != -1)) {
				return i;
			}
	}
	return -1;
}

void Game::replaceAllowedBrick(int val) {
	allowedBricks[val]->clear();
	delete allowedBricks[val];
	allowedBricks[val] = new Brick((BrickType)(rand()%6), xOrigin, yOrigin, rand() % 15 + 1);
	allowedBricks[val]->visible = false;
	allowedBricks[val]->draw(xSize + allowedBrickXoffset, allowedBrickYoffset + 5 * val);
}

void Game::input(int dir) {
	lastDirection = dir;
	if (!isSnake) {
		if (lastDirection == DIRECTION_LEFT || lastDirection == DIRECTION_RIGHT || lastDirection == DIRECTION_DOWN) {
			nextFrame();
		}
		lastDirection = DIRECTION_DOWN;
	}
}


void Game::nextFrame() {

	if (isSnake) { //The player is snake
		if (collideSnakeBoard(lastDirection) == false) {
			snake->move(lastDirection);
		}
		else {
			gameOver = true;
			return;
		}

		if (snakeGotFood()) {
			delete brick;
			
			brick = snake->makeBrick();
			lastDirection = DIRECTION_DOWN;

			int check = checkAllowedBricks();
			if (check == -1) {
				delete snake;
				brick->draw();
				while (!collideBrickBoard(DIRECTION_DOWN)) {
					brick->move(DIRECTION_DOWN);
				}
				isSnake = false;
				input(DIRECTION_DOWN);
			}
			else {
				replaceAllowedBrick(check);
				delete snake;
				brick->draw();
				isSnake = false;
			}
		}
	}
	else { //The player is brick
		int brickCollides = collideBrickBoard(lastDirection);
		if (brickCollides == 0) {
			brick->move(lastDirection);
		}
		else if (brickCollides == 2) {
			brick->clear();
			board->imprintBrick(brick);

			int line;
			while(line = board->checkLines()){
				score += 1;
				board->popLine(line);
				board->draw();
				drawScore();
			}
			
			snake = new Snake(rand()% (xSize - 2) + 2, 0, 1, xOrigin, yOrigin, rand()% 15 + 1);
			isSnake = true;
			snake->draw();
			placeFood();
			food->draw();
			nextFrame();
		}
	}

}


void Game::drawScore() {
	gotoxyColor(xOrigin + 2, yOrigin - 2, COLOR_WHITE);
	std::cout << "Score: " << score;
}

void Game::clearScore() {
	gotoxy(xOrigin + 2, yOrigin - 2);
	std::cout << "           " << score;
}


void Game::draw() {
	board->draw();

	drawScore();

	gotoxyColor(xOrigin + xSize + 3, yOrigin - 2, COLOR_WHITE);
	std::cout << "Allowed";
	gotoxy(xOrigin + xSize + 3, yOrigin - 1);
	std::cout << "bricks:";

	for (int i = 0; i < noOfAllowedBricks; i++) {
		allowedBricks[i]->draw(xSize + allowedBrickXoffset, allowedBrickYoffset + 5 * i);
	}

	if (isSnake) {
		snake->draw();
		food->draw();
	}
	else {
		brick->draw();
	}
	
}

void Game::clear() {
	
	if (isSnake) {
		snake->clear();
		food->clear();
	}
	else {
		brick->clear();
	}

	for (int i = 0; i < noOfAllowedBricks; i++) {
		allowedBricks[i]->clear();
	}

	board->clear();
	clearScore();

	gotoxy(xOrigin + xSize + 3, yOrigin - 2);
	std::cout << "       ";
	gotoxy(xOrigin + xSize + 3, yOrigin - 1);
	std::cout << "       ";

}