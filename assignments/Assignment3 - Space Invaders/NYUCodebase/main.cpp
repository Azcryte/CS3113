
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <string>
using namespace std;

#define MAX_BULLETS 1
#define NUM_ENEMIES_STAGE_1 28
#define NUM_BLOCKADES 4
SDL_Window* displayWindow;

const Uint8 *keys = SDL_GetKeyboardState(NULL);

const float PLAYER_HEIGHT = 0.15f;
const float PLAYER_WIDTH = 0.2f;

int score;

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
	//GLfloat quadUVs[] = { 0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 0.5 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

void DrawSpriteAnimation(GLint texture, int index, int spriteCountX, int spriteCountY) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glMatrixMode(GL_MODELVIEW);
	 
	// our regular sprite drawing 
	float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
	float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
	float width = 1.0f / (float)spriteCountX;
	float height = 1.0f / (float)spriteCountY;

	GLfloat quad[] = { (float)(0 - width / 2), (float)(height / 2),
				   	   (float)(0 - width / 2), (float)(0 - height / 2),
					   (float)(width / 2), (float)(0 - height / 2),
					   (float)(width / 2), (float)(height / 2) };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { u, v,
		u, v + height,
		u + width, v + height,
		u + width, v
	};
	// our regular sprite drawing 

	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

void DrawText(int fontTexture, string text, float size, float spacing, float xDisp, float yDisp/*, float r, float g, float b, float a*/) {

	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float texture_size = 1.0 / 16.0f;

	vector<float> vertexData;
	vector<float> texCoordData;
	//vector<float> colorData;

	for (unsigned int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
		//colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });

		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f * size) + xDisp, 0.5f * size + yDisp,
											  ((size + spacing) * i) + (-0.5f * size) + xDisp, -0.5f * size + yDisp,
											  ((size + spacing) * i) + (0.5f * size) + xDisp, -0.5f * size + yDisp,
											  ((size + spacing) * i) + (0.5f * size) + xDisp, 0.5f * size + yDisp });

		texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +
			texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}
	//glColorPointer(4, GL_FLOAT, 0, colorData.data());
	//glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);
}

/* */
class SheetSprite {
private:
	GLuint textureID;
	float u;
	float v;
	float width;
	float height;
public:
	SheetSprite() {};
	SheetSprite(GLuint textureID, float u, float v, float width, float height) 
		: textureID(textureID), u(u), v(v), width(width), height(height) {};

	void draw(float scale, float x, float y, float rotation) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();
		glTranslatef(x, y, 0.0);
		glRotatef(rotation, 0.0, 0.0, 1.0);

		GLfloat quad[] = { -width * scale, height * scale, -width * scale, -height * scale,
			width * scale, -height * scale, width * scale, height * scale };

		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);

		GLfloat quadUVs[] = { u, v, u, v + height, u + width, v + height, u + width, v };

		glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
	}

};

class Player;

class Bullet {
public:
	SheetSprite bulletTexture;
	float xPos, yPos;
	float width, height;
	bool visible;
	Bullet() :xPos(0.0f), yPos(-0.9f), width((float)2 * 13.0f / 1024.0f), height((float)2 * 54.0f / 1024.0f), visible(false) {
		GLuint spriteSheetTexture = LoadTexture("sheet.png");
		bulletTexture = SheetSprite(spriteSheetTexture, 843.0f / 1024.0f, 62.0f / 1024.0f, width / 2, height / 2);
	}
	Bullet(float x, float y) :visible(false) {}
	void move(float speed, float xResetPos) {
		yPos += speed / 1000.0f;
		if (yPos > 1.1f) {
			yPos = -0.9f;
			xPos = xResetPos;
			visible = false;
		}
	}
	void draw() {
		bulletTexture.draw(1.0f, xPos, yPos, 0.0);
	}
};

