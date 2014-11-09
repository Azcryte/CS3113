/*
	AnQi Liu
	up, down, left, right to move
	demonstration of collision with matrices
*/

#include "game.h"

int main(int argc, char *argv[])
{
	Game app;
	while (!app.UpdateAndRender()) {}
	return 0;
}