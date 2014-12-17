 #pragma once
//shader
#include <GL/glew.h>
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
#include "stalagtite.h"
#include "bossOne.h"
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
#define SPRITE_COUNT_X_TER 13
#define SPRITE_COUNT_Y_TER 5
#define TILE_SIZE 0.10f

//#define MAX_ENEMIES 5
#define NUM_DUMBMOBS 10
#define DUMBMOB_SPAWN_TIMER 2.0f
#define NUM_BATS 5
#define BAT_SPAWN_TIMER 3.0f
#define NUM_STALAGTITES 7
#define STALAGTITE_SPAWN_TIMER 1.0f

#define ENEMY_TYPES 3
#define WEAPON_TYPES 2

enum GameState{ MAIN_MENU, GAME_PLAY, GAME_PAUSE, GAME_OVER, CUTSCENE, VICTORY};
enum EnemyID{ DUMB_MOB, BAT, STALAGTITE };

/*
entityID:
	-21 = bossOne fireball
	-10 = player
	-9 = altar
	-6 = mine
	-5 = fireball
	-1 = default
	0 = dumb mob
	1 = bat
	2 = stalagtite

weapons:
	1 = fireball
	2 = mine
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
	void RenderCutscene();
	void RenderVictory();
	void Update(float elapsed);
	void FixedUpdate();
private:
	//shader
	//GLuint vertexShaderID;
	//GLuint fragmentShaderID;
	//GLuint programID;
	//GLuint playerShaderID;
	//GLint projectionMatrixUniform;
	//GLint modelviewMatrixUniform;
	//GLuint positionAttribute;
	//GLuint texCoordAttribute;
	//GLint projectionMatrixUniformPlayer;
	//GLint modelviewMatrixUniformPlayer;
	//GLuint positionAttributePlayer;
	//GLuint texCoordAttributePlayer;

	const Uint8* keys;
	GameState state;
	bool done;
	bool paused;
	int score;
	float lastFrameTicks;
	float timeLeftOver;
	float timeElapsed;
	float cutsceneTime;
	bool screenShake;
	float screenShakeValue;
	float screenShakeTimer;
	float screenShakeStarted;
	vector<float> timeSinceLastSpawn;
	bool bossFight;
	int activeWeapon;
	//float timeSinceLastSpawn;

	SDL_Window* displayWindow;
	GLuint backgroundTexture;
	GLuint gameNameTexture;
	GLuint youWinTexture;
	GLuint healthBarTexture;
	GLuint fullHeartTexture;
	GLuint halfHeartTexture;
	GLuint emptyHeartTexture;
	GLuint inventoryTexture;
	GLuint inventoryFireball;
	GLuint inventoryMine;
	GLuint altarTexture;
	GLuint bossOneTexture;
	GLuint brickTileTexture;
	GLuint spriteSheet;
	GLuint spriteSheet2;
	GLuint spriteSheet3;
	GLuint explosionSpriteSheet;
	GLuint playerWizardTexture;
	GLuint zombieTexture;
	GLuint zombieTexture2;
	GLuint zombieTexture3;
	GLuint zombieTexture4;
	GLuint zombieTexture5;
	GLuint batTexture;
	GLuint stalagtiteTexture;
	GLuint fireballTexture;
	GLuint fireThingTexture;
	GLuint mineTexture;

	SheetSprite item;
	SheetSprite zombieSprite;
	SheetSprite zombieSprite2;
	SheetSprite zombieSprite3;
	SheetSprite zombieSprite4;
	SheetSprite zombieSprite5;

	vector<float> explosion_u;
	vector<float> explosion_v;
	vector<Explosion*> explosions;
	GLuint font;
	Mix_Music* music;
	Mix_Chunk* shoot;
	Mix_Chunk* enemyHit;
	Mix_Chunk* bombExplode;
	Mix_Chunk* selectSound;
	Mix_Chunk* interactSound;
	//Mix_Chunk* gameOver;
	//Mix_Chunk* blop;

	Entity* player;
	vector<Entity*> enemies;
	vector<DumbMob*> dumbMobs;
	vector<Bat*> bats;
	vector<Stalagtite*> stalagtites;
	vector<Entity*> enemyStuff;
	BossOne* bossOne;
	Entity* fireball;
	Entity* mine;
	Entity* altar;

	ParticleEmitter ParticleEmitterTest;
	//ParticleEmitter BulletShooterTest;
	//vector<Entity*> asteroids;
	
	int** levelData;
	bool** cellmap;
	int mapWidth;
	int mapHeight;
	int spawn_x;
	int spawn_y;
	vector<Vector> itemSpawns;
	float altarSpawn_x;
	float altarSpawn_y;

	//void explosion(int index, float x_pos, float y_pos);

	void reset();

	bool isSolid(int tile);
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
	//void spawnBoss();
};

Game::Game() {
	Initialize();

	keys = SDL_GetKeyboardState(NULL);

	state = MAIN_MENU;
	done = false;
	score = 0;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;
	cutsceneTime = 0.0f;
	//timeSinceLastSpawn = 0.0f;
	activeWeapon = 1;

	timeSinceLastSpawn.clear();
	for (size_t i = 0; i < ENEMY_TYPES; i++) {
		timeSinceLastSpawn.push_back(0.0f);
	}
	bossFight = false;
	//timeSinceLastSpawn[STALAGTITE] += 2.0f;

	srand(static_cast <unsigned> (time(0)));

	mapHeight = 100;
	mapWidth = 100;
	levelData = new int*[mapHeight];
	for (int i = 0; i < mapHeight; ++i) {
		levelData[i] = new int[mapWidth];
	}
	//cellmap = new bool[MAP_WIDTH][MAP_HEIGHT];

	//for (int y = 0; y < mapHeight; y++) {
	//	for (int x = 0; x < mapWidth; x++) {
	//		levelData[y][x] = 3;
	//	}s
	//}
	//buildLevel();
	
	//backgroundTexture = LoadTexture("background.png");
	backgroundTexture = LoadTexture("Wall_105.png");
	gameNameTexture = LoadTexture("name.png");
	youWinTexture = LoadTexture("youwin.png");
	healthBarTexture = LoadTexture("healthBar.png");
	fullHeartTexture = LoadTexture("heartFull.png");
	halfHeartTexture = LoadTexture("heartHalf.png");
	emptyHeartTexture = LoadTexture("heartEmpty.png");
	inventoryTexture = LoadTexture("inventoryBack.png");
	inventoryFireball = LoadTexture("inventory_fireball.png");
	inventoryMine = LoadTexture("inventory_mine.png");
	altarTexture = LoadTexture("altar.png");
	bossOneTexture = LoadTexture("bossOne.png");
	brickTileTexture = LoadTexture("brickTile.png");
	spriteSheet = LoadTexture("tilemap.png");
	spriteSheet2 = LoadTexture("arne_sprites.png");
	spriteSheet3 = LoadTexture("characters_1.png");
	playerWizardTexture = LoadTexture("playerWizard.png");
	zombieTexture = LoadTexture("zombie.png");
	zombieTexture2 = LoadTexture("zombie2.png");
	zombieTexture3 = LoadTexture("zombie3.png");
	zombieTexture4 = LoadTexture("zombie4.png");
	zombieTexture5 = LoadTexture("zombie5.png");
	batTexture = LoadTexture("bat.png");
	stalagtiteTexture = LoadTexture("boulder.png");
	fireballTexture = LoadTexture("fireball.png");
	fireThingTexture = LoadTexture("fireBallThing.png");
	mineTexture = LoadTexture("mine.png");

	float adjust_zombie_u = 1.0f / 72.0f;
	float adjust_zombie_v = 1.0f / 144.0f;
	vector<float> zombie_u = { 0.0f / 2.0f + adjust_zombie_u, 0.0f / 2.0f + adjust_zombie_u, 0.0f / 2.0f + adjust_zombie_u,
		1.0f / 2.0f + adjust_zombie_u, 1.0f / 2.0f + adjust_zombie_u, 1.0f / 2.0f + adjust_zombie_u };
	vector<float> zombie_v = { 0.0f / 3.0f + adjust_zombie_v, 1.0f / 3.0f + adjust_zombie_v, 2.0f / 3.0f + adjust_zombie_v,
		0.0f / 3.0f + adjust_zombie_v, 1.0f / 3.0f + adjust_zombie_v, 2.0f / 3.0f + adjust_zombie_v };
	zombieSprite = SheetSprite(zombieTexture, zombie_u, zombie_v, 1.0f / 2.0f - 2 * adjust_zombie_u, 1.0f / 3.0f - 2 * adjust_zombie_v);
	zombieSprite2 = SheetSprite(zombieTexture2, zombie_u, zombie_v, 1.0f / 2.0f - 2 * adjust_zombie_u, 1.0f / 3.0f - 2 * adjust_zombie_v);
	zombieSprite3 = SheetSprite(zombieTexture3, zombie_u, zombie_v, 1.0f / 2.0f - 2 * adjust_zombie_u, 1.0f / 3.0f - 2 * adjust_zombie_v);
	zombieSprite4 = SheetSprite(zombieTexture4, zombie_u, zombie_v, 1.0f / 2.0f - 2 * adjust_zombie_u, 1.0f / 3.0f - 2 * adjust_zombie_v);
	zombieSprite5 = SheetSprite(zombieTexture5, zombie_u, zombie_v, 1.0f / 2.0f - 2 * adjust_zombie_u, 1.0f / 3.0f - 2 * adjust_zombie_v);

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
	bombExplode = Mix_LoadWAV("bombExplode.wav");
	selectSound = Mix_LoadWAV("select.wav");
	interactSound = Mix_LoadWAV("interact.wav");
	//gameOver = Mix_LoadWAV("game_over.wav");
	//blop = Mix_LoadWAV("blop.wav");

	Matrix initial;
	glLoadMatrixf(initial.ml);

	//reset();
}

Game::~Game() {
	Mix_FreeMusic(music);
	Mix_FreeChunk(shoot);
	Mix_FreeChunk(enemyHit);
	Mix_FreeChunk(selectSound);
	Mix_FreeChunk(bombExplode);
	//Mix_FreeChunk(gameOver);
	//Mix_FreeChunk(blop);

	SDL_Quit();
}

void Game::Initialize() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glClearColor(0.0f, 0.2f, 0.7f, 1.0f);

	//shader
	//glewInit();
	//
	//GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	//const char *vertexShaderString = vertexShader.c_str();
	//int vertexShaderStringLength = vertexShader.size();
	//glShaderSource(vertexShaderID, 1, &vertexShaderString, &vertexShaderStringLength);
	//glCompileShader(vertexShaderID);
	//
	//GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	//const char *fragmentShaderString = fragmentShader.c_str();
	//int fragmentShaderStringLength = fragmentShader.size();
	//glShaderSource(fragmentShaderID, 1, &fragmentShaderString, &fragmentShaderStringLength);
	//glCompileShader(fragmentShaderID);
	//
	//programID = glCreateProgram();
	//glAttachShader(programID, vertexShaderID);
	//glAttachShader(programID, fragmentShaderID);
	//glLinkProgram(programID);
	//
	//playerShaderID = glCreateProgram();
	//glAttachShader(playerShaderID, vertexShaderID);
	//glAttachShader(playerShaderID, fragmentShaderID);
	//glLinkProgram(playerShaderID);
	//
	//glDeleteShader(vertexShaderID);
	//glDeleteShader(fragmentShaderID);
	//
	//projectionMatrixUniform = glGetUniformLocation(programID, "projection");
	//modelviewMatrixUniform = glGetUniformLocation(programID, "modelView");
	//positionAttribute = glGetAttribLocation(programID, "position");
	//texCoordAttribute = glGetAttribLocation(programID, "texCoord");
	//
	//projectionMatrixUniformPlayer = glGetUniformLocation(playerShaderID, "projection");
	//modelviewMatrixUniformPlayer = glGetUniformLocation(playerShaderID, "modelView");
	//positionAttributePlayer = glGetAttribLocation(playerShaderID, "position");
	//texCoordAttributePlayer = glGetAttribLocation(playerShaderID, "texCoord");

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	//shader
	//
	//Matrix projectionMatrix;
	//float l = -1.33f;
	//float r = 1.33f;
	//float t = 1.0f;
	//float b = -1.0f;
	//float f = 1.0f;
	//float n = -1.0f;
	//
	//projectionMatrix.identity();
	//projectionMatrix.m[0][0] = 2.0 / (r - l);
	//projectionMatrix.m[1][1] = 2.0 / (t - b);
	//projectionMatrix.m[2][2] = -2.0 / (f - n);
	//
	//Matrix modelviewMatrix;
	//
	//glUseProgram(programID);
	//glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, projectionMatrix.ml);
	//glUniformMatrix4fv(modelviewMatrixUniform, 1, GL_FALSE, modelviewMatrix.ml);
	//glDisableVertexAttribArray(positionAttribute);
	//glDisableVertexAttribArray(texCoordAttribute);
}

bool Game::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (keys[SDL_SCANCODE_E]) {
				if (activeWeapon == WEAPON_TYPES) {
					activeWeapon = 1;
				}
				else {
					activeWeapon++;
				}
			}
			else if (keys[SDL_SCANCODE_Q]) {
				if (activeWeapon == 1) {
					activeWeapon = WEAPON_TYPES;
				}
				else {
					activeWeapon--;
				}
			}
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
	else if (keys[SDL_SCANCODE_5]) { state = CUTSCENE; }
	else if (keys[SDL_SCANCODE_6]) { state = VICTORY; }

	if (keys[SDL_SCANCODE_ESCAPE]) { SDL_Quit(); }
		
	switch (state) {
	case MAIN_MENU:
		if (event.button.button == 1 && event.type == SDL_MOUSEBUTTONDOWN) {
			//Matrix initial;
			//glLoadMatrixf(initial.ml);
			Mix_PlayChannel(-1, selectSound, 0);
			itemSpawns.clear();
			reset();
			Mix_PlayMusic(music, -1);
			state = GAME_PLAY;
		}
		break;
	case GAME_PLAY:
		//fireball
		if (event.button.button == 1 && event.type == SDL_MOUSEBUTTONDOWN && activeWeapon == 1) {
			if (!fireball->bulletActive) {
				fireball->bulletReset(player->position);
				fireball->bulletShoot(Vector(((((float)event.motion.x / 800.0f) * 2.666f) - 1.333f),
					(((float)(600 - event.motion.y) / 600.0f) * 2.0f) - 1.0f));
				//Mix_PlayChannel(-1, shoot, 0);
			}
		}
		//mine
		else if (event.button.button == 1 && event.type == SDL_MOUSEBUTTONDOWN && activeWeapon == 2) {
			if (!mine->bulletActive) {
				mine->bulletReset(player->position);
				mine->bulletShoot(Vector(((((float)event.motion.x / 800.0f) * 2.666f) - 1.333f),
					(((float)(600 - event.motion.y) / 600.0f) * 2.0f) - 1.0f));
				mine->velocity = mine->velocity * 2.0f;
				mine->randomUse = false;
				mine->rotation = 0.0f;
			}
		}
			
		if (keys[SDL_SCANCODE_W]) {
			player->hover();
			//if (!player->isJumping) {
			//	player->jump();
			//}
		}
		else {
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

		//interaction
		if (keys[SDL_SCANCODE_R]) {
			//activate altar
			Mix_PlayChannel(-1, interactSound, 0);
			if (fabs(player->position.x - altarSpawn_x) < 0.15f && 
				fabs(player->position.y - altarSpawn_y) < 0.15f) {
				player->position.x = altarSpawn_x - 0.003f;
				player->position.y = altarSpawn_y;
				for (size_t i = 0; i < enemies.size(); i++) {
					enemies[i]->dead = true;
				}
				//bossFight = true;
				state = CUTSCENE;
			}
		}

		if (keys[SDL_SCANCODE_P]) {
			paused = true;
			state = GAME_PAUSE;
			//fireball->bulletActive = false;
		}
		break;
	case GAME_PAUSE:
		if (keys[SDL_SCANCODE_O]) {
			paused = false;
			state = GAME_PLAY;
			//fireball->bulletActive = true;
		}
		break;
	case GAME_OVER:
		if (event.button.button == 3 && event.type == SDL_MOUSEBUTTONDOWN) {
			Mix_PlayChannel(-1, selectSound, 0);
			state = MAIN_MENU; 
			//Mix_HaltChannel(3);
		}
		break;
	case CUTSCENE:
		break;
	case VICTORY:
		if (event.button.button == 3 && event.type == SDL_MOUSEBUTTONDOWN) {
			//Matrix initial;
			//glLoadMatrixf(initial.ml);
			//Mix_HaltChannel(3);
			Mix_PlayChannel(-1, selectSound, 0);
			state = MAIN_MENU;
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
	case CUTSCENE:
		RenderGame();
		RenderCutscene();
		break;
	case VICTORY:
		RenderGame();
		RenderVictory();
		break;
	}
	SDL_GL_SwapWindow(displayWindow);
}

void Game::RenderLevel() {
	glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, spriteSheet2);
	glBindTexture(GL_TEXTURE_2D, spriteSheet);

	vector<float> vertexData;
	vector<float> textureCoordData;

	int player_grid_x, player_grid_y;
	worldToTileCoordinates(player->position.x, player->position.y, &player_grid_x, &player_grid_y);
	int altar_grid_x, altar_grid_y;
	worldToTileCoordinates(altarSpawn_x, altarSpawn_y, &altar_grid_x, &altar_grid_y);


	int numVertices = 0;
	
	if (state == CUTSCENE || bossFight) {
		for (int y = 0; y < mapHeight; y++) {
			for (int x = 0; x < mapWidth; x++) {
				if (abs(x - altar_grid_x) < 11 &&
					y < altar_grid_y + 2 &&
					y > altar_grid_y - 20
					//true
					)
				{
					float adjust_u = 1.5f / 468.0f;
					float adjust_v = 1.5f / 180.0f;
					float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X_TER) / (float)SPRITE_COUNT_X_TER;
					float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X_TER) / (float)SPRITE_COUNT_Y_TER;
					float spriteWidth = (1.0f / (float)SPRITE_COUNT_X_TER) - (2 * adjust_u);
					float spriteHeight = (1.0f / (float)SPRITE_COUNT_Y_TER) - (2 * adjust_v);
					//vertexData.insert(vertexData.end(), {
					//	TILE_SIZE* x - adjust_u, -TILE_SIZE* y + adjust_v,
					//	TILE_SIZE* x - adjust_u, (-TILE_SIZE* y) - TILE_SIZE - adjust_v,
					//	(TILE_SIZE* x) + TILE_SIZE + adjust_u, (-TILE_SIZE* y) - TILE_SIZE - adjust_v,
					//	(TILE_SIZE* x) + TILE_SIZE + adjust_u, -TILE_SIZE* y + adjust_v
					//});
					//textureCoordData.insert(textureCoordData.end(), { u + adjust_u, v + adjust_v,
					//	u + adjust_u, v + spriteHeight - adjust_v,
					//	u + spriteWidth - adjust_u, v + spriteHeight - adjust_v,
					//	u + spriteWidth - adjust_u, v + adjust_v
					//});
					vertexData.insert(vertexData.end(), {
						TILE_SIZE* x, -TILE_SIZE* y,
						TILE_SIZE* x, (-TILE_SIZE* y) - TILE_SIZE,
						(TILE_SIZE* x) + TILE_SIZE, (-TILE_SIZE* y) - TILE_SIZE,
						(TILE_SIZE* x) + TILE_SIZE, -TILE_SIZE* y
					});
					textureCoordData.insert(textureCoordData.end(), {
						u + adjust_u, v + adjust_v,
						u + adjust_u, v + spriteHeight - adjust_v,
						u + spriteWidth - adjust_u, v + spriteHeight - adjust_v,
						u + spriteWidth - adjust_u, v + adjust_v
					});

					numVertices += 4;
				}
			}
		}
	}
	else {
		for (int y = 0; y < mapHeight; y++) {
			for (int x = 0; x < mapWidth; x++) {
				if (abs(x - player_grid_x) < 12 &&
					abs(y - player_grid_y) < 12
					//true
					)
				{
					float adjust_u = 1.5f / 468.0f;
					float adjust_v = 1.5f / 180.0f;
					float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X_TER) / (float)SPRITE_COUNT_X_TER;
					float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X_TER) / (float)SPRITE_COUNT_Y_TER;
					float spriteWidth = (1.0f / (float)SPRITE_COUNT_X_TER) - (2 * adjust_u);
					float spriteHeight = (1.0f / (float)SPRITE_COUNT_Y_TER) - (2 * adjust_v);
					//vertexData.insert(vertexData.end(), {
					//	TILE_SIZE* x - adjust_u, -TILE_SIZE* y + adjust_v,
					//	TILE_SIZE* x - adjust_u, (-TILE_SIZE* y) - TILE_SIZE - adjust_v,
					//	(TILE_SIZE* x) + TILE_SIZE + adjust_u, (-TILE_SIZE* y) - TILE_SIZE - adjust_v,
					//	(TILE_SIZE* x) + TILE_SIZE + adjust_u, -TILE_SIZE* y + adjust_v
					//});
					//textureCoordData.insert(textureCoordData.end(), { u + adjust_u, v + adjust_v,
					//	u + adjust_u, v + spriteHeight - adjust_v,
					//	u + spriteWidth - adjust_u, v + spriteHeight - adjust_v,
					//	u + spriteWidth - adjust_u, v + adjust_v
					//});
					vertexData.insert(vertexData.end(), {
						TILE_SIZE* x, -TILE_SIZE* y,
						TILE_SIZE* x, (-TILE_SIZE* y) - TILE_SIZE,
						(TILE_SIZE* x) + TILE_SIZE, (-TILE_SIZE* y) - TILE_SIZE,
						(TILE_SIZE* x) + TILE_SIZE, -TILE_SIZE* y
					});
					textureCoordData.insert(textureCoordData.end(), {
						u + adjust_u, v + adjust_v,
						u + adjust_u, v + spriteHeight - adjust_v,
						u + spriteWidth - adjust_u, v + spriteHeight - adjust_v,
						u + spriteWidth - adjust_u, v + adjust_v
					});

					numVertices += 4;
				}
			}
		}
	}

	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	//glEnableVertexAttribArray(positionAttribute);

	glTexCoordPointer(2, GL_FLOAT, 0, textureCoordData.data());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, false, 0, textureCoordData.data());
	//glEnableVertexAttribArray(texCoordAttribute);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glTranslatef(-TILE_SIZE* mapWidth / 2, TILE_SIZE* mapHeight / 2, 0.0f);

	glDrawArrays(GL_QUADS, 0, numVertices);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	//glDisableVertexAttribArray(positionAttribute);
	//glDisableVertexAttribArray(texCoordAttribute);
	//glPopMatrix();
}

void Game::RenderMainMenu() {
	glLoadIdentity();
	//Matrix identity;
	//glUniformMatrix4fv(modelviewMatrixUniform, 1, GL_FALSE, identity.ml);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	drawSprite(backgroundTexture, 0.0f, 0.0f, 0.0f, 2.0f, 2.0f);

	float animationValue = mapValue(timeElapsed, 0.0f, 3.0f, 0.0f, 1.0f);
	//drawText(modelviewMatrixUniform, positionAttribute, texCoordAttribute, font, "<Game Name>", 0.3f, -0.13f, easeOut(-3.0f, -0.85f, animationValue), 0.7f);
	//drawText(font, "<Game Name>", 0.3f, -0.13f, easeOut(-3.0f, -0.85f, animationValue), 0.7f);
	drawSprite(gameNameTexture, 0.0f, easeOutElastic(3.0f, 0.3f, animationValue), 0.0f, 1.0f, 2.0f);
	//drawText(font, "<Game Name>", 0.3f, -0.13f, -0.85f, 0.7f);
	animationValue = mapValue(timeElapsed, 1.5f, 2.3f, 0.0f, 1.0f);
	//drawText(modelviewMatrixUniform, positionAttribute, texCoordAttribute, font, "Click to Start Game", 0.1f, -0.05f, easeOut(-2.25f, -0.42f, animationValue), 0.2f);
	drawText(font, "Click to begin", 0.075f, -0.03f, easeOut(-2.25f, -0.34f, animationValue), 0.0f);

	animationValue = mapValue(timeElapsed, 2.0f, 2.8f, 0.0f, 1.0f);
	//drawText(modelviewMatrixUniform, positionAttribute, texCoordAttribute, font, "WASD to move", 0.1f, -0.05f, easeOut(-2.0, -0.23f, animationValue), -0.2f);
	drawText(font, "WASD to move, QE to switch weapons", 0.06f, -0.03f, easeOut(-3.5, -0.5f, animationValue), -0.3f);

	animationValue = mapValue(timeElapsed, 2.5f, 3.3f, 0.0f, 1.0f);
	//drawText(modelviewMatrixUniform, positionAttribute, texCoordAttribute, font, "mouse to aim, left click to shoot", 0.1f, -0.05f, easeOut(-3.0f, -0.8f, animationValue), -0.4f);
	drawText(font, "Use the mouse to aim and shoot", 0.06f, -0.03f, easeOut(-3.5f, -0.42f, animationValue), -0.45f);

	animationValue = mapValue(timeElapsed, 3.0f, 3.8f, 0.0f, 1.0f);
	//drawText(modelviewMatrixUniform, positionAttribute, texCoordAttribute, font, "bullets can break blocks", 0.1f, -0.05f, easeOut(-2.5f, -0.53f, animationValue), -0.6f);
	drawText(font, "Bombs can be used to break blocks", 0.06f, -0.03f, easeOut(-3.5f, -0.49f, animationValue), -0.6f);

	animationValue = mapValue(timeElapsed, 3.5f, 4.3f, 0.0f, 1.0f);
	drawText(font, "R to interact", 0.06f, -0.03f, easeOut(-2.5f, -0.16f, animationValue), -0.75f);

	animationValue = mapValue(timeElapsed, 4.0f, 4.5f, 0.0f, 1.0f);
	drawText(font, "ESC to quit", 0.04f, -0.02f, easeOut(-2.5f, 0.7f, animationValue), -0.9f);

	//animationValue = mapValue(timeElapsed, 5.0f, 6.0f, 0.0f, 1.0f);
	//drawText(font, "press 1 to restart game", 0.1f, -0.05f, easeOut(-2.5f, -0.5f, animationValue), -0.8f);
}

void Game::RenderGame() {

	glLoadIdentity();
	//Matrix identity;
	//glUniformMatrix4fv(modelviewMatrixUniform, 1, GL_FALSE, identity.ml);

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
	if (state == CUTSCENE) {
		trans_x = -altarSpawn_x;
		trans_y = -altarSpawn_y - 0.9f;
	}
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(trans_x, trans_y, 0.0f);
	//Matrix matrix;
	//float tx = trans_x;
	//float ty = trans_y;
	//matrix.identity();
	//matrix.m[3][0] = trans_x;
	//matrix.m[3][1] = trans_y;

	if (state == GAME_OVER) {
		//float screenShake_x = sin(screenShakeValue * 70.0f) * 0.02f;
		//float screenShake_y = sin(screenShakeValue * 50.0f) * 0.02f;
		//Matrix screenShake;
		//screenShake.m[3][0] = screenShake_x;a
		glTranslatef(sin(screenShakeValue * 70.0f) * 0.02f, sin(screenShakeValue * 50.0f) * 0.02f, 0.0f);
	}
	if (screenShake || state == CUTSCENE) {
		glTranslatef(sin(screenShakeValue * 70.0f) * 0.01f, sin(screenShakeValue * 50.0f) * 0.01f, 0.0f);
	}
	//glUniformMatrix4fv(modelviewMatrixUniform, 1, GL_FALSE, matrix.ml);

	//glLoadIdentity();
	//glMatrixMode(GL_MODELVIEW);
	//glTranslatef(trans_x, trans_y, 0.0f);

	for (size_t i = 0; i < enemies.size(); i++) {
		if (!enemies[i]->dead) {
			//enemies[i]->Render(modelviewMatrixUniform, positionAttribute, texCoordAttribute);
			enemies[i]->Render();
		}
	}
	RenderLevel();
	altar->Render();
	//player->Render(modelviewMatrixUniform, positionAttribute, texCoordAttribute);
	player->Render();
	if (!bossOne->dead) {
		bossOne->Render();
	}
	if (fireball->bulletActive) {
		//fireball->Render(modelviewMatrixUniform, positionAttribute, texCoordAttribute);
		fireball->Render();
	}
	if (mine->bulletActive) {
		//mine->Render(modelviewMatrixUniform, positionAttribute, texCoordAttribute);
		mine->Render();
	}
	for (size_t i = 0; i < enemyStuff.size(); i++) {
		if (!enemyStuff[i]->dead) {
			enemyStuff[i]->Render();
		}
	}
	for (size_t i = 0; i < explosions.size(); i++) {
		explosions[i]->Render();

	}
	ParticleEmitterTest.Render();

	//drawSprite(backgroundTexture, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	//drawText(modelviewMatrixUniform, positionAttribute, texCoordAttribute, font, "Score:" + to_string(score), 0.1f, -0.05f, -0.95f, -0.95f);
	drawText(font, "Score:" + to_string(score), 0.1f, -0.05f, -0.95f, -0.95f);
	drawSprite(inventoryTexture, 0.86f, 0.86f, 0.0f, 0.2f, 0.2f);
	if (activeWeapon == 1) {
		drawSprite(inventoryFireball, 0.861f, 0.856f, 0.0f, 0.12f, 0.12f);
	}
	else if (activeWeapon == 2) {
		drawSprite(inventoryMine, 0.861f, 0.856f, 0.0f, 0.12f, 0.12f);
	}
	double healthLeftToDraw = player->currentHealth;
	//double healthLeftToDraw = 75.0;
	for (size_t i = 0; i < 10; i++) {
		if (healthLeftToDraw >= 10.0) {
			drawSprite(fullHeartTexture, -0.90f + i * 0.12f, 0.90f, 0.0f, 0.12f, 0.12f);
		}
		else if (healthLeftToDraw >= 5.0) {
			drawSprite(halfHeartTexture, -0.90f + i * 0.12f, 0.90f, 0.0f, 0.12f, 0.12f);
		}
		else {
			drawSprite(emptyHeartTexture, -0.90f + i * 0.12f, 0.90f, 0.0f, 0.12f, 0.12f);
		}
		healthLeftToDraw -= 10.0;
	}
	//BulletShooterTest.Render();
}

void Game::RenderGameOver() {
	glLoadIdentity();
	//glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	//glClear(GL_COLOR_BUFFER_BIT);

	//drawText(modelviewMatrixUniform, positionAttribute, texCoordAttribute, font, "GAME OVER", 0.3f, -0.1f, -0.8f, 0.45f);
	//drawText(modelviewMatrixUniform, positionAttribute, texCoordAttribute, font, "Final Score:", 0.2f, -0.1f, -0.5f, 0.1f);
	//drawText(modelviewMatrixUniform, positionAttribute, texCoordAttribute, font, to_string(score), 0.2f, -0.1f, -0.15f, -0.1f);
	//drawText(modelviewMatrixUniform, positionAttribute, texCoordAttribute, font, "Right click to return to Main Menu", 0.1f, -0.05f, -0.8f, -0.6f);
	drawText(font, "GAME OVER", 0.3f, -0.1f, -0.8f, 0.45f);
	drawText(font, "Final Score:", 0.2f, -0.1f, -0.5f, 0.1f);
	drawText(font, to_string(score), 0.2f, -0.1f, -0.15f, -0.1f);
	drawText(font, "Right click to return to Main Menu", 0.1f, -0.05f, -0.8f, -0.6f);

}

void Game::RenderCutscene() {
	glLoadIdentity();

	float animationValue = mapValue(cutsceneTime, 0.0f, 0.5f, 0.0f, 1.0f);
	drawSprite(brickTileTexture, 0.0f, easeIn(0.0f, -1.0f, animationValue), 0.0f, 0.1f, 0.1f);

	for (float i = 0.1f; i < 1.1f; i = i + 0.1f) {
		animationValue = mapValue(cutsceneTime, i, 0.5f + i, 0.0f, 1.0f);
		drawSprite(brickTileTexture, easeIn(0.0f, i, animationValue), easeIn(0.0f, -1.0f, animationValue), 0.0f, 0.1f, 0.1f);
		drawSprite(brickTileTexture, easeIn(0.0f, -i, animationValue), easeIn(0.0f, -1.0f, animationValue), 0.0f, 0.1f, 0.1f);
	}
	for (float i = -1.0f; i < 1.1f; i = i + 0.1f) {
		animationValue = mapValue(cutsceneTime, 2.1f + i, 2.6f + i, 0.0f, 1.0f);
		drawSprite(brickTileTexture, easeIn(0.0f, 1.0f, animationValue), easeIn(0.0f, i, animationValue), 0.0f, 0.1f, 0.1f);
		drawSprite(brickTileTexture, easeIn(0.0f, -1.0f, animationValue), easeIn(0.0f, i, animationValue), 0.0f, 0.1f, 0.1f);
	}
	for (float i = 1.0f; i > 0.0f; i = i - 0.1f) {
		animationValue = mapValue(cutsceneTime, 4.1f - i, 4.6f - i, 0.0f, 1.0f);
		drawSprite(brickTileTexture, easeIn(0.0f, i, animationValue), easeIn(0.0f, 1.0f, animationValue), 0.0f, 0.1f, 0.1f);
		drawSprite(brickTileTexture, easeIn(0.0f, -i, animationValue), easeIn(0.0f, 1.0f, animationValue), 0.0f, 0.1f, 0.1f);
	}
	animationValue = mapValue(cutsceneTime, 4.1f, 4.6f, 0.0f, 1.0f);
	drawSprite(brickTileTexture, 0.0f, easeIn(0.0f, 1.0f, animationValue), 0.0f, 0.1f, 0.1f);

	for (float i = 0.1f; i < 0.5f; i = i + 0.1f) {
		animationValue = mapValue(cutsceneTime, 4.1f + i, 4.6f + i, 0.0f, 1.0f);
		drawSprite(brickTileTexture, easeIn(0.0f, 0.3f + i, animationValue), easeIn(0.0f, 0.4f, animationValue), 0.0f, 0.1f, 0.1f);
		drawSprite(brickTileTexture, easeIn(0.0f, -0.3f - i, animationValue), easeIn(0.0f, 0.4f, animationValue), 0.0f, 0.1f, 0.1f);
	}
	for (float i = 0.1f; i < 0.3f; i = i + 0.1f) {
		animationValue = mapValue(cutsceneTime, 4.5f + i, 5.0f + i, 0.0f, 1.0f);
		drawSprite(brickTileTexture, easeIn(0.0f, 0.3f - i, animationValue), easeIn(0.0f, -0.1f, animationValue), 0.0f, 0.1f, 0.1f);
		drawSprite(brickTileTexture, easeIn(0.0f, -0.3f + i, animationValue), easeIn(0.0f, -0.1f, animationValue), 0.0f, 0.1f, 0.1f);
	}
	animationValue = mapValue(cutsceneTime, 4.8f, 5.3f, 0.0f, 1.0f);
	drawSprite(brickTileTexture, easeIn(0.0f, 0.0f, animationValue), easeIn(0.0f, -0.1f, animationValue), 0.0f, 0.1f, 0.1f);
	for (float i = 0.1f; i < 0.5f; i = i + 0.1f) {
		animationValue = mapValue(cutsceneTime, 4.8f + i, 5.3f + i, 0.0f, 1.0f);
		drawSprite(brickTileTexture, easeIn(0.0f, 0.3f + i, animationValue), easeIn(0.0f, -0.6f, animationValue), 0.0f, 0.1f, 0.1f);
		drawSprite(brickTileTexture, easeIn(0.0f, -0.3f - i, animationValue), easeIn(0.0f, -0.6f, animationValue), 0.0f, 0.1f, 0.1f);
	}
	//float animationValue = mapValue(timeElapsed, 0.0f, 3.0f, 0.0f, 1.0f);
	//drawText(font, "<Game Name>", 0.3f, -0.13f, easeOut(-3.0f, -0.85f, animationValue), 0.7f);
}

void Game::RenderVictory() {
	glLoadIdentity();

	//drawText(font, "GAME OVER", 0.3f, -0.1f, -0.8f, 0.45f);
	drawSprite(youWinTexture, 0.0f, 0.3f, 0.0f, 1.0f, 2.0f);
	drawText(font, "Final Score:", 0.2f, -0.1f, -0.5f, 0.0f);
	drawText(font, to_string(score), 0.2f, -0.1f, -0.15f, -0.2f);
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
	for (size_t i = 0; i < ENEMY_TYPES; i++) {
		timeSinceLastSpawn[i] += FIXED_TIMESTEP;
	}
	//timeSinceLastSpawn += FIXED_TIMESTEP;
	float animation;

	switch (state) {
	case GAME_PLAY:
		//particle emitter test
		if (player->isIdle && fabs(player->velocity.y) < 0.2  && !player->isHovering) {
			ParticleEmitterTest.active = false;
		}
		else if (player->isHovering) {
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
				dumbMobs[i]->currentHealth = 0;
				dumbMobs[i]->dead = true;
			}

			if (dumbMobs[i]->dead && timeSinceLastSpawn[DUMB_MOB] > DUMBMOB_SPAWN_TIMER && !bossFight) {
				//Vector temp = checkForValidSpawnLocation(1);
				//if (temp.x != 0.0f && temp.y != 0.0f) {
				dumbMobs[i]->dead = false;
				dumbMobs[i]->currentHealth = dumbMobs[i]->maxHealth;
				//float scale = randomFloat(0.25f, 0.25f);
				//dumbMobs[i]->scale_x = 0.7f;
				//dumbMobs[i]->scale_y = 0.7f;
				float rand = randomFloat(0.0f, 1.0f);
				if (rand < 0.2f) { dumbMobs[i]->sprite = zombieSprite; }
				else if (rand < 0.4f) { dumbMobs[i]->sprite = zombieSprite2; }
				else if (rand < 0.6f) { dumbMobs[i]->sprite = zombieSprite3; }
				else if (rand < 0.8f) { dumbMobs[i]->sprite = zombieSprite4; }
				else { dumbMobs[i]->sprite = zombieSprite5; }

				dumbMobs[i]->position = checkForValidSpawnLocation(1);
				dumbMobs[i]->jumpRightCheck = Vector(dumbMobs[i]->position.x + 0.05f, dumbMobs[i]->position.y);
				dumbMobs[i]->jumpLeftCheck = Vector(dumbMobs[i]->position.x - 0.05f, dumbMobs[i]->position.y);
				dumbMobs[i]->highJumpRightCheck = Vector(dumbMobs[i]->position.x + 0.05f, dumbMobs[i]->position.y + 0.1f);
				dumbMobs[i]->highJumpLeftCheck = Vector(dumbMobs[i]->position.x - 0.05f, dumbMobs[i]->position.y + 0.1f);
				dumbMobs[i]->fallRightCheck1 = Vector(dumbMobs[i]->position.x + 0.09f, dumbMobs[i]->position.y - 0.1f);
				dumbMobs[i]->fallRightCheck2 = Vector(dumbMobs[i]->position.x + 0.09f, dumbMobs[i]->position.y - 0.2f);
				dumbMobs[i]->fallRightCheck3 = Vector(dumbMobs[i]->position.x + 0.09f, dumbMobs[i]->position.y - 0.3f);
				dumbMobs[i]->fallLeftCheck1 = Vector(dumbMobs[i]->position.x - 0.09f, dumbMobs[i]->position.y - 0.1f);
				dumbMobs[i]->fallLeftCheck2 = Vector(dumbMobs[i]->position.x - 0.09f, dumbMobs[i]->position.y - 0.2f);
				dumbMobs[i]->fallLeftCheck3 = Vector(dumbMobs[i]->position.x - 0.09f, dumbMobs[i]->position.y - 0.3f);
				//dumbMobs[i]->velocity.x = randomFloat(8.0f, 11.0f);
				dumbMobs[i]->jumpTimeGap = randomFloat(1.0f, 3.0f);
				timeSinceLastSpawn[DUMB_MOB] = 0.0f;
				//}
				//timeSinceLastSpawn[DUMB_MOB] = 0.0f;
			}

			if (!dumbMobs[i]->dead) {
				if (checkCollision(dumbMobs[i], player)) {
					//Mix_HaltMusic();
					//Mix_PlayChannel(3, gameOver, 0);
					//state = GAME_OVER;
					if (!player->invulnerable) {
						player->currentHealth -= 20.0;
						Mix_PlayChannel(-1, enemyHit, 0);
						//if (dumbMobs[i]->position.x < player->position.x) {
						//Vector temp = Vector(player->position.x, player->position.y) - Vector(dumbMobs[i]->position.x, dumbMobs[i]->position.y);
						Vector temp = player->position - dumbMobs[i]->position;
						temp.normalize();
						temp = temp * 2.0f;
						player->velocity = temp;

						screenShake = true;
						screenShakeStarted = timeElapsed;
						screenShakeTimer = 0.4f;
						//player->velocity = Vector(2.5f, 1.0f);
						//}
						//else {
						//player->velocity = Vector(-2.5f, 1.0f);
						//}
					}
					if (player->currentHealth <= 0.0) {
						player->currentHealth = 0.0;
						state = GAME_OVER;
					}
					else {
						player->invulnerable = true;
					}
				}
				if (player->invulnerableFrames > 2.0f) {
					player->invulnerable = false;
					player->invulnerableFrames = 0.0f;
				}
				if (!dumbMobs[i]->isJumping) {
					if (dumbMobs[i]->facingRight) {
						int temp_x, temp_y;
						worldToTileCoordinates(dumbMobs[i]->jumpRightCheck.x, dumbMobs[i]->jumpRightCheck.y, &temp_x, &temp_y);
						if (isSolid(levelData[temp_y][temp_x])) {
							worldToTileCoordinates(dumbMobs[i]->highJumpRightCheck.x, dumbMobs[i]->highJumpRightCheck.y, &temp_x, &temp_y);
							if (isSolid(levelData[temp_y][temp_x])) {
								dumbMobs[i]->jump2();
							}
							else {
								dumbMobs[i]->jump();
							}
							dumbMobs[i]->timeSinceLastJump = 0.0f;
						}
						//worldToTileCoordinates(dumbMobs[i]->highJumpRightCheck.x, dumbMobs[i]->highJumpRightCheck.y, &temp_x, &temp_y);
						//if (isSolid(levelData[temp_y][temp_x])) {
						//	dumbMobs[i]->jump2();
						//	dumbMobs[i]->timeSinceLastJump = 0.0f;
						//}
					}
					else {
						int temp_x, temp_y;
						worldToTileCoordinates(dumbMobs[i]->jumpLeftCheck.x, dumbMobs[i]->jumpLeftCheck.y, &temp_x, &temp_y);
						if (isSolid(levelData[temp_y][temp_x])) {
							worldToTileCoordinates(dumbMobs[i]->highJumpLeftCheck.x, dumbMobs[i]->highJumpLeftCheck.y, &temp_x, &temp_y);
							if (isSolid(levelData[temp_y][temp_x])) {
								dumbMobs[i]->jump2();
							}
							else {
								dumbMobs[i]->jump();
							}
							dumbMobs[i]->timeSinceLastJump = 0.0f;
						}
						//worldToTileCoordinates(dumbMobs[i]->highJumpLeftCheck.x, dumbMobs[i]->highJumpLeftCheck.y, &temp_x, &temp_y);
						//if (isSolid(levelData[temp_y][temp_x])) {
						//	dumbMobs[i]->jump2();
						//	dumbMobs[i]->timeSinceLastJump = 0.0f;
						//}
					}
				}
				if (dumbMobs[i]->collidedBot) {
					if (dumbMobs[i]->facingRight) {
						int temp_x1, temp_x2, temp_x3;
						int temp_y1, temp_y2, temp_y3;
						worldToTileCoordinates(dumbMobs[i]->fallRightCheck1.x, dumbMobs[i]->fallRightCheck1.y, &temp_x1, &temp_y1);
						worldToTileCoordinates(dumbMobs[i]->fallRightCheck2.x, dumbMobs[i]->fallRightCheck2.y, &temp_x2, &temp_y2);
						worldToTileCoordinates(dumbMobs[i]->fallRightCheck3.x, dumbMobs[i]->fallRightCheck3.y, &temp_x3, &temp_y3);
						//make sure coordinates exist
						if (temp_y3 < mapHeight - 2) {
							if (!isSolid(levelData[temp_y1][temp_x1]) &&
								!isSolid(levelData[temp_y2][temp_x2]) &&
								!isSolid(levelData[temp_y3][temp_x3])) {
								dumbMobs[i]->facingRight = false;
							}
						}
					}
					else {
						int temp_x1, temp_x2, temp_x3;
						int temp_y1, temp_y2, temp_y3;
						worldToTileCoordinates(dumbMobs[i]->fallLeftCheck1.x, dumbMobs[i]->fallLeftCheck1.y, &temp_x1, &temp_y1);
						worldToTileCoordinates(dumbMobs[i]->fallLeftCheck2.x, dumbMobs[i]->fallLeftCheck2.y, &temp_x2, &temp_y2);
						worldToTileCoordinates(dumbMobs[i]->fallLeftCheck3.x, dumbMobs[i]->fallLeftCheck3.y, &temp_x3, &temp_y3);
						//make sure coordinates exist
						if (temp_y3 < mapHeight - 2) {
							if (!isSolid(levelData[temp_y1][temp_x1]) &&
								!isSolid(levelData[temp_y2][temp_x2]) &&
								!isSolid(levelData[temp_y3][temp_x3])) {
								dumbMobs[i]->facingRight = true;
							}
						}
					}
				}

				dumbMobs[i]->FixedUpdate();
				//dumbMobs[i]->timeSinceLastJump += FIXED_TIMESTEP;

				dumbMobs[i]->position.y += dumbMobs[i]->velocity.y * FIXED_TIMESTEP;
				doLevelCollisionY(dumbMobs[i]);

				dumbMobs[i]->position.x += dumbMobs[i]->velocity.x * FIXED_TIMESTEP;
				doLevelCollisionX(dumbMobs[i]);

				//dumbMobs[i]->elapsed += FIXED_TIMESTEP;
				animation = fmod(dumbMobs[i]->elapsed, FIXED_TIMESTEP * 40.0f);
				if (dumbMobs[i]->facingRight) {
					/*if (player->velocity.x < 0.2f) {
					player->sprite.frame = 4;
					}
					else */if (animation < FIXED_TIMESTEP * 10.0f) {
					dumbMobs[i]->sprite.frame = 3;
					}
					else if (animation < FIXED_TIMESTEP * 20.0f) {
						dumbMobs[i]->sprite.frame = 4;
					}
					else if (animation < FIXED_TIMESTEP * 30.0f) {
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
					else */if (animation < FIXED_TIMESTEP * 10.0f) {
					dumbMobs[i]->sprite.frame = 0;
					}
					else if (animation < FIXED_TIMESTEP * 20.0f) {
						dumbMobs[i]->sprite.frame = 1;
					}
					else if (animation < FIXED_TIMESTEP * 30.0f) {
						dumbMobs[i]->sprite.frame = 2;
					}
					else {
						dumbMobs[i]->sprite.frame = 1;
					}
				}
				//dumbMobs[i]->healthBar.frame = (int)(floor((dumbMobs[i]->currentHealth / dumbMobs[i]->maxHealth) / 0.05));
				if (dumbMobs[i]->currentHealth / dumbMobs[i]->maxHealth < 0.05) { dumbMobs[i]->healthBar.frame = 0; }
				else if (dumbMobs[i]->currentHealth / dumbMobs[i]->maxHealth < 0.10) { dumbMobs[i]->healthBar.frame = 1; }
				else if (dumbMobs[i]->currentHealth / dumbMobs[i]->maxHealth < 0.15) { dumbMobs[i]->healthBar.frame = 2; }
				else if (dumbMobs[i]->currentHealth / dumbMobs[i]->maxHealth < 0.20) { dumbMobs[i]->healthBar.frame = 3; }
				else if (dumbMobs[i]->currentHealth / dumbMobs[i]->maxHealth < 0.25) { dumbMobs[i]->healthBar.frame = 4; }
				else if (dumbMobs[i]->currentHealth / dumbMobs[i]->maxHealth < 0.30) { dumbMobs[i]->healthBar.frame = 5; }
				else if (dumbMobs[i]->currentHealth / dumbMobs[i]->maxHealth < 0.35) { dumbMobs[i]->healthBar.frame = 6; }
				else if (dumbMobs[i]->currentHealth / dumbMobs[i]->maxHealth < 0.40) { dumbMobs[i]->healthBar.frame = 7; }
				else if (dumbMobs[i]->currentHealth / dumbMobs[i]->maxHealth < 0.45) { dumbMobs[i]->healthBar.frame = 8; }
				else if (dumbMobs[i]->currentHealth / dumbMobs[i]->maxHealth < 0.50) { dumbMobs[i]->healthBar.frame = 9; }
				else if (dumbMobs[i]->currentHealth / dumbMobs[i]->maxHealth < 0.55) { dumbMobs[i]->healthBar.frame = 10; }
				else if (dumbMobs[i]->currentHealth / dumbMobs[i]->maxHealth < 0.60) { dumbMobs[i]->healthBar.frame = 11; }
				else if (dumbMobs[i]->currentHealth / dumbMobs[i]->maxHealth < 0.65) { dumbMobs[i]->healthBar.frame = 12; }
				else if (dumbMobs[i]->currentHealth / dumbMobs[i]->maxHealth < 0.70) { dumbMobs[i]->healthBar.frame = 13; }
				else if (dumbMobs[i]->currentHealth / dumbMobs[i]->maxHealth < 0.75) { dumbMobs[i]->healthBar.frame = 14; }
				else if (dumbMobs[i]->currentHealth / dumbMobs[i]->maxHealth < 0.80) { dumbMobs[i]->healthBar.frame = 15; }
				else if (dumbMobs[i]->currentHealth / dumbMobs[i]->maxHealth < 0.85) { dumbMobs[i]->healthBar.frame = 16; }
				else if (dumbMobs[i]->currentHealth / dumbMobs[i]->maxHealth < 0.90) { dumbMobs[i]->healthBar.frame = 17; }
				else if (dumbMobs[i]->currentHealth / dumbMobs[i]->maxHealth < 0.95) { dumbMobs[i]->healthBar.frame = 18; }
				else { dumbMobs[i]->healthBar.frame = 19; }
			}
		}

		// enemy - bat
		for (size_t i = 0; i < NUM_BATS; i++) {
			if (fabs(bats[i]->position.x - player->position.x) > 4.0f ||
				fabs(bats[i]->position.y - player->position.y) > 3.0f) {
				bats[i]->dead = true;
				bats[i]->currentHealth = 0;
			}

			if (bats[i]->dead && timeSinceLastSpawn[BAT] > BAT_SPAWN_TIMER && !bossFight) {
				//Vector temp = checkForValidSpawnLocation(2);
				//if (temp.x != 0.0f && temp.y != 0.0f) {
				bats[i]->dead = false;
				bats[i]->currentHealth = bats[i]->maxHealth;
				//float scale = 0.3f;
				//bats[i]->scale_x = scale;
				//bats[i]->scale_y = scale;
				bats[i]->position = checkForValidSpawnLocation(2);
				//bats[i]->velocity.x = randomFloat(8.0f, 11.0f);
				bats[i]->jumpTimeGap = randomFloat(1.0f, 2.0f);
				timeSinceLastSpawn[BAT] = 0.0f;
				//}
				//timeSinceLastSpawn[BAT] = 0.0f;
			}

			if (!bats[i]->dead) {
				if (checkCollision(bats[i], player)) {
					//Mix_HaltMusic();
					//Mix_PlayChannel(3, gameOver, 0);
					//state = GAME_OVER;
					if (!player->invulnerable) {
						player->currentHealth -= 15.0;
						Mix_PlayChannel(-1, enemyHit, 0);
						Vector temp = player->position - bats[i]->position;
						temp.normalize();
						temp = temp * 2.0f;
						player->velocity = temp;

						screenShake = true;
						screenShakeStarted = timeElapsed;
						screenShakeTimer = 0.5f;
					}
					if (player->currentHealth <= 0.0) {
						player->currentHealth = 0.0;
						state = GAME_OVER;
					}
					else {
						player->invulnerable = true;
					}
				}
				if (player->invulnerableFrames > 2.0f) {
					player->invulnerable = false;
					player->invulnerableFrames = 0.0f;
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
				animation = fmod(bats[i]->elapsed, FIXED_TIMESTEP * 12.0f);
				if (bats[i]->facingRight) {
					/*if (player->velocity.x < 0.2f) {
					player->sprite.frame = 4;
					}
					else */if (animation < FIXED_TIMESTEP * 3.0f) {
					bats[i]->sprite.frame = 5;
					}
					else if (animation < FIXED_TIMESTEP * 6.0f) {
						bats[i]->sprite.frame = 6;
					}
					else if (animation < FIXED_TIMESTEP * 9.0f) {
						bats[i]->sprite.frame = 7;
					}
					//else if (animation < FIXED_TIMESTEP * 12.0f) {
					//	bats[i]->sprite.frame = 8;
					//}
					else {
						bats[i]->sprite.frame = 8;
					}
				}
				else if (!bats[i]->facingRight) {
					/*if (player->velocity.x > -0.2f) {
					player->sprite.frame = 1;
					}
					else */if (animation < FIXED_TIMESTEP * 3.0f) {
					bats[i]->sprite.frame = 0;
					}
					else if (animation < FIXED_TIMESTEP * 6.0f) {
						bats[i]->sprite.frame = 1;
					}
					else if (animation < FIXED_TIMESTEP * 9.0f) {
						bats[i]->sprite.frame = 2;
					}
					//else if (animation < FIXED_TIMESTEP * 12.0f) {
					//	bats[i]->sprite.frame = 3;
					//}
					else {
						bats[i]->sprite.frame = 3;
					}
				}
				//bats[i]->healthBar.frame = floor((bats[i]->currentHealth / bats[i]->maxHealth) / 0.05);
				if (bats[i]->currentHealth / bats[i]->maxHealth < 0.05) { bats[i]->healthBar.frame = 0; }
				else if (bats[i]->currentHealth / bats[i]->maxHealth < 0.10) { bats[i]->healthBar.frame = 1; }
				else if (bats[i]->currentHealth / bats[i]->maxHealth < 0.15) { bats[i]->healthBar.frame = 2; }
				else if (bats[i]->currentHealth / bats[i]->maxHealth < 0.20) { bats[i]->healthBar.frame = 3; }
				else if (bats[i]->currentHealth / bats[i]->maxHealth < 0.25) { bats[i]->healthBar.frame = 4; }
				else if (bats[i]->currentHealth / bats[i]->maxHealth < 0.30) { bats[i]->healthBar.frame = 5; }
				else if (bats[i]->currentHealth / bats[i]->maxHealth < 0.35) { bats[i]->healthBar.frame = 6; }
				else if (bats[i]->currentHealth / bats[i]->maxHealth < 0.40) { bats[i]->healthBar.frame = 7; }
				else if (bats[i]->currentHealth / bats[i]->maxHealth < 0.45) { bats[i]->healthBar.frame = 8; }
				else if (bats[i]->currentHealth / bats[i]->maxHealth < 0.50) { bats[i]->healthBar.frame = 9; }
				else if (bats[i]->currentHealth / bats[i]->maxHealth < 0.55) { bats[i]->healthBar.frame = 10; }
				else if (bats[i]->currentHealth / bats[i]->maxHealth < 0.60) { bats[i]->healthBar.frame = 11; }
				else if (bats[i]->currentHealth / bats[i]->maxHealth < 0.65) { bats[i]->healthBar.frame = 12; }
				else if (bats[i]->currentHealth / bats[i]->maxHealth < 0.70) { bats[i]->healthBar.frame = 13; }
				else if (bats[i]->currentHealth / bats[i]->maxHealth < 0.75) { bats[i]->healthBar.frame = 14; }
				else if (bats[i]->currentHealth / bats[i]->maxHealth < 0.80) { bats[i]->healthBar.frame = 15; }
				else if (bats[i]->currentHealth / bats[i]->maxHealth < 0.85) { bats[i]->healthBar.frame = 16; }
				else if (bats[i]->currentHealth / bats[i]->maxHealth < 0.90) { bats[i]->healthBar.frame = 17; }
				else if (bats[i]->currentHealth / bats[i]->maxHealth < 0.95) { bats[i]->healthBar.frame = 18; }
				else { bats[i]->healthBar.frame = 19; }
			}
		}

		//stalagtite
		for (size_t i = 0; i < NUM_STALAGTITES; i++) {
			if (fabs(stalagtites[i]->position.x - player->position.x) > 4.0f ||
				fabs(stalagtites[i]->position.y - player->position.y) > 3.0f) {
				stalagtites[i]->dead = true;
				stalagtites[i]->currentHealth = 0;
			}

			if (stalagtites[i]->dead && timeSinceLastSpawn[STALAGTITE] > STALAGTITE_SPAWN_TIMER && !bossFight) {
				//Vector temp = checkForValidSpawnLocation(3);
				//if (temp.x != 0.0f && temp.y != 0.0f) {
				stalagtites[i]->dead = false;
				stalagtites[i]->currentHealth = stalagtites[i]->maxHealth;
				if (randomFloat(0.0f, 1.0f) < 0.5f) { stalagtites[i]->facingRight = true; }
				else { stalagtites[i]->facingRight = false; }
				stalagtites[i]->timer = 0.0f;
				stalagtites[i]->rotation = 0.0f;
				stalagtites[i]->position = checkForValidSpawnLocation(3);
				stalagtites[i]->active = false;
				timeSinceLastSpawn[STALAGTITE] = 0.0f;
				//}
				//timeSinceLastSpawn[STALAGTITE] = 0.0f;
			}

			if (!stalagtites[i]->dead) {
				if (checkCollision(stalagtites[i], player)) {
					//Mix_HaltMusic();
					//Mix_PlayChannel(3, gameOver, 0);
					//state = GAME_OVER;
					if (!player->invulnerable) {
						player->currentHealth -= 55.0;
						Mix_PlayChannel(-1, enemyHit, 0);
						Vector temp = player->position - stalagtites[i]->position;
						temp.normalize();
						temp = temp * 2.0f;
						player->velocity = temp;

						screenShake = true;
						screenShakeStarted = timeElapsed;
						screenShakeTimer = 0.5f;
					}
					if (player->currentHealth <= 0.0) {
						player->currentHealth = 0.0;
						state = GAME_OVER;
					}
					else {
						player->invulnerable = true;
					}
				}
				if (player->invulnerableFrames > 2.0f) {
					player->invulnerable = false;
					player->invulnerableFrames = 0.0f;
				}

				if (player->position.y < stalagtites[i]->position.y &&
					fabs(player->position.x - stalagtites[i]->position.x) < 0.2f) {
					stalagtites[i]->active = true;
				}
				int temp_x, temp_y;
				worldToTileCoordinates(stalagtites[i]->position.x, stalagtites[i]->position.y + 0.6f, &temp_x, &temp_y);
				if (temp_y > 0) {
					if (!isSolid(levelData[temp_y][temp_x])){
						stalagtites[i]->active = true;
					}
				}

				stalagtites[i]->FixedUpdate();

				if (stalagtites[i]->active) {
					stalagtites[i]->position.x += stalagtites[i]->velocity.x * FIXED_TIMESTEP;
					doLevelCollisionX(stalagtites[i]);

					stalagtites[i]->position.y += stalagtites[i]->velocity.y * FIXED_TIMESTEP;
					doLevelCollisionY(stalagtites[i]);
					if (stalagtites[i]->collidedBot) {
						if (stalagtites[i]->collided) {
							stalagtites[i]->timer += FIXED_TIMESTEP;
						}
						//do the breaking animation stuffs

						if (stalagtites[i]->timer > 1.0f) {
							stalagtites[i]->dead = true;
							stalagtites[i]->currentHealth = 0;
							stalagtites[i]->active = false;
						}
					}
				}
				//stalagtites[i]->healthBar.frame = floor((stalagtites[i]->currentHealth / stalagtites[i]->maxHealth) / 0.05);
				if (stalagtites[i]->currentHealth / stalagtites[i]->maxHealth < 0.05) { stalagtites[i]->healthBar.frame = 0; }
				else if (stalagtites[i]->currentHealth / stalagtites[i]->maxHealth < 0.10) { stalagtites[i]->healthBar.frame = 1; }
				else if (stalagtites[i]->currentHealth / stalagtites[i]->maxHealth < 0.15) { stalagtites[i]->healthBar.frame = 2; }
				else if (stalagtites[i]->currentHealth / stalagtites[i]->maxHealth < 0.20) { stalagtites[i]->healthBar.frame = 3; }
				else if (stalagtites[i]->currentHealth / stalagtites[i]->maxHealth < 0.25) { stalagtites[i]->healthBar.frame = 4; }
				else if (stalagtites[i]->currentHealth / stalagtites[i]->maxHealth < 0.30) { stalagtites[i]->healthBar.frame = 5; }
				else if (stalagtites[i]->currentHealth / stalagtites[i]->maxHealth < 0.35) { stalagtites[i]->healthBar.frame = 6; }
				else if (stalagtites[i]->currentHealth / stalagtites[i]->maxHealth < 0.40) { stalagtites[i]->healthBar.frame = 7; }
				else if (stalagtites[i]->currentHealth / stalagtites[i]->maxHealth < 0.45) { stalagtites[i]->healthBar.frame = 8; }
				else if (stalagtites[i]->currentHealth / stalagtites[i]->maxHealth < 0.50) { stalagtites[i]->healthBar.frame = 9; }
				else if (stalagtites[i]->currentHealth / stalagtites[i]->maxHealth < 0.55) { stalagtites[i]->healthBar.frame = 10; }
				else if (stalagtites[i]->currentHealth / stalagtites[i]->maxHealth < 0.60) { stalagtites[i]->healthBar.frame = 11; }
				else if (stalagtites[i]->currentHealth / stalagtites[i]->maxHealth < 0.65) { stalagtites[i]->healthBar.frame = 12; }
				else if (stalagtites[i]->currentHealth / stalagtites[i]->maxHealth < 0.70) { stalagtites[i]->healthBar.frame = 13; }
				else if (stalagtites[i]->currentHealth / stalagtites[i]->maxHealth < 0.75) { stalagtites[i]->healthBar.frame = 14; }
				else if (stalagtites[i]->currentHealth / stalagtites[i]->maxHealth < 0.80) { stalagtites[i]->healthBar.frame = 15; }
				else if (stalagtites[i]->currentHealth / stalagtites[i]->maxHealth < 0.85) { stalagtites[i]->healthBar.frame = 16; }
				else if (stalagtites[i]->currentHealth / stalagtites[i]->maxHealth < 0.90) { stalagtites[i]->healthBar.frame = 17; }
				else if (stalagtites[i]->currentHealth / stalagtites[i]->maxHealth < 0.95) { stalagtites[i]->healthBar.frame = 18; }
				else { stalagtites[i]->healthBar.frame = 19; }
			}
		}

		if (bossFight) {
			if (bossOne->dead) {
				state = VICTORY;
			}
			else {
				if (checkCollision(bossOne, player)) {
					//Mix_HaltMusic();
					//Mix_PlayChannel(3, gameOver, 0);
					//state = GAME_OVER;
					if (!player->invulnerable) {
						player->currentHealth -= 15.0;
						Vector temp = player->position - bossOne->position;
						temp.normalize();
						temp = temp * 2.0f;
						player->velocity = temp;

						screenShake = true;
						screenShakeStarted = timeElapsed;
						screenShakeTimer = 0.5f;
					}
					if (player->currentHealth <= 0.0) {
						player->currentHealth = 0.0;
						state = GAME_OVER;
					}
					else {
						player->invulnerable = true;
					}
				}
				if (player->invulnerableFrames > 2.0f) {
					player->invulnerable = false;
					player->invulnerableFrames = 0.0f;
				}

				if (bossOne->timeSinceLastJump > bossOne->jumpTimeGap) {
					if (bossOne->position.y < player->position.y) {
						bossOne->hover();
						bossOne->timeSinceLastJump = 0.0f;
					}
					else {
						bossOne->stopHovering();
						bossOne->timeSinceLastJump = 0.0f;
					}
					if (bossOne->position.x < player->position.x) {
						bossOne->facingRight = true;
					}
					else {
						bossOne->facingRight = false;
					}
				}

				float rand = randomFloat(0.0f, 1.0f);
				if (bossOne->currentHealth > 200) {
					if (bossOne->cooldown > 3.0f) {
						if (rand < 0.3) {
							for (size_t i = 0; i < randomInt(2, 3); i++) {
								bats[i]->dead = false;
								bats[i]->currentHealth = bats[i]->maxHealth;
								bats[i]->position = bossOne->position;
								bats[i]->jumpTimeGap = randomFloat(2.0f, 3.0f);
							}
							bossOne->cooldown = 0.0f;
						}
						else {
							for (size_t i = 0; i < randomInt(3, 5); i++) {
								enemyStuff[i]->position = bossOne->position;
								enemyStuff[i]->dead = false;
								enemyStuff[i]->velocity = Vector(randomFloat(-2.0f, 2.0f), randomFloat(1.0f, 4.0f));
								enemyStuff[i]->speed = randomFloat(0.8f, 1.2f);
								enemyStuff[i]->entityID = -21;
							}
							bossOne->cooldown = 0.0f;
						}
					}
				}
				else if (bossOne->currentHealth > 100) {
					if (bossOne->cooldown > 2.0f) {
						if (rand < 0.2) {
							for (size_t i = 0; i < 4; i++) {
								bats[i]->dead = false;
								bats[i]->maxHealth = 15;
								bats[i]->currentHealth = bats[i]->maxHealth;
								bats[i]->position = bossOne->position;
								bats[i]->jumpTimeGap = randomFloat(1.5f, 2.5f);
							}
							bossOne->cooldown = 0.0f;
						}
						else {
							for (size_t i = 0; i < randomInt(5, 7); i++) {
								enemyStuff[i]->position = bossOne->position;
								enemyStuff[i]->dead = false;
								enemyStuff[i]->velocity = Vector(randomFloat(-3.0f, 3.0f), randomFloat(3.0f, 6.0f));
								enemyStuff[i]->speed = randomFloat(0.8f, 1.2f);
								enemyStuff[i]->entityID = -21;
							}
							bossOne->cooldown = 0.0f;
						}
					}
				}
				else {
					if (bossOne->cooldown > 1.3f) {
						if (rand < 0.2) {
							for (size_t i = 0; i < 5; i++) {
								bats[i]->dead = false;
								bats[i]->maxHealth = 20;
								bats[i]->currentHealth = bats[i]->maxHealth;
								bats[i]->position = bossOne->position;
								bats[i]->jumpTimeGap = randomFloat(0.75f, 1.5f);
								bats[i]->speed = 0.65f;
							}
							bossOne->cooldown = 0.0f;
						}
						else {
							for (size_t i = 0; i < randomInt(7, 9); i++) {
								enemyStuff[i]->position = bossOne->position;
								enemyStuff[i]->dead = false;
								enemyStuff[i]->velocity = Vector(randomFloat(-3.0f, 3.0f), randomFloat(5.0f, 8.0f));
								enemyStuff[i]->speed = randomFloat(0.8f, 1.2f);
								enemyStuff[i]->entityID = -21;
							}
							bossOne->cooldown = 0.0f;
						}
					}
				}

				bossOne->FixedUpdate();
				//bossOne->timeSinceLastJump += FIXED_TIMESTEP;

				bossOne->position.y += bossOne->velocity.y * FIXED_TIMESTEP;
				doLevelCollisionY(bossOne);

				bossOne->position.x += bossOne->velocity.x * FIXED_TIMESTEP;
				doLevelCollisionX(bossOne);

				//bossOne->elapsed += FIXED_TIMESTEP;
				animation = fmod(bossOne->elapsed, FIXED_TIMESTEP * 10.0f);
				if (bossOne->facingRight) {
					if (animation < FIXED_TIMESTEP * 5.0f) {
						bossOne->sprite.frame = 2;
					}
					else {
						bossOne->sprite.frame = 3;
					}
				}
				else if (!bossOne->facingRight) {
					if (animation < FIXED_TIMESTEP * 5.0f) {
						bossOne->sprite.frame = 0;
					}
					else {
						bossOne->sprite.frame = 1;
					}
				}

				//bossOne->healthBar.frame = floor((bossOne->currentHealth / bossOne->maxHealth) / 0.05);
				if (bossOne->currentHealth / bossOne->maxHealth < 0.05) { bossOne->healthBar.frame = 0; }
				else if (bossOne->currentHealth / bossOne->maxHealth < 0.10) { bossOne->healthBar.frame = 1; }
				else if (bossOne->currentHealth / bossOne->maxHealth < 0.15) { bossOne->healthBar.frame = 2; }
				else if (bossOne->currentHealth / bossOne->maxHealth < 0.20) { bossOne->healthBar.frame = 3; }
				else if (bossOne->currentHealth / bossOne->maxHealth < 0.25) { bossOne->healthBar.frame = 4; }
				else if (bossOne->currentHealth / bossOne->maxHealth < 0.30) { bossOne->healthBar.frame = 5; }
				else if (bossOne->currentHealth / bossOne->maxHealth < 0.35) { bossOne->healthBar.frame = 6; }
				else if (bossOne->currentHealth / bossOne->maxHealth < 0.40) { bossOne->healthBar.frame = 7; }
				else if (bossOne->currentHealth / bossOne->maxHealth < 0.45) { bossOne->healthBar.frame = 8; }
				else if (bossOne->currentHealth / bossOne->maxHealth < 0.50) { bossOne->healthBar.frame = 9; }
				else if (bossOne->currentHealth / bossOne->maxHealth < 0.55) { bossOne->healthBar.frame = 10; }
				else if (bossOne->currentHealth / bossOne->maxHealth < 0.60) { bossOne->healthBar.frame = 11; }
				else if (bossOne->currentHealth / bossOne->maxHealth < 0.65) { bossOne->healthBar.frame = 12; }
				else if (bossOne->currentHealth / bossOne->maxHealth < 0.70) { bossOne->healthBar.frame = 13; }
				else if (bossOne->currentHealth / bossOne->maxHealth < 0.75) { bossOne->healthBar.frame = 14; }
				else if (bossOne->currentHealth / bossOne->maxHealth < 0.80) { bossOne->healthBar.frame = 15; }
				else if (bossOne->currentHealth / bossOne->maxHealth < 0.85) { bossOne->healthBar.frame = 16; }
				else if (bossOne->currentHealth / bossOne->maxHealth < 0.90) { bossOne->healthBar.frame = 17; }
				else if (bossOne->currentHealth / bossOne->maxHealth < 0.95) { bossOne->healthBar.frame = 18; }
				else { bossOne->healthBar.frame = 19; }
			}
		}

		//enemyprojectiles
		for (size_t i = 0; i < enemyStuff.size(); i++) {
			if (fabs(enemyStuff[i]->position.x - player->position.x) > 4.0f ||
				fabs(enemyStuff[i]->position.y - player->position.y) > 3.0f) {
				enemyStuff[i]->dead = true;
			}
			if (!enemyStuff[i]->dead) {
				//bossOne fireballs;
				if (enemyStuff[i]->entityID == -21) {
					enemyStuff[i]->rotation += FIXED_TIMESTEP * 50.0f;
					enemyStuff[i]->FixedUpdate();
					enemyStuff[i]->position.y += enemyStuff[i]->velocity.y * FIXED_TIMESTEP;
					enemyStuff[i]->position.x += enemyStuff[i]->velocity.x * FIXED_TIMESTEP;
					if (checkCollision(enemyStuff[i], player)) {
						if (!player->invulnerable) {
							Explosion* temp = new Explosion(enemyStuff[i]->matrix);
							temp->sprite = SheetSprite(explosionSpriteSheet, explosion_u, explosion_v, 0.2f, 0.2f);
							temp->scale = 3.0f;
							explosions.push_back(temp);

							player->currentHealth -= 30.0;
							Vector knockback = player->position - enemyStuff[i]->position;
							knockback.normalize();
							knockback = knockback * 2.0f;
							player->velocity = knockback;

							screenShake = true;
							screenShakeStarted = timeElapsed;
							screenShakeTimer = 0.7f;

							Mix_PlayChannel(-1, bombExplode, 0);
							enemyStuff[i]->dead = true;
						}
						if (player->currentHealth <= 0.0) {
							player->currentHealth = 0.0;
							state = GAME_OVER;
						}
						else {
							player->invulnerable = true;
						}
					}
					if (checkPointForGridCollisionX(enemyStuff[i]->position.x, enemyStuff[i]->position.y) != 0.0f || checkPointForGridCollisionY(enemyStuff[i]->position.x, enemyStuff[i]->position.y) != 0.0f) {
						Explosion* temp = new Explosion(enemyStuff[i]->matrix);
						temp->sprite = SheetSprite(explosionSpriteSheet, explosion_u, explosion_v, 0.2f, 0.2f);
						temp->scale = 3.0f;
						explosions.push_back(temp);

						Mix_PlayChannel(-1, bombExplode, 0);
						enemyStuff[i]->dead = true;
					}
				}
			}
		}

		//gun
		if (fireball->bulletActive) {
			fireball->rotation += FIXED_TIMESTEP * 20.0f;
			fireball->FixedUpdate();
			fireball->position.y += fireball->velocity.y * FIXED_TIMESTEP;
			//doLevelCollisionY(fireball);
			fireball->position.x += fireball->velocity.x * FIXED_TIMESTEP;
			//doLevelCollisionX(fireball);
			for (size_t i = 0; i < enemies.size(); i++) {
				if (!enemies[i]->dead) {
					if (checkCollision(fireball, enemies[i])) {
						Explosion* temp = new Explosion(fireball->matrix);
						temp->sprite = SheetSprite(explosionSpriteSheet, explosion_u, explosion_v, 0.2f, 0.2f);
						temp->scale = 2.0f;
						explosions.push_back(temp);
						Mix_PlayChannel(-1, shoot, 0);

						//flamethrower - comment out line below and same line a bit down
						fireball->bulletReset(player->position);
						//fireball->bulletActive = true;
						//enemies[i]->dead = true;
						//enemies[i]->currentHealth = 0;
						//Mix_PlayChannel(-1, enemyHit, 0);
						//score += (enemies[i]->entityID + 1) * 100;
						enemies[i]->currentHealth -= 7.0;
						if (enemies[i]->currentHealth <= 0.0) {
							enemies[i]->dead = true;
							enemies[i]->currentHealth = 0;
							score += (enemies[i]->entityID + 1) * 100;
						}
					}
				}
			}
			if (checkPointForGridCollisionX(fireball->position.x, fireball->position.y) != 0.0f || checkPointForGridCollisionY(fireball->position.x, fireball->position.y) != 0.0f) {
				//int grid_x, grid_y;
				//worldToTileCoordinates(fireball->position.x, fireball->position.y, &grid_x, &grid_y);
				//if (levelData[grid_y][grid_x] != 14) {
				//	Mix_PlayChannel(-1, blop, 0);
				//	levelData[grid_y][grid_x] = 15;
				//}
				Explosion* temp = new Explosion(fireball->matrix);
				temp->sprite = SheetSprite(explosionSpriteSheet, explosion_u, explosion_v, 0.2f, 0.2f);
				temp->scale = 2.0f;
				explosions.push_back(temp);
				Mix_PlayChannel(-1, shoot, 0);

				fireball->bulletReset(player->position);
				//fireball->bulletActive = true;
			}
			if (fireball->lifetime > fireball->maxLifetime) {
				fireball->bulletActive = false;
			}
		}

		//mine
		if (mine->bulletActive) {
			if (mine->collidedBot) {
				if (mine->velocity.x < 0.0f && !mine->randomUse) {
					mine->rotation += FIXED_TIMESTEP * (2.0f - mine->lifetime);
				}
				else if (mine->velocity.x > 0.0f && !mine->randomUse) {
					mine->rotation -= FIXED_TIMESTEP * (2.0f - mine->lifetime);
				}
			}
			mine->FixedUpdate();
			mine->position.y += mine->velocity.y * FIXED_TIMESTEP;
			doLevelCollisionY(mine);
			mine->position.x += mine->velocity.x * FIXED_TIMESTEP;
			doLevelCollisionX(mine);

			if (mine->collidedRight || mine->collidedLeft) {
				mine->randomUse = true;
			}
			if (mine->lifetime > mine->maxLifetime) {
				Explosion* temp = new Explosion(mine->matrix);
				temp->sprite = SheetSprite(explosionSpriteSheet, explosion_u, explosion_v, 0.2f, 0.2f);
				temp->scale = 4.0f;
				explosions.push_back(temp);
				Mix_PlayChannel(-1, bombExplode, 0);

				screenShake = true;
				screenShakeStarted = timeElapsed;
				screenShakeTimer = 0.2f;

				if (fabs(player->position.x - mine->position.x) < 0.15f &&
					fabs(player->position.y - mine->position.y) < 0.15f) {
					//state = GAME_OVER;
					if (!player->invulnerable) {
						player->currentHealth -= 85.0;
						Mix_PlayChannel(-1, enemyHit, 0);
						Vector temp = player->position - mine->position;
						temp.normalize();
						temp = temp * 2.0f;
						player->velocity = temp;

						screenShake = true;
						screenShakeStarted = timeElapsed;
						screenShakeTimer = 0.7f;
					}
					if (player->currentHealth <= 0.0) {
						player->currentHealth = 0.0;
						state = GAME_OVER;
					}
					else {
						player->invulnerable = true;
					}
				}
				if (player->invulnerableFrames > 2.0f) {
					player->invulnerable = false;
					player->invulnerableFrames = 0.0f;
				}

				for (size_t i = 0; i < enemies.size(); i++) {
					if (!enemies[i]->dead) {
						if (fabs(enemies[i]->position.x - mine->position.x) < 0.15f &&
							fabs(enemies[i]->position.y - mine->position.y) < 0.15f) {
							//enemies[i]->dead = true;
							//enemies[i]->currentHealth = 0;
							//score += (enemies[i]->entityID + 1) * 100;
							enemies[i]->currentHealth -= 25.0;
							if (enemies[i]->currentHealth <= 0.0) {
								enemies[i]->dead = true;
								enemies[i]->currentHealth = 0;
								score += (enemies[i]->entityID + 1) * 100;
							}
						}
					}
				}

				int grid_x, grid_y;
				worldToTileCoordinates(mine->position.x, mine->position.y, &grid_x, &grid_y);
				if (levelData[grid_y - 1][grid_x + 1] != 14) { levelData[grid_y - 1][grid_x + 1] = 61; }
				if (levelData[grid_y][grid_x + 1] != 14) { levelData[grid_y][grid_x + 1] = 61; }
				if (levelData[grid_y + 1][grid_x + 1] != 14) { levelData[grid_y + 1][grid_x + 1] = 61; }
				if (levelData[grid_y - 1][grid_x] != 14) { levelData[grid_y - 1][grid_x] = 61; }
				if (levelData[grid_y][grid_x] != 14) { levelData[grid_y][grid_x] = 61; }
				if (levelData[grid_y + 1][grid_x] != 14) { levelData[grid_y + 1][grid_x] = 61; }
				if (levelData[grid_y - 1][grid_x - 1] != 14) { levelData[grid_y - 1][grid_x - 1] = 61; }
				if (levelData[grid_y][grid_x - 1] != 14) { levelData[grid_y][grid_x - 1] = 61; }
				if (levelData[grid_y + 1][grid_x - 1] != 14) { levelData[grid_y + 1][grid_x - 1] = 61; }

				mine->bulletReset(player->position);
			}

			animation = fmod(mine->elapsed, FIXED_TIMESTEP * 10.0f);
			if (animation < FIXED_TIMESTEP * 5.0f) {
				mine->sprite.frame = 0;
			}
			else {
				mine->sprite.frame = 1;
			}
		}

		//player
		player->FixedUpdate();
		player->position.y += player->velocity.y * FIXED_TIMESTEP;
		doLevelCollisionY(player);
		player->position.x += player->velocity.x * FIXED_TIMESTEP;
		doLevelCollisionX(player);

		//player->elapsed += FIXED_TIMESTEP;
		animation = fmod(player->elapsed, FIXED_TIMESTEP * 20.0f);
		if (player->isHovering) {
			if (player->facingRight) {
				player->sprite.frame = 6;
			}
			else {
				player->sprite.frame = 1;
			}
		}
		else if (player->isIdle) {
			if (player->facingRight) {
				player->sprite.frame = 5;
			}
			else {
				player->sprite.frame = 0;
			}
		}
		else {
			if (player->facingRight) {
				if (animation < FIXED_TIMESTEP * 5.0f) {
					player->sprite.frame = 7;
				}
				else if (animation < FIXED_TIMESTEP * 10.0f) {
					player->sprite.frame = 8;
				}
				else if (animation < FIXED_TIMESTEP * 15.0f) {
					player->sprite.frame = 7;
				}
				else {
					player->sprite.frame = 9;
				}
			}
			else {
				if (animation < FIXED_TIMESTEP * 5.0f) {
					player->sprite.frame = 2;
				}
				else if (animation < FIXED_TIMESTEP * 10.0f) {
					player->sprite.frame = 3;
				}
				else if (animation < FIXED_TIMESTEP * 15.0f) {
					player->sprite.frame = 2;
				}
				else {
					player->sprite.frame = 4;
				}
			}
		}

		//altar
		altar->FixedUpdate();

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

		if (screenShake) {
			if (timeElapsed - screenShakeStarted >= screenShakeTimer) {
				screenShake = false;
				screenShakeStarted = 0.0f;
				screenShakeTimer = 0.0f;
			}
		}
		screenShakeValue += FIXED_TIMESTEP;
		break;
	case GAME_OVER:
		screenShakeValue += FIXED_TIMESTEP;
		break;
	case CUTSCENE:
		player->position.x = altarSpawn_x - 0.003f;
		player->position.y = altarSpawn_y;
		int altar_x, altar_y;
		worldToTileCoordinates(altarSpawn_x, altarSpawn_y, &altar_x, &altar_y);
		for (int i = 0; i < 19; i++) {
			for (int j = 0; j < 19; j++) {
				if (cutsceneTime > 5.7f + (float)(i)* 0.1f) {
					levelData[altar_y - i][altar_x - 9 + j] = 61;

				}
			}
		}


		cutsceneTime += FIXED_TIMESTEP;
		if (cutsceneTime > 10.0f) {
			bossFight = true;
			bossOne->dead = false;
			bossOne->position = Vector(altarSpawn_x, altarSpawn_y + 1.0f);
			for (int i = 0; i < 21; i++) {
				levelData[altar_y + 1][altar_x - 10 + i] = 14;
				levelData[altar_y - 19][altar_x - 10 + i] = 14;
				levelData[altar_y - i][altar_x - 10] = 14;
				levelData[altar_y - i][altar_x + 10] = 14;
			}
			for (int i = 4; i < 8; i++) {
				levelData[altar_y - 3][altar_x + i] = 14;
				levelData[altar_y - 3][altar_x - i] = 14;
				levelData[altar_y - 13][altar_x + i] = 14;
				levelData[altar_y - 13][altar_x - i] = 14;
			}
			levelData[altar_y - 8][altar_x + 2] = 14;
			levelData[altar_y - 8][altar_x + 1] = 14;
			levelData[altar_y - 8][altar_x] = 14;
			levelData[altar_y - 8][altar_x - 1] = 14;
			levelData[altar_y - 8][altar_x - 2] = 14;
			cutsceneTime = 0.0f;
			state = GAME_PLAY;
		}
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
	screenShake = false;
	screenShakeValue = 0.0f;
	screenShakeTimer = 0.0f;
	screenShakeStarted = 0.0f;
	bossFight = false;

	//while (itemSpawns.size() == 0) {
		buildLevel();
	//}
	explosions.clear();
	enemies.clear();
	dumbMobs.clear();
	bats.clear();
	stalagtites.clear();
	enemyStuff.clear();
	activeWeapon = 1;

	vector<float> healthBar_u = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	vector<float> healthBar_v = { 0.0f / 20.0f, 1.0f / 20.0f, 2.0f / 20.0f, 3.0f / 20.0f, 4.0f / 20.0f, 5.0f / 20.0f, 6.0f / 20.0f, 7.0f / 20.0f, 8.0f / 20.0f, 9.0f / 20.0f,
		10.0f / 20.0f, 11.0f / 20.0f, 12.0f / 20.0f, 13.0f / 20.0f, 14.0f / 20.0f, 15.0f / 20.0f, 16.0f / 20.0f, 17.0f / 20.0f, 18.0f / 20.0f, 19.0f / 20.0f };

	player = new Entity();
	float adjust_player_u = 1.0f / 40.0f;
	float adjust_player_v = 1.0f / 160.0f;
	vector<float> player_u = { 0.0f / 2.0f + adjust_player_u, 0.0f / 2.0f + adjust_player_u, 0.0f / 2.0f + adjust_player_u, 0.0f / 2.0f + adjust_player_u, 0.0f / 2.0f + adjust_player_u, 
		1.0f / 2.0f + adjust_player_u, 1.0f / 2.0f + adjust_player_u, 1.0f / 2.0f + adjust_player_u, 1.0f / 2.0f + adjust_player_u, 1.0f / 2.0f + adjust_player_u, };
	vector<float> player_v = { 0.0f / 5.0f + adjust_player_v, 1.0f / 5.0f + adjust_player_v, 2.0f / 5.0f + adjust_player_v, 3.0f / 5.0f + adjust_player_v, 4.0f / 5.0f + adjust_player_v, 
		0.0f / 5.0f + adjust_player_v, 1.0f / 5.0f + adjust_player_v, 2.0f / 5.0f + adjust_player_v, 3.0f / 5.0f + adjust_player_v, 4.0f / 5.0f + adjust_player_v, };
	player->sprite = SheetSprite(playerWizardTexture, player_u, player_v, 1.0f / 2.0f - 2 * adjust_player_u, 1.0f / 5.0f - 2 * adjust_player_v);
	player->maxHealth = 100;
	player->currentHealth = 100;
	player->dead = false;
	float playerSpawn_x, playerSpawn_y;
	tileToWorldCoordinates(spawn_x, spawn_y, &playerSpawn_x, &playerSpawn_y);
	player->position.y += playerSpawn_y;
	player->position.x += playerSpawn_x;
	player->entityID = -10;
	player->scale_x = 0.35f;
	player->scale_y = 0.6f;
	player->width = 0.1f;
	player->height = 0.12f;
	//player->speed = 5.0f;
	//player->x += (float) (-TILE_SIZE* mapWidth / 2);i
	//player->y += (float) (TILE_SIZE* mapHeight / 2);
	//entities.push_back(player);

	fireball = new Entity();
	vector<float> fireball_u = { 0.0f };
	vector<float> fireball_v = { 0.0f };
	fireball->sprite = SheetSprite(fireballTexture, fireball_u, fireball_v, 1.0f, 1.0f);
	fireball->position = player->position;
	fireball->width = 0.1f;
	fireball->height = 0.1f;
	fireball->scale_x = 0.2f;
	fireball->scale_y = 0.2f;
	fireball->speed = 2.0f;
	fireball->fric_x = 0.0f;
	fireball->fric_y = 0.0f;
	fireball->grav_x = 0.0f;
	fireball->grav_y = 0.0f;
	fireball->maxLifetime = 0.6f;
	fireball->entityID = -5;

	mine = new Entity();
	vector<float> mine_u = { 0.0f / 1.0f, 0.0f / 1.0f };
	vector<float> mine_v = { 0.0f / 2.0f, 1.0f / 2.0f };
	mine->sprite = SheetSprite(mineTexture, mine_u, mine_v, 1.0f, 1.0f / 2.0f);
	mine->position = player->position;
	mine->width = 0.15f;
	mine->height = 0.05f;
	mine->scale_x = 0.16f;
	mine->scale_y = 0.25f;
	mine->speed = 1.0f;
	mine->fric_x = 5.0f;
	mine->fric_y = 2.0f;
	mine->maxLifetime = 1.5f;
	mine->entityID = -6;

	altar = new Entity();
	vector<float> altar_u = { 0.0f };
	vector<float> altar_v = { 0.0f };
	altar->sprite = SheetSprite(altarTexture, altar_u, altar_v, 1.0f, 1.0f);
	altar->position = Vector(altarSpawn_x, altarSpawn_y);
	altar->width = 0.25f;
	altar->height = 0.20f;
	altar->scale_x = 0.30f;
	altar->scale_y = 0.25f;
	altar->speed = 0.0f;
	altar->entityID = -9;
	altar->isStatic = true;

	ParticleEmitterTest = ParticleEmitter(2.0f, 300);
	ParticleEmitterTest.position = Vector(player->position.x, player->position.y);
	ParticleEmitterTest.gravity = Vector(0.0f, 0.0f);
	ParticleEmitterTest.velocity = Vector(0.0f, 0.0f);
	//ParticleEmitterTest = ParticleEmitter(Vector(player->x, player->y), Vector(0.0f, 0.0f), Vector(0.0f, 0.0f), 2.0f, 300);
	ParticleEmitterTest.velocityDeviation = Vector(0.3f, 0.3f);

	////enemy
	//float adjust_enemy_u = 2.0f / 192.0f;
	//float adjust_enemy_v = 2.0f / 128.0f;
	////vector<float> enemy_u = { 3.0f / 12.0f + adjust_u, 4.0f / 12.0f + adjust_u, 5.0f / 12.0f + adjust_u,
	////	3.0f / 12.0f + adjust_u, 4.0f / 12.0f + adjust_u, 5.0f / 12.0f + adjust_u };
	////vector<float> enemy_v = { 5.0f / 8.0f + adjust_v, 5.0f / 8.0f + adjust_v, 5.0f / 8.0f + adjust_v,
	////	6.0f / 8.0f + adjust_v, 6.0f / 8.0f + adjust_v, 6.0f / 8.0f + adjust_v, };
	//vector<float> enemy_u = { 3.0f / 12.0f + adjust_enemy_u, 4.0f / 12.0f + adjust_enemy_u, 5.0f / 12.0f + adjust_enemy_u,
	//	3.0f / 12.0f + adjust_enemy_u, 4.0f / 12.0f + adjust_enemy_u, 5.0f / 12.0f + adjust_enemy_u };
	//vector<float> enemy_v = { 5.0f / 8.0f + adjust_enemy_v, 5.0f / 8.0f + adjust_enemy_v, 5.0f / 8.0f + adjust_enemy_v,
	//	6.0f / 8.0f + adjust_enemy_v, 6.0f / 8.0f + adjust_enemy_v, 6.0f / 8.0f + adjust_enemy_v };
	//SheetSprite enemyTexture = SheetSprite(spriteSheet3, enemy_u, enemy_v, 16.0f / 192.0f - 2 * adjust_u, 16.0f / 128.0f );
	
	//SheetSprite zombieTexture = SheetSprite(zombieTexture, zombie_u, zombie_v, 0.1f, 0.15f);
	for (size_t i = 0; i < NUM_DUMBMOBS; i++) {
		DumbMob* enemy = new DumbMob();
		enemy->sprite = zombieSprite;
		enemy->healthBar = SheetSprite(healthBarTexture, healthBar_u, healthBar_v, 1.0f, 1.0f / 20.0f);
		enemy->healthBar.frame = 19;
		enemy->maxHealth = 20.0;
		enemy->currentHealth = 20.0;
		enemy->dead = true;
		enemy->grav_y = -9.8f;
		enemy->speed = 0.2f;
		enemy->width = 0.2f;
		enemy->height = 0.1f;
		enemy->scale_x = 0.35f;
		enemy->scale_y = 0.4f;
		//enemy->scale_x = 0.5f;
		//enemy->scale_y = 0.5f;
		enemy->entityID = DUMB_MOB; //0
		//enemy->velocity = Vector(9.0f, 0.0f);
		dumbMobs.push_back(enemy);
		enemies.push_back(enemy);
	}

	float adjust_bat_u = 1.0f / 36.0f;
	float adjust_bat_v = 1.0f / 80.0f;
	vector<float> bat_u = { 0.0f / 2.0f + adjust_bat_u, 0.0f / 2.0f + adjust_bat_u, 0.0f / 2.0f + adjust_bat_u, 0.0f / 2.0f + adjust_bat_u, 0.0f / 2.0f + adjust_bat_u,
		1.0f / 2.0f + adjust_bat_u, 1.0f / 2.0f + adjust_bat_u, 1.0f / 2.0f + adjust_bat_u, 1.0f / 2.0f + adjust_bat_u, 1.0f / 2.0f + adjust_bat_u };
	vector<float> bat_v = { 0.0f / 5.0f + adjust_bat_v, 1.0f / 5.0f + adjust_bat_v, 2.0f / 5.0f + adjust_bat_v, 3.0f / 5.0f + adjust_bat_v, 4.0f / 5.0f + adjust_bat_v,
		0.0f / 5.0f + adjust_bat_v, 1.0f / 5.0f + adjust_bat_v, 2.0f / 5.0f + adjust_bat_v, 3.0f / 5.0f + adjust_bat_v, 4.0f / 5.0f + adjust_bat_v };
	//SheetSprite batSprite = SheetSprite(batTexture, bat_u, bat_v, 1.0f / 2.0f - 2 * adjust_bat_u, 1.0f / 5.0f - 2 * adjust_bat_v);
	for (size_t i = 0; i < NUM_BATS; i++) {
		Bat* enemy = new Bat();
		enemy->sprite = SheetSprite(batTexture, bat_u, bat_v, 1.0f / 2.0f - 2 * adjust_bat_u, 1.0f / 5.0f - 2 * adjust_bat_v);;
		enemy->healthBar = SheetSprite(healthBarTexture, healthBar_u, healthBar_v, 1.0f, 1.0f / 20.0f);
		enemy->healthBar.frame = 19;
		enemy->maxHealth = 10;
		enemy->currentHealth = 10;
		enemy->dead = true;
		enemy->grav_y = -3.75f;
		enemy->speed = 0.4f;
		enemy->width = 0.2f;
		enemy->height = 0.1f;
		enemy->scale_x = 0.35f;
		enemy->scale_y = 0.6f;
		enemy->entityID = BAT; //1
		bats.push_back(enemy);
		enemies.push_back(enemy);
	}

	vector<float> stalagtite_u = { 0.0f };
	vector<float> stalagtite_v = { 0.0f };
	for (size_t i = 0; i < NUM_STALAGTITES; i++) {
		Stalagtite* enemy = new Stalagtite();
		enemy->sprite = SheetSprite(stalagtiteTexture, stalagtite_u, stalagtite_v, 1.0f, 1.0f);
		enemy->healthBar = SheetSprite(healthBarTexture, healthBar_u, healthBar_v, 1.0f, 1.0f / 20.0f);
		enemy->healthBar.frame = 19;
		enemy->maxHealth = 10;
		enemy->currentHealth = 10;
		enemy->dead = true;
		enemy->active = false;
		enemy->grav_y = -8.0f;
		enemy->speed = 0.7f;
		enemy->height = 0.1f;
		enemy->width = 0.2f;
		enemy->scale_x = 0.2f;
		enemy->scale_y = 0.2f;
		enemy->entityID = STALAGTITE; //2
		stalagtites.push_back(enemy);
		enemies.push_back(enemy);
	}

	vector<float> bossOne_u = { 0.0f / 2.0f, 0.0f / 2.0f, 1.0f / 2.0f, 1.0f / 2.0f };
	vector<float> bossOne_v = { 0.0f / 2.0f, 1.0f / 2.0f, 0.0f / 2.0f, 1.0f / 2.0f };
	bossOne = new BossOne();
	bossOne->sprite = SheetSprite(bossOneTexture, bossOne_u, bossOne_v, 1.0f / 2.0f, 1.0f / 2.0f);
	bossOne->healthBar = SheetSprite(healthBarTexture, healthBar_u, healthBar_v, 1.0f, 1.0f / 20.0f);
	bossOne->healthBar.frame = 19;
	bossOne->maxHealth = 300;
	bossOne->currentHealth = 300;
	bossOne->dead = true;
	bossOne->grav_y = -3.75f;
	bossOne->speed = 0.5f;
	bossOne->width = 0.3f;
	bossOne->height = 0.3f;
	bossOne->scale_x = 0.7f;
	bossOne->scale_y = 0.7f;
	bossOne->jumpTimeGap = 0.3f;
	bossOne->cooldown = 0.0f;
	bossOne->entityID = 10;
	enemies.push_back(bossOne);

	for (size_t i = 0; i < 10; i++) {
		Entity* temp = new Entity();
		vector<float> u = { 0.0f };
		vector<float> v = { 0.0f };
		temp->sprite = SheetSprite(fireThingTexture, u, v, 1.0f, 1.0f);
		temp->position = bossOne->position;
		temp->dead = true;
		temp->velocity = Vector(randomFloat(-2.0f, 2.0f), randomFloat(1.0f, 5.0f));
		temp->width = 0.2f;
		temp->height = 0.2f;
		temp->scale_x = 0.2f;
		temp->scale_y = 0.2f;
		temp->speed = 1.0f;
		temp->fric_x = 5.0f;
		temp->fric_y = 2.0f;
		//temp->maxLifetime = 1.5f;
		temp->entityID = -21;
		enemyStuff.push_back(temp);
	}
}

