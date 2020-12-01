#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <random>
#include <time.h>
#include <iostream>
using namespace std;

#define foothold_sizex 0.8f
#define foothold_sizey 0.3f
#define foothold_sizez 0.8f
#define N 5
#define foothold_vertex 36
#define MAX 32767.0f			// 실수 rand() 위한 변수값

class Foothold {
public:
	float mx, my, mz;
	float cx, cy, cz;
	float rx, ry, rz;
	float r, g, b;
	int model, color_location;
	int size;
	int ani;
	float theta;
	glm::mat4 Move;
	glm::mat4 Scale;
	glm::mat4 Rotate;
	glm::mat4 Drawing;
	bool Del = FALSE;

public:
	Foothold(float x, float y, float z, float R, float G, float B) :
		mx{ x }, my{ y }, mz{ z },
		cx{ foothold_sizex }, cy{ foothold_sizey }, cz{ foothold_sizez },
		r{ R }, g{ G }, b{ B }, size{ foothold_vertex }{
			ani = rand() % 2;
	}

	~Foothold(){}
	void Init();
	void Pos_Drawing();
	void Draw_Start();
	void Delete();
};

void MakeFoothold(vector<Foothold> &Bottom);