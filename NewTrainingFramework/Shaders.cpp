#include <stdafx.h>
#include "Shaders.h"

int Shaders::Init(char * fileVertexShader, char * fileFragmentShader)
{
	vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);

	if ( vertexShader == 0 )
		return -1;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);

	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	//finding location of uniforms / attributes
	positionAttribute = glGetAttribLocation(program, "a_posL");
	uvAttribute = glGetAttribLocation(program, "a_uv");
	normalAttribute = glGetAttribLocation(program, "a_normalL");
	binormalAttribute = glGetAttribLocation(program, "a_bitangentL");
	tangentAttribute = glGetAttribLocation(program, "a_tangentL");

	maWorldUniform = glGetUniformLocation(program, "u_maWorld");	//world matrix
	wvpUniform = glGetUniformLocation(program, "u_mvp");			//MVP matrix
	cameraPosUniform = glGetUniformLocation(program, "u_camPos");
	lightPosUniform = glGetUniformLocation(program, "u_lightPos");
	lightColorUniform = glGetUniformLocation(program, "u_lightColor");
	fresnelPowerUniform = glGetUniformLocation(program, "u_fresnelPower"); //for water effect

	textureCubeUniform = glGetUniformLocation(program, "u_textureCube");
	u_time=glGetUniformLocation(program,"u_time");
	u_dMax=glGetUniformLocation(program,"u_dMax");

	char textureUniformString[]="u_texture0";
	int length=strlen(textureUniformString);
	for(int i=0;i<MAX_TEXTURE_UNIT;i++)
	{
		textureUniformString[length-1]=(char)(i+48);
		textureUniform[i]=glGetUniformLocation(program,textureUniformString);
	}

	esLogMessage("\n");
	esLogMessage("\n Finished compile shader:");
	esLogMessage("\n \t %s", fileVertexShader);
	esLogMessage("\n \t %s", fileFragmentShader);

	return 0;
}

Shaders::~Shaders()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}