bool Game::isSolid(int tile) {
	switch (tile) {
	case 61:
		return false;
		break;
	case 63:
		return false;
		break;
	//case 0:
	//	return true;
	//	break;
	//case 1:
	//	return true;
	//	break;
	//case 2:
	//	return true;
	//	break;
	//case 3:
	//	return true;
	//	break;
	//case 16:
	//	return true;
	//	break;
	//case 17:
	//	return true;
	//	break;
	//case 18:
	//	return true;
	//	break;
	//case 19:
	//	return true;
	//	break;
	//case 32:
	//	return true;
	//	break;
	//case 33:
	//	return true;
	//	break;
	//case 34:
	//	return true;
	//	break;
	//case 35:
	//	return true;
	//	break;
	default:
		return true;
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

	float aWidth = a->width /** a->scale_x */* 0.9f;
	float aHeight = a->height /** a->scale_y */* 0.9f;
	float bWidth = b->width /** b->scale_x */* 0.9f;
	float bHeight = b->height /** b->scale_y */* 0.9f;

	//float aWidth = a->width * a->scale_x * 0.9f;
	//float aHeight = a->height * a->scale_y * 0.9f;
	//float bWidth = b->width * b->scale_x * 0.9f;
	//float bHeight = b->height * b->scale_y * 0.9f;

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
	if (grid_x < 0 || grid_x > mapWidth - 1 || grid_y < 0 || grid_y > mapHeight - 1) {
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
	if (grid_x < 0 || grid_x > mapWidth - 1 || grid_y < 0 || grid_y > mapHeight - 1) {
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
	float adjust1 = checkPointForGridCollisionX(entity->position.x + entity->width * entity->scale_x * 0.5f, entity->position.y);
	//float adjust2 = checkPointForGridCollisionX(entity->position.x + entity->sprite.width * entity->scale_x * 0.5f, entity->position.y - entity->sprite.height * 0.5f);
	if (adjust1 != 0.0f) {
		entity->position.x -= adjust1;
		entity->velocity.x = 0.0f;
		entity->collidedRight = true;
	}
	//else if (adjust2 != 0.0f) {
	//	entity->position.x -= adjust2;
	//	entity->velocity.x = 0.0f;
	//	entity->collidedRight = true;
	//}

	//left
	adjust1 = checkPointForGridCollisionX(entity->position.x - entity->width * entity->scale_x * 0.5f, entity->position.y);
	//adjust2 = checkPointForGridCollisionX(entity->position.x - entity->sprite.width * entity->scale_x * 0.5f, entity->position.y - entity->sprite.height * 0.5f);
	if (adjust1 != 0.0f) {
		entity->position.x += adjust1;
		entity->velocity.x = 0.0f;
		entity->collidedLeft = true;
	}
	//else if (adjust2 != 0.0f) {
	//	entity->position.x += adjust2;
	//	entity->velocity.x = 0.0f;
	//	entity->collidedLeft = true;
	//}
}

void Game::doLevelCollisionY(Entity *entity) {
	//bottom
	float adjust1 = checkPointForGridCollisionY(entity->position.x, entity->position.y - entity->height *0.5f);
	//float adjust2 = checkPointForGridCollisionY(entity->position.x - entity->sprite.width * entity->scale_x * 0.5f, entity->position.y - entity->sprite.height *0.5f);
	if (adjust1 != 0.0f) {
		entity->position.y += adjust1;
		entity->velocity.y = 0.0f;
		entity->collidedBot = true;
	}
	//else if (adjust2 != 0.0f) {
	//	entity->position.y += adjust2 * 0.5;
	//	entity->velocity.y = 0.0f;
	//	entity->collidedBot = true;
	//}

	//top
	adjust1 = checkPointForGridCollisionY(entity->position.x, entity->position.y + entity->height * 0.5f);
	//adjust2 = checkPointForGridCollisionY(entity->position.x - entity->sprite.width * entity->scale_x * 0.5f, entity->position.y + entity->sprite.height * 0.5f);
	if (adjust1 != 0.0f) {
		entity->position.y -= adjust1 * 0.05f;
		entity->velocity.y = 0.0f;
		entity->collidedTop = true;
	}	
	//else if (adjust2 != 0.0f) {
	//	entity->position.y -= adjust2 * 0.05f;
	//	entity->velocity.y = 0.0f;
	//	entity->collidedTop = true;
	//}
}

/*
14 = border block (last tile in pic)
*/
void Game::buildLevel() {
	spawn_x = 0;
	spawn_y = 0;
	itemSpawns.clear();
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
	//bool spawnSet = false;
	vector<Vector> altarSpawns;
	altarSpawns.clear();
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			float temp = randomFloat(0.0f, 1.0f);
			if (i == 0 || j == 0 || i == mapWidth - 1 || j == mapHeight - 1) {
				levelData[i][j] = 14;
			}
			else if (cellmap[i][j]) {
				if (!cellmap[i - 1][j]) {
					if (spawn_x == 0 && spawn_y == 0) {
						spawn_x = j;
						spawn_y = i;
						//spawnSet = true;
					}
				}
				if (temp < 0.11f) { levelData[i][j] = 22; }
				else if (temp < 0.22f) { levelData[i][j] = 23; }
				else if (temp < 0.33f) { levelData[i][j] = 24; }
				else if (temp < 0.44f) { levelData[i][j] = 35; }
				else if (temp < 0.55f) { levelData[i][j] = 36; }
				else if (temp < 0.66f) { levelData[i][j] = 37; }
				else if (temp < 0.77f) { levelData[i][j] = 48; }
				else if (temp < 0.88f) { levelData[i][j] = 49; }
				else { levelData[i][j] = 50; }

				//if (!cellmap[i-1][j]) {
				//	if (!spawnSet) {
				//		spawn_x = j;
				//		spawn_y = i;
				//		spawnSet = true;
				//	}
				//	//topLeft grass corner
				//	if (!cellmap[i][j-1]) {
				//		if (temp < 0.16f) { levelData[i][j] = 48; }
				//		else if (temp < 0.33f) { levelData[i][j] = 50; }
				//		else if (temp < 0.50f) { levelData[i][j] = 52; }
				//		else if (temp < 0.66f) { levelData[i][j] = 199; }
				//		else if (temp < 0.83f) { levelData[i][j] = 202; }
				//		else { levelData[i][j] = 205; }
				//		//levelData[i][j] = 0;
				//	}
				//	//topRight grass corner
				//	else if (!cellmap[i][j+1]) {
				//		if (temp < 0.16f) { levelData[i][j] = 49; }
				//		else if (temp < 0.33f) { levelData[i][j] = 51; }
				//		else if (temp < 0.50f) { levelData[i][j] = 53; }
				//		else if (temp < 0.66f) { levelData[i][j] = 201; }
				//		else if (temp < 0.83f) { levelData[i][j] = 204; }
				//		else { levelData[i][j] = 207; }
				//		//levelData[i][j] = 2;
				//	}
				//	//center top grass
				//	else {
				//		if (temp < 0.066f) { levelData[i][j] = 1; }
				//		else if (temp < 0.133f) { levelData[i][j] = 2; }
				//		else if (temp < 0.200f) { levelData[i][j] = 3; }
				//		else if (temp < 0.266f) { levelData[i][j] = 176; }
				//		else if (temp < 0.333f) { levelData[i][j] = 177; }
				//		else if (temp < 0.400f) { levelData[i][j] = 178; }
				//		else if (temp < 0.466f) { levelData[i][j] = 179; }
				//		else if (temp < 0.533f) { levelData[i][j] = 180; }
				//		else if (temp < 0.600f) { levelData[i][j] = 181; }
				//		else if (temp < 0.666f) { levelData[i][j] = 200; }
				//		else if (temp < 0.733f) { levelData[i][j] = 203; }
				//		else if (temp < 0.800f) { levelData[i][j] = 206; }
				//		else if (temp < 0.866f) { levelData[i][j] = 242; }
				//		else if (temp < 0.933f) { levelData[i][j] = 243; }
				//		else { levelData[i][j] = 244; }
				//		//if (temp < 0.2f) {levelData[i][j] = 1;}
				//		//else if (temp < 0.4f) {levelData[i][j] = 16;}
				//		//else if (temp < 0.6f) {levelData[i][j] = 17;}
				//		//else if (temp < 0.8f) {levelData[i][j] = 18;}
				//		//else {levelData[i][j] = 19;}
				//	}
				//}
				//else if (!cellmap[i + 1][j]) {
				//	//bottomLeft grass corner
				//	if (!cellmap[i][j - 1]) {
				//		if (temp < 0.16f) { levelData[i][j] = 64; }
				//		else if (temp < 0.33f) { levelData[i][j] = 66; }
				//		else if (temp < 0.50f) { levelData[i][j] = 68; }
				//		else if (temp < 0.66f) { levelData[i][j] = 231; }
				//		else if (temp < 0.83f) { levelData[i][j] = 234; }
				//		else { levelData[i][j] = 237; }
				//	}
				//	//bottomRight grass corner
				//	else if (!cellmap[i][j + 1]) {
				//		if (temp < 0.16f) { levelData[i][j] = 65; }
				//		else if (temp < 0.33f) { levelData[i][j] = 67; }
				//		else if (temp < 0.50f) { levelData[i][j] = 69; }
				//		else if (temp < 0.66f) { levelData[i][j] = 233; }
				//		else if (temp < 0.83f) { levelData[i][j] = 236; }
				//		else { levelData[i][j] = 239; }
				//	}
				//	//center bottom grass
				//	else {
				//		if (temp < 0.066f) { levelData[i][j] = 33; }
				//		else if (temp < 0.133f) { levelData[i][j] = 34; }
				//		else if (temp < 0.200f) { levelData[i][j] = 35; }
				//		else if (temp < 0.266f) { levelData[i][j] = 192; }
				//		else if (temp < 0.333f) { levelData[i][j] = 193; }
				//		else if (temp < 0.400f) { levelData[i][j] = 194; }
				//		else if (temp < 0.466f) { levelData[i][j] = 195; }
				//		else if (temp < 0.533f) { levelData[i][j] = 196; }
				//		else if (temp < 0.600f) { levelData[i][j] = 197; }
				//		else if (temp < 0.666f) { levelData[i][j] = 232; }
				//		else if (temp < 0.733f) { levelData[i][j] = 235; }
				//		else if (temp < 0.800f) { levelData[i][j] = 238; }
				//		else if (temp < 0.866f) { levelData[i][j] = 258; }
				//		else if (temp < 0.933f) { levelData[i][j] = 259; }
				//		else { levelData[i][j] = 260; }
				//	}
				//	//else if (temp < 0.35f) {levelData[i][j] = 32;}
				//	//else if (temp < 0.5f) {levelData[i][j] = 33;}
				//	//else if (temp < 0.65f) {levelData[i][j] = 34;}
				//	//else {levelData[i][j] = 35;}
				//}
				//else if (!cellmap[i][j + 1]) {
				//	//center right
				//	if (temp < 0.066f) { levelData[i][j] = 4; }
				//	else if (temp < 0.133f) { levelData[i][j] = 20; }
				//	else if (temp < 0.200f) { levelData[i][j] = 36; }
				//	else if (temp < 0.266f) { levelData[i][j] = 85; }
				//	else if (temp < 0.333f) { levelData[i][j] = 101; }
				//	else if (temp < 0.400f) { levelData[i][j] = 117; }
				//	else if (temp < 0.466f) { levelData[i][j] = 133; }
				//	else if (temp < 0.533f) { levelData[i][j] = 149; }
				//	else if (temp < 0.600f) { levelData[i][j] = 165; }
				//	else if (temp < 0.666f) { levelData[i][j] = 217; }
				//	else if (temp < 0.733f) { levelData[i][j] = 220; }
				//	else if (temp < 0.800f) { levelData[i][j] = 223; }
				//	else if (temp < 0.866f) { levelData[i][j] = 241; }
				//	else if (temp < 0.933f) { levelData[i][j] = 257; }
				//	else { levelData[i][j] = 273; }
				//}
				//else if (!cellmap[i][j - 1]) {
				//	//center right
				//	if (temp < 0.066f) { levelData[i][j] = 0; }
				//	else if (temp < 0.133f) { levelData[i][j] = 16; }
				//	else if (temp < 0.200f) { levelData[i][j] = 32; }
				//	else if (temp < 0.266f) { levelData[i][j] = 84; }
				//	else if (temp < 0.333f) { levelData[i][j] = 100; }
				//	else if (temp < 0.400f) { levelData[i][j] = 116; }
				//	else if (temp < 0.466f) { levelData[i][j] = 132; }
				//	else if (temp < 0.533f) { levelData[i][j] = 148; }
				//	else if (temp < 0.600f) { levelData[i][j] = 164; }
				//	else if (temp < 0.666f) { levelData[i][j] = 215; }
				//	else if (temp < 0.733f) { levelData[i][j] = 218; }
				//	else if (temp < 0.800f) { levelData[i][j] = 221; }
				//	else if (temp < 0.866f) { levelData[i][j] = 240; }
				//	else if (temp < 0.933f) { levelData[i][j] = 256; }
				//	else { levelData[i][j] = 272; }
				//}
				//else {
				//	if (temp < 0.111f) { levelData[i][j] = 29; }
				//	else if (temp < 0.22f) { levelData[i][j] = 30; }
				//	else if (temp < 0.33f) { levelData[i][j] = 31; }
				//	else if (temp < 0.44f) { levelData[i][j] = 45; }
				//	else if (temp < 0.55f) { levelData[i][j] = 46; }
				//	else if (temp < 0.66f) { levelData[i][j] = 47; }
				//	else if (temp < 0.77f) { levelData[i][j] = 61; }
				//	else if (temp < 0.88f) { levelData[i][j] = 62; }
				//	else { levelData[i][j] = 63; }
				//}
			}// end solid
			//background: 89, 90, 91, 92, 105, 106, 107, 108, 121, 122, 123, 124
			else {
				//int count = 0;
				//levelData[i][j] = 61;
				if (cellmap[i + 1][j]) {
					if (abs(i - spawn_y) > mapHeight - (mapHeight / 5) &&
						//i < mapHeight - 11 &&
						j > 10 &&
						j < mapHeight - 11) {
						float altar_x, altar_y;
						tileToWorldCoordinates(j, i, &altar_x, &altar_y);
						altarSpawns.push_back(Vector(altar_x, altar_y));
						//levelData[i][j] = 62;
						//count++;
					}
				}

				levelData[i][j] = 61;

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
				
				
			}// end nonsolid blocks
		}
	}
	//resolve item spawns 
	int select = randomInt(0, altarSpawns.size() - 1);
	altarSpawn_x = altarSpawns[select].x + 0.05f;
	altarSpawn_y = altarSpawns[select].y - 0.05f;

	int altar_x, altar_y;
	worldToTileCoordinates(altarSpawns[select].x, altarSpawns[select].y, &altar_x, &altar_y);
	levelData[altar_y + 1][altar_x - 1] = 14;
	levelData[altar_y + 1][altar_x] = 14;
	levelData[altar_y + 1][altar_x + 1] = 14;
	levelData[altar_y][altar_x - 1] = 61;
	levelData[altar_y][altar_x] = 61;
	levelData[altar_y][altar_x + 1] = 61;
	levelData[altar_y - 1][altar_x - 1] = 61;
	levelData[altar_y - 1][altar_x] = 61;
	levelData[altar_y - 1][altar_x + 1] = 61;
	altarSpawns.clear();
	//levelData[2][3] = 62;
	//levelData[altarSpawn_y][altarSpawn_x] = 63;
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
	Type 1:
		open block with solid block underneath
			dumbMob
	Type 2:
		any open block
			bats
	Type 3:
		open block with solid block above
			stalagtites
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
		while ( isSolid(levelData[grid_y][grid_x]) || !isSolid(levelData[grid_y + 1][grid_x]) ||
				grid_x < 1 || grid_x > mapWidth - 1 || grid_y < 1 || grid_y > mapHeight - 1) {
			float rand = randomFloat(0.0f, 1.0f);
			if (rand < 0.25f) {
				x = player->position.x - randomFloat(1.33f, 2.0f);
				y = player->position.y - randomFloat(1.0f, 2.0f);
			}
			else if (rand < 0.50f) {
				x = player->position.x - randomFloat(1.33f, 2.0f);
				y = player->position.y + randomFloat(1.0f, 2.0f);
			}
			else if (rand < 0.75f) {
				x = player->position.x + randomFloat(1.33f, 2.0f);
				y = player->position.y + randomFloat(1.0f, 2.0f);
			}
			else {
				x = player->position.x + randomFloat(1.33f, 2.0f);
				y = player->position.y - randomFloat(1.0f, 2.0f);
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
	
	if (type == 2) {
		while ( isSolid(levelData[grid_y][grid_x]) || 
			grid_x < 1 || grid_x > mapWidth - 1 || grid_y < 1 || grid_y > mapHeight - 1) {
			float rand = randomFloat(0.0f, 1.0f);
			if (rand < 0.25f) {
				x = player->position.x - randomFloat(1.33f, 2.0f);
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
				x = player->position.x + randomFloat(1.33f, 2.0f);
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

	if (type == 3) {
		while (isSolid(levelData[grid_y][grid_x]) || !isSolid(levelData[grid_y - 1][grid_x]) ||
			grid_x < 1 || grid_x > mapWidth - 1 || grid_y < 1 || grid_y > mapHeight - 1) {
			float rand = randomFloat(0.0f, 1.0f);
			if (rand < 0.25f) {
				x = player->position.x - randomFloat(1.33f, 2.6f);
				y = player->position.y - randomFloat(1.0f, 2.0f);
			}
			else if (rand < 0.50f) {
				x = player->position.x - randomFloat(1.33f, 2.6f);
				y = player->position.y + randomFloat(0.0f, 1.5f);
			}
			else if (rand < 0.75f) {
				x = player->position.x + randomFloat(1.33f, 2.6f);
				y = player->position.y + randomFloat(0.0f, 1.5f);
			}
			else {
				x = player->position.x + randomFloat(1.33f, 2.6f);
				y = player->position.y - randomFloat(1.0f, 2.0f);
			}

			int temp_x, temp_y;
			worldToTileCoordinates(x, y, &temp_x, &temp_y);
			if (temp_x > 0 && temp_x < mapWidth - 1 && temp_y > 0 && temp_y < mapHeight - 1) {
				grid_x = temp_x;
				grid_y = temp_y;
				tileToWorldCoordinates(temp_x, temp_y, &x, &y);
			}
		}
		return Vector(x, y - 0.01f);
	}

}

//void Game::spawnBoss() {
//	int altar_x, altar_y;
//	worldToTileCoordinates(altarSpawn_x, altarSpawn_y, &altar_x, &altar_y);
//
//
//
//}

//float animationValue = mapValue(timeElapsed, 0.0f, 3.0f, 0.0f, 1.0f);
//drawText(font, "<Game Name>", 0.3f, -0.13f, easeOut(-3.0f, -0.85f, animationValue), 0.7f);