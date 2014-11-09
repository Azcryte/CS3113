#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include "vector.h"
#include "loader.h"
using namespace std;

float randomFloat(float min, float max){
	float random = ((float)rand()) / (float)RAND_MAX;
	return (random*(max - min)) + min;
}

float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

class Particle {
public:
	Particle(Vector pos, Vector vel, float t);

	Vector position;
	Vector velocity;
	float lifetime;
};

Particle::Particle(Vector pos, Vector vel, float t)
	: position(pos), velocity(vel), lifetime(t) {}

class ParticleEmitter {
public:
	ParticleEmitter();
	ParticleEmitter(Vector pos, Vector grav, Vector vel, float t, unsigned int particleCount);
	~ParticleEmitter();

	void Update(float elapsed);
	void Render();

	bool active;
	Vector position;
	Vector gravity;
	Vector velocity;
	float maxLifetime;
	
	float startSize;
	float endSize;
	
	vector<Particle*> particles;
};

ParticleEmitter::ParticleEmitter() {
	active = false;
	position = Vector(0.0f, 0.0f);
	gravity = Vector(0.0f, -9.8f);
	velocity = Vector(0.0f, 0.0f);
	maxLifetime = 0.0f;
	startSize = 0.0f;
	endSize = 0.0f;

	particles.clear();
}

ParticleEmitter::ParticleEmitter(Vector pos, Vector grav, Vector vel, float t, unsigned int particleCount) {
	active = false;
	position = pos;
	gravity = grav;
	velocity = vel;
	maxLifetime = t;
	startSize = 0.0f;
	endSize = 0.5f;

	particles.clear();
	for (size_t i = 0; i < particleCount; i++) {
		Particle* temp = new Particle(position, Vector(0.0f, 0.0f), randomFloat(0.0f, t));
		particles.push_back(temp);
	}
}

void ParticleEmitter::Update(float elapsed) {
	for (size_t i = 0; i < particles.size(); i++) {
		particles[i]->velocity.x += gravity.x;
		particles[i]->velocity.y += gravity.y;

		particles[i]->position.x += particles[i]->velocity.x;
		particles[i]->position.y += particles[i]->velocity.y;

		particles[i]->lifetime += elapsed;
		if (particles[i]->lifetime > maxLifetime) {
			particles[i]->velocity = velocity;
			particles[i]->position = position;
			particles[i]->lifetime = 0.0f;
		}
	}
}

void ParticleEmitter::Render() {
	if (active) {

		vector<float> particleVertices;
		vector<float> particleUVs;
		vector<unsigned int> indices;

		//for (size_t i = 0; i < particles.size(); i++) {
		//	particleVertices.push_back(particles[i]->position.x);
		//	particleVertices.push_back(particles[i]->position.y);
		//}
		//glVertexPointer(2, GL_FLOAT, 0, particleVertices.data());
		//glEnableClientState(GL_VERTEX_ARRAY);

		//glDrawArrays(GL_POINTS, 0, particleVertices.size() / 2);
		//}
		//glLoadIdentity();

		for (int i = 0; i < particles.size(); i++) {

			float relativeLifetime = (particles[i]->lifetime / maxLifetime);
			float size = lerp(startSize, endSize, relativeLifetime);// +particles[i]->sizeDeviation;
			unsigned int vertexOffset = particleVertices.size() / 2;
			particleVertices.insert(particleVertices.end(), {
				particles[i]->position.x - size, particles[i]->position.y + size,
				particles[i]->position.x - size, particles[i]->position.y - size,
				particles[i]->position.x + size, particles[i]->position.y - size,
				particles[i]->position.x + size, particles[i]->position.y + size
			});

			particleUVs.insert(particleUVs.end(), {
				0.0f, 0.0f,
				0.0f, 1.0f,
				1.0f, 1.0f,
				1.0f, 0.0f
			});

			indices.insert(indices.end(), { vertexOffset + 0, vertexOffset + 1, 
				vertexOffset + 2, vertexOffset + 0, 
				vertexOffset + 2, vertexOffset + 3 });
		}
		glTexCoordPointer(2, GL_FLOAT, 0, particleUVs.data());
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
	}

}