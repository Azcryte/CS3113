#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include "sheetSprite.h"
#include "matrix.h"
//#include "particleEmitter.h"
//#include "entityEmitter.h"
//using namespace std;

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

class Entity{
private:
public:
	Entity();
	~Entity();

	virtual void FixedUpdate();
	//virtual void Render(GLint modelviewMatrixUniform, GLuint positionAttribute, GLuint texCoordAttribute);
	virtual void Render();
	void buildMatrix();

	SheetSprite sprite;
	Matrix matrix;
	Vector vector;
	//void setVector();

	//bool collidesWith(Entity* entity);

	void moveRight();
	void moveLeft();
	void setIdle();
	virtual void jump();
	void hover();
	void stopHovering();

	//void shootBullet(Vector target);
	bool facingRight;
	bool isIdle;
	bool isJumping;
	bool isHovering;
	float elapsed;

	//float x;
	//float y;
	float width;
	float height;
	float scale_x;
	float scale_y;
	float rotation;
	float speed;

	//float velocity.x;
	//float velocity.y;
	//Vector acceleration;
	float acc_x;
	float acc_y;
	float fric_x;
	float fric_y;
	float grav_x;
	float grav_y;
	float mass;

	Vector position;
	Vector velocity;

	float maxLifetime;//
	float lifetime;
	bool bulletActive;
	void bulletShoot(Vector pos);
	void bulletReset(Vector pos);
	bool bulletShot = false;

	bool isStatic;
	bool collidedTop;
	bool collidedBot;
	bool collidedLeft;
	bool collidedRight;
	
	SheetSprite healthBar;
	double maxHealth;
	double currentHealth;
	
	bool dead;
	float timeSinceLastJump;
	float jumpTimeGap;
	int entityID;
	bool randomUse = false;
	bool invulnerable;
	float invulnerableFrames;
};

//void Entity::setVector(){
//	//vector = Vector(float(sin(rotation + M_PI / 2.0f)), float(cos(rotation + M_PI / 2.0f)));
//	//vector = Vector(float(cos(rotation + M_PI / 2.0f)), float(sin(rotation + M_PI / 2.0f)));
//}

Entity::Entity() {


	facingRight = false;
	isIdle = true;
	isJumping = false;
	isHovering = false;
	elapsed = 0.0f;

	//x = 0.0f;
	//y = 0.0f;
	width = 1.0f;
	height = 1.0f;
	scale_x = 0.8f;
	scale_y = 0.8f;
	rotation = 0.0f;
	speed = 1.0f;

	//velocity.x = 0.0f;
	//velocity.y = 0.0f;
	acc_x = 0.0f;
	acc_y = 0.0f;
	fric_x = 5.0f;
	fric_y = 5.0f;
	grav_x = 0.0f;
	grav_y = -9.8f;
	mass = 0.0f;

	position = Vector(0.0f, 0.0f);
	velocity = Vector(0.0f, 0.0f);
	maxLifetime = 0.0f;
	lifetime = 0.0f;
	bulletActive = false;
	//lifetime = 0.0f;
	//active = false;
	
	isStatic = false;
	collidedTop = false;
	collidedBot = false;
	collidedLeft = false;
	collidedRight = false;
	
	maxHealth = 1;
	currentHealth = 1;
	dead = false;
	timeSinceLastJump = 0.0f;
	jumpTimeGap = 0.0f;
	entityID = -1;
	invulnerable = false;
	invulnerableFrames = 0.0f;
}

Entity::~Entity() {}

void Entity::moveRight() {
	isIdle = false;
	facingRight = true;
	if (velocity.x < 0.0f) {
		velocity.x = 0.0f;
	}
	if (velocity.x < 2.0f) {
		acc_x = 5.0f * speed;
	}
}

void Entity::moveLeft() {
	isIdle = false;
	facingRight = false;
	if (velocity.x > 0.0f) {
		velocity.x = 0.0f;
	}
	if (velocity.x > -2.0f) {
		acc_x = -5.0f * speed;
	}
}

void Entity::setIdle() {
	isIdle = true;
	velocity.x = 0.0f;
	acc_x = 0.0f;
}

void Entity::jump() {
	collidedBot = false;
	isJumping = true;
	velocity.y = 3.5f;
}

void Entity::hover() {
	collidedBot = false; 
	isHovering = true;
	acc_y = 15.0f * speed;
}

void Entity::stopHovering() {
	isHovering = false;
	//velocity.y = 0.0f;
	acc_y = 0.0f;
}

