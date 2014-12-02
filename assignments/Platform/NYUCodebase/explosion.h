#pragma once
#include "sheetSprite.h"

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

class Explosion {
private:
public:
	Explosion();
	Explosion(Matrix m);
	~Explosion();

	void FixedUpdate();
	void Render();

	//void buildMatrix;

	SheetSprite sprite;
	Matrix matrix;
	float scale;
	//Vector position;
	//int index;
};

Explosion::Explosion(){
	scale = 1.0f;
	//index = 0;
}

Explosion::Explosion(Matrix m) {
	scale = 1.0f;
	matrix = m;
	//index = 0;
}

Explosion::~Explosion() {
}

void Explosion::FixedUpdate() {
	sprite.frame++;
	//index++;
}

void Explosion::Render() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix.ml);
	sprite.draw(scale, scale);
	glPopMatrix();
}
