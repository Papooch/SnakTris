#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

#include "console.h"
#include "global.h"
#include "functions.h"
#include "game.h"


using std::cout;
using std::endl;
using std::vector;

//****RECTANGLE*****/

Rect::Rect(int x, int y, int w, int h, int col) {
	xPos = x;
	yPos = y;
	xSize = w;
	ySize = h;
	color = col;
}

void Rect::draw() {
	//draw top and bottom
	setColor(color);
	for (int n = 0; n < 2; n++){
		for (int i = 1; i < xSize - 1; i++) {
			gotoxy(xPos + i, yPos + n*(ySize-1));
			cout << (char) 205;
		}
	}
	//draw left and right
	setColor(color);
	for (int n = 0; n < 2; n++) {
		for (int i = 1; i < ySize - 1; i++) {
			gotoxy(xPos + n*(xSize-1), yPos + i);
			cout << (char) 186;
		}
	}
	//draw top left
	gotoxy(xPos, yPos);
	cout << (char) 201;

	//draw top right
	gotoxy(xPos + xSize - 1, yPos);
	cout << (char) 187;

	//draw bottom left
	gotoxy(xPos, yPos + ySize - 1);
	cout << (char) 200;

	//draw bottom right
	gotoxy(xPos + xSize - 1, yPos + ySize - 1);
	cout << (char) 188;

}

void Rect::draw(int x, int y, int w, int h, int col) {
	xPos = x;
	yPos = y;
	xSize = w;
	ySize = h;
	color = col;
	draw();
}

void Rect::draw(int col) {
	color = col;
	draw();
}

void Rect::clear() {

	for (int i = 0; i < ySize; i++) {
		gotoxy(xPos, yPos + i);
		for (int u = 0; u < xSize; u++) {
			cout << " ";
		}
	}
}

/*****MENU******/

Menu::Menu(vector<std::string> items) {

	for (int i = 0; i < items.size(); i++) {
		menuItems.push_back(items[i]);
	}
	xSize = longestLine(items) + 2;

}


void Menu::draw() {
	if (visible) {
		clear();
	}
	visible = true;
	for (int i = 0; i < menuItems.size(); i++) {

		gotoxyColor(xPos, yPos + i, selectorColor);

		if (i == selectedItem) {
			cout << "> ";
		}
		else {
			cout << "  ";
		}

		setColor(textColor);
		cout << menuItems[i];
	}
}

void Menu::draw(unsigned x, unsigned y) {
	xPos = x;
	yPos = y;
	draw();
}



void Menu::update(bool nextItem) {
	if (!visible) {
		draw(xPos, yPos);
		return;
	}
	// clear last position of the cursor
	gotoxy(xPos, yPos + selectedItem);
	cout << " ";
	// sets and draws the new position of the cursor
	gotoxyColor(xPos, yPos + (selectedItem = clamp((selectedItem - 1 + nextItem * 2), 0, menuItems.size() - 1)), selectorColor);
	cout << "> ";

}

void Menu::setPosition(int x, int y) {
	xPos = x;
	yPos = y;
}

void Menu::clear() {
	if (!visible) return;
	visible = false;
	for (int i = 0; i < menuItems.size(); i++) {
		gotoxy(xPos, yPos + i);
		for (int u = 0; u < xSize; u++) {
			cout << " ";
		}
	}
}

/*****FOOD*****/

Food::Food(int x, int y, int xO, int yO, int col){
	xPos = x;
	yPos = y;
	xOrigin = xO;
	yOrigin = yO;
	color = col;
}

void Food::draw() {
	if (visible) {
		clear();
	}
	visible = true;
	gotoxyColor(xPos + xOrigin, yPos + yOrigin, color);
	cout << FOOD_SYMBOL;
}

void Food::draw(int x, int y) {
	xPos = x;
	yPos = y;
	draw();
}

void Food::clear() {
	visible = false;
	gotoxyColor(xPos + xOrigin, yPos + yOrigin, color);
	cout << " ";
}

/*****SNAKE_PIECE*****/

SnakePiece::SnakePiece(int x_, int y_) {
	x = x_;
	y = y_;
}

