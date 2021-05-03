#include "ConsoleGameEngine.h"
#include "Sample.h"

//initialize instance of ConsoleGameEngine
ConsoleGameEngine cge(game_loop);
 
//register keys and hold pointer to check changes
CGEKey* key_w = cge.key_registry.registerKey('W');
CGEKey* key_a = cge.key_registry.registerKey('A');
CGEKey* key_s = cge.key_registry.registerKey('S');
CGEKey* key_d = cge.key_registry.registerKey('D');

//init and get the pointer to the map
CGEMap* map = cge.init(30, 10);

bool moved = false;

int x = 10;
int y = 5;
int x_old = 10;
int y_old = 5;

char player = L'#';

int main() {
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
		//set char in map with position <x><y>
		map->setChar(x, y, player);
		map->setForegroudColor(x, y, TF_COLOR_FOREGROUND_CYAN);
	}
	if (moved) {
		if (x_old >= 0 && x_old < 30 && y_old >= 0 && y_old < 10) {
			map->setChar(x_old, y_old, L'+');
			map->setForegroudColor(x_old, y_old, TF_COLOR_FOREGROUND_GREEN);
		}
		x_old = x;
		y_old = y;
		moved = false;
	}
}
