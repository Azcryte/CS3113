#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include "entity.h"
#include "loader.h"
#include "matrix.h"
using namespace std;

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define NUM_ASTEROIDS 13
#define TILE_SIZE 0.1f

#define CHANCE_TO_START_ALIVE 0.6f
#define DEATH_LIMIT 3
#define BIRTH_LIMIT 4
#define NUM_SIMULATION_STEPS 50

#define SPRITE_COUNT_X 16
#define SPRITE_COUNT_Y 8

class Game{
public:
	Game();
	~Game();

	void Initialize();
	bool UpdateAndRender();
	void Render();
	void RenderLevel();
	void Update(float elapsed);
	void FixedUpdate();
private:
	const Uint8* keys;
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;
	GLuint spriteSheet;
	GLuint spriteSheet2;
	GLuint spriteSheet3;

	Entity* user;
	vector<Entity*> entities;
	//vector<Entity*> asteroids;

	unsigned char** levelData;
	int mapWidth;
	int mapHeight;
	bool** cellmap;
	//bool cellmap[MAP_WIDTH][MAP_HEIGHT];
	//float grav_x;
	//float grav_y;

	void reset();

	bool isSolid(unsigned char tile);
	bool checkCollision(Entity* a, Entity* b);

	void worldToTileCoordinates(float x, float y, int* grid_x, int* grid_y);
	float checkPointForGridCollisionX(float x, float y);
	float checkPointForGridCollisionY(float x, float y);
	void doLevelCollisionX(Entity* entity);
	void doLevelCollisionY(Entity* entity);

	void buildLevel();
	bool** doSimulationStep(bool** oldMap);
	int countAliveNeighbors(bool** map, int x, int y);

	//void buildLevel();
	//bool readHeader(ifstream& stream);
	//bool readLayerData(ifstream& stream);
	//bool readEntityData(ifstream& stream);

};

Game::Game() {
	Initialize();

	keys = SDL_GetKeyboardState(NULL);

	done = false;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;

	srand(static_cast <unsigned> (time(0)));

	mapHeight = 100;
	mapWidth = 100;
	levelData = new unsigned char*[mapHeight];
	for (int i = 0; i < mapHeight; ++i) {
		levelData[i] = new unsigned char[mapWidth];
	}
	//cellmap = new bool[MAP_WIDTH][MAP_HEIGHT];

	//for (int y = 0; y < mapHeight; y++) {
	//	for (int x = 0; x < mapWidth; x++) {
	//		levelData[y][x] = 3;
	//	}
	//}
	buildLevel();

	spriteSheet = LoadTexture("sheet.png");
	spriteSheet2 = LoadTexture("arne_sprites.png");
	spriteSheet3 = LoadTexture("characters_1.png");

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
	glClearColor(0.4f, 0.2f, 0.4f, 1.0f);

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
		user->hover();
		if (!user->isJumping) {
			//user->jump();
		}

	} 
	else if (user->isHovering) {
		user->stopHovering();
	}
	if (keys[SDL_SCANCODE_RIGHT]) {
		user->walkRight();
	}
	else if (keys[SDL_SCANCODE_LEFT]) {
		user->walkLeft();
	}
	else {
		user->setIdle();
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
	float trans_x = -user->x;
	float trans_y = -user->y;
	//if (trans_y > 2.0f) {
	//	trans_y = 2.0f;
	//}
	//if (trans_y < -2.0f) {
	//	trans_y = -2.0f;
	//}
	//if (trans_x > 2.0f) {
	//	trans_x = 2.0f;
	//}
	//if (trans_x < -2.0f) {
	//	trans_x = -2.0f;
	//}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(trans_x, trans_y, 0.0f);

	user->Render();
	//for (size_t i = 0; i < asteroids.size(); i++) {
	//	asteroids[i]->Render();
	//}
	RenderLevel();
	SDL_GL_SwapWindow(displayWindow);
}

