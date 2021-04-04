#include "ConsoleGameEngine.h"
#include "Source.h"

//ConsoleGameEngine cge(game_loop);
//std::wostringstream * out = cge.getOut();
int i = 10000;

int main() {
	//cge.run();

	while (true) {
		std::wcout << GetKeyState('A');
	}

	return 0;
}

void game_loop(double time_passed) {
	//*out << i-- << L"time: " << time_passed;
}