class Player {
public:
	SheetSprite playerTexture;
	vector<Bullet*> bullets;
	//Bullet* bullets<MAX_BULLETS>;
	float xPos, yPos, scale;
	int speed;
	int bulletSpeed;
	int bulletIndex;
	Player() : xPos(0.0f), yPos(-0.95f), scale(1.0f), speed(1.5), bulletSpeed(2.5), bulletIndex(0) {
		GLuint sheet = LoadTexture("sheet.png");
		playerTexture = SheetSprite(sheet, 211.0f / 1024.0f, 941.0f / 1024.0f, 99.0f / 1024.0f, 75.0f / 1024.0f);
		Bullet* temp = new Bullet(); 
		bullets.push_back(temp);
	}
	void shootBullet() {
		if (!bullets[bulletIndex]->visible) {
			bullets[bulletIndex]->visible = true;
			bullets[bulletIndex]->xPos = xPos;
			bullets[bulletIndex]->yPos = -1.0f;
		}
		bulletIndex++;
		if (bulletIndex > MAX_BULLETS - 1) {
			bulletIndex = 0;
		}
	};
	void move() {
		if (keys[SDL_SCANCODE_D] && xPos < 1.33 - PLAYER_WIDTH / 2) { xPos += speed / 1000.0f; }
		if (keys[SDL_SCANCODE_A] && xPos > -1.33 + PLAYER_WIDTH / 2) { xPos -= speed / 1000.0f; }
		if (keys[SDL_SCANCODE_SPACE]) {
			shootBullet();
		}
		for (unsigned int i = 0; i < MAX_BULLETS; i++) {
			bullets[i]->move(bulletSpeed, xPos);
		}
	}
	void draw() {
		playerTexture.draw(scale, xPos, yPos, 0.0);
		for (int i = 0; i < MAX_BULLETS; i++) {
			if (bullets[i]->visible) {
				bullets[i]->draw();
			}
			else
				bullets[i]->yPos = -0.9f;
		}
	}
};

class Enemy {
public:
	SheetSprite enemyTexture;
	float xPos, yPos;
	float width, height;
	float scale;
	bool alive;
	int health;
	int strength;
	double speed;
	bool movingRight;
	Enemy() : xPos(0), yPos(0), width((float)2 * 93.0f / 1024.0f), height((float)2 * 84.0f / 1024.0f),  scale(1.0), movingRight(true), alive(true), health(1), strength(1), speed(1.0) {
		GLuint spriteSheetTexture = LoadTexture("sheet.png");
		enemyTexture = SheetSprite(spriteSheetTexture, 423.0f / 1024.0f, 728.0f / 1024.0f, width / 2, height / 2);
	};
	void changeDirection() {
		movingRight = !movingRight;
	}
	void goFaster() {
		if (speed < 3.0) {
			speed += 0.08;
		}
	}
	void move(int elapsed) {
		if ( movingRight ) {
			xPos += speed / 1000.0f;
		} 
		else {
			xPos -= speed / 1000.0f;
		}
	}
	void draw() {
		if (alive) {
			enemyTexture.draw(scale, xPos, yPos, 0.0);
		}
	}
};

class Blockade{
public:
	SheetSprite blockadeTextureBig;
	SheetSprite blockadeTextureMed;
	SheetSprite blockadeTextureSmall;
	SheetSprite blockadeTextureTiny;
	float xPos, yPos;
	float scale;
	int health;
	float width, height;
	Blockade() : xPos(0.0), yPos(-0.6), scale(1.5), health(8), width(0.0), height(0.0) {
		GLuint spriteSheetTexture = LoadTexture("sheet.png");
		blockadeTextureBig = SheetSprite(spriteSheetTexture, 0.0f / 1024.0f, 520.0f / 1024.0f, 120.0f / 1024.0f, 98.0f / 1024.0f);
		blockadeTextureMed = SheetSprite(spriteSheetTexture, 224.0f / 1024.0f, 664.0f / 1024.0f, 101.0f / 1024.0f, 84.0f / 1024.0f);
		blockadeTextureSmall = SheetSprite(spriteSheetTexture, 327.0f / 1024.0f, 452.0f / 1024.0f, 98.0f / 1024.0f, 96.0f / 1024.0f);
		blockadeTextureTiny = SheetSprite(spriteSheetTexture, 518.0f / 1024.0f, 810.0f / 1024.0f, 89.0f / 1024.0f, 82.0f / 1024.0f);
	};
	void draw() {
		if (health > 6) {
			width = (float)2 * scale * 120.0f / 1024.0f;
			height = (float)2 * scale * 98.0f / 1024.0f;
			blockadeTextureBig.draw(scale, xPos, yPos, 0.0);
		}
		else if (health > 4 && health <= 6) {
			width = (float)2 * scale * 101.0f / 1024.0f;
			height = (float)2 * scale * 84.0f / 1024.0f;
			blockadeTextureMed.draw(scale, xPos, yPos, 0.0);
		}
		else if (health > 2 && health <= 4) {
			scale = 1.35;
			width = (float)2 * scale * 98.0f / 1024.0f;
			height = (float)2 * scale * 96.0f / 1024.0f;
			blockadeTextureSmall.draw(scale, xPos, yPos, 0.0);
		}
		else if (health > 0 && health <= 2) {
			scale = 1.2;
			width = (float)2 * scale * 89.0f / 1024.0f;
			height = (float)2 * scale * 82.0f / 1024.0f;
			blockadeTextureTiny.draw(scale, xPos, yPos, 0.0);
		}
		blockadeTextureBig.draw(0.0, xPos, yPos, 0.0);
	}

};

