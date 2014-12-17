#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include "matrix.h"
#include "loader.h"
#include "sheetSprite.h"
#include "entity.h"

class EntityEmitter {
public:
	EntityEmitter();
	EntityEmitter(float time, unsigned int particleCount);
	EntityEmitter(Vector pos, Vector grav, Vector vel, float t, unsigned int particleCount);
	~EntityEmitter();

	void Update(float elapsed);
	void Render();
	void reset();

	bool active;
	Vector position;
	Vector gravity;
	Vector velocity;
	Vector velocityDeviation;
	float maxLifetime;

	float startSize;
	float endSize;

	vector<Entity*> bullets;
};

EntityEmitter::EntityEmitter() {
	active = false;
	position = Vector(0.0f, 0.0f);
	gravity = Vector(0.0f, -9.8f);
	velocity = Vector(0.0f, 0.0f);
	maxLifetime = 0.0f;
	startSize = 0.0f;
	endSize = 0.0f;

	bullets.clear();
}

EntityEmitter::~EntityEmitter() {

}

EntityEmitter::EntityEmitter(float time, unsigned int particleCount) {
	active = false;
	position = Vector(0.0f, 0.0f);
	gravity = Vector(0.0f, -9.8f);
	velocity = Vector(0.0f, 0.0f);
	maxLifetime = time;
	startSize = 0.0f;
	endSize = 0.0f;

	bullets.clear();
	for (size_t i = 0; i < particleCount; i++) {
		Entity* temp = new Entity();
		//temp.lifetime = 0.0f;
		temp->lifetime = randomFloat(0.0f, time);
		temp->position = position;
		temp->active = false;
		bullets.push_back(temp);
	}
}

//EntityEmitter::EntityEmitter(Vector pos, Vector grav, Vector vel, float t, unsigned int particleCount) {
//	active = false;
//	position = pos;
//	gravity = grav;
//	velocity = vel;
//	maxLifetime = t;
//	startSize = 0.0f;
//	endSize = 0.5f;
//
//	bullets.clear();
//	for (size_t i = 0; i < particleCount; i++) {
//		Particle temp;
//		//temp.lifetime = 0.0f;
//		temp.lifetime = randomFloat(0.0f, t);
//		temp.position = position;
//		temp.active = true;
//		bullets.push_back(temp);
//	}
//}

void EntityEmitter::Update(float elapsed) {
	for (size_t i = 0; i < bullets.size(); i++) {
		//bullets[i]->velocity.x += gravity.x;
		//bullets[i]->velocity.y += gravity.y;
		bullets[i]->velocity = bullets[i]->velocity + gravity;
		bullets[i]->position = bullets[i]->position + bullets[i]->velocity * elapsed;
		bullets[i]->lifetime += elapsed;

		//bullets[i]->position.x += bullets[i]->velocity.x;
		//bullets[i]->position.y += bullets[i]->velocity.y;

		//bullets[i]->lifetime += elapsed;
		if (bullets[i]->lifetime > maxLifetime) {
			bullets[i]->velocity = velocity + Vector(randomFloat(-velocityDeviation.x, velocityDeviation.x),
				randomFloat(-velocityDeviation.y, velocityDeviation.y));
			bullets[i]->position = position;
			bullets[i]->lifetime = 0.0f;
			bullets[i]->active = active;
		}
	}
}

void EntityEmitter::Render() {
	//if (active) {

	vector<float> particleVertices;

	for (size_t i = 0; i < bullets.size(); i++) {
		if (bullets[i]->active) {
			particleVertices.push_back(bullets[i]->position.x);
			particleVertices.push_back(bullets[i]->position.y);
		}
	}
	glVertexPointer(2, GL_FLOAT, 0, particleVertices.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_POINTS, 0, particleVertices.size() / 2);
	//vector<float> particleVertices;
	//vector<float> particleUVs;
	//vector<unsigned int> indices;
	//
	//for (int i = 0; i < bullets.size(); i++) {
	//
	//	float relativeLifetime = (bullets[i]->lifetime / maxLifetime);
	//	float size = lerp(startSize, endSize, relativeLifetime);// +bullets[i]->sizeDeviation;
	//	unsigned int vertexOffset = particleVertices.size() / 2;
	//	particleVertices.insert(particleVertices.end(), {
	//		bullets[i]->position.x - size, bullets[i]->position.y + size,
	//		bullets[i]->position.x - size, bullets[i]->position.y - size,
	//		bullets[i]->position.x + size, bullets[i]->position.y - size,
	//		bullets[i]->position.x + size, bullets[i]->position.y + size
	//	});
	//
	//	particleUVs.insert(particleUVs.end(), {
	//		0.0f, 0.0f,
	//		0.0f, 1.0f,
	//		1.0f, 1.0f,
	//		1.0f, 0.0f
	//	});
	//
	//	indices.insert(indices.end(), { vertexOffset + 0, vertexOffset + 1, 
	//		vertexOffset + 2, vertexOffset + 0, 
	//		vertexOffset + 2, vertexOffset + 3 });
	//}
	//glTexCoordPointer(2, GL_FLOAT, 0, particleUVs.data());
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
	//}

}

void EntityEmitter::reset() {
	for (size_t i = 0; i < bullets.size(); i++) {
		bullets[i]->position = position;
	}
}