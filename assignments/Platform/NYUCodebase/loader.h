#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <string>

GLuint LoadTexture(const char *image_path/*, GLenum format*/) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA/*format*/, GL_UNSIGNED_BYTE, surface->pixels);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);

	return textureID;
}

void DrawSprite(GLint texture, float x, float y, float rotation) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = { -0.1f, 0.1f, -0.1f, -0.1f, 0.1f, -0.1f, 0.1f, 0.1f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

//void drawText(int fontTexture, string text, float size, float spacing, float xDisp, float yDisp, float r, float g, float b, float a) {
//	glBindTexture(GL_TEXTURE_2D, fontTexture);
//	glEnable(GL_TEXTURE_2D);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glLoadIdentity();
//	glTranslatef(xDisp, yDisp, 0.0f);
//	float texture_size = 1.0 / 16.0f;
//	vector<float> vertexData;
//	vector<float> texCoordData;
//	vector<float> colorData;
//	for (unsigned int i = 0; i < text.size(); i++) {
//		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
//		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
//
//		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
//		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f * size), 0.5f * size,
//			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
//			((size + spacing) * i) + (0.5f * size), -0.5f * size,
//			((size + spacing) * i) + (0.5f * size), 0.5f * size });
//		texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +
//			texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
//	}
//	glColorPointer(4, GL_FLOAT, 0, colorData.data());
//	glEnableClientState(GL_COLOR_ARRAY);
//	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//	glDrawArrays(GL_QUADS, 0, text.size() * 4);
//	glDisableClientState(GL_COLOR_ARRAY);
//	glDisableClientState(GL_VERTEX_ARRAY);
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//}