void Game::RenderLevel() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, spriteSheet2);

	vector<float> vertexData;
	vector<float> textureCoordData;

	int grid_x, grid_y;
	worldToTileCoordinates(user->x, user->y, &grid_x, &grid_y);

	int numVertices = 0;

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (levelData[y][x] != 0 &&
				abs(x - grid_x) < 12 &&
				abs(y - grid_y) < 12 )
			{
				float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
				float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
				float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
				float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
				vertexData.insert(vertexData.end(), {
					TILE_SIZE* x, -TILE_SIZE* y,
					TILE_SIZE* x, (-TILE_SIZE* y) - TILE_SIZE,
					(TILE_SIZE* x) + TILE_SIZE, (-TILE_SIZE* y) - TILE_SIZE,
					(TILE_SIZE* x) + TILE_SIZE, -TILE_SIZE* y
				});
				textureCoordData.insert(textureCoordData.end(), { u, v,
					u, v + (spriteHeight),
					u + spriteWidth, v + (spriteHeight),
					u + spriteWidth, v
				});

				numVertices += 4;
			}
		}
	}

	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, textureCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glTranslatef(-TILE_SIZE* mapWidth / 2, TILE_SIZE* mapHeight / 2, 0.0f);

	glDrawArrays(GL_QUADS, 0, numVertices);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Game::Update(float elapsed) {
	//user->FixedUpdate();
	//for (int i = 0; i < asteroids.size(); i++) {
	//	asteroids[i]->FixedUpdate();
	//}
}

void Game::FixedUpdate() {
	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->FixedUpdate();

		// ****** put in fixed update v *******
		if (entities[i]->collidedBot) {
			entities[i]->isJumping = false;
			//if (entities[i]->vel_y < 0.0f) {
				entities[i]->vel_y = 0.0f;
			//}
		}
		//if (entities[i]->collidedTop) {
		//	if (entities[i]->vel_y > 0.0f) {
		//		//entities[i]->vel_y = 0.0f;
		//	}
		//}
		if (entities[i]->collidedRight) {
			//if (entities[i]->facingRight) {
				entities[i]->vel_x = 0.0f;
			//}
		}
		if (entities[i]->collidedLeft) {
			//if (!entities[i]->facingRight) {
				entities[i]->vel_x = 0.0f;
			//}
		}

		entities[i]->collidedBot = false;
		entities[i]->collidedTop = false;
		entities[i]->collidedRight = false;
		entities[i]->collidedLeft = false;

		entities[i]->vel_x += entities[i]->grav_x * FIXED_TIMESTEP;
		entities[i]->vel_y += entities[i]->grav_y * FIXED_TIMESTEP;

		entities[i]->vel_x = lerp(entities[i]->vel_x, 0.0f, FIXED_TIMESTEP * entities[i]->fric_x);
		entities[i]->vel_y = lerp(entities[i]->vel_y, 0.0f, FIXED_TIMESTEP * entities[i]->fric_y);

		entities[i]->vel_x += entities[i]->acc_x * FIXED_TIMESTEP;
		entities[i]->vel_y += entities[i]->acc_y * FIXED_TIMESTEP;
		
		entities[i]->y += entities[i]->vel_y * FIXED_TIMESTEP;
		for (size_t j = 0; j < entities.size(); j++) {
			if (checkCollision(entities[i], entities[j]) && entities[i] != entities[j]) {
				float yPenetration = fabs(fabs(entities[j]->y - entities[i]->y) - (entities[i]->sprite.height * entities[i]->scale_y) / 2.0f - (entities[j]->sprite.height * entities[j]->scale_y) / 2.0f);
				if (entities[i]->y > entities[j]->y) {
					entities[i]->y += yPenetration + 0.001f;
					entities[i]->collidedBot = true;
				}
				else if (entities[i]->y < entities[j]->y) {
					entities[i]->y -= yPenetration + 0.001f;
					entities[i]->collidedTop = true;
				}
			}
		}
		doLevelCollisionY(entities[i]);

		entities[i]->x += entities[i]->vel_x * FIXED_TIMESTEP;
		for (size_t j = 0; j < entities.size(); j++) {
			if (checkCollision(entities[i], entities[j]) && entities[i] != entities[j]) {
				float xPenetration = fabs(fabs(entities[j]->x - entities[i]->x) - (entities[i]->sprite.width * entities[i]->scale_x) / 2.0f - (entities[j]->sprite.width * entities[j]->scale_x) / 2.0f);
				if (entities[i]->x > entities[j]->x) {
					entities[i]->x += xPenetration + 0.001f;
					entities[i]->collidedLeft = true;
				}
				else if (entities[i]->x < entities[j]->x) {
					entities[i]->x -= xPenetration + 0.001f;
					entities[i]->collidedRight = true;
				}
			}
		}
		//looks like a tunnel, if hit bottom/left/top crash
		doLevelCollisionX(entities[i]);

	}
	//user->FixedUpdate();

	float animation;
	user->elapsed += FIXED_TIMESTEP;
	animation = fmod(user->elapsed, FIXED_TIMESTEP * 40.0f);
	if (user->facingRight) {
		/*if (user->vel_x < 0.2f) {
			user->sprite.frame = 4;
		}
		else */if (animation < FIXED_TIMESTEP * 10.0f) {
			user->sprite.frame = 3;
		}
		else if (animation < FIXED_TIMESTEP * 20.0f) {
			user->sprite.frame = 4;
		}
		else if (animation < FIXED_TIMESTEP * 30.0f) {
			user->sprite.frame = 5;
		}
		else {
			user->sprite.frame = 4;
		}
	}
	else if (!user->facingRight ) {
		/*if (user->vel_x > -0.2f) {
			user->sprite.frame = 1;
		}
		else */if (animation < FIXED_TIMESTEP * 10.0f) {
			user->sprite.frame = 0;
		}
		else if (animation < FIXED_TIMESTEP * 20.0f) {
			user->sprite.frame = 1;
		}
		else if (animation < FIXED_TIMESTEP * 30.0f) {
			user->sprite.frame = 2;
		}
		else {
			user->sprite.frame = 1;
		}
	}

	//for (size_t i = 0; i < asteroids.size(); i++) {
	//	asteroids[i]->FixedUpdate();
	//	
	//	//user vs asteroid
	//	if (checkCollision(user, asteroids[i])) {
	//		Vector distance = Vector(user->x - asteroids[i]->x, user->y - asteroids[i]->y);
	//		float distLength = distance.length();
	//		distance.normalize();
	//
	//		user->x += distance.x * 0.0002f / pow(distLength, 2);
	//		user->y += distance.y * 0.0002f / pow(distLength, 2);
	//
	//		asteroids[i]->x -= distance.x * 0.0002f / pow(distLength, 2);
	//		asteroids[i]->y -= distance.y * 0.0002f / pow(distLength, 2);
	//	}
	//
	//	// asteroid v asteroid
	//	for (size_t j = 0; j < asteroids.size(); j++) {
	//		if (checkCollision(asteroids[i], asteroids[j])) {
	//			Vector distance = Vector(asteroids[i]->x - asteroids[j]->x, asteroids[i]->y - asteroids[j]->y);
	//			float distLength = distance.length();
	//			distance.normalize();
	//
	//			asteroids[i]->x += distance.x * 0.00015f / pow(distLength, 2);
	//			asteroids[i]->y += distance.y * 0.00015f / pow(distLength, 2);
	//
	//			asteroids[j]->x -= distance.x * 0.00015f / pow(distLength, 2);
	//			asteroids[j]->y -= distance.y * 0.00015f / pow(distLength, 2);
	//		}
	//	}
	//
	//}
}

