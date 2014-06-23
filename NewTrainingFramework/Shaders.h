#pragma once
#include "../Utilities/utilities.h"
#include "Globals.h"

class Shaders 
{
public:
	GLuint program, vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];

	GLint positionAttribute;
	GLint normalAttribute;
	GLint binormalAttribute;
	GLint tangentAttribute;
	GLint uvAttribute;

	GLint wvpUniform;
	GLint maWorldUniform;
	GLint cameraPosUniform;
	GLint lightPosUniform;
	GLint lightColorUniform;
	GLint fresnelPowerUniform;

	GLint textureUniform[MAX_TEXTURE_UNIT];
	GLint textureCubeUniform;

	int Init(char * fileVertexShader, char * fileFragmentShader);
	~Shaders();
};