/*****SNAKE*****/

Snake::Snake(int x, int y, int dir, int xO, int yO, int color) {
	xOrigin = xO;
	yOrigin = yO;
	bodyColor = color;

	direction = dir;

	// Tail offset from the head
	// we are creating the head of the snake on a desired position with a desired direction,
	// so we need to find the position of it's tail
	int xoff = 0;
	int yoff = 0;

	switch (dir) {
	case 0:
		xoff = -3;
		break;
	case 1:
		yoff = -3;
		break;
	case 2:
		xoff = 3;
		break;
	case 3:
		yoff = 3;
		break;
	}

	//TODO: FIX
	// A dirty way of creating a snake of a fixed length
	tail = new SnakePiece(x + xoff, y + yoff);
	tail->front = new SnakePiece(x + xoff - sgn(xoff) * 1, y + yoff - sgn(yoff) * 1);
	tail->front->front = new SnakePiece(x + xoff - sgn(xoff) * 2, y + yoff - sgn(yoff) * 2);
	head = tail->front->front->front = new SnakePiece(x, y);

}

Snake::~Snake() {

	clear();

	SnakePiece* current;
	SnakePiece* next;

	current = tail;
	while (current != nullptr) {
		next = current->front;
		delete current;
		current = next;
	}

}

void Snake::draw() {
	visible = true;
	SnakePiece* snakePiece = tail;
	setColor(bodyColor);
	for (int i = 0; i < 4; i++) {
		if (snakePiece->y <= 0) return;
		gotoxy(snakePiece->x + xOrigin, snakePiece->y + yOrigin);
		cout << SNAKE_PIECE_SYMBOL;
		snakePiece = snakePiece->front;
	}
}
void Snake::clear() {
	visible = false;
	SnakePiece* snakePiece = tail;
	if (tail == nullptr) return;
	for (int i = 0; i < 4; i++) {
		gotoxy(snakePiece->x + xOrigin, snakePiece->y + yOrigin);
		cout << " ";
		snakePiece = snakePiece->front;
	}
}


void Snake::move(int dir) {
	if (!(dir == direction + 2 || dir == direction - 2)) {
		direction = dir;
	}
	move();
}

void Snake::move() {

	if (visible && tail->y > 0) {
			gotoxy(tail->x + xOrigin, tail->y + yOrigin);
			cout << " ";
	}

	head->front = tail;
	tail = tail->front;

	switch (direction) {
	case DIRECTION_RIGHT:
		head->front->x = head->x + 1;
		head->front->y = head->y;
		break;
	case DIRECTION_DOWN:
		head->front->x = head->x;
		head->front->y = head->y + 1;
		break;
	case DIRECTION_LEFT:
		head->front->x = head->x - 1;
		head->front->y = head->y;
		break;
	case DIRECTION_UP:
		head->front->x = head->x;
		head->front->y = head->y - 1;
		break;
	}

	head = head->front;
	head->front = nullptr;

	if (visible) {
		gotoxyColor(head->x + xOrigin, head->y + yOrigin, bodyColor);
		cout << SNAKE_PIECE_SYMBOL;
	}
}

