#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include "entity.h"
#include "dumbMob.h"
#include "bat.h"
#include "explosion.h"
//#include "loader.h"
//#include "matrix.h"
#include "particleEmitter.h"
//using namespace std;

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
//#define NUM_ASTEROIDS 13

#define CHANCE_TO_START_ALIVE 0.6f
#define DEATH_LIMIT 3
#define BIRTH_LIMIT 4
#define NUM_SIMULATION_STEPS 20
#define SPRITE_COUNT_X 16
#define SPRITE_COUNT_Y 8
#define TILE_SIZE 0.1f

//#define MAX_ENEMIES 5
#define NUM_DUMBMOBS 7
#define DUMBMOB_SPAWN_TIMER 1.0f
#define NUM_BATS 3
#define BAT_SPAWN_TIMER 2.0f

#define ENEMY_TYPES 2

enum GameState{ MAIN_MENU, GAME_PLAY, GAME_PAUSE, GAME_OVER};
enum EnemyID{DUMB_MOB, BAT};

/*
entityID:
	-10 = player
	-5 = bullet
	-1 = default
	0 = dumb mob
	1 = bat

*/

class Game{
public:
	Game();
	~Game();

	void Initialize();
	bool UpdateAndRender();
	void Render();
	void RenderLevel();
	void RenderMainMenu();
	void RenderGame();
	void RenderGameOver();
	void Update(float elapsed);
	void FixedUpdate();
private:
	const Uint8* keys;
	GameState state;
	bool done;
	bool paused;
	int score;
	float lastFrameTicks;
	float timeLeftOver;
	float timeElapsed;
	float screenShakeValue;
	vector<float> timeSinceLastSpawn;
	//float timeSinceLastSpawn;

	SDL_Window* displayWindow;
	GLuint spriteSheet;
	GLuint spriteSheet2;
	GLuint spriteSheet3;
	GLuint explosionSpriteSheet;
	vector<float> explosion_u;
	vector<float> explosion_v;
	vector<Explosion*> explosions;
	GLuint font;
	Mix_Music* music;
	Mix_Chunk* shoot;
	Mix_Chunk* enemyHit;
	Mix_Chunk* gameOver;
	Mix_Chunk* blop;

	Entity* player;
	vector<Entity*> enemies;
	vector<DumbMob*> dumbMobs;
	vector<Bat*> bats;
	Entity* gun1;

	ParticleEmitter ParticleEmitterTest;
	//ParticleEmitter BulletShooterTest;
	//vector<Entity*> asteroids;
	
	unsigned char** levelData;
	bool** cellmap;
	int mapWidth;
	int mapHeight;
	int spawn_x;
	int spawn_y;

	//void explosion(int index, float x_pos, float y_pos);

	void reset();

	bool isSolid(unsigned char tile);
	bool checkCollision(Entity* a, Entity* b);
	void worldToTileCoordinates(float x, float y, int* grid_x, int* grid_y);
	void tileToWorldCoordinates(int grid_x, int grid_y, float* x, float* y);
	float checkPointForGridCollisionX(float x, float y);
	float checkPointForGridCollisionY(float x, float y);
	void doLevelCollisionX(Entity* entity);
	void doLevelCollisionY(Entity* entity);

	void buildLevel();
	bool** doSimulationStep(bool** oldMap);
	int countAliveNeighbors(bool** map, int x, int y);

	Vector checkForValidSpawnLocation(int type);
};

Game::Game() {
	Initialize();

	keys = SDL_GetKeyboardState(NULL);

	state = MAIN_MENU;
	done = false;
	score = 0;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;
	//timeSinceLastSpawn = 0.0f;
	screenShakeValue = 0.0f;

	for (size_t i = 0; i < ENEMY_TYPES; i++) {
		timeSinceLastSpawn.push_back(0.0f);
	}

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
	//	}s
	//}
	//buildLevel();
	
	spriteSheet2 = LoadTexture("arne_sprites.png");
	spriteSheet3 = LoadTexture("characters_1.png");
	explosionSpriteSheet = LoadTexture("explosion.png");
	explosion_u = { 0.0f / 5.0f, 1.0f / 5.0f, 2.0f / 5.0f, 3.0f / 5.0f, 4.0f / 5.0f, 
		0.0f / 5.0f, 1.0f / 5.0f, 2.0f / 5.0f, 3.0f / 5.0f, 4.0f / 5.0f, 
		0.0f / 5.0f, 1.0f / 5.0f, 2.0f / 5.0f, 3.0f / 5.0f, 4.0f / 5.0f, 
		0.0f / 5.0f, 1.0f / 5.0f, 2.0f / 5.0f, 3.0f / 5.0f, 4.0f / 5.0f, 
		0.0f / 5.0f, 1.0f / 5.0f, 2.0f / 5.0f, 3.0f / 5.0f, 4.0f / 5.0f,
		4.0f / 5.0f };
	explosion_v = { 0.0f / 5.0f, 0.0f / 5.0f, 0.0f / 5.0f, 0.0f / 5.0f, 0.0f / 5.0f, 
		1.0f / 5.0f, 1.0f / 5.0f, 1.0f / 5.0f, 1.0f / 5.0f, 1.0f / 5.0f, 
		2.0f / 5.0f, 2.0f / 5.0f, 2.0f / 5.0f, 2.0f / 5.0f, 2.0f / 5.0f, 
		3.0f / 5.0f, 3.0f / 5.0f, 3.0f / 5.0f, 3.0f / 5.0f, 3.0f / 5.0f, 
		4.0f / 5.0f, 4.0f / 5.0f, 4.0f / 5.0f, 4.0f / 5.0f, 4.0f / 5.0f, 
		4.0f / 5.0f };
	font = LoadTexture("font.png");

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	music = Mix_LoadMUS("music.mp3");
	shoot = Mix_LoadWAV("shoot.wav");
	enemyHit = Mix_LoadWAV("enemy_hit.wav");
	gameOver = Mix_LoadWAV("game_over.wav");
	blop = Mix_LoadWAV("blop.wav");

	Matrix initial;
	glLoadMatrixf(initial.ml);

	//reset();
}

