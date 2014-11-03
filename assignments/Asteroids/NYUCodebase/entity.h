#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include "sheetSprite.h"
#include "matrix.h"
using namespace std;

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

float randomFloat(float min, float max){
	float random = ((float)rand()) / (float)RAND_MAX;
	return (random*(max - min)) + min;
}

class Entity{
private:
public:
	Entity();
	~Entity();

	void FixedUpdate();
	void Render();
	void buildMatrix();

	SheetSprite sprite;
	Matrix matrix;
	Vector vector;
	void setVector();

	float x;
	float y;
	float scale_x;
	float scale_y;
	float rotation;
	float speed;

	float vel_x;
	float vel_y;
	float acc_x;
	float acc_y;
	float fric_x;
	float fric_y;
	float mass;

	bool isStatic;
	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;
	
};

void Entity::setVector(){
	vector = Vector(float(cos(rotation + M_PI / 2.0f)), float(sin(rotation + M_PI / 2.0f)), 0.0f);
}

Entity::Entity() {
	x = 0.0f;
	y = 0.0f;
	scale_x = 1.0f;
	scale_y = 1.0f;
	rotation = 0.0f;
	speed = 1.0f;

	vel_x = 0.0f;
	vel_y = 0.0f;
	acc_x = 0.0f;
	acc_y = 0.0f;
	fric_x = 0.0f;
	fric_y = 0.0f;
	mass = 0.0f;

	isStatic = false;
	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;
}

Entity::~Entity() {}

void Entity::FixedUpdate() {

	setVector();
	vector.x *= speed;
	vector.y *= speed;
	buildMatrix();

	vel_x = lerp(vel_x, 0.0f, FIXED_TIMESTEP * fric_x);
	vel_y = lerp(vel_y, 0.0f, FIXED_TIMESTEP * fric_y);

	vel_x += acc_x * FIXED_TIMESTEP;
	vel_y += acc_y * FIXED_TIMESTEP;

	x += vel_x * FIXED_TIMESTEP;
	y += vel_y * FIXED_TIMESTEP;

	if (x <= -1.33f || x >= 1.33f) {
		x *= -0.95f;
	}
	if (y <= -1.0f || y >= 1.0f) {
		y *= -0.95f;
	}
}

void Entity::Render() {
	buildMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix.ml);
	sprite.draw(scale_x, scale_y, rotation);
	glPopMatrix();
}

void Entity::buildMatrix() {
	Matrix scale;
	scale.identity();
	scale.m[0][0] = scale_x;
	scale.m[1][1] = scale_y;

	Matrix rotate;
	rotate.identity();
	rotate.m[0][0] = cos(rotation);
	rotate.m[0][1] = sin(rotation);
	rotate.m[1][0] = -sin(rotation);
	rotate.m[1][1] = cos(rotation);

	Matrix translate;
	translate.identity();
	translate.m[3][0] = x;
	translate.m[3][1] = y;

	matrix.identity();
	matrix = scale * rotate * translate;
}