#include "ConsoleGameEngine.h"
#include "Sample.h"

//initialize instance of ConsoleGameEngine
ConsoleGameEngine cge(game_loop);
 
//register keys and hold pointer to check changes
CGEKey* key_w = cge.key_registry.registerKey('W');
CGEKey* key_a = cge.key_registry.registerKey('A');
CGEKey* key_s = cge.key_registry.registerKey('S');
CGEKey* key_d = cge.key_registry.registerKey('D');

//enable the map (now it gets drawn automatically what is in the map) and hold pointer
CGEMap* map = cge.enableMap(100, 30);

bool moved = false;

int x = 10;
int y = 10;
int x_old = 10;
int y_old = 10;

char player = L'#';

int main() {
	//start the ConsoleGameEngine
	cge.start();
	//waits until ConsoleGameEngine::stop gets called

	return 0;
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

	if (x >= 0 && x < 100 && y >= 0 && y < 30) {
		//set char in map with position <x><y>
		map->setChar(x, y, player);
	}
	if (moved) {
		if (x_old >= 0 && x_old < 100 && y_old >= 0 && y_old < 30) {
			map->setChar(x_old, y_old, L'+');
		}
		x_old = x;
		y_old = y;
		moved = false;
	}
}