Game::~Game() {
	Mix_FreeMusic(music);
	Mix_FreeChunk(shoot);
	Mix_FreeChunk(enemyHit);
	Mix_FreeChunk(gameOver);
	Mix_FreeChunk(blop);

	SDL_Quit();
}

void Game::Initialize() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glClearColor(0.0f, 0.2f, 0.7f, 1.0f);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

bool Game::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	//player->buildMatrix();
	//gun1->buildMatrix();
	//player->setVector();

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
		}
		else if (event.type == SDL_MOUSEMOTION) {
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) {
		}
	}
	//if (keys[SDL_SCANCODE_SPACE]) {
	//	ParticleEmitterTest.active = true;
	//}
	//else {
	//	ParticleEmitterTest.active = false;
	////}

	if (keys[SDL_SCANCODE_1]) { state = MAIN_MENU; }
	else if (keys[SDL_SCANCODE_2]) { state = GAME_PLAY; }
	else if (keys[SDL_SCANCODE_3]) { state = GAME_PAUSE; }
	else if (keys[SDL_SCANCODE_4]) { state = GAME_OVER; }
	switch (state) {
	case MAIN_MENU:
		if (event.button.button == 1 && event.type == SDL_MOUSEBUTTONDOWN) {
			//Matrix initial;
			//glLoadMatrixf(initial.ml);
			reset();
			Mix_PlayMusic(music, -1);
			state = GAME_PLAY;
		}
		break;
	case GAME_PLAY:
		if (event.button.button == 1 && event.type == SDL_MOUSEBUTTONDOWN) {
			if (!gun1->bulletActive) {
				gun1->bulletReset(player->position);
				gun1->bulletShoot(Vector(((((float)event.motion.x / 800.0f) * 2.666f) - 1.333f),
					(((float)(600 - event.motion.y) / 600.0f) * 2.0f) - 1.0f));
				Mix_PlayChannel(-1, shoot, 0);
			}
		}
		if (keys[SDL_SCANCODE_W]) {
			player->hover();
			//if (!player->isJumping) {
			//	player->jump();
			//}
		}
		else if (player->isHovering) {
			player->stopHovering();
		}

		if (keys[SDL_SCANCODE_D]) {
			player->moveRight();
			//for (size_t i = 0; i < MAX_ENEMIES; i++) {
			//	enemies[i]->moveRight();
			//}
		}
		else if (keys[SDL_SCANCODE_A]) {
			player->moveLeft();
			//for (size_t i = 0; i < MAX_ENEMIES; i++) {
			//	enemies[i]->moveLeft();
			//}
		}
		else {
			player->setIdle();
			//for (size_t i = 0; i < MAX_ENEMIES; i++) {
			//	enemies[i]->setIdle();
			//}
		}

		if (keys[SDL_SCANCODE_P]) {
			paused = true;
			state = GAME_PAUSE;
			//gun1->bulletActive = false;
		}
		break;
	case GAME_PAUSE:
		if (keys[SDL_SCANCODE_O]) {
			paused = false;
			state = GAME_PLAY;
			//gun1->bulletActive = true;
		}
		break;
	case GAME_OVER:
		if (event.button.button == 3 && event.type == SDL_MOUSEBUTTONDOWN) {
			state = MAIN_MENU; 
			Mix_HaltChannel(3);
		}
		break;
	}
	//if (keys[SDL_SCANCODE_R] /*&& event.type == SDL_KEYDOWN*/) {
	//	reset();
	//}

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
	switch (state) {
	case MAIN_MENU:
		RenderMainMenu();
		break;
	case GAME_PLAY:
		RenderGame();
		break;
	case GAME_PAUSE:
		RenderGame();
		break;
	case GAME_OVER:
		RenderGame();
		RenderGameOver();
		break;
	}
	SDL_GL_SwapWindow(displayWindow);
}

