#include "ConsoleGameEngine.h"
#include "Sample.h"

//#define SAMPLE_ONE
#define SAMPLE_TWO

//initialize instance of ConsoleGameEngine
ConsoleGameEngine cge(game_loop);
 
//register keys and hold pointer to check changes
CGEKey* key_w = cge.key_registry->registerKey('w');
CGEKey* key_a = cge.key_registry->registerKey('a');
CGEKey* key_s = cge.key_registry->registerKey('s');
CGEKey* key_d = cge.key_registry->registerKey('d');

bool moved = false;

int x = 5;
int y = 5;
int x_old = 5;
int y_old = 5;

#ifdef SAMPLE_ONE

CGECharMap* map;
char player = L'#';

int main() {
	map = cge.init_CharMap(30, 10);

	//start the ConsoleGameEngine
	return cge.start(); //runs until ConsoleGameEngine::stop gets called
}

//game_loop for game mechanics called every time before draws
void game_loop(double time_passed) {
	//check if key gotPressed
	if (key_w->gotPressed()) {
		y--;
		moved = true;
	}
	if (key_a->gotPressed()) {
		x--;
		moved = true;
	}
	if (key_s->gotPressed()) {
		y++;
		moved = true;
	}
	if (key_d->gotPressed()) {
		x++;
		moved = true;
	}

	if (x >= 0 && x < 30 && y >= 0 && y < 10) {
		//set char in map at position <x><y>
		map->setChar(x, y, player);
		map->setForegroundColor(x, y, TF_COLOR_FOREGROUND_CYAN);
	}
	if (moved) {
		if (x_old >= 0 && x_old < 30 && y_old >= 0 && y_old < 10) {
			map->setChar(x_old, y_old, L'+');
			map->setForegroundColor(x_old, y_old, TF_COLOR_FOREGROUND_GREEN);
		}
		x_old = x;
		y_old = y;
		moved = false;
	}
}
#endif

#ifdef SAMPLE_TWO

CGEPixelMap* map;

int main() {
	cge.setFontSize(1, 1);
	map = cge.init_PixelMap(20, 20, 3, 1);

	//start the ConsoleGameEngine
	return cge.start(); //runs until ConsoleGameEngine::stop gets called
}

//game_loop for game mechanics called every time before draws
void game_loop(double time_passed) {
	//check if key gotPressed
	if (key_w->gotPressed()) {
		y--;
		moved = true;
	}
	if (key_a->gotPressed()) {
		x--;
		moved = true;
	}
	if (key_s->gotPressed()) {
		y++;
		moved = true;
	}
	if (key_d->gotPressed()) {
		x++;
		moved = true;
	}

	if (x >= 0 && x < 20 && y >= 0 && y < 20) {
		//set pixel colors in map at position <x><y>
		map->setPixel(x, y, 200, 200, 100);
	}
	if (moved) {
		if (x_old >= 0 && x_old < 20 && y_old >= 0 && y_old < 20) {
			map->setPixel(x_old, y_old, 40, 40, 40);
		}
		x_old = x;
		y_old = y;
		moved = false;
	}
}
#endif
