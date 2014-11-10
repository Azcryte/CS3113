#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include "sheetSprite.h"
#include "matrix.h"
#include "particleEmitter.h"
using namespace std;

//#define FIXED_TIMESTEP 0.0166666f
//#define MAX_TIMESTEPS 6

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

	//bool collidesWith(Entity* entity);

	void walkRight();
	void walkLeft();
	void setIdle();
	void jump();
	void hover();
	void stopHovering();

	bool facingRight;
	bool isIdle;
	bool isJumping;
	bool isHovering;
	float elapsed;

	float x;
	float y;
	float scale_x;
	float scale_y;
	float rotation;
	float speed;

	//Vector velocity;
	float vel_x;
	float vel_y;
	//Vector acceleration;
	float acc_x;
	float acc_y;
	float fric_x;
	float fric_y;
	float grav_x;
	float grav_y;
	float mass;

	bool isStatic;
	bool collidedTop;
	bool collidedBot;
	bool collidedLeft;
	bool collidedRight;
	
};

void Entity::setVector(){
	//vector = Vector(float(sin(rotation + M_PI / 2.0f)), float(cos(rotation + M_PI / 2.0f)));
	//vector = Vector(float(cos(rotation + M_PI / 2.0f)), float(sin(rotation + M_PI / 2.0f)));
}

Entity::Entity() {
	facingRight = false;
	isIdle = true;
	isJumping = false;
	isHovering = false;
	elapsed = 0.0f;

	x = 0.0f;
	y = 0.0f;
	scale_x = 1.0f;
	scale_y = 1.0f;
	rotation = 0.0f;
	speed = 1.0f;

	//velocity = Vector(0.0f, 0.0f);
	vel_x = 0.0f;
	vel_y = 0.0f;
	acc_x = 0.0f;
	acc_y = 0.0f;
	fric_x = 0.0f;
	fric_y = 0.0f;
	grav_x = 0.0f;
	grav_y = -9.8f;
	mass = 0.0f;

	isStatic = false;
	collidedTop = false;
	collidedBot = false;
	collidedLeft = false;
	collidedRight = false;
}

Entity::~Entity() {}

void Entity::walkRight() {
	isIdle = false;
	facingRight = true;
	if (vel_x < 0.0f) {
		vel_x = 0.0f;
	}
	if (vel_x < 2.0f) {
		acc_x = 5.0f * speed;
	}
}

void Entity::walkLeft() {
	isIdle = false;
	facingRight = false;
	if (vel_x > 0.0f) {
		vel_x = 0.0f;
	}
	if (vel_x > -2.0f) {
		acc_x = -5.0f * speed;
	}
}

void Entity::setIdle() {
	isIdle = true;
	vel_x = 0.0f;
	acc_x = 0.0f;
}

void Entity::jump() {
	collidedBot = false;
	isJumping = true;
	vel_y = 3.5f;
}

void Entity::hover() {
	collidedBot = false;
	isHovering = true;
	acc_y = 15.0f;
}

void Entity::stopHovering() {
	isHovering = false;
	//vel_y = 0.0f;
	acc_y = 0.0f;
}

void Entity::FixedUpdate() {

	//setVector();
	//vector.x *= vel_x;
	//vector.y *= vel_y;

	//vel_x = lerp(vel_x, 0.0f, FIXED_TIMESTEP * fric_x);
	//vel_y = lerp(vel_y, 0.0f, FIXED_TIMESTEP * fric_y);

	//vel_x += acc_x * FIXED_TIMESTEP;
	//vel_y += acc_y * FIXED_TIMESTEP;

	//x += vel_x * FIXED_TIMESTEP;
	//y += vel_y * FIXED_TIMESTEP;

	//buildMatrix();

	//if (x <= -1.33f || x >= 1.33f) {
	//	x *= -0.95f;
	//}
	//if (y <= -1.0f || y >= 1.0f) {
	//	y *= -0.95f;
	//}
}

void Entity::Render() {
	buildMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix.ml);
	sprite.draw(scale_x, scale_y);
	glPopMatrix();
}

//bool Entity::collidesWith(Entity* entity) {
//	float e1_y_max = y + height / 2;
//	float e1_y_min = y - height / 2;
//	float e1_x_max = x + width / 2;
//	float e1_x_min = x - width / 2;
//
//	float e2_y_max = entity->y + entity->height / 2;
//	float e2_y_min = entity->y - entity->height / 2;
//	float e2_x_max = entity->x + entity->width / 2;
//	float e2_x_min = entity->x - entity->width / 2;
//
//	if (e1_y_min > e2_y_max)
//		return false;
//	if (e1_y_max < e2_y_min)
//		return false;
//	if (e1_x_min > e2_x_max)
//		return false;
//	if (e1_x_max < e2_x_min)
//		return false;
//
//	return true;
//}

void Entity::buildMatrix() {
	Matrix scale;
	scale.identity();
	scale.m[0][0] = scale_x;
	scale.m[1][1] = scale_y;
	scale.m[2][2] = 1;

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
	translate.m[3][2] = 1;
	matrix.identity();
	//matrix = translate * rotate * scale;
	matrix = scale * rotate * translate;
}