//shoot the bullet at a poin

void Entity::FixedUpdate() {
	
	//if (collidedBot) {
	//	isJumping = false;
	//	//if (entities[i]->velocity.y < 0.0f) {
	//	velocity.y = 0.0f;
	//	//}
	//}
	////if (entities[i]->collidedTop) {
	////	if (entities[i]->velocity.y > 0.0f) {
	////		//entities[i]->velocity.y = 0.0f;v
	////	}
	////}
	//if (collidedRight) {
	//	//if (entities[i]->facingRight) {
	//	velocity.x = 0.0f;
	//	//}
	//}
	//if (collidedLeft) {
	//	//if (!entities[i]->facingRight) {
	//	velocity.x = 0.0f;re
	//	//}
	//}
	collidedBot = false;
	collidedTop = false;
	collidedRight = false;
	collidedLeft = false;

	velocity.x += grav_x * FIXED_TIMESTEP;
	velocity.y += grav_y * FIXED_TIMESTEP;

	velocity.x = lerp(velocity.x, 0.0f, FIXED_TIMESTEP * fric_x);
	velocity.y = lerp(velocity.y, 0.0f, FIXED_TIMESTEP * fric_y);

	velocity.x += acc_x * FIXED_TIMESTEP;
	velocity.y += acc_y * FIXED_TIMESTEP;
	if (bulletActive) {
		lifetime += FIXED_TIMESTEP;
	}

	if (invulnerable) {
		invulnerableFrames += FIXED_TIMESTEP;
	}
	elapsed += FIXED_TIMESTEP;

	//setVector();
	//vector.x *= velocity.x;
	//vector.y *= velocity.y;

	//velocity.x = lerp(velocity.x, 0.0f, FIXED_TIMESTEP * fric_x);
	//velocity.y = lerp(velocity.y, 0.0f, FIXED_TIMESTEP * fric_y);

	//velocity.x += acc_x * FIXED_TIMESTEP;
	//velocity.y += acc_y * FIXED_TIMESTEP;

	//x += velocity.x * FIXED_TIMESTEP;
	//y += velocity.y * FIXED_TIMESTEP;
	
	//buildMatrix();

	//if (x <= -1.33f || x >= 1.33f) {
	//	x *= -0.95f;
	//}
	//if (y <= -1.0f || y >= 1.0f) {
	//	y *= -0.95f;
	//}
}

//void Entity::Render(GLint modelviewMatrixUniform, GLuint positionAttribute, GLuint texCoordAttribute) {
void Entity::Render() {
	buildMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix.ml);
	//Matrix identity;
	//glUniformMatrix4fv(modelviewMatrixUniform, 1, GL_FALSE, identity.ml);

	//glUseProgram(programID);
	//GLint projectionMatrixUniform = glGetUniformLocation(programID, "projection");
	//GLint modelviewMatrixUniform = glGetUniformLocation(programID, "modelView");
	//GLuint positionAttribute = glGetAttribLocation(programID, "position");
	//GLuint texCoordAttribute = glGetAttribLocation(programID, "texCoord");

	//glUniformMatrix4fv(modelviewMatrixUniform, 1, GL_FALSE, matrix.ml);
	if (invulnerable) {
		if (randomUse) {
			//sprite.drawShader(positionAttribute, texCoordAttribute, scale_x, scale_y);
			sprite.draw(scale_x, scale_y);
			randomUse = false;
		}
		else {
			randomUse = true;
		}
	}
	else {
		//sprite.drawShader(positionAttribute, texCoordAttribute, scale_x, scale_y);
		sprite.draw(scale_x, scale_y);
	}
	buildMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix.ml);
	glTranslatef(0.0f, -1.0f * height, 0.0f);
	if (entityID >= 0 && currentHealth < maxHealth) {
		healthBar.draw(scale_x * 0.5f, scale_y);
	}
	glPopMatrix();
}

void Entity::bulletShoot(Vector target) {
	velocity = target;// -position;
	velocity.normalize();
	velocity = velocity * speed;
	bulletActive = true;
}

void Entity::bulletReset(Vector target) {
	position = target;
	lifetime = 0.0f;
	bulletActive = false;
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
	translate.m[3][0] = position.x;
	translate.m[3][1] = position.y;
	translate.m[3][2] = 1;
	matrix.identity();
	//matrix = translate * rotate * scale;
	matrix = scale * rotate * translate;
}