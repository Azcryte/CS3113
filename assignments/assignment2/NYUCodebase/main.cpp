#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <time.h>

SDL_Window* displayWindow;

const Uint8 *keys = SDL_GetKeyboardState(NULL);

const float PLAYER_HEIGHT = 0.15f;
const float PLAYER_WIDTH = 0.2f;

const float ENEMY_HEIGHT = 0.1f;
const float ENEMY_WIDTH = 0.2f;

float BALL_HEIGHT = 0.3f;
float BALL_WIDTH = 0.3f;

const float PADDLE_HEIGHT = 0.35f;
const float PADDLE_WIDTH = 0.025f;
const float PADDLE_POSITION = 1.25f;

GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);

	return textureID;
}

void DrawSprite(GLint texture, float x, float y, float rotation, float height, float width) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = { (float)(0 - width / 2), (float)(height / 2),
		(float)(0 - width / 2), (float)(0 - height / 2),
		(float)(width / 2), (float)(0 - height / 2),
		(float)(width / 2), (float)(height / 2) };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	//GLfloat quadUVs[] = { 0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 0.5 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

void DrawSpriteSheetSprite(int spriteTexture, int index, int spriteCountX, int spriteCountY) {
	// our regular sprite drawing 
	float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
	float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
	float spriteWidth = 1.0 / (float)spriteCountX;
	float spriteHeight = 1.0 / (float)spriteCountY;
	GLfloat quadUVs[] = { u, v,
		u, v + spriteHeight,
		u + spriteWidth, v + spriteHeight,
		u + spriteWidth, v
	};
	// our regular sprite drawing 
}

class Game
{
public:
	bool gameState;
	int p1Score, p2Score;
	Game() : gameState(false), p1Score(0), p2Score(0) {}
};

class Ball
{
private:
	GLuint ballTexture;
public:
	float xPos, yPos;
	bool movingRight;
	bool movingUp;
	Ball() : xPos(0.0f), yPos(0.0f), movingRight(false), movingUp(true) {
		ballTexture = LoadTexture("whiteBall.png");
	}
	void reset(Game* theGame) {
		xPos = 0.0f;
		yPos = 0.0f;
		movingRight = false;
		movingUp = true;
		theGame->gameState = false;
	}
	void move(Game* theGame, float tick, int r1) {
		if (theGame->gameState) {
			if (movingRight) { xPos += tick; }
			else { xPos -= tick; }

			if (movingUp) { yPos += (float)(r1 * 0.0002); }
			else { yPos -= (float)(r1 * 0.0002); }
		}
	}
	void draw(/*int gameState*/) {

		DrawSprite(ballTexture, xPos, yPos, 0.0, BALL_WIDTH, BALL_HEIGHT);
	}
};

class Paddle
{
private:
	GLuint paddleTexture;
public:
	float xPos, yPos;
	Paddle() : xPos(0.0f), yPos(0.0f) {
		paddleTexture = LoadTexture("paddle.png");
	}
	Paddle(float position) : xPos(position), yPos(0.0f) {
		paddleTexture = LoadTexture("paddle.png");
	}
	void move(float tick, bool moveUp, bool moveDown) {
		if (moveUp && yPos <= 0.85f) { yPos += 4 * tick; }
		if (moveDown && yPos >= -0.85f) { yPos -= 4 * tick; }
	}
	void draw() {
		DrawSprite(paddleTexture, xPos, yPos, 90.0, PADDLE_WIDTH, PADDLE_HEIGHT);
	}
};

bool checkCollisions(Ball* aBall, Paddle* aPaddle) {
	float ballT = aBall->yPos + BALL_HEIGHT / 2;
	float ballB = aBall->yPos - BALL_HEIGHT / 2;
	float ballR = aBall->xPos + BALL_WIDTH / 2;
	float ballL = aBall->xPos - BALL_WIDTH / 2;

	float paddleT = aPaddle->yPos + PADDLE_HEIGHT / 2;
	float paddleB = aPaddle->yPos - PADDLE_HEIGHT / 2;
	float paddleR = aPaddle->xPos + PADDLE_WIDTH / 2;
	float paddleL = aPaddle->xPos - PADDLE_WIDTH / 2;

	if (ballB >= paddleT) { return false; }
	if (ballT <= paddleB) { return false; }
	if (ballR <= paddleL) { return false; }
	if (ballL >= paddleR) { return false; }

	if (aBall->movingRight) { aBall->movingRight = false; }
	else { aBall->movingRight = true; }
	BALL_HEIGHT *= 0.85f;
	BALL_WIDTH *= 0.85f;

	return true;
}

void checkOB(Ball* aBall) {
	float ballT = aBall->yPos + BALL_HEIGHT / 2;
	float ballB = aBall->yPos - BALL_HEIGHT / 2;
	float ballR = aBall->xPos + BALL_WIDTH / 2;
	float ballL = aBall->xPos - BALL_WIDTH / 2;

	if (ballT > 1.00 && aBall->movingUp) { aBall->movingUp = false; }
	if (ballB < -1.00 && !aBall->movingUp) { aBall->movingUp = true; }
}

