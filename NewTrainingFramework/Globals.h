#pragma once

#include "Math.h"

#define PI			3.14f
#define RADIAN		PI/180.0f
#define MAX_TEXTURE_UNIT	10

#define FOV_DEGREE	45.0f
#define FOV			RADIAN * FOV_DEGREE
#define ASPECT		(GLfloat)Globals::screenWidth / (GLfloat)Globals::screenHeight
#define NEAR_PLANE	0.1f
#define FAR_PLANE	1000.0f

#define KEY_W		(1 << 0)
#define KEY_S		(1 << 1)
#define KEY_A		(1 << 2)
#define KEY_D		(1 << 3)
#define KEY_UP		(1 << 4)
#define KEY_DOWN	(1 << 5)
#define KEY_LEFT    (1 << 6)
#define KEY_RIGHT   (1 << 7)
#define KEY_SPACEBAR (1 << 8)

#define POSITION_OFFSET	0
#define NORMAL_OFFSET	(char*)0 + sizeof(Vector3)*1
#define BINORMAL_OFFSET	(char*)0 + sizeof(Vector3)*2
#define TANGENT_OFFSET	(char*)0 + sizeof(Vector3)*2
#define TEXCOORD_OFFSET	(char*)0 + sizeof(Vector3)*4

typedef struct
{
	Vector3 lightPos;
	Vector4 lightColor;
} Light;

class Globals
{
public:
	static const int screenWidth = 960;
	static const int screenHeight = 720;
};