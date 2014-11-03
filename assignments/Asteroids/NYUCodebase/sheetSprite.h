#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
using namespace std;

class SheetSprite {
private:
public:
	GLuint textureID;
	float u;
	float v;
	float width;
	float height;

	SheetSprite() {};
	SheetSprite(GLuint textureID, float u, float v, float width, float height)
		: textureID(textureID), u(u), v(v), width(width), height(height) {};

	void draw(float scale_x, float scale_y, float rotation) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);

		GLfloat quad[] = { -width * scale_x, height * scale_y, 
						   -width * scale_x, -height * scale_y, 
						   width * scale_x, -height * scale_y, 
						   width * scale_x, height * scale_y };

		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);

		GLfloat quadUVs[] = { u, v, 
							  u, v + height, 
							  u + width, v + height, 
							  u + width, v };

		glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}

};