void Game::RenderLevel() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, spriteSheet2);

	vector<float> vertexData;
	vector<float> textureCoordData;

	int grid_x, grid_y;
	worldToTileCoordinates(player->position.x, player->position.y, &grid_x, &grid_y);

	int numVertices = 0;

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (levelData[y][x] != 12 &&
				abs(x - grid_x) < 12 &&
				abs(y - grid_y) < 12 
				)
			{
				float adjust = 0.001f;
				float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
				float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
				float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
				float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
				vertexData.insert(vertexData.end(), {
					TILE_SIZE* x - adjust, -TILE_SIZE* y + adjust,
					TILE_SIZE* x - adjust, (-TILE_SIZE* y) - TILE_SIZE - adjust,
					(TILE_SIZE* x) + TILE_SIZE + adjust, (-TILE_SIZE* y) - TILE_SIZE - adjust,
					(TILE_SIZE* x) + TILE_SIZE + adjust, -TILE_SIZE* y + adjust
				});
				textureCoordData.insert(textureCoordData.end(), { u + adjust, v + adjust,
					u + adjust, v + spriteHeight - adjust,
					u + spriteWidth - adjust, v + spriteHeight - adjust,
					u + spriteWidth - adjust, v + adjust
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

void Game::RenderMainMenu() {
	glLoadIdentity();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	float animationValue = mapValue(timeElapsed, 0.0f, 3.0f, 0.0f, 1.0f);
	drawText(font, "<Game Name>", 0.3f, -0.13f, easeOut(-3.0f, -0.85f, animationValue), 0.7f);
	//drawText(font, "<Game Name>", 0.3f, -0.13f, -0.85f, 0.7f);
	animationValue = mapValue(timeElapsed, 1.8f, 2.8f, 0.0f, 1.0f);
	drawText(font, "Click to Start Game", 0.1f, -0.05f, easeOut(-2.25f, -0.42f, animationValue), 0.2f);

	animationValue = mapValue(timeElapsed, 2.6f, 3.6f, 0.0f, 1.0f);
	drawText(font, "WASD to move", 0.1f, -0.05f, easeOut(-2.0, -0.23f, animationValue), -0.2f);

	animationValue = mapValue(timeElapsed, 3.4f, 4.4f, 0.0f, 1.0f);
	drawText(font, "mouse to aim, left click to shoot", 0.1f, -0.05f, easeOut(-3.0f, -0.8f, animationValue), -0.4f);

	animationValue = mapValue(timeElapsed, 4.2f, 5.2f, 0.0f, 1.0f);
	drawText(font, "bullets can break blocks", 0.1f, -0.05f, easeOut(-2.5f, -0.53f, animationValue), -0.6f);

	//animationValue = mapValue(timeElapsed, 5.0f, 6.0f, 0.0f, 1.0f);
	//drawText(font, "press 1 to restart game", 0.1f, -0.05f, easeOut(-2.5f, -0.5f, animationValue), -0.8f);
}

void Game::RenderGame() {
	glLoadIdentity();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	float trans_x = -player->position.x;
	float trans_y = -player->position.y;
	//if (trans_y > 3.0f) {
	//	trans_y = 3.0f;
	//}
	//if (trans_y < 1.0f) {
	//	trans_y = 1.0f;
	//}
	//if (trans_x > -1.0f) {
	//	trans_x = -1.0f;
	//}
	//if (trans_x < - mapWidth * TILE_SIZE) {
	//	trans_x = - mapWidth * TILE_SIZE;
	//}

	glMatrixMode(GL_MODELVIEW);
	glTranslatef(trans_x, trans_y, 0.0f);
	if (state == GAME_OVER) {
		glTranslatef(sin(screenShakeValue * 70.0f) * 0.02f, sin(screenShakeValue * 50.0f) * 0.02f, 0.0f);
	}

	//for (size_t i = 0; i < asteroids.size(); i++) {
	//	asteroids[i]->Render();
	//}
	RenderLevel();
	player->Render();
	if (gun1->bulletActive) {
		gun1->Render();
	}
	for (size_t i = 0; i < enemies.size(); i++) {
		if (!enemies[i]->dead) {
			enemies[i]->Render();
		}
	}
	for (size_t i = 0; i < explosions.size(); i++) {
		explosions[i]->Render();
	}
	ParticleEmitterTest.Render();
	drawText(font, "Score:" + to_string(score), 0.1f, -0.05f, -0.95f, 0.95f);
	//BulletShooterTest.Render();
}

void Game::RenderGameOver() {
	glLoadIdentity();
	//glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	//glClear(GL_COLOR_BUFFER_BIT);

	drawText(font, "GAME OVER", 0.3f, -0.1f, -0.8f, 0.45f);
	drawText(font, "Final Score:", 0.2f, -0.1f, -0.5f, 0.1f);
	drawText(font, to_string(score), 0.2f, -0.1f, -0.15f, -0.1f);
	drawText(font, "Right click to return to Main Menu", 0.1f, -0.05f, -0.8f, -0.6f);
}

void Game::Update(float elapsed) {
	//player->FixedUpdate();
	//for (int i = 0; i < asteroids.size(); i++) {
	//	asteroids[i]->FixedUpdate();
	//}
}

void Game::FixedUpdate() {
	timeElapsed += FIXED_TIMESTEP;
	for (size_t i = 0; i < timeSinceLastSpawn.size(); i++) {
		timeSinceLastSpawn[i] += FIXED_TIMESTEP;
	}
	//timeSinceLastSpawn += FIXED_TIMESTEP;

	switch (state) {
	case GAME_PLAY:
		//player
		player->FixedUpdate();
		player->position.y += player->velocity.y * FIXED_TIMESTEP;
		doLevelCollisionY(player);
		player->position.x += player->velocity.x * FIXED_TIMESTEP;
		doLevelCollisionX(player);

		float animation;
		//elapplayer->elapsed += FIXED_TIMESTEP;
		animation = fmod(player->elapsed, FIXED_TIMESTEP * 100.0f);
		if (player->facingRight) {
			/*if (player->velocity.x < 0.2f) {
			player->sprite.frame = 4;
			}
			else */if (animation < FIXED_TIMESTEP * 25.0f) {
			player->sprite.frame = 3;
			}
			else if (animation < FIXED_TIMESTEP * 50.0f) {
				player->sprite.frame = 4;
			}
			else if (animation < FIXED_TIMESTEP * 75.0f) {
				player->sprite.frame = 5;
			}
			else {
				player->sprite.frame = 4;
			}
		}
		else if (!player->facingRight) {
			/*if (player->velocity.x > -0.2f) {
			player->sprite.frame = 1;
			}
			else */if (animation < FIXED_TIMESTEP * 25.0f) {
			player->sprite.frame = 0;
			}
			else if (animation < FIXED_TIMESTEP * 50.0f) {
				player->sprite.frame = 1;
			}
			else if (animation < FIXED_TIMESTEP * 75.0f) {
				player->sprite.frame = 2;
			}
			else {
				player->sprite.frame = 1;
			}
		}

		//particle emitter test
		if (player->isIdle && fabs(player->velocity.y) < 0.2  && !player->isHovering) {
			ParticleEmitterTest.active = false;
		}
		else {
			ParticleEmitterTest.active = true;
		}
		ParticleEmitterTest.position = Vector(player->position.x, player->position.y);
		ParticleEmitterTest.velocity = Vector(-player->velocity.x * 0.3f, -player->velocity.y * 0.3f);
		if (player->isHovering) {
			ParticleEmitterTest.velocity = ParticleEmitterTest.velocity + Vector(0.0f, -0.5f);
		}
		ParticleEmitterTest.FixedUpdate();

		//enemies dumbmob
		//for (size_t i = 0; i < MAX_ENEMIES; i++) {
		//	if (fabs(enemies[i]->position.x - player->position.x) > 4.0f ||
		//		fabs(enemies[i]->position.y - player->position.y) > 3.0f) {
		//		enemies[i]->dead = true;
		//	}
		//
		//	if (enemies[i]->dead && timeSinceLastSpawn > 2.0f) {
		//		enemies[i]->dead = false;
		//		float scale = randomFloat(0.8f, 1.2f);
		//		enemies[i]->scale_x = scale;
		//		enemies[i]->scale_y = scale;
		//		enemies[i]->position = checkForValidSpawnLocation(1);
		//		enemies[i]->velocity.x = randomFloat(8.0f, 11.0f);
		//		enemies[i]->jumpTimeGap = randomFloat(1.0f, 3.0f);
		//		timeSinceLastSpawn = 0.0f;
		//	}
		//
		//	if (!enemies[i]->dead) {
		//		if (checkCollision(enemies[i], player)) {
		//			Mix_HaltMusic();
		//			Mix_PlayChannel(3, gameOver, 0);
		//			state = GAME_OVER;
		//		}
		//
		//		//if (enemies[i]->facingRight) {
		//		//	enemies[i]->velocity.x = 0.5f;
		//		//}
		//		//else {
		//		//	enemies[i]->velocity.x = -0.5f;
		//		//}
		//
		//		//// ****** put in fixed update v *******
		//
		//		//if (enemies[i]->collidedBot) {
		//		//	enemies[i]->isJumping = false;
		//		//	//if (entities[i]->velocity.y < 0.0f) {
		//		//	enemies[i]->velocity.y = 0.0f;
		//		//	//}
		//		//}sp
		//		//else {
		//		//	enemies[i]->isJumping = true;
		//		//}
		//		//if (enemies[i]->collidedTop) {
		//		//	if (enemies[i]->velocity.y > 0.0f) {
		//		//		//entities[i]->velocity.y = 0.0f;
		//		//	}
		//		//}
		//		//if (enemies[i]->collidedRight) {
		//		//	if (enemies[i]->facingRight) {
		//		//		enemies[i]->facingRight = false;
		//		//		//enemies[i]->velocity.x = 0.0f;
		//		//	}
		//		//}
		//		//if (enemies[i]->collidedLeft) {
		//		//	if (!enemies[i]->facingRight) {
		//		//		enemies[i]->facingRight = true;
		//		//		//enemies[i]->velocity.x = 0.0f;
		//		//	}
		//		//}
		//		//if (enemies[i]->timeSinceLastJump > enemies[i]->jumpTimeGap) {
		//		//	if (!enemies[i]->isJumping) {
		//		//		enemies[i]->jump();
		//		//	}
		//		//	enemies[i]->timeSinceLastJump = 0.0f;
		//		//}
		//		if (enemies[i]->timeSinceLastJump > enemies[i]->jumpTimeGap) {
		//			if (enemies[i]->position.y < player->position.y) {
		//				enemies[i]->hover();
		//				enemies[i]->timeSinceLastJump = 0.0f;
		//			}
		//			else {
		//				enemies[i]->stopHovering();
		//				enemies[i]->timeSinceLastJump = 0.0f;
		//			}
		//			if (enemies[i]->position.x < player->position.x) {
		//				enemies[i]->facingRight = true;
		//			}
		//			else {
		//				enemies[i]->facingRight = false;
		//			}
		//		}
		//		
		//		enemies[i]->FixedUpdate();
		//		//enemies[i]->timeSinceLastJump += FIXED_TIMESTEP;
		//
		//		enemies[i]->position.y += enemies[i]->velocity.y * FIXED_TIMESTEP;
		//		doLevelCollisionY(enemies[i]);
		//
		//		enemies[i]->position.x += enemies[i]->velocity.x * FIXED_TIMESTEP;
		//		doLevelCollisionX(enemies[i]);
		//
		//		//enemies[i]->elapsed += FIXED_TIMESTEP;
		//		animation = fmod(enemies[i]->elapsed, FIXED_TIMESTEP * 60.0f);
		//		if (enemies[i]->facingRight) {
		//			/*if (player->velocity.x < 0.2f) {
		//			player->sprite.frame = 4;
		//			}
		//			else */if (animation < FIXED_TIMESTEP * 15.0f) {
		//			enemies[i]->sprite.frame = 3;
		//			}
		//			else if (animation < FIXED_TIMESTEP * 30.0f) {
		//				enemies[i]->sprite.frame = 4;
		//			}
		//			else if (animation < FIXED_TIMESTEP * 45.0f) {
		//				enemies[i]->sprite.frame = 5;
		//			}
		//			else {
		//				enemies[i]->sprite.frame = 4;
		//			}
		//		}
		//		else if (!enemies[i]->facingRight) {
		//			/*if (player->velocity.x > -0.2f) {
		//			player->sprite.frame = 1;
		//			}
		//			else */if (animation < FIXED_TIMESTEP * 15.0f) {
		//			enemies[i]->sprite.frame = 0;
		//			}
		//			else if (animation < FIXED_TIMESTEP * 30.0f) {
		//				enemies[i]->sprite.frame = 1;
		//			}
		//			else if (animation < FIXED_TIMESTEP * 45.0f) {
		//				enemies[i]->sprite.frame = 2;
		//			}
		//			else {
		//				enemies[i]->sprite.frame = 1;
		//			}
		//		}
		//	}
		//
		//	//shapes
		//}

		// enemy - dumbmob
		for (size_t i = 0; i < NUM_DUMBMOBS; i++) {
			if (fabs(dumbMobs[i]->position.x - player->position.x) > 4.0f ||
				fabs(dumbMobs[i]->position.y - player->position.y) > 3.0f) {
				dumbMobs[i]->dead = true;
			}

			if (dumbMobs[i]->dead && timeSinceLastSpawn[DUMB_MOB] > DUMBMOB_SPAWN_TIMER) {
				dumbMobs[i]->dead = false;
				float scale = randomFloat(1.1f, 1.2f);
				dumbMobs[i]->scale_x = scale;
				dumbMobs[i]->scale_y = scale;
				dumbMobs[i]->position = checkForValidSpawnLocation(1);
				//dumbMobs[i]->velocity.x = randomFloat(8.0f, 11.0f);
				dumbMobs[i]->jumpTimeGap = randomFloat(1.0f, 3.0f);
				timeSinceLastSpawn[DUMB_MOB] = 0.0f;
			}

			if (!dumbMobs[i]->dead) {
				if (checkCollision(dumbMobs[i], player)) {
					Mix_HaltMusic();
					Mix_PlayChannel(3, gameOver, 0);
					state = GAME_OVER;
				}

				dumbMobs[i]->FixedUpdate();
				//dumbMobs[i]->timeSinceLastJump += FIXED_TIMESTEP;

				dumbMobs[i]->position.y += dumbMobs[i]->velocity.y * FIXED_TIMESTEP;
				doLevelCollisionY(dumbMobs[i]);

				dumbMobs[i]->position.x += dumbMobs[i]->velocity.x * FIXED_TIMESTEP;
				doLevelCollisionX(dumbMobs[i]);

				//dumbMobs[i]->elapsed += FIXED_TIMESTEP;
				animation = fmod(dumbMobs[i]->elapsed, FIXED_TIMESTEP * 60.0f);
				if (dumbMobs[i]->facingRight) {
					/*if (player->velocity.x < 0.2f) {
					player->sprite.frame = 4;
					}
					else */if (animation < FIXED_TIMESTEP * 15.0f) {
					dumbMobs[i]->sprite.frame = 3;
					}
					else if (animation < FIXED_TIMESTEP * 30.0f) {
						dumbMobs[i]->sprite.frame = 4;
					}
					else if (animation < FIXED_TIMESTEP * 45.0f) {
						dumbMobs[i]->sprite.frame = 5;
					}
					else {
						dumbMobs[i]->sprite.frame = 4;
					}
				}
				else if (!dumbMobs[i]->facingRight) {
					/*if (player->velocity.x > -0.2f) {
					player->sprite.frame = 1;
					}
					else */if (animation < FIXED_TIMESTEP * 15.0f) {
					dumbMobs[i]->sprite.frame = 0;
					}
					else if (animation < FIXED_TIMESTEP * 30.0f) {
						dumbMobs[i]->sprite.frame = 1;
					}
					else if (animation < FIXED_TIMESTEP * 45.0f) {
						dumbMobs[i]->sprite.frame = 2;
					}
					else {
						dumbMobs[i]->sprite.frame = 1;
					}
				}
			}
		}

		// enemy - bat
		for (size_t i = 0; i < NUM_BATS; i++) {
			if (fabs(bats[i]->position.x - player->position.x) > 4.0f ||
				fabs(bats[i]->position.y - player->position.y) > 3.0f) {
				bats[i]->dead = true;
			}

			if (bats[i]->dead && timeSinceLastSpawn[BAT] > BAT_SPAWN_TIMER) {
				bats[i]->dead = false;
				float scale = 0.9f;
				bats[i]->scale_x = scale;
				bats[i]->scale_y = scale;
				bats[i]->position = checkForValidSpawnLocation(1);
				//bats[i]->velocity.x = randomFloat(8.0f, 11.0f);
				bats[i]->jumpTimeGap = randomFloat(1.0f, 3.0f);
				timeSinceLastSpawn[BAT] = 0.0f;
			}

			if (!bats[i]->dead) {
				if (checkCollision(bats[i], player)) {
					Mix_HaltMusic();
					Mix_PlayChannel(3, gameOver, 0);
					state = GAME_OVER;
				}
				if (bats[i]->timeSinceLastJump > bats[i]->jumpTimeGap) {
					if (bats[i]->position.y < player->position.y) {
						bats[i]->hover();
						bats[i]->timeSinceLastJump = 0.0f;
					}
					else {
						bats[i]->stopHovering();
						bats[i]->timeSinceLastJump = 0.0f;
					}
					if (bats[i]->position.x < player->position.x) {
						bats[i]->facingRight = true;
					}
					else {
						bats[i]->facingRight = false;
					}
				}

				bats[i]->FixedUpdate();
				//bats[i]->timeSinceLastJump += FIXED_TIMESTEP;

				bats[i]->position.y += bats[i]->velocity.y * FIXED_TIMESTEP;
				doLevelCollisionY(bats[i]);

				bats[i]->position.x += bats[i]->velocity.x * FIXED_TIMESTEP;
				doLevelCollisionX(bats[i]);

				//bats[i]->elapsed += FIXED_TIMESTEP;
				animation = fmod(bats[i]->elapsed, FIXED_TIMESTEP * 60.0f);
				if (bats[i]->facingRight) {
					/*if (player->velocity.x < 0.2f) {
					player->sprite.frame = 4;
					}
					else */if (animation < FIXED_TIMESTEP * 15.0f) {
					bats[i]->sprite.frame = 3;
					}
					else if (animation < FIXED_TIMESTEP * 30.0f) {
						bats[i]->sprite.frame = 4;
					}
					else if (animation < FIXED_TIMESTEP * 45.0f) {
						bats[i]->sprite.frame = 5;
					}
					else {
						bats[i]->sprite.frame = 4;
					}
				}
				else if (!bats[i]->facingRight) {
					/*if (player->velocity.x > -0.2f) {
					player->sprite.frame = 1;
					}
					else */if (animation < FIXED_TIMESTEP * 15.0f) {
					bats[i]->sprite.frame = 0;
					}
					else if (animation < FIXED_TIMESTEP * 30.0f) {
						bats[i]->sprite.frame = 1;
					}
					else if (animation < FIXED_TIMESTEP * 45.0f) {
						bats[i]->sprite.frame = 2;
					}
					else {
						bats[i]->sprite.frame = 1;
					}
				}
			}
		}

		//gun
		if (gun1->bulletActive) {
			gun1->FixedUpdate();
			gun1->position.y += gun1->velocity.y * FIXED_TIMESTEP;
			//doLevelCollisionY(gun1);
			gun1->position.x += gun1->velocity.x * FIXED_TIMESTEP;
			//doLevelCollisionX(gun1);
			for (size_t i = 0; i < enemies.size(); i++) {
				if (!enemies[i]->dead) {
					if (checkCollision(gun1, enemies[i])) {
						Explosion* temp = new Explosion(gun1->matrix);
						temp->sprite = SheetSprite(explosionSpriteSheet, explosion_u, explosion_v, 0.2f, 0.2f);
						temp->scale = 0.5f;
						explosions.push_back(temp);

						gun1->bulletReset(player->position);
						enemies[i]->dead = true;
						Mix_PlayChannel(-1, enemyHit, 0);
						score += (enemies[i]->entityID + 1) * 100;
					}
				}
			}
			if (checkPointForGridCollisionX(gun1->position.x, gun1->position.y) != 0.0f || checkPointForGridCollisionY(gun1->position.x, gun1->position.y) != 0.0f) {
				int grid_x, grid_y;
				worldToTileCoordinates(gun1->position.x, gun1->position.y, &grid_x, &grid_y);
				if (levelData[grid_y][grid_x] != 3) {
					Mix_PlayChannel(-1, blop, 0);
					levelData[grid_y][grid_x] = 12;
				}
				Explosion* temp = new Explosion(gun1->matrix);
				temp->sprite = SheetSprite(explosionSpriteSheet, explosion_u, explosion_v, 0.2f, 0.2f);
				temp->scale = 0.5f;
				explosions.push_back(temp);

				gun1->bulletReset(player->position);
			}
			if (gun1->lifetime > gun1->maxLifetime) {
				gun1->bulletActive = false;
			}
		}

		//explosion
		for (size_t i = 0; i < explosions.size(); i++) {
			explosions[i]->FixedUpdate();
			if (explosions[i]->sprite.frame > 24) {
				explosions.erase(explosions.begin() + i);
			}
		}
		//vector<Explosion*>::iterator iter;
		//for (iter = explosions.begin(); iter != explosions.end(); iter++) {
		//	(*iter)->FixedUpdate();
		//	if ((*iter)->sprite.frame > 24) {
		//		explosions.erase(iter);
		//	}
		//}

		screenShakeValue += FIXED_TIMESTEP;
		break;
	case GAME_OVER:
		screenShakeValue += FIXED_TIMESTEP;
		break;
	}




	//particleEmitterTest
	////bulletshooter
	//BulletShooterTest.position = Vector(player->position.x, player->position.y);
	//if (player->facingRight) {
	//	BulletShooterTest.velocity = Vector(3.0f, 0.0f);
	//}
	//else {
	//	BulletShooterTest.velocity = Vector(-3.0f, 0.0f);
	//}
	//BulletShooterTest.FixedUpdate();
}

void Game::reset() {
	//grav_x = 0.0f;
	//grav_y = -9.8f;
	paused = false;
	score = 0;

	buildLevel();
	explosions.clear();
	enemies.clear();
	dumbMobs.clear();
	bats.clear();

	player = new Entity();
	float adjust_u = 2.0f / 192.0f;
	float adjust_v = 1.0f / 128.0f;
	vector<float> player_u = { 6.0f / 12.0f + adjust_u, 7.0f / 12.0f + adjust_u, 8.0f / 12.0f + adjust_u,
						6.0f / 12.0f + adjust_u, 7.0f / 12.0f + adjust_u, 8.0f / 12.0f + adjust_u};
	vector<float> player_v = { 5.0f / 8.0f + adjust_v, 5.0f / 8.0f + adjust_v, 5.0f / 8.0f + adjust_v,
						6.0f / 8.0f + adjust_v, 6.0f / 8.0f + adjust_v, 6.0f / 8.0f + adjust_v };
	player->sprite = SheetSprite(spriteSheet3, player_u, player_v, 16.0f / 192.0f - 2 * adjust_u, 16.0f / 128.0f - 2 * adjust_v);
	player->dead = false;
	float playerSpawn_x, playerSpawn_y;
	tileToWorldCoordinates(spawn_x, spawn_y, &playerSpawn_x, &playerSpawn_y);
	player->position.y += playerSpawn_y;
	player->position.x += playerSpawn_x;
	player->entityID = -10;
	//player->speed = 5.0f;
	//player->x += (float) (-TILE_SIZE* mapWidth / 2);i
	//player->y += (float) (TILE_SIZE* mapHeight / 2);
	//entities.push_back(player);

	gun1 = new Entity();
	vector<float> gun_u = { 6.0f / 12.0f + adjust_u };
	vector<float> gun_v = { 5.0f / 8.0f + adjust_v };
	gun1->sprite = SheetSprite(spriteSheet3, gun_u, gun_v, 16.0f / 192.0f - 2 * adjust_u, 16.0f / 128.0f - 2 * adjust_v);
	gun1->position = player->position;
	gun1->scale_x = 0.5f;
	gun1->scale_y = 0.5f;
	gun1->speed = 2.0f;
	gun1->fric_x = 0.0f;
	gun1->fric_y = 0.0f;
	gun1->grav_x = 0.0f;
	gun1->grav_y = 0.0f;
	gun1->maxLifetime = 0.5f;
	gun1->entityID = -5;

	ParticleEmitterTest = ParticleEmitter(2.0f, 300);
	ParticleEmitterTest.position = Vector(player->position.x, player->position.y);
	ParticleEmitterTest.gravity = Vector(0.0f, 0.0f);
	ParticleEmitterTest.velocity = Vector(0.0f, 0.0f);
	//ParticleEmitterTest = ParticleEmitter(Vector(player->x, player->y), Vector(0.0f, 0.0f), Vector(0.0f, 0.0f), 2.0f, 300);
	ParticleEmitterTest.velocityDeviation = Vector(0.3f, 0.3f);

	//enemy
	float adjust_bat_u = 2.0f / 192.0f;
	float adjust_bat_v = 2.0f / 128.0f;
	//vector<float> enemy_u = { 3.0f / 12.0f + adjust_u, 4.0f / 12.0f + adjust_u, 5.0f / 12.0f + adjust_u,
	//	3.0f / 12.0f + adjust_u, 4.0f / 12.0f + adjust_u, 5.0f / 12.0f + adjust_u };
	//vector<float> enemy_v = { 5.0f / 8.0f + adjust_v, 5.0f / 8.0f + adjust_v, 5.0f / 8.0f + adjust_v,
	//	6.0f / 8.0f + adjust_v, 6.0f / 8.0f + adjust_v, 6.0f / 8.0f + adjust_v, };
	vector<float> enemy_u = { 3.0f / 12.0f + adjust_bat_u, 4.0f / 12.0f + adjust_bat_u, 5.0f / 12.0f + adjust_bat_u,
		3.0f / 12.0f + adjust_bat_u, 4.0f / 12.0f + adjust_bat_u, 5.0f / 12.0f + adjust_bat_u };
	vector<float> enemy_v = { 5.0f / 8.0f + adjust_bat_v, 5.0f / 8.0f + adjust_bat_v, 5.0f / 8.0f + adjust_bat_v,
		6.0f / 8.0f + adjust_bat_v, 6.0f / 8.0f + adjust_bat_v, 6.0f / 8.0f + adjust_bat_v, };
	SheetSprite enemyTexture = SheetSprite(spriteSheet3, enemy_u, enemy_v, 16.0f / 192.0f - 2 * adjust_u, 16.0f / 128.0f - 6.0f / 128.0f);
	for (int i = 0; i < NUM_DUMBMOBS; i++) {
		DumbMob* enemy = new DumbMob();
		enemy->sprite = enemyTexture;
		enemy->dead = true;
		enemy->grav_y = -9.8f;
		enemy->speed = 0.7f;
		enemy->entityID = DUMB_MOB;
		//enemy->velocity = Vector(9.0f, 0.0f);
		dumbMobs.push_back(enemy);
		enemies.push_back(enemy);
	}

	for (int i = 0; i < NUM_BATS; i++) {
		Bat* enemy = new Bat();
		enemy->sprite = enemyTexture;
		enemy->dead = true;
		enemy->grav_y = -3.75f;
		enemy->speed = 0.5f;
		enemy->entityID = BAT;
		bats.push_back(enemy);
		enemies.push_back(enemy);
	}

	//vector<float> u2 = { 6.0f / 12.0f + adrenderjust_u };
	//vector<float> v2 = { 5.0f / 8.0f + adjust_v };
	//BulletShooterTest = ParticleEmitter(0.5f, 10, SheetSprite(spriteSheet3, u, v, 16.0f / 192.0f - 2 * adjust_u, 16.0f / 128.0f - 2 * adjust_v));
	//BulletShooterTest.position = Vector(player->position.x, player->position.y);
	//BulletShooterTest.gravity = Vector(0.0f, 0.0f);
	//BulletShooterTest.velocity = Vector(0.0f, 0.0f);
	//BulletShooterTest.velocityDeviation = Vector(0.0f, 0.0f);

	/*SheetSprite asteroidTexture = SheetSprite(spriteSheet, 518.0f / 1024.0f, 810.0f / 1024.0f, 89.0f / 1024.0f, 82.0f / 1024.0f);
	for (int i = 0; i < NUM_ASTEROIDS; i++) {
		Entity* asteroid = new Entity();
		asteroid->sprite = asteroidTexture;
		asteroid->scale_x = 1.4f * randomFloat(0.5f, 1.1f);
		asteroid->scale_y = 1.4f * randomFloat(0.5f, 1.1f);
		asteroid->x = randomFloat(-1.33f, 1.33f);
		asteroid->y = randomFloat(-1.0f, 1.0f);
		asteroid->rotation = randomFloat(0.0f, 2.0f * (float)M_PI);
		asteroid->velocity.x = randomFloat(-0.3f, 0.3f);
		asteroid->velocity.y = randomFloat(-0.3f, 0.3f);
		asteroids.push_back(asteroid);
	}*/

}

bool Game::isSolid(unsigned char tile) {
	switch (tile) {
	case 0:
		return true;
		break;
	case 1:
		return true;
		break;
	case 2:
		return true;
		break;
	case 3:
		return true;
		break;
	case 16:
		return true;
		break;
	case 17:
		return true;
		break;
	case 18:
		return true;
		break;
	case 19:
		return true;
		break;
	case 32:
		return true;
		break;
	case 33:
		return true;
		break;
	case 34:
		return true;
		break;
	case 35:
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

}

void Game::tileToWorldCoordinates(int grid_x, int grid_y, float* x, float* y) {
	*x = (float)(grid_x * TILE_SIZE);
	*y = (float)(-grid_y * TILE_SIZE);
}

float Game::checkPointForGridCollisionX(float x, float y) {
	int grid_x, grid_y;
	worldToTileCoordinates(x, y, &grid_x, &grid_y);
	if (grid_x < 0 || grid_x > mapWidth || grid_y < 0 || grid_y > mapHeight) {
		return 0.0f;
	}

	if (isSolid(levelData[grid_y][grid_x])) {
		return 0.004f;
	}
	return 0.0f;
}

float Game::checkPointForGridCollisionY(float x, float y) {
	int grid_x, grid_y;
	worldToTileCoordinates(x, y, &grid_x, &grid_y);
	if (grid_x < 0 || grid_x > mapWidth || grid_y < 0 || grid_y > mapHeight) {
		return 0.0f;
	}

	if (isSolid(levelData[grid_y][grid_x])) {
		float yCoordinate = (grid_y * TILE_SIZE);
		return -y - yCoordinate;
	}
	return 0.0f;
}

void Game::doLevelCollisionX(Entity *entity) {
	//right
	float adjust = checkPointForGridCollisionX(entity->position.x + entity->sprite.width * entity->scale_x * 0.5f, entity->position.y);
	if (adjust != 0.0f) {
		entity->position.x -= adjust;
		entity->velocity.x = 0.0f;
		entity->collidedRight = true;
	}

	//left
	adjust = checkPointForGridCollisionX(entity->position.x - entity->sprite.width * entity->scale_x * 0.5f, entity->position.y);
	if (adjust != 0.0f) {
		entity->position.x += adjust;
		entity->velocity.x = 0.0f;
		entity->collidedLeft = true;
	}
}

void Game::doLevelCollisionY(Entity *entity) {
	//bottom
	float adjust = checkPointForGridCollisionY(entity->position.x, entity->position.y - entity->sprite.height *0.5f);
	if (adjust != 0.0f) {
		entity->position.y += adjust;
		entity->velocity.y = 0.0f;
		entity->collidedBot = true;
	}

	//top
	adjust = checkPointForGridCollisionY(entity->position.x, entity->position.y + entity->sprite.height * 0.5f);
	if (adjust != 0.0f) {
		entity->position.y -= adjust * 0.05f;
		entity->velocity.y = 0.0f;
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
			else {
				cellmap[i][j] = true;
			}
		}
	}
	//do sim steps
	for (int i = 0; i < NUM_SIMULATION_STEPS; i++) {
		cellmap = doSimulationStep(cellmap);
	}
	//transfer cellmap to leveldata
	bool spawnSet = false;
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			float temp = randomFloat(0.0f, 1.0f);
			if (i == 0 || j == 0 || i == mapWidth - 1 || j == mapHeight - 1) {
				levelData[i][j] = 3;
			}
			else if (cellmap[i][j]) {
				if (!cellmap[i-1][j]) {
					if (!spawnSet) {
						spawn_x = j;
						spawn_y = i;
						spawnSet = true;
					}
					if (!cellmap[i][j-1]) {
						levelData[i][j] = 0;
					}
					else if (!cellmap[i][j+1]) {
						levelData[i][j] = 2;
					}
					else {
						if (temp < 0.2f) {
							levelData[i][j] = 1;
						}
						else if (temp < 0.4f) {
							levelData[i][j] = 16;
						}
						else if (temp < 0.6f) {
							levelData[i][j] = 17;
						}
						else if (temp < 0.8f) {
							levelData[i][j] = 18;
						}
						else {
							levelData[i][j] = 19;
						}
					}
				}
				else if (temp < 0.35f) {
					levelData[i][j] = 32;
				}
				else if (temp < 0.5f) {
					levelData[i][j] = 33;
				}
				else if (temp < 0.65f) {
					levelData[i][j] = 34;
				}
				else {
					levelData[i][j] = 35;
				}
			}
			//background: 89, 90, 91, 92, 105, 106, 107, 108, 121, 122, 123, 124
			else {
				//if (temp < 0.083f) {
				//	levelData[i][j] = 89;
				//}
				//else if (temp < 0.167f) {
				//	levelData[i][j] = 90;
				//}
				//else if (temp < 0.25f) {
				//	levelData[i][j] = 91;
				//}
				//else if (temp < 0.333f) {
				//	levelData[i][j] = 92;
				//}
				//else if (temp < 0.417f) {
				//	levelData[i][j] = 105;
				//}
				//else if (temp < 0.5f) {
				//	levelData[i][j] = 106;
				//}
				//else if (temp < 0.583f) {
				//	levelData[i][j] = 107;
				//}
				//else if (temp < 0.667f) {
				//	levelData[i][j] = 108;
				//}
				//else if (temp < 0.75f) {
				//	levelData[i][j] = 121;
				//}
				//else if (temp < 0.833f) {
				//	levelData[i][j] = 122;
				//}
				//else if (temp < 0.917f) {
				//	levelData[i][j] = 123;
				//}
				//else {
				//	levelData[i][j] = 124;
				//}
				levelData[i][j] = 12;
			}
		}
	}
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

/*
	Type 1
		enemy spawn
*/
Vector Game::checkForValidSpawnLocation(int type) {
	float x = 0.0f;
	float y = 0.0f;
	int grid_x = 0;
	int grid_y = 0;
	if (type == 1) {
		//while leveldata[grid coord] IS solid or leveldata[gridcoord, y-1] is NOT solid or x < 0, x > width-1, y < 0, y > height-1
		//	if rand(0.0, 1.0) < 2.5
		//		x = player->pos.x - rand(1.33, 1.6) 
		//		y = player->pos.y - rand(1.0, 1.2)
		//	x, y to grid coordinates
		//while (isSolid(levelData[grid_x][grid_y]) || !isSolid(levelData[grid_x][grid_y - 1]))
		while ( isSolid(levelData[grid_y][grid_x]) || !isSolid(levelData[grid_y - 1][grid_x]) ||
				grid_x < 1 || grid_x > mapWidth - 1 || grid_y < 1 || grid_y > mapHeight - 1) {
			float rand = randomFloat(0.0f, 1.0f);
			if (rand < 0.25f) {
				x = player->position.x - randomFloat(0.0f, 2.0f);
				y = player->position.y - randomFloat(1.0f, 1.5f);
			}
			else if (rand < 0.50f) {
				x = player->position.x - randomFloat(1.33f, 2.0f);
				y = player->position.y + randomFloat(1.0f, 1.5f);
			}
			else if (rand < 0.75f) {
				x = player->position.x + randomFloat(1.33f, 2.0f);
				y = player->position.y + randomFloat(1.0f, 1.5f);
			}
			else {
				x = player->position.x + randomFloat(0.0f, 2.0f);
				y = player->position.y - randomFloat(1.0f, 1.5f);
			}

			int temp_x, temp_y;
			worldToTileCoordinates(x, y, &temp_x, &temp_y);
			if (temp_x > 0 && temp_x < mapWidth - 1 && temp_y > 0 && temp_y < mapHeight - 1) {
				grid_x = temp_x;
				grid_y = temp_y;
			}
		}
		return Vector(x, y);
	}
}
