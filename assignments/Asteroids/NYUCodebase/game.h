#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <algorithm>
#include "entity.h"
#include "loader.h"
#include "matrix.h"
using namespace std;

#define NUM_ASTEROIDS 13

class Game{
public:
	Game();
	~Game();

	void Initialize();
	bool UpdateAndRender();
	void Render();
	void Update(float elapsed);
	void FixedUpdate();
private:
	const Uint8* keys;
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;
	GLuint spriteSheet;

	Entity* user;
	vector<Entity*> asteroids;

	void reset();
	bool checkCollision(Entity* a, Entity* b);
};

Game::Game() {
	Initialize();

	keys = SDL_GetKeyboardState(NULL);

	done = false;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;

	spriteSheet = LoadTexture("sheet.png");

	Matrix initial;
	glLoadMatrixf(initial.ml);

	reset();
}

Game::~Game() {
	SDL_Quit();
}

void Game::Initialize() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

bool Game::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	user->buildMatrix();
	user->setVector();

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
		}
	}
	if (keys[SDL_SCANCODE_UP]) {
		user->x += 0.5f * user->vector.x * elapsed;
		user->y += 0.5f * user->vector.y * elapsed;
	}
	else if (keys[SDL_SCANCODE_DOWN]) {
		user->x -= 0.3f * user->vector.x * elapsed;
		user->y -= 0.3f * user->vector.y * elapsed;
	}
	if (keys[SDL_SCANCODE_RIGHT]) {
		user->rotation -= 2.0f * elapsed;
	}
	else if (keys[SDL_SCANCODE_LEFT]) {
		user->rotation += 2.0f * elapsed;
	}

	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
	}
	timeLeftOver = fixedElapsed;

	Update(elapsed);
	Render();
	return done;
}

void Game::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	user->Render();
	for (size_t i = 0; i < asteroids.size(); i++) {
		asteroids[i]->Render();
	}
	SDL_GL_SwapWindow(displayWindow);
}

void Game::Update(float elapsed) {
	//user->FixedUpdate();
	//for (int i = 0; i < asteroids.size(); i++) {
	//	asteroids[i]->FixedUpdate();
	//}
}

void Game::FixedUpdate() {
	user->FixedUpdate();
	for (size_t i = 0; i < asteroids.size(); i++) {
		asteroids[i]->FixedUpdate();
		
		//user vs asteroid
		if (checkCollision(user, asteroids[i])) {
			Vector distance = Vector(user->x - asteroids[i]->x, user->y - asteroids[i]->y);
			float distLength = distance.length();
			distance.normalize();

			user->x += distance.x * 0.0002f / pow(distLength, 2);
			user->y += distance.y * 0.0002f / pow(distLength, 2);

			asteroids[i]->x -= distance.x * 0.0002f / pow(distLength, 2);
			asteroids[i]->y -= distance.y * 0.0002f / pow(distLength, 2);
		}

		// asteroid v asteroid
		for (size_t j = 0; j < asteroids.size(); j++) {
			if (checkCollision(asteroids[i], asteroids[j])) {
				Vector distance = Vector(asteroids[i]->x - asteroids[j]->x, asteroids[i]->y - asteroids[j]->y);
				float distLength = distance.length();
				distance.normalize();

				asteroids[i]->x += distance.x * 0.00015f / pow(distLength, 2);
				asteroids[i]->y += distance.y * 0.00015f / pow(distLength, 2);

				asteroids[j]->x -= distance.x * 0.00015f / pow(distLength, 2);
				asteroids[j]->y -= distance.y * 0.00015f / pow(distLength, 2);
			}
		}

	}
}

void Game::reset() {
	user = new Entity();
	user->sprite = SheetSprite(spriteSheet, 211.0f / 1024.0f, 941.0f / 1024.0f, 99.0f / 1024.0f, 75.0f / 1024.0f);
	user->fric_x = 0.5f;
	user->fric_y = 0.5f;

	SheetSprite asteroidTexture = SheetSprite(spriteSheet, 518.0f / 1024.0f, 810.0f / 1024.0f, 89.0f / 1024.0f, 82.0f / 1024.0f);
	for (int i = 0; i < NUM_ASTEROIDS; i++) {
		Entity* asteroid = new Entity();
		asteroid->sprite = asteroidTexture;
		asteroid->scale_x = 1.4f * randomFloat(0.5f, 1.1f);
		asteroid->scale_y = 1.4f * randomFloat(0.5f, 1.1f);
		asteroid->x = randomFloat(-1.33f, 1.33f);
		asteroid->y = randomFloat(-1.0f, 1.0f);
		asteroid->rotation = randomFloat(0.0f, 2.0f * (float)M_PI);
		asteroid->vel_x = randomFloat(-0.3f, 0.3f);
		asteroid->vel_y = randomFloat(-0.3f, 0.3f);
		asteroids.push_back(asteroid);
	}
}

bool Game::checkCollision(Entity* a, Entity* b) {
	if (a == b) {
		return false;
	}
	a->buildMatrix();
	b->buildMatrix();

	Matrix aInverse = a->matrix.inverse();
	Matrix bInverse = b->matrix.inverse();

	float aWidth = a->sprite.width * a->scale_x * 0.9f;
	float aHeight = a->sprite.height * a->scale_y * 0.9f;
	float bWidth = b->sprite.width * b->scale_x * 0.9f;
	float bHeight = b->sprite.height * b->scale_y * 0.9f;

	Vector aTL = Vector(-aWidth, aHeight);
	Vector aBL = Vector(-aWidth, -aHeight);
	Vector aBR = Vector(aWidth, -aHeight);
	Vector aTR = Vector(aWidth, aHeight);

	aTL = a->matrix * aTL;
	aBL = a->matrix * aBL;
	aBR = a->matrix * aBR;
	aTR = a->matrix * aTR;

	aTL = bInverse * aTL;
	aBL = bInverse * aBL;
	aBR = bInverse * aBR;
	aTR = bInverse * aTR;

	float minX, maxX, minY, maxY;
	minX = min(min(min(aTL.x, aBL.x), aTR.x), aBR.x);
	maxX = max(max(max(aTL.x, aBL.x), aTR.x), aBR.x);
	if (!(minX <= bWidth && maxX >= -bWidth)) {
		return false;
	}
	minY = min(min(min(aTL.y, aBL.y), aTR.y), aBR.y);
	maxY = max(max(max(aTL.y, aBL.y), aTR.y), aBR.y);
	if (!(minY <= bHeight && maxY >= -bHeight)) {
		return false;
	}

	Vector bTL = Vector(-bWidth, bHeight);
	Vector bBL = Vector(-bWidth, -bHeight);
	Vector bBR = Vector(bWidth, -bHeight);
	Vector bTR = Vector(bWidth, bHeight);

	bTL = b->matrix * bTL;
	bBL = b->matrix * bBL;
	bTR = b->matrix * bTR;
	bBR = b->matrix * bBR;

	bTL = aInverse * bTL;
	bBL = aInverse * bBL;
	bBR = aInverse * bBR;
	bTR = aInverse * bTR;

	minX = min(min(min(bTL.x, bBL.x), bTR.x), bBR.x);
	maxX = max(max(max(bTL.x, bBL.x), bTR.x), bBR.x);
	if (!(minX <= aWidth && maxX >= -aWidth)) {
		return false;
	}
	minY = min(min(min(bTL.y, bBL.y), bTR.y), bBR.y);
	maxY = max(max(max(bTL.y, bBL.y), bTR.y), bBR.y);
	if (!(minY <= aHeight && maxY >= -aHeight)) {
		return false;
	}

	return true;
}