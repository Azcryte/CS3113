#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
//#include "matrix.h"
#include "loader.h"
//#include "sheetSprite.h"
#include "entity.h"

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

class Particle {
public:
	//Particle(Vector pos, Vector vel, float t);
	Vector position;
	Vector velocity;
	float lifetime;
	bool active;
};

//Particle::Particle(Vector pos, Vector vel, float t)
//	: position(pos), velocity(vel), lifetime(t) {}

class ParticleEmitter {
public:
	ParticleEmitter();
	ParticleEmitter(float time, unsigned int particleCount);
	//ParticleEmitter(float time, unsigned int bulletCount, SheetSprite sprite);
	//ParticleEmitter(Vector pos, Vector grav, Vector vel, float t, unsigned int particleCount);
	~ParticleEmitter();

	void FixedUpdate();
	void Render();
	void reset();

	//void shootBullet();

	bool active;
	Vector position;
	Vector gravity;
	Vector velocity;
	Vector velocityDeviation;
	float maxLifetime;
	
	float startSize;
	float endSize;
	//bool shootingBullets;
	//bool shootingParticles;
	//vector<Entity*> bullets;
	vector<Particle> particles;
};

ParticleEmitter::ParticleEmitter() {
	//shootingBullets = false;
	//shootingParticles = false;
	active = false;
	position = Vector(0.0f, 0.0f);
	gravity = Vector(0.0f, -9.8f);
	velocity = Vector(0.0f, 0.0f);
	maxLifetime = 0.0f;
	startSize = 0.0f;
	endSize = 0.0f;

	particles.clear();
	//bullets.clear();
}

ParticleEmitter::~ParticleEmitter() {

}

ParticleEmitter::ParticleEmitter(float time, unsigned int particleCount) {
	active = false;
	//shootingParticles = true;
	//shootingBullets = false;
	position = Vector(0.0f, 0.0f);
	gravity = Vector(0.0f, -9.8f);
	velocity = Vector(0.0f, 0.0f);
	maxLifetime = time;
	startSize = 0.0f;
	endSize = 0.0f;

	//bullets.clear();
	particles.clear();
	for (size_t i = 0; i < particleCount; i++) {
		Particle temp;
		//temp.lifetime = 0.0f;
		temp.lifetime = randomFloat(0.0f, time);
		temp.position = position;
		temp.active = false;
		particles.push_back(temp);
	}
}

//ParticleEmitter::ParticleEmitter(float time, unsigned int bulletCount, SheetSprite sprite) {
//	active = false;
//	shootingParticles = false;
//	shootingBullets = true;
//	position = Vector(0.0f, 0.0f);
//	gravity = Vector(0.0f, 0.0f);
//	velocity = Vector(0.0f, 0.0f);
//	maxLifetime = time;
//	startSize = 0.0f;
//	endSize = 0.0f;
//	
//	particles.clear();
//	bullets.clear();
//	for (size_t i = 0; i < bulletCount; i++) {
//		Entity* temp = new Entity();
//		temp->lifetime = randomFloat(0.0f, time);
//		temp->position = position;
//		temp->active = false;
//		temp->sprite = sprite;
//		temp->scale_x = 0.5f;
//		temp->scale_y = 0.5f;
//		bullets.push_back(temp);
//	}
//}

//ParticleEmitter::ParticleEmitter(Vector pos, Vector grav, Vector vel, float t, unsigned int particleCount) {
//	active = false;
//	position = pos;
//	gravity = grav;
//	velocity = vel;
//	maxLifetime = t;
//	startSize = 0.0f;
//	endSize = 0.5f;
//
//	particles.clear();
//	for (size_t i = 0; i < particleCount; i++) {
//		Particle temp;
//		//temp.lifetime = 0.0f;
//		temp.lifetime = randomFloat(0.0f, t);
//		temp.position = position;
//		temp.active = true;
//		particles.push_back(temp);
//	}
//}

