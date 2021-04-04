#include "ConsoleGameEngine.h"
#include "Source.h"

ConsoleGameEngine cge(game_loop);
std::wostringstream * out = cge.getOut();
CGEKey* key_N = cge.key_registry.registerKey('N');
CGEKey* key_M = cge.key_registry.registerKey('M');

int main() {

	cge.start();

	return 0;
}

void game_loop(double time_passed) {
	*out << key_N->isDown() <<  L" : " << key_N->gotPressed() << L" ";
	*out << key_M->isDown() <<  L" : " << key_M->gotPressed() << L" ";
}