void Game::reset() {
	//grav_x = 0.0f;
	//grav_y = -9.8f;

	user = new Entity();
	float adjust_u = 0.2f / 192.0f;
	float adjust_v = 0.2f / 128.0f;
	/*vector<float> u = {	92.0f / 192.0f, 108.0f / 192.0f, 124.0f / 192.0f,
						92.0f / 192.0f, 108.0f / 192.0f, 124.0f / 192.0f };*/
	vector<float> u = { 6.0f / 12.0f + adjust_u, 7.0f / 12.0f + adjust_u, 8.0f / 12.0f + adjust_u,
						6.0f / 12.0f + adjust_u, 7.0f / 12.0f + adjust_u, 8.0f / 12.0f + adjust_u};
	//vector<float> v = { 80.0f / 128.0f, 80.0f / 128.0f, 80.0f / 128.0f,
	//					96.0f / 128.0f, 96.0f / 128.0f, 96.0f / 128.0f };
	vector<float> v = { 5.0f / 8.0f + adjust_v, 5.0f / 8.0f + adjust_v, 5.0f / 8.0f + adjust_v,
						6.0f / 8.0f + adjust_v, 6.0f / 8.0f + adjust_v, 6.0f / 8.0f + adjust_v };
	user->sprite = SheetSprite(spriteSheet3, u, v, 16.0f / 192.0f - 2 * adjust_u, 16.0f / 128.0f - 2 * adjust_v);
	user->scale_x = 0.8;
	user->scale_y = 0.8;
	user->fric_x = 5.0f;
	user->fric_y = 5.0f;
	user->elapsed = 0.0f;
	user->y -= 0.5f;
	user->x += 0.5f;
	//user->x += (float) (-TILE_SIZE* mapWidth / 2);
	//user->y += (float) (TILE_SIZE* mapHeight / 2);

	entities.clear();
	entities.push_back(user);

	buildLevel();
	/*SheetSprite asteroidTexture = SheetSprite(spriteSheet, 518.0f / 1024.0f, 810.0f / 1024.0f, 89.0f / 1024.0f, 82.0f / 1024.0f);
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
	}*/

}