void ParticleEmitter::FixedUpdate() {
	//if (shootingBullets) {
	//	for (size_t i = 0; i < bullets.size(); i++) {
	//		if (bullets[i]->active) {
	//			bullets[i]->velocity = bullets[i]->velocity + gravity;
	//			bullets[i]->position = bullets[i]->position + bullets[i]->velocity * FIXED_TIMESTEP;
	//			bullets[i]->lifetime += FIXED_TIMESTEP;
	//		}
	//	
	//	

	//		if (bullets[i]->lifetime > maxLifetime) {
	//			bullets[i]->velocity = velocity + Vector(randomFloat(-velocityDeviation.x, velocityDeviation.x),
	//													 randomFloat(-velocityDeviation.y, velocityDeviation.y));
	//			bullets[i]->bulletReset(position);
	//			/*bullets[i]->position = position;
	//			bullets[i]->lifetime = 0.0f;
	//			bullets[i]->active = false;*/
	//		}
	//		//if (bullets[i]->lifetime = 0.0f || bullets[i]->lifetime > maxLifetime) {
	//			//bullets[i]->active = active;
	//		//}
	//	}
	//}
	//if (shootingParticles) {
	for (size_t i = 0; i < particles.size(); i++) {
		//particles[i].velocity.x += gravity.x;
		//particles[i].velocity.y += gravity.y;
		particles[i].velocity = particles[i].velocity + gravity;
		particles[i].position = particles[i].position + particles[i].velocity * FIXED_TIMESTEP;
		particles[i].lifetime += FIXED_TIMESTEP;


		//particles[i].position.x += particles[i].velocity.x;
		//particles[i].position.y += particles[i].velocity.y;

		//particles[i].lifetime += elapsed;
		if (particles[i].lifetime > maxLifetime) {
			particles[i].velocity = velocity + Vector(randomFloat(-velocityDeviation.x, velocityDeviation.x),
				randomFloat(-velocityDeviation.y, velocityDeviation.y));
			particles[i].position = position;
			particles[i].lifetime = 0.0f;
			particles[i].active = active;
		}
		//particles[i].active = active;
	}
	//}
}

//void ParticleEmitter::shootBullet() {
//	if (shootingBullets) {
//		for (size_t i = 0; i < bullets.size(); i++) {
//			if (!bullets[i]->active) {
//				bullets[i]->position = position;
//				bullets[i]->lifetime = 0.0f;
//				bullets[i]->active = true;
//				break;
//			}
//		}
//	}
//}

void ParticleEmitter::Render() {
	//if (active) {
	//if (shootingBullets) {
	//	//glLoadIdentity();
	//	for (size_t i = 0; i < bullets.size(); i++) {
	//		//bullets[i]->buildMatrix();
	//		if (bullets[i]->active) {
	//			bullets[i]->Render();
	//		}
	//	}
	//}

	//if (shootingParticles) {
		vector<float> particleVertices;

		for (size_t i = 0; i < particles.size(); i++) {
			if (particles[i].active) {
				particleVertices.push_back(particles[i].position.x);
				particleVertices.push_back(particles[i].position.y);
			}
		}
		glVertexPointer(2, GL_FLOAT, 0, particleVertices.data());
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(GL_POINTS, 0, particleVertices.size() / 2);
	//}
		//vector<float> particleVertices;
		//vector<float> particleUVs;
		//vector<unsigned int> indices;
		//
		//for (int i = 0; i < particles.size(); i++) {
		//
		//	float relativeLifetime = (particles[i].lifetime / maxLifetime);
		//	float size = lerp(startSize, endSize, relativeLifetime);// +particles[i].sizeDeviation;
		//	unsigned int vertexOffset = particleVertices.size() / 2;
		//	particleVertices.insert(particleVertices.end(), {
		//		particles[i].position.x - size, particles[i].position.y + size,
		//		particles[i].position.x - size, particles[i].position.y - size,
		//		particles[i].position.x + size, particles[i].position.y - size,
		//		particles[i].position.x + size, particles[i].position.y + size
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
//
//void ParticleEmitter::reset() {
//	for (size_t i = 0; i < particles.size(); i++) {
//		particles[i].position = position;
//	}
//}