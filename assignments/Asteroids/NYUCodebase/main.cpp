#include "game.h"

int main(int argc, char *argv[])
{
	Game app;
	while (!app.UpdateAndRender()) {}
	return 0;
}