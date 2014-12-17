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

//void drawText(GLint modelviewMatrixUniform, GLuint positionAttribute, GLuint texCoordAttribute, int fontTexture, string text, float size, float spacing, float xDisp, float yDisp, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) {
void drawText(int fontTexture, string text, float size, float spacing, float xDisp, float yDisp, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLoadIdentity();
	glTranslatef(xDisp, yDisp, 0.0f);

	//GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	//const char *vertexShaderString = vertexShader.c_str();
	//int vertexShaderStringLength = vertexShader.size();
	//glShaderSource(vertexShaderID, 1, &vertexShaderString, &vertexShaderStringLength);
	//glCompileShader(vertexShaderID);
	//GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	//const char *fragmentShaderString = fragmentShader.c_str();
	//int fragmentShaderStringLength = fragmentShader.size();
	//glShaderSource(fragmentShaderID, 1, &fragmentShaderString, &fragmentShaderStringLength);
	//glCompileShader(fragmentShaderID);
	//GLuint exampleProgram = glCreateProgram();
	//glAttachShader(exampleProgram, vertexShaderID);
	//glAttachShader(exampleProgram, fragmentShaderID);
	//glLinkProgram(exampleProgram);
	//GLint projectionMatrixUniform = glGetUniformLocation(exampleProgram, "projection");
	//GLint modelviewMatrixUniform = glGetUniformLocation(exampleProgram, "modelView");
	//GLuint positionAttribute = glGetAttribLocation(exampleProgram, "position");
	//GLuint texCoordAttribute = glGetAttribLocation(exampleProgram, "texCoord");

	float texture_size = 1.0 / 16.0f;
	vector<float> vertexData;
	vector<float> texCoordData;
	vector<float> colorData;
	for (unsigned int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size });
		texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +
			texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}

	//Matrix modelviewMatrix;
	//float tx = xDisp;
	//float ty = yDisp;

	//modelviewMatrix.identity();
	//modelviewMatrix.m[3][0] = xDisp;
	//modelviewMatrix.m[3][1] = yDisp;

	//glUniformMatrix4fv(modelviewMatrixUniform, 1, GL_FALSE, modelviewMatrix.ml);

	//glColorPointer(4, GL_FLOAT, 0, colorData.data());
	//glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	//glEnableVertexAttribArray(positionAttribute);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	//glEnableVertexAttribArray(texCoordAttribute);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	//glDisableVertexAttribArray(positionAttribute);
	//glDisableVertexAttribArray(texCoordAttribute);
}

void drawSprite(GLint texture, float x, float y, float rotation, float height, float width) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = { (float)(0 - width / 2), (float)(height / 2),
		(float)(0 - width / 2), (float)(0 - height / 2),
		(float)(width / 2), (float)(0 - height / 2),
		(float)(width / 2), (float)(height / 2) };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	//GLfloat quadUVs[] = { 0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 0.5 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

void DrawSpriteAnimation(GLint texture, int index, int spriteCount_x, int spriteCount_y, float disp_x, float disp_y) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(disp_x, disp_y, 0.0f);

	// our regular sprite drawing 
	float u = (float)(((int)index) % spriteCount_x) / (float)spriteCount_x;
	float v = (float)(((int)index) / spriteCount_x) / (float)spriteCount_y;
	float width = 1.0f / (float)spriteCount_x;
	float height = 1.0f / (float)spriteCount_y;

	GLfloat quad[] = { (float)(0 - width * 4), (float)(height * 4),
		(float)(0 - width * 4), (float)(0 - height * 4),
		(float)(width * 4), (float)(0 - height * 4),
		(float)(width * 4), (float)(height * 4) };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { u, v,
		u, v + height,
		u + width, v + height,
		u + width, v
	};
	// our regular sprite drawing 

	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