int Snake::verifyBrick(const Brick* brick) {
	// minimal x and y coordinates of snake blocks
	int xMinS = 300;
	int yMinS = 300;

	//find xMin and yMin of snake
	SnakePiece* temp = tail;
	for (int i = 0; i < 4; i++) {
		if (temp->x < xMinS) xMinS = temp->x;
		if (temp->y < yMinS) yMinS = temp->y;
		temp = temp->front;
	}

	//Unnecessary commented code that I'm afraid to delete should I ever need it
	/*//Create testing array
	bool snakeArray[4 * 4] = { 0 };

	temp = tail;
	for (int i = 0; i < 4; i++) {
		snakeArray[temp->x - xMinS + 4 * (temp->y - yMinS)] = true;
		temp = temp->front;
	}*/
	
	int rotation = 0;
	bool failed = false;

	// minimal x and y coordinates of brick
	int xMinB;
	int yMinB;

	//test all 4 rotations of the brick
	for (int i = 0; i < 4; i++) {

		Brick testBrick(BRICK_SNAKE);
		testBrick.replaceBrick(brick->brick);

		testBrick.visible = false;

		//rotate the brick 0 to 3 times depending on the loop number
		for (int u = 0; u < i; u++) {
			testBrick.rotate();
		}

		//Find xMin and yMin of brick
		xMinB = 4;
		yMinB = 4;
		for (int i = 0; i < 4; i++) {
			for (int u = 0; u < 4; u++) {
				if (testBrick.brick[u + 4 * i] == 1) {
					if (u < xMinB) xMinB = u;
					if (i < yMinB) yMinB = i;
				}
			}
		}

		//shift values to top left array of the brick array
		for (int i = 0; i < 4; i++) {
			for (int u = 0; u < 4; u++) {
				if (u + xMinB >= 4 || i + yMinB >= 4) testBrick.brick[u + 4 * i] = 0;
				else testBrick.brick[u + 4 * i] = testBrick.brick[u + xMinB + 4 * (i + yMinB)];
			}
		}

		//match snake blocks coordinates to brick array (normalised by the xMin and yMin values)
		temp = tail;
		int tempval;
		for (int i = 0; i < 4; i++) {
			if ((testBrick.brick[tempval = (temp->x - xMinS + 4 * (temp->y - yMinS))] != 1)) {
				failed = true;
				break;
			}
			temp = temp->front;
		}

		//if succeeded, return rotation of block
		if (!failed) return i;

		//if failed try again
		else failed = false;
	}

	//if all tests fail, return -1;
	return -1;
}

Brick* Snake::makeBrick() {
	int xMinS = 300;
	int yMinS = 300;

	//create new brick with the snake's properties
	Brick* snakeBrick = new Brick(BRICK_SNAKE, xOrigin, yOrigin, bodyColor);

	//find xMin and yMin of snake
	SnakePiece* temp = tail;
	for (int i = 0; i < 4; i++) {
		if (temp->x < xMinS) xMinS = temp->x;
		if (temp->y < yMinS) yMinS = temp->y;
		temp = temp->front;
	}

	snakeBrick->setPosition(xMinS, yMinS);
	snakeBrick->xOrigin = xOrigin;
	snakeBrick->yOrigin = yOrigin;

	//Insert snake shape into the created brick
	temp = tail;
	for (int i = 0; i < 4; i++) {
		snakeBrick->brick[temp->x - xMinS + 4 * (temp->y - yMinS)] = true;
		temp = temp->front;
	}

	return snakeBrick;
}

/****BRICK****/

Brick::Brick(BrickType type, int xO, int yO, int col) {
	xOrigin = xO;
	yOrigin = yO;
	color = col;

	switch (type) {
	case BRICK_O:
		replaceBrick(brick_o);
		break;
	case BRICK_I:
		replaceBrick(brick_i);
		break;
	case BRICK_L1:
		replaceBrick(brick_l1);
		break;
	case BRICK_L2:
		replaceBrick(brick_l2);
		break;
	case BRICK_S1:
		replaceBrick(brick_s1);
		break;
	case BRICK_S2:
		replaceBrick(brick_s2);
		break;
	}

}


Brick::~Brick() {
	if (visible) {
		clear();
	}
}



void Brick::draw() {
	if (visible) {
		clear();
	}
	visible = true;
	setColor(color);
	for (int i = 0; i < 4; i++) {
		for (int u = 0; u < 4; u++) {
			gotoxy(xPos + u + xOrigin, yPos + i + yOrigin);
			if (brick[u + 4 * i] == true)
				cout << TETRIS_PIECE_SYMBOL;
		}
	}
}


void Brick::draw(int x, int y) {
	if (visible) {
		clear();
	}
	visible = true;
	xPos = x;
	yPos = y;
	setColor(color);
	for (int i = 0; i < 4; i++) {
		for (int u = 0; u < 4; u++) {
			gotoxy(xPos + u + xOrigin, yPos + i + yOrigin);
			if (brick[u + 4 * i] == true)
				cout << TETRIS_PIECE_SYMBOL;
		}
	}
}