void checkForScore(Ball* aBall, Game* theGame) {
	float ballR = aBall->xPos + BALL_WIDTH / 2;
	float ballL = aBall->xPos - BALL_WIDTH / 2;

	if (ballR > 1.33f) {
		aBall->reset(theGame);
		theGame->p2Score++;
		BALL_HEIGHT = 0.3f;
		BALL_WIDTH = 0.3f;

	}
	if (ballL < -1.33f) {
		aBall->reset(theGame);
		theGame->p1Score++;
		BALL_HEIGHT = 0.3f;
		BALL_WIDTH = 0.3f;

	}
}

/* */
class Player {
private:
	GLuint playerTexture;
	float xPos, yPos;
	int speed;

public:
	Player() : xPos(0.0f), yPos(-0.95f), speed(20) {
		playerTexture = LoadTexture("whiteBall.png");
	}
	void move() {
		if (keys[SDL_SCANCODE_D]) { xPos += speed / 1000.0f; }
		if (keys[SDL_SCANCODE_A]) { xPos -= speed / 1000.0f; }
	}
	void draw() {
		DrawSprite(playerTexture, xPos, yPos, 0.0, PLAYER_HEIGHT, PLAYER_WIDTH);
	}
};

class Enemy {
private:
	GLuint enemyTexture;
	float xPos, yPos;
	bool movingRight;
	int health;
	int strength;
	int speed;
public:
	Enemy() : xPos(0), yPos(0), movingRight(true), health(1), strength(1), speed(15) {
		enemyTexture = LoadTexture("paddle.png");
	};
	void move(int frame) {
		//if (frame % 15 == 0) { xPos += speed / 100.0f; }
		if (movingRight) {
			xPos += speed / 1000.0f;
		}
		else {
			xPos -= speed / 1000.0f;
		}
		if (movingRight && xPos + ENEMY_WIDTH / 2 > 1.30f) {
			movingRight = false;
			yPos -= 0.1f;
		}
		else if (!movingRight && xPos - ENEMY_WIDTH / 2 < -1.30f) {
			movingRight = true;
			yPos -= 0.1f;
		}
	}
	void draw() {
		DrawSprite(enemyTexture, xPos, yPos, 0.0, ENEMY_HEIGHT, ENEMY_WIDTH);
	}
};

void Setup() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(0.0, 0.0, 0.0);
}

int main(int argc, char *argv[])
{
	Setup();

	bool done = false;
	/*
	gameState
	0 = main menu
	1 = play
	2 = pause
	3 = game over
	*/
	int gameState = 1;
	int frame = 0;

	int playerLives;
	int playerScore;

	float lastFrameTicks = 0.0f;

	SDL_Event event;

	//Game theGame;
	//Ball theBall;
	//Paddle paddleR(PADDLE_POSITION);
	//Paddle paddleL(0 - PADDLE_POSITION);

	/* test area */

	//const int runAnimation[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	//const int numFrames = 9;
	//float animationElapsed = 0.0f;
	//float framesPerSecond = 5.0f;
	//int currentIndex = 0;

	GLuint testSpriteSheet = LoadTexture("bleach.png");

	/*    end    */

	Player thePlayer;
	Enemy theEnemy;

	while (!done) {

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		/* test area */

		//animationElapsed += elapsed;
		//if (animationElapsed > 1.0 / framesPerSecond) {
		//	currentIndex++;
		//	animationElapsed = 0.0;
		//	if (currentIndex > numFrames - 1) {
		//		currentIndex = 0;
		//	}
		//}

		//DrawSpriteSheetSprite(testSpriteSheet, runAnimation[currentIndex], 9, 18);

		/*    end    */

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			/*else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {}
			}*/
			// Right click to unpause
			// Left click to pause
			else if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == 1) { gameState = 2; }
				if (event.button.button == 3) { gameState = 1; }
			}
		}

		thePlayer.draw();
		//theEnemy.draw();

		if (gameState != 1) {
			thePlayer.move();
			theEnemy.move(frame);

			frame++;
		}

		//int r = rand() % 2;
		//int r1 = rand() % 15 + 1;
		//int r2 = rand() % 20 + 10;
		//int multiply = rand() % 10;
		//srand(time(NULL));
		//theBall.move(&theGame, elapsed, r1);
		//paddleL.move(elapsed, keys[SDL_SCANCODE_W], keys[SDL_SCANCODE_S]);
		//paddleR.move(elapsed, keys[SDL_SCANCODE_UP], keys[SDL_SCANCODE_DOWN]);

		//checkCollisions(&theBall, &paddleR);
		//checkCollisions(&theBall, &paddleL);
		//checkOB(&theBall);
		//checkForScore(&theBall, &theGame);

		//theBall.draw();
		//paddleL.draw();
		//paddleR.draw();

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}