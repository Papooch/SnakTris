#include <iostream>
#include <cstdio>

#include <time.h>
#include <chrono>

#include "global.h"
#include "console.h"
#include "functions.h"
#include "assets.h"
#include "controls.h"
#include "game.h"
#include "logic.h"


#define NOW chrono::high_resolution_clock::now()

using namespace std;

enum GlobalState { gMENU_INIT, gMENU, gGAME_INIT, gGAME, gPAUSE_INIT, gPAUSE, gSETTINGS_INIT, gSETTINGS, gABOUT_INIT, gABOUT, gGAMEOVER_INIT, gGAMEOVER };

struct Settings {
	int speed;
	int allowedBricks;
	int height;
	int width;
};


void fixConsole();
void printLogo();
void clearLogo();

int main(int argc, char* argv[]) {

	//prepare the console
	{
		system("cls");
		fontSize(FONTSIZE);
		resizeConsole(800, 600);
		showConsoleCursor(false);
	}

	srand(time(NULL));

	Menu mainMenu({ "New Game", "Settings", "About", "Exit" });
	Menu pauseMenu({ "Continue", "Back to menu", "Exit" });

	mainMenu.setPosition((WINDOWW - mainMenu.xSize - 2) / 2, (WINDOWH - 4) / 2);
	pauseMenu.setPosition((WINDOWW - pauseMenu.xSize - 2) / 2, (WINDOWH - 3) / 2);

	Rect menuRect = Rect(mainMenu.xPos - 1, mainMenu.yPos - 1, mainMenu.xSize + 4, 6);
	Rect pauseRect = Rect(pauseMenu.xPos - 1, pauseMenu.yPos - 1, pauseMenu.xSize + 4, 5);
	Rect gameOverRect = Rect((WINDOWW - 9) / 2 - 2, (WINDOWH - 4) / 2 - 2, 13, 6);
	Rect aboutRect = Rect(4, 11, 34, 16);
	Rect settingsRect = Rect(5, 11, 32, 11);
	int x, y; // temporay variables for various positioning


	Controls controls;
	//  settings: frames per second, allowed bricks, game height, game width
	Settings settings = { 4, 2, BOARDH, BOARDW };

	//Extract settings from the command line
	if (argc > 1) {
		settings.speed = atoi(argv[1]);
	}
	if (argc > 2) {
		settings.allowedBricks = atoi(argv[2]);
	}
	if (argc > 3) {
		settings.height = atoi(argv[3]);
	}
	if (argc > 4) {
		settings.width = atoi(argv[4]);
	}



	Game* game = new Game();

	int finalScore = 0;
	
	GlobalState gameState = gMENU_INIT;
	
	Button lastButton;
	
	auto startTime = NOW;

	bool exitGame = false;


//-----------------MAIN Control loop----------------------//
	do {
		controls.scan();
		lastButton = controls.get();

		switch (gameState) {

	/*****MAIN MENU****/
		case gMENU_INIT:
			printLogo();

			mainMenu.selectedItem = 0;
			mainMenu.draw();
			menuRect.draw(rand() % 15 + 1);
			gameState = gMENU;
			break;
		case gMENU:

			switch (lastButton) {
			case BUTTON_RIGHT:
				break;
			case BUTTON_DOWN:
				mainMenu.update(MENU_NEXT);
				break;
			case BUTTON_LEFT:
				break;
			case BUTTON_UP:
				mainMenu.update(MENU_PREV);
				break;
			case BUTTON_ENTER:
				menuRect.clear();

				switch (mainMenu.selectedItem) {
				case 0:
					clearLogo();
					gameState = gGAME_INIT;
					break;
				case 1:
					gameState = gSETTINGS_INIT;
					break;
				case 2:
					gameState = gABOUT_INIT;
					break;
				case 3:
					exitGame = true;
					break;
				}
				break;
			case BUTTON_SPACE:
				break;
			case BUTTON_ESCAPE:
				exitGame = true;
				break;
			}

			break;

	/*****GAME****/
		case gGAME_INIT:
			delete game;
			game = new Game();
			game->init((WINDOWW - (10+settings.width))/2,\
						(WINDOWH -(settings.height))/2, \
				settings.width, settings.height, settings.allowedBricks);
			game->draw();

			finalScore = 0;
			startTime = NOW;

			gameState = gGAME;
			break;

		case gGAME:			

			switch (lastButton) {
			case BUTTON_RIGHT:
				game->input(DIRECTION_RIGHT);
				break;
			case BUTTON_DOWN:
				game->input(DIRECTION_DOWN);
				break;
			case BUTTON_LEFT:
				game->input(DIRECTION_LEFT);
				break;
			case BUTTON_UP:
				game->input(DIRECTION_UP);
				break;
			case BUTTON_ENTER:
				break;
			case BUTTON_SPACE:
				break;
			case BUTTON_ESCAPE:
				gameState = gPAUSE_INIT;
				break;
			}

			if ((NOW - startTime) >= chrono::milliseconds(1000/settings.speed) ) {
				startTime = NOW;
				game->nextFrame();
			}


			if (game->gameOver) {
				finalScore = game->score;
				gameState = gGAMEOVER_INIT;
			}

			break;

	/*****PAUSE****/
		case gPAUSE_INIT:

			pauseRect.clear();
			pauseRect.draw(rand() % 15 + 1);
			pauseMenu.draw();
			gameState = gPAUSE;
			break;
		case gPAUSE:

			switch (lastButton)
			{
			case BUTTON_UP:
				pauseMenu.update(MENU_PREV);
				break;
			case BUTTON_RIGHT:
				break;
			case BUTTON_DOWN:
				pauseMenu.update(MENU_NEXT);
				break;
			case BUTTON_LEFT:
				break;
			case BUTTON_ENTER:
				pauseRect.clear();

				switch (pauseMenu.selectedItem){
				case 0:					
					game->draw();
					gameState = gGAME;
					startTime = NOW;
					break;
				case 1:
					game->clear();
					gameState = gMENU_INIT;
					break;
				case 2:
					exitGame = true;
					break;
				}
				break;
			case BUTTON_ESCAPE:
				break;
			case BUTTON_SPACE:
				break;
			case BUTTON_NONE:
				break;
			default:
				break;
			}

			break;

	/*****SETTINGS****/
		case gSETTINGS_INIT:
			settingsRect.draw(rand() % 15 + 1);
			x = 6;
			y = 12;
			gotoxyColor(x, y, COLOR_WHITE);
			cout << "Settings have not yet been";
			gotoxy(x, y + 1);
			cout << "implemented :(";
			gotoxy(x, y + 3);
			cout << "You can change the game";
			gotoxy(x, y + 4);
			cout << "settings by calling it from";
			gotoxy(x, y + 5);
			cout << "the command line as \"SnakTris";
			gotoxy(x, y + 6);
			cout << "<speed> <allowedbricks>";
			gotoxy(x, y + 7);
			cout << "<board_height> <board_width>\".";
			gotoxy(x, y + 8);
			cout << "All parameters are optional.";

			gameState = gSETTINGS;
			break;
		case gSETTINGS:

			if (lastButton == BUTTON_ESCAPE || lastButton == BUTTON_ENTER) {
				settingsRect.clear();
				gameState = gMENU_INIT;
			}
			break;

	/*****ABOUT****/
		case gABOUT_INIT:

			aboutRect.draw(rand() % 15 + 1);
			x = 5;
			y = 12;
			gotoxyColor(x, y, COLOR_WHITE);

			cout << "Made by Ondrej Svanda, 2019,";
			gotoxy(x, y + 1);
			cout << "as a school project at the";
			gotoxy(x, y + 2);
			cout << "Brno University of Technology.";
			gotoxy(x, y + 4);
			cout << "Play as a snake until you eat";
			gotoxy(x, y + 5);
			cout << "food and turn into a Tetris";
			gotoxy(x, y + 6);
			cout << "brick. You can create any of the";
			gotoxy(x, y + 7);
			cout << "allowed bricks in any rotation.";
			gotoxy(x, y + 8);
			cout << "If you fail to do so, you will";
			gotoxy(x, y + 9);
			cout << "fall down instantly.";
			gotoxy(x, y + 10);
			cout << "Once the snake rams into a wall";
			gotoxy(x, y + 11);
			cout << "or a brick, the game is over.";

			gotoxy(x, y + 13);
			cout << "Controls: ^,>,<,v,Enter,Esc";

			gameState = gABOUT;
			break;
		case gABOUT:
			
			if (lastButton == BUTTON_ESCAPE || lastButton == BUTTON_ENTER) {
				aboutRect.clear();
				gameState = gMENU_INIT;
			}
			break;
	/*****GAMEOVER****/
		case gGAMEOVER_INIT:

			gameOverRect.clear();
			gameOverRect.draw(rand() % 15 + 1);

			gotoxyColor((WINDOWW - 9) / 2, (WINDOWH - 4) / 2, COLOR_WHITE);
			cout << "GAME OVER";
			gotoxy((WINDOWW - 9) / 2, (WINDOWH - 4) / 2 + 1);
			cout << "Score: " << finalScore;
			
			gameState = gGAMEOVER;
			break;
		case gGAMEOVER:
			if (lastButton == BUTTON_ESCAPE || lastButton == BUTTON_ENTER) {
				game->clear();
				gameOverRect.clear();
				gameState = gMENU_INIT;
			}

		}

		if (lastButton == BUTTON_SPACE) {
			fixConsole();
		}

	}while ( exitGame != true );

	system("cls");
	gotoxy(0, 0);
	resetColor();
	showConsoleCursor(true);

	return 0;
}

void fixConsole() {
	fontSize(FONTSIZE);
	showConsoleCursor(false);
}

void printLogo() {
	int x = 7;
	int y = 5;
	gotoxyColor(x, y, rand() % 7 + 9);
	cout << "\xdb\xdb\xdb         \xdb  \xdb\xdb\xdb\xdb\xdb       ";
	gotoxy(x, y+1);
	cout << "\xdb           \xdb    \xdb     \xdb \xdb\xdb";
	gotoxy(x, y+2);
	cout << "\xdb\xdb\xdb \xdb\xdb   \xdb\xdb \xdb \xdb  \xdb  \xdb\xdb   \xdb ";
	gotoxy(x, y+3);
	cout << "  \xdb \xdb \xdb \xdb \xdb \xdb\xdb   \xdb  \xdb  \xdb  \xdb";
	gotoxy(x, y+4);
	cout << "\xdb\xdb\xdb \xdb \xdb \xdb\xdb\xdb \xdb \xdb  \xdb  \xdb  \xdb \xdb\xdb";

}
void clearLogo() {
	Rect temp = Rect(7, 5, 27, 5);
	temp.clear();
}