void Brick::setPosition(int x, int y) {

	xPos = x;
	yPos = y;

}

void Brick::clear() {
	if (!visible) return;
	for (int i = 0; i < 4; i++) {
		for (int u = 0; u < 4; u++) {
			gotoxy(xPos + u + xOrigin, yPos + i + yOrigin);
			if (brick[u + 4 * i] == true)
				cout << " ";
		}
	}
	visible = false;
}

void Brick::rotate() {
	if (visible) {
		clear();
		visible = true;
	}

	// in-place rotation of the brick array
	for (int u = 0; u < 3; u++) {
		for (int i = u; i < 3 - u; i++) {

			bool temp = brick[u + 4 * i];

			brick[u + 4 * i] = brick[i + 4 * (3 - u)];
			brick[i + 4 * (3 - u)] = brick[3 - u + 4 * (3 - i)];
			brick[3 - u + 4 * (3 - i)] = brick[3 - i + 4 * u];
			brick[3 - i + 4 * u] = temp;
		}
	}
	if (visible) {
		draw();
	}
}

void Brick::move(int dir) {
	if (visible) {
		clear();
		visible = true;
	}
	switch (dir) {
	case DIRECTION_RIGHT:
		xPos += 1;
		break;
	case DIRECTION_LEFT:
		xPos -= 1;
		break;
	case DIRECTION_DOWN:
		yPos += 1;
		break;
	}
	if (visible) {
		draw();
	}

}


void Brick::replaceBrick(const bool source[16]) {
	for (int i = 0; i < 16; i++) {
		brick[i] = source[i];
	}
}

/*****BOARD*****/

Board::Board(int w, int h, int x, int y, int col) {
	xSize = w + 2;
	ySize = h + 2;
	xPos = x;
	yPos = y;
	wallColor = col;

	board = new int[xSize * ySize]();

	//make top & bottom walls
	for (int i = 0; i < xSize; i++) {
		board[i] = wallColor;
		board[i + xSize * (ySize - 1)] = wallColor;
	}

	//make left & right walls
	for (int i = 0; i < ySize; i++) {
		board[0 + xSize * i] = wallColor;
		board[(xSize - 1) + xSize * i] = wallColor;
	}
}


void Board::draw(int x, int y) {
	xPos = x;
	yPos = y;
	draw();
}

void Board::draw() {
	for (int i = 0; i < ySize; i++) {
		gotoxy(xPos, yPos + i);
		for (int u = 0; u < xSize; u++) {
			setColor(board[u + xSize * i]);
			if (i == 0 || i == ySize - 1 || u == 0 || u == xSize - 1) {
				cout << BOARD_EDGE_SYMBOL;
			}
			else {
				cout << TETRIS_PIECE_SYMBOL;
			}
			
		}
	}
}

void Board::clear() {
	for (int i = 0; i < ySize; i++) {
		gotoxy(xPos, yPos + i);
		for (int u = 0; u < xSize; u++) {

			cout << " ";
		}
	}

}



void Board::imprintBrick(Brick* brick) {


	for (int i = 0; i < 4; i++) {
		for (int u = 0; u < 4; u++) {
			if (brick->brick[u + 4 * i] == true){
				board[(brick->xPos + u) + xSize * (brick->yPos + i)] = brick->color;
				gotoxyColor(brick->xPos + u + xPos, brick->yPos + i + yPos, brick->color);
				cout << TETRIS_PIECE_SYMBOL;
			}
		}
	}
}


int Board::checkLines() {


	int lineNo = 0;

	for (int i = 1; i < ySize - 1; i++) {
		for (int u = 1; u < xSize - 1; u++) {
			if (board[u + xSize * i] == 0) {
				break;
			}
			else if (u == xSize - 2) {
				return i;
			}
		}
	}

	return lineNo;
}


void Board::popLine(int line) {
	if (line == 0) return;

	for (int i = line; i > 2 - 1; i--) {
		for (int u = 1; u < xSize - 1; u++) {
			board[u + xSize * i] = board[u + xSize * (i - 1)];
		}
	}
}