bool Game::isSolid(unsigned char tile) {
	switch (tile) {
	case 32:
		return true;
		break;
	case 33:
		return true;
		break;
	default:
		return false;
		break;
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

void Game::worldToTileCoordinates(float x, float y, int* grid_x, int* grid_y) {
	*grid_x = (int)(x / TILE_SIZE);
	*grid_y = (int)(-y / TILE_SIZE);
	//*grid_x = (int)(((x * 16.0f) / TILE_SIZE) + (128.0f * 16.0f / 2.0f)) / 16.0f;
	//*grid_y = (int)(((y * 16.0f) / -TILE_SIZE) + (32.0f * 16.0f / 2.0f)) / 16.0f;
}

float Game::checkPointForGridCollisionX(float x, float y) {
	int grid_x, grid_y;
	worldToTileCoordinates(x, y, &grid_x, &grid_y);
	//if (grid_x < 0 || grid_x > 128 || grid_y < 0 || grid_y > 32) {
	//	return 0.0f;
	//}

	if (isSolid(levelData[grid_y][grid_x])) {
		return 0.004f;
	}
	return 0.0f;
}

float Game::checkPointForGridCollisionY(float x, float y) {
	int grid_x, grid_y;
	worldToTileCoordinates(x, y, &grid_x, &grid_y);
	//if (grid_x < 0 || grid_x > mapWidth || grid_y < 0 || grid_y > mapHeight) {
	//	return 0.0f;
	//}

	if (isSolid(levelData[grid_y][grid_x])) {

		float yCoordinate = (grid_y * TILE_SIZE);// -(TILE_SIZE*16.0);
		return -y - yCoordinate;

	}
	return 0.0f;
}

void Game::doLevelCollisionX(Entity *entity) {
	//check right

	float adjust = checkPointForGridCollisionX(entity->x + entity->sprite.width * entity->scale_x * 0.5, entity->y);
	if (adjust != 0.0f) {
		entity->x -= adjust;
		entity->vel_x = 0.0f;
		entity->collidedRight = true;
	}

	//check left

	adjust = checkPointForGridCollisionX(entity->x - entity->sprite.width * entity->scale_x * 0.5, entity->y);
	if (adjust != 0.0f) {
		entity->x += adjust;
		entity->vel_x = 0.0f;
		entity->collidedLeft = true;
	}

}

void Game::doLevelCollisionY(Entity *entity) {
	//check bottom

	float adjust = checkPointForGridCollisionY(entity->x, entity->y - entity->sprite.height * 1.5 *0.5);
	if (adjust != 0.0f) {
		entity->y += adjust;
		entity->vel_y = 0.0f;
		entity->collidedBot = true;
	}

	////check top
	adjust = checkPointForGridCollisionY(entity->x, entity->y + entity->sprite.height * 0.5);
	if (adjust != 0.0f) {
		entity->y -= adjust * 0.05f;
		entity->vel_y = 0.0f;
		entity->collidedTop = true;
	}
}

void Game::buildLevel() {
	// initialize everything to default false
	cellmap = new bool*[mapHeight];
	for (int i = 0; i < mapHeight; i++) {
		cellmap[i] = new bool[mapWidth];
	}
	//set some cells to true
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++){
			if (randomFloat(0.0f, 1.0f) < CHANCE_TO_START_ALIVE) {
				cellmap[i][j] = false;
			}
		}
	}
	//do sim steps
	for (int i = 0; i < NUM_SIMULATION_STEPS; i++) {
		cellmap = doSimulationStep(cellmap);
	}
	//transfer cellmap to leveldata
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			float temp = randomFloat(0.0f, 1.0f);
			if (cellmap[i][j] || i == 0 || j == 0 || i == mapWidth-1 || j == mapHeight-1) {
				if (temp < 0.7f) {
					levelData[i][j] = 32;
				}
				else if (temp >= 0.7f) {
					levelData[i][j] = 33;
				}
			}
			else {
				levelData[i][j] = 0;
			}
		}
	}
	//for (int y = 0; y < mapHeight; y++) {
	//	for (int x = 0; x < mapWidth; x++) {
	//		levelData[y][x] = 3;
	//	}
	//}
}

