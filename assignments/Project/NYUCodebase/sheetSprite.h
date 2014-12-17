#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "matrix.h"
#include <cmath>
using namespace std;

int randomInt(int min, int max) {
	return rand() % (max - min) + min;
}

float randomFloat(float min, float max){
	float random = ((float)rand()) / (float)RAND_MAX;
	return (random*(max - min)) + min;
}

bool randomBool() {
	return rand() % 2 == 1;
}

int randomSign() {
	return -1 + (2 * (rand() % 2));
}

float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax) {
	float retVal = dstMin + ((value - srcMin) / (srcMax - srcMin) * (dstMax - dstMin));
	if (retVal < dstMin) {
		retVal = dstMin;
	}
	if (retVal > dstMax) {
		retVal = dstMax;
	}
	return retVal;
}

float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

float easeOut(float from, float to, float time) {
	float oneMinusT = 1.0f - time;
	float tVal = 1.0f - (oneMinusT * oneMinusT * oneMinusT *
		oneMinusT * oneMinusT);
	return (1.0f - tVal)*from + tVal*to;
}

float easeIn(float from, float to, float time) {
	float tVal = time*time*time*time*time;
	return (1.0f - tVal) * from + tVal * to;
}

float easeOutElastic(float from, float to, float time) {
	float p = 0.5f;
	float s = p / 4.0f;
	float diff = (to - from);
	return from + diff + (diff*pow(2.0f, -10.0f*time) * sin((time - s) * (2 * M_PI) / p));
}

//shader
//string vertexShader =
//"attribute vec4 position;\n\
//attribute vec2 texCoord;\n\
//uniform mat4 modelView;\n\
//uniform mat4 projection;\n\
//varying vec2 texCoordVar;\n\
//void main()\n\
//{\n\
// vec4 p = modelView * position;\n\
//  gl_Position = projection * p;\n\
//   texCoordVar = texCoord;\n\
//   }\n";
//
//string fragmentShader =
//"uniform sampler2D texture;\n\
//varying vec2 texCoordVar;\n\
//void main()\n\
//{\n\
// gl_FragColor = texture2D( texture, texCoordVar);\n\
// }\n";

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
	//Matrix matrix;

	SheetSprite() {};
	SheetSprite(GLuint textureID, vector<float> u, vector<float> v, float width, float height)
		: textureID(textureID), u(u), v(v), width(width), height(height) { frame = 0; };

	//shader
	void drawShader(GLuint positionAttribute, GLuint texCoordAttribute, float scale_x, float scale_y) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);

		GLfloat quad[] = { -width * scale_x, height * scale_y,
			-width * scale_x, -height * scale_y,
			width * scale_x, -height * scale_y,
			width * scale_x, height * scale_y };
		vector<float> vertexData;
		vertexData.insert(vertexData.end(), {
			-width * scale_x, height * scale_y,
			-width * scale_x, -height * scale_y,
			width * scale_x, -height * scale_y,
			width * scale_x, height * scale_y
		});

		//glVertexPointer(2, GL_FLOAT, 0, quad);
		//glEnableClientState(GL_VERTEX_ARRAY);
		glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, false, 0, quad);
		//glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
		glEnableVertexAttribArray(positionAttribute);

		GLfloat quadUVs[] = { u[frame], v[frame],
			u[frame], v[frame] + height,
			u[frame] + width, v[frame] + height,
			u[frame] + width, v[frame] };

		vector<float> textureCoordData;
		textureCoordData.insert(textureCoordData.end(), {
			u[frame], v[frame],
			u[frame], v[frame] + height,
			u[frame] + width, v[frame] + height,
			u[frame] + width, v[frame]
		});

		//glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
		//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, false, 0, quadUVs);
		//glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, false, 0, textureCoordData.data());
		glEnableVertexAttribArray(texCoordAttribute);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glDisableVertexAttribArray(positionAttribute);
		glDisableVertexAttribArray(texCoordAttribute);
		//glPopMatrix();
	}

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