struct Point {
	float x, y;
};

bool checkForCollision(Point l1, Point r1, Point l2, Point r2) {
	if (r1.x < l2.x || r2.x < l1.x)
		return false;
	if (r1.y > l2.y || r2.y > l1.y)
		return false;
	return true;
}

void updateCollisions(Player player, vector<Bullet*>& bullets, vector<Enemy*>& enemies, vector<Blockade*>& blockades);

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

void boom(GLuint spriteTestTexture, int runAnimation[], int numFrames, float animationElapsed, float framesPerSecond, int currentIndex, int frame, float elapsed) {
	animationElapsed += elapsed;
	if (animationElapsed > 1.0 / framesPerSecond) {
		currentIndex++;
		animationElapsed = 0.0;
		if ((currentIndex > numFrames - 1) && (frame % 50 == 0)) {
			currentIndex = 0;
		}
	}
	if (currentIndex < 25) {
		DrawSpriteAnimation(spriteTestTexture, runAnimation[currentIndex], 5, 5);
	}
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
	int gameState = 0;
	int frame = 0;
	int count = 0;
	float lastFrameTicks = 0.0f;
	SDL_Event event;
	GLuint fontTexture = LoadTexture("font.png");

	/* 

	const int runAnimation[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
	const int numFrames = 25;
	float animationElapsed = 0.0f;
	float framesPerSecond = 30.0f;
	int currentIndex = 0;

	GLuint spriteTestTexture = LoadTexture("explosion.png");

	*/

	Player thePlayer;
	vector<Enemy*> enemies;
	for (int i = 0; i < NUM_ENEMIES_STAGE_1; i++) {
		Enemy* temp = new Enemy();
		enemies.push_back(temp);
		float y = 0.6f - (float)((i / 7) * (enemies[i]->height + 0.05f));
		float x = -1.33 + (float)((i % 7) * (enemies[i]->width + 0.1f)) + enemies[i]->width / 2;
		enemies[i]->yPos = y;
		enemies[i]->xPos = x;
	}

	vector<Blockade*> blockades;
	for (int i = 0; i < NUM_BLOCKADES; i++) {
		Blockade* temp = new Blockade();
		blockades.push_back(temp);
		float x = -1.33 + (float)((2.66f / 5) * (i + 1));
		blockades[i]->xPos = x;
	}

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
				//if (event.button.button == 4) { gameState = 3; }
				if (event.button.button == 1) { gameState = 2; }
				if (event.button.button == 3) { gameState = 1; }
				if (event.button.button == 2) { gameState = 0; }
			}
		}

		enum GameState {MAIN_MENU, PLAY_GAME, PAUSE_GAME, GAME_OVER, WIN};
		switch (gameState) {
		case MAIN_MENU:

			DrawText(fontTexture, "Main Menu", 0.5f, -0.25f, -1.0f, 0.5f);
			if (frame % 50 < 40) {
				DrawText(fontTexture, "Right click to start", 0.2f, -0.1f, -0.9f, -0.3f);
			}
			frame++;

			break;

		case PLAY_GAME:

			count = 0;
			thePlayer.move();
			//theEnemy.move(frame);
			for (int i = 0; i < NUM_ENEMIES_STAGE_1; i++) {
				enemies[i]->move(elapsed);
				if (((enemies[i]->movingRight && enemies[i]->xPos + enemies[i]->width / 2 > 1.30f) || (!enemies[i]->movingRight && enemies[i]->xPos - enemies[i]->width / 2 < -1.30f))
					&& enemies[i]->alive)
				{
					for (int j = 0; j < NUM_ENEMIES_STAGE_1; j++) {
						enemies[j]->changeDirection();
						enemies[j]->yPos -= 0.02f;
						enemies[j]->goFaster();
						//xPos correction (fix)
						enemies[7]->xPos += 0.00002f;
						enemies[14]->xPos += 0.00005f;
						enemies[21]->xPos += 0.00005f;
					}
				}
				if (enemies[i]->yPos < -0.8 && enemies[i]->alive) {
					gameState = 3;
				}
				if (enemies[i]->alive)
					count++;
			}
			if (count == 0)
				gameState = 4;
			updateCollisions(thePlayer, thePlayer.bullets, enemies, blockades);
			for (int i = 0; i < NUM_ENEMIES_STAGE_1; i++) {
				enemies[i]->draw();
			}
			thePlayer.draw();
			for (int i = 0; i < NUM_BLOCKADES; i++) {
				blockades[i]->draw();
			}
			DrawText(fontTexture, "Score: " + to_string(score), 0.15f, -0.08f, -0.2f, 1.5f);

			frame++;
			break;

		case PAUSE_GAME:

			for (int i = 0; i < NUM_ENEMIES_STAGE_1; i++) {
				enemies[i]->draw();
			}
			thePlayer.draw();
			for (int i = 0; i < NUM_BLOCKADES; i++) {
				blockades[i]->draw();
			}
			DrawText(fontTexture, "Score: " + to_string(score), 0.15f, -0.08f, -0.2f, 1.5f);

			frame++;
			break;

		case GAME_OVER:

			glLoadIdentity();
			glTranslatef(0.0, 0.0, 0.0);

			DrawText(fontTexture, "GAME OVER", 0.5f, -0.25f, -1.0f, 0.5f);
			DrawText(fontTexture, "Final Score: " + to_string(score), 0.2f, -0.1f, -0.65f, -0.3f);

			break;

		case WIN:
			glLoadIdentity();
			glTranslatef(0.0, 0.0, 0.0);

			DrawText(fontTexture, "WINNER", 0.5f, -0.2f, -0.75f, 0.5f);
			DrawText(fontTexture, "Final Score: " + to_string(score), 0.2f, -0.1f, -0.6f, -0.3f);
			break;
		}

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}