bool** Game::doSimulationStep(bool** oldMap) {
	bool** newMap = new bool*[mapHeight];
	for (int i = 0; i < mapHeight; i++) {
		newMap[i] = new bool[mapWidth];
	}
	for (int i = 0; i < mapWidth; i++) {
		for (int j = 0; j < mapHeight; j++) {
			int n = countAliveNeighbors(oldMap, i, j);

			if (oldMap[i][j]){
				if (n < DEATH_LIMIT) {
					newMap[i][j] = false;
				}
				else {
					newMap[i][j] = true;
				}
			}
			else {
				if (n > BIRTH_LIMIT) {
					newMap[i][j] = true;
				}
				else {
					newMap[i][j] = false;
				}
			}
		}
	}
	return newMap;
}

int Game::countAliveNeighbors(bool** map, int x, int y) {
	int count = 0;
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			int neighbor_x = x + i;
			int neighbor_y = y + j;
			if (i == 0 && j == 0) {}
			else if (neighbor_x < 0 || neighbor_y < 0 || neighbor_x >= mapHeight || neighbor_y >= mapWidth) {
				count = count + 1;
			}
			else if (map[neighbor_x][neighbor_y]) {
				count = count + 1;
			}
		}
	}
	return count;
}

//void Game::buildLevel() {
//	ifstream infile("levelFile.txt");
//	string line;
//	while (getline(infile, line)) {
//		if (line == "[header]") {
//			if (!readHeader(infile)) {
//				return;
//			}
//		}
//		else if (line == "[layer]") {
//			readLayerData(infile);
//		}
//		else if (line == "[Object Layer 1]") {
//			readEntityData(infile);
//		}
//	}
//}
//
//bool Game::readHeader(ifstream& stream) {
//	string line;
//	mapWidth = -1;
//	mapHeight = -1;
//	while (getline(stream, line)) {
//		if (line == "") { break; }
//
//		istringstream sStream(line);
//		string key, value;
//		getline(sStream, key, '=');
//		getline(sStream, value);
//
//		if (key == "width") {
//			mapWidth = atoi(value.c_str());
//		}
//		else if (key == "height"){
//			mapHeight = atoi(value.c_str());
//		}
//	}
//
//	if (mapWidth == -1 || mapHeight == -1) {
//		return false;
//	}
//	else { // allocate our map data
//		levelData = new unsigned char*[mapHeight];
//		for (int i = 0; i < mapHeight; ++i) {
//			levelData[i] = new unsigned char[mapWidth];
//		}
//		return true;
//	}
//}
//bool Game::readLayerData(ifstream& stream) {
//	string line;
//	while (getline(stream, line)) {
//		if (line == "") { break; }
//		istringstream sStream(line);
//		string key, value;
//		getline(sStream, key, '=');
//		getline(sStream, value);
//		if (key == "data") {
//			for (int y = 0; y < mapHeight; y++) {
//				getline(stream, line);
//				istringstream lineStream(line);
//				string tile;
//
//				for (int x = 0; x < mapWidth; x++) {
//					getline(lineStream, tile, ',');
//					unsigned char val = (unsigned char)atoi(tile.c_str());
//					if (val > 0) {
//						levelData[y][x] = val;
//					}
//					else {
//						levelData[y][x] = 0;
//					}
//				}
//
//			}
//		}
//	}
//	return true;
//}
//bool Game::readEntityData(ifstream& stream) {
//	string line;
//	string type;
//	while (getline(stream, line)) {
//		if (line == "") { break; }
//		istringstream sStream(line);
//		string key, value;
//		getline(sStream, key, '=');
//		getline(sStream, value);
//		if (key == "type") {
//			type = value;
//		}
//		else if (key == "location") {
//			istringstream lineStream(value);
//			string xPosition, yPosition;
//			getline(lineStream, xPosition, ',');
//			getline(lineStream, yPosition, ',');
//
//			float placeX = (atoi(xPosition.c_str()) + 8.0f - 128.0f * 16.0f / 2.0f) / 16.0f * TILE_SIZE;
//			float placeY = (atoi(yPosition.c_str()) - 8.0f - 32.0f * 16.0f / 2.0f) / 16.0f * -TILE_SIZE;
//
//			//placeEntity(type, placeX, placeY);
//		}
//	}
//	return true;
//}