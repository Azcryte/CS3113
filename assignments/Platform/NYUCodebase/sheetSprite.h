#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "matrix.h"
using namespace std;

class SheetSprite {
private:
public:
	GLuint textureID;
	vector<float> u;
	vector<float> v;
	int frame;
	//float u;
	//float v;
	float width;
	float height;
	Matrix matrix;

	SheetSprite() {};
	SheetSprite(GLuint textureID, vector<float> u, vector<float> v, float width, float height)
		: textureID(textureID), u(u), v(v), width(width), height(height) { frame = 0; };

	void draw(float scale_x, float scale_y) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);

		GLfloat quad[] = { -width * scale_x, height * scale_y, 
						   -width * scale_x, -height * scale_y, 
						   width * scale_x, -height * scale_y, 
						   width * scale_x, height * scale_y };

		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);

		GLfloat quadUVs[] = { u[frame], v[frame], 
							  u[frame], v[frame] + height, 
							  u[frame] + width, v[frame] + height, 
							  u[frame] + width, v[frame] };

		glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glPopMatrix();
	}

};