void updateCollisions(Player player, vector<Bullet*>& bullets, vector<Enemy*>& enemies, vector<Blockade*>& blockades) {
	Point l1, r1;
	Point l2, r2;

	l1 = { bullets[0]->xPos - bullets[0]->width / 2, bullets[0]->yPos + bullets[0]->height / 2 - 0.05f};
	r1 = { bullets[0]->xPos + bullets[0]->width / 2, bullets[0]->yPos - bullets[0]->height / 2};

	// bullet vs enemy

	for (int i = 0; i < NUM_ENEMIES_STAGE_1; i++) {
		l2 = { enemies[i]->xPos - enemies[i]->width / 2, enemies[i]->yPos + enemies[i]->height / 2 };
		r2 = { enemies[i]->xPos + enemies[i]->width / 2, enemies[i]->yPos - enemies[i]->height / 2 };
		if (checkForCollision(l1, r1, l2, r2) && enemies[i]->alive && bullets[0]->visible) {
			bullets[0]->yPos = -0.9;
			bullets[0]->xPos = player.xPos;
			bullets[0]->visible = false;

			enemies[i]->alive = false;

			score += (enemies[i]->strength + enemies[i]->speed) * enemies[i]->health;
		}
	}

	// bullet vs blockade

	for (int i = 0; i < NUM_BLOCKADES; i++) {
		l2 = { blockades[i]->xPos - blockades[i]->width / 2, blockades[i]->yPos + blockades[i]->height / 2 };
		r2 = { blockades[i]->xPos + blockades[i]->width / 2, blockades[i]->yPos - blockades[i]->height / 2 };
		if (checkForCollision(l1, r1, l2, r2) && blockades[i]->health > 0) {
			bullets[0]->yPos = -0.9;
			bullets[0]->xPos = player.xPos;
			bullets[0]->visible = false;

			blockades[i]->health--;
		}
	}

	// blockade vs enemy

	for (int i = 0; i < NUM_BLOCKADES; i++) {
		for (int j = 0; j < NUM_ENEMIES_STAGE_1; j++) {
			l1 = { blockades[i]->xPos - blockades[i]->width / 2, blockades[i]->yPos + blockades[i]->height / 2 };
			r1 = { blockades[i]->xPos + blockades[i]->width / 2, blockades[i]->yPos - blockades[i]->height / 2 };

			l2 = { enemies[j]->xPos - enemies[i]->width / 2, enemies[j]->yPos + enemies[i]->height / 2 };
			r2 = { enemies[j]->xPos + enemies[i]->width / 2, enemies[j]->yPos - enemies[i]->height / 2 };

			if (checkForCollision(l1, r1, l2, r2) && blockades[i]->health > 0 && enemies[j]->alive) {
				enemies[j]->alive = false;

				blockades[i]->health -= 3;
			}
		}

	}

}
