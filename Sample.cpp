#include "ConsoleGameEngine.h"
#include "Sample.h"

//#define SAMPLE_ONE
#define SAMPLE_TWO

//pointer to ConsoleGameEngine reference
ConsoleGameEngine* cge = &ConsoleGameEngine::GetInstance();
 
//register keys and hold pointer to check changes
CGEKey* key_w = cge->getKeyRegistry().registerKey(CGE_KEY_W);
CGEKey* key_a = cge->getKeyRegistry().registerKey(CGE_KEY_A);
CGEKey* key_s = cge->getKeyRegistry().registerKey(CGE_KEY_S);
CGEKey* key_d = cge->getKeyRegistry().registerKey(CGE_KEY_D);

bool moved = false;

int x = 5;
int y = 5;
int x_old = 5;
int y_old = 5;

CGEPixelMap* map;

int main() {
	//init ConsoleGameEngine has to be called first
	cge->init(&game_loop);

	//set map and get pointer to it
	map = cge->setPixelMap(20, 20, 2, 1);

	//start the ConsoleGameEngine
	return cge->start(); //runs until ConsoleGameEngine::stop gets called
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