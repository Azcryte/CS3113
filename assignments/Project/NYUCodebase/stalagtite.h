#pragma once
#include "entity.h"

class Stalagtite : public Entity {
private:
public:
	Stalagtite();
	void FixedUpdate();
	void Render();

	bool active;
	float timer;
	bool collided;
};

Stalagtite::Stalagtite() {
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
	entityID = 1;

	active = false;
	timer = 0.0f;
	collided = false;
}

void Stalagtite::FixedUpdate() {
	if (collidedBot) {
		//if (entities[i]->velocity.y < 0.0f) {
		velocity.y = 0.0f;
		//}
	}

	if (active) {
		if (collidedBot) {
			if ((facingRight && collidedRight) || (!facingRight && collidedLeft)) {
				collided = true;
			}
			if (facingRight) {
				moveRight();
				if (active && !collided) {
					rotation -= FIXED_TIMESTEP * 5.0f;
				}
				//velocity.x = 0.5f;
			}
			else {
				moveLeft();
				if (active && !collided) {
					rotation += FIXED_TIMESTEP * 5.0f;
				}
				//velocity.x = -0.5f;
			}

			velocity.x += grav_x * FIXED_TIMESTEP;

			velocity.x = lerp(velocity.x, 0.0f, FIXED_TIMESTEP * fric_x);

			velocity.x += acc_x * FIXED_TIMESTEP;
		}
		velocity.y += grav_y * FIXED_TIMESTEP;

		velocity.y = lerp(velocity.y, 0.0f, FIXED_TIMESTEP * fric_y);

		velocity.y += acc_y * FIXED_TIMESTEP;
	}

	collidedBot = false;
	collidedTop = false;
	collidedRight = false;
	collidedLeft = false;

	elapsed += FIXED_TIMESTEP;
}

void Stalagtite::Render() {
	buildMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix.ml);
	sprite.draw(scale_x, scale_y);
	//buildMatrix();
	//glMatrixMode(GL_MODELVIEW);
	//glRotatef(-rotation, 0.0f, 0.0f, 0.0f);
	if (entityID >= 0 && currentHealth < maxHealth && !active) {
		glPushMatrix();
		glMultMatrixf(matrix.ml);
		glTranslatef(0.0f, -3.0f * height, 0.0f);
		healthBar.draw(0.25f, 0.7f);
	}
	glPopMatrix();
}