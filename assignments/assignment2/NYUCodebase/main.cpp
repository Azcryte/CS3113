
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

SDL_Window* displayWindow;
const float BALL_HEIGHT = 0.1f;
const float BALL_WIDTH = 0.1f;

const float PADDLE_HEIGHT = 0.35f;
const float PADDLE_WIDTH = 0.1f;
const float PADDLE_POSITION = 0.9f;

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

	GLfloat quad[] = { (float)(0-width/2), (float)(height/2), 
					   (float)(0-width/2), (float)(0-height/2), 
					   (float)(width/2), (float)(0-height/2), 
					   (float)(width/2), (float)(height/2) };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
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
	void move(Game* theGame, float tick) {
		if (theGame->gameState) {
			if (movingRight) { xPos += tick; }
			else { xPos -= tick; }

			if (movingUp) { yPos += tick / 2; }
			else { yPos -= tick / 2; }
		}
	}
	void draw() {
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
		if (moveUp && yPos <= 0.85f) { yPos += tick;  }
		if (moveDown && yPos >= -0.85f) { yPos -= tick;  }
	}
	void draw() {
		DrawSprite(paddleTexture, xPos, yPos, 90.0, PADDLE_WIDTH, PADDLE_HEIGHT );
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

	if (ballR > 1.0f) { 
		aBall->reset(theGame); 
		theGame->p2Score++;
	}
	if (ballL < -1.0f) { 
		aBall->reset(theGame); 
		theGame->p1Score++;
	}
}

void Setup() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, 0.0);
}

int main(int argc, char *argv[])
{
	Setup();

	bool done = false;

	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	float lastFrameTicks = 0.0f;

	SDL_Event event;

	Game theGame;
	Ball theBall;
	Paddle paddleR(PADDLE_POSITION);
	Paddle paddleL(0 - PADDLE_POSITION);

	while (!done) {

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

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
				if (event.button.button == 1) { theGame.gameState = true; }
				if (event.button.button == 3) { theGame.gameState = false; }
			}
		}

		theBall.move(&theGame, elapsed);
		paddleL.move(elapsed, keys[SDL_SCANCODE_W], keys[SDL_SCANCODE_S]);
		paddleR.move(elapsed, keys[SDL_SCANCODE_UP], keys[SDL_SCANCODE_DOWN]);

		checkCollisions(&theBall, &paddleR);
		checkCollisions(&theBall, &paddleL);
		checkOB(&theBall);
		checkForScore(&theBall, &theGame);

		theBall.draw();
		paddleL.draw();
		paddleR.draw();

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}