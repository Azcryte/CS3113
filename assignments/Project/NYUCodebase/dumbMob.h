#pragma once
#include "entity.h"

class DumbMob : public Entity {
private: 
public:
	DumbMob();
	void FixedUpdate();
	void Render();

	void jump();
	void jump2();

	Vector jumpRightCheck;
	Vector jumpLeftCheck;
	Vector highJumpRightCheck;
	Vector highJumpLeftCheck;
	Vector fallRightCheck1;
	Vector fallRightCheck2;
	Vector fallRightCheck3;
	Vector fallLeftCheck1;
	Vector fallLeftCheck2;
	Vector fallLeftCheck3;
};

DumbMob::DumbMob() {
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

	jumpRightCheck = Vector(0.1f, 0.0f);
	jumpLeftCheck = Vector(-0.1f, 0.0f);
	highJumpRightCheck = Vector(0.1f, 0.1f);
	highJumpLeftCheck = Vector(-0.1f, 0.1f);
	fallRightCheck1 = Vector(0.1f, -0.1f);
	fallRightCheck2 = Vector(0.1f, -0.2f);
	fallRightCheck3 = Vector(0.1f, -0.3f);
	fallLeftCheck1 = Vector(-0.1f, -0.1f);
	fallLeftCheck2 = Vector(-0.1f, -0.2f);
	fallLeftCheck3 = Vector(-0.1f, -0.3f);
}

void DumbMob::jump() {
	collidedBot = false;
	isJumping = true;
	velocity.y = 2.5f;
	//velocity.y = 3.5f;
}

void DumbMob::jump2() {
	collidedBot = false;
	isJumping = true;
	velocity.y = 3.5f;
	//velocity.y = 3.5f;
}

void DumbMob::FixedUpdate() {

	if (facingRight) {
		moveRight();
		//velocity.x = 0.5f;
	}
	else {
		moveLeft();
		//velocity.x = -0.5f;
	}
	jumpRightCheck = Vector(position.x + 0.06f, position.y);
	jumpLeftCheck = Vector(position.x - 0.06f, position.y);
	highJumpRightCheck = Vector(position.x + 0.09f, position.y + 0.1f);
	highJumpLeftCheck = Vector(position.x - 0.09f, position.y + 0.1f);
	fallRightCheck1 = Vector(position.x + 0.03f, position.y - 0.1f);
	fallRightCheck2 = Vector(position.x + 0.03f, position.y - 0.2f);
	fallRightCheck3 = Vector(position.x + 0.03f, position.y - 0.3f);
	fallLeftCheck1 = Vector(position.x - 0.03f, position.y - 0.1f);
	fallLeftCheck2 = Vector(position.x - 0.03f, position.y - 0.2f);
	fallLeftCheck3 = Vector(position.x - 0.03f, position.y - 0.3f);

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
	//if (timeSinceLastJump > jumpTimeGap) {
	//	if (!isJumping) {
	//		jump();
	//	}
	//	timeSinceLastJump = 0.0f;
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

	elapsed += FIXED_TIMESTEP;
	timeSinceLastJump += FIXED_TIMESTEP;

}

void DumbMob::Render() {
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