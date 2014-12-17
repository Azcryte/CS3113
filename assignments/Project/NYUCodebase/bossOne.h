#pragma once
#include "entity.h"

class BossOne : public Entity {
private:
public:
	BossOne();
	void FixedUpdate();
	void Render();

	void jump();
	void jump2();

	float cooldown;
};

BossOne::BossOne() {
	facingRight = false;
	isIdle = true;
	isJumping = false;
	isHovering = false;
	elapsed = 0.0f;

	//x = 0.0f;
	//y = 0.0f;
	scale_x = 0.8f;
	scale_y = 0.8f;
	rotation = 0.0f;
	speed = 0.5f;

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

	dead = false;
	timeSinceLastJump = 0.0f;
	jumpTimeGap = 0.0f;
	entityID = 0;
	cooldown = 0.0f;
}

void BossOne::jump() {
	collidedBot = false;
	isJumping = true;
	velocity.y = 2.5f;
	//velocity.y = 3.5f;
}

void BossOne::jump2() {
	collidedBot = false;
	isJumping = true;
	velocity.y = 3.5f;
	//velocity.y = 3.5f;
}

void BossOne::FixedUpdate() {

	if (facingRight) {
		moveRight();
		//velocity.x = 0.5f;
	}
	else {
		moveLeft();
		//velocity.x = -0.5f;
	}

	// ****** put in fixed update v *******

	if (collidedBot) {
		isJumping = false;
		//if (entities[i]->velocity.y < 0.0f) {
		velocity.y = 0.0f;
		//}
	}
	else {
		isJumping = true;
	}
	if (collidedTop) {
		if (velocity.y > 0.0f) {
			//entities[i]->velocity.y = 0.0f;
		}
	}
	if (collidedRight) {
		if (facingRight) {
			facingRight = false;
			//velocity.x *= -1.0f;
			//velocity.x = 0.0f;
		}
	}
	if (collidedLeft) {
		if (!facingRight) {
			facingRight = true;
			//velocity.x *= -1.0f;
			//velocity.x = 0.0f;
		}
	}

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

	elapsed += FIXED_TIMESTEP;
	timeSinceLastJump += FIXED_TIMESTEP;
	cooldown += FIXED_TIMESTEP;
}

void BossOne::Render() {
	buildMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix.ml);
	sprite.draw(scale_x, scale_y);
	//buildMatrix();
	//glMatrixMode(GL_MODELVIEW);
	if (entityID >= 0 && currentHealth < maxHealth) {
		glPushMatrix();
		glMultMatrixf(matrix.ml);
		glTranslatef(0.0f, -1.0f * height, 0.0f);
		healthBar.draw(0.2f, 0.6f);
	}
	glPopMatrix();
}