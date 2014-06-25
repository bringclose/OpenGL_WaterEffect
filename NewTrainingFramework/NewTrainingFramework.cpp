// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include <conio.h>

#include "Globals.h"
#include "Object3D.h"
#include "Vertex.h"
#include "CModel.h"
#include "Shaders.h"
#include "Camera.h"
#include "Texture.h"
#include "Globals.h"

#define NUMBER_OBJECT		4
#define NUMBER_TEXTURE2D	12

CCamera myCamera;
Matrix maWVP[NUMBER_OBJECT];
Light light;

Object3D objectList[NUMBER_OBJECT];
CModel modelList[NUMBER_OBJECT];
CTexture textureList[NUMBER_TEXTURE2D];
CTextureCube textureCube;
Shaders shaderList[NUMBER_OBJECT];

GLfloat fresnelPower = 1.0;
//FBO
GLuint textureId;
GLuint rboId;
GLuint fboId;
GLuint mDepthTexFBO;

float moveStepLenght = 100.0f;
float rotateAngle = 1.0f;
short keyPressed = 0;

bool WasKeyPressed(short);
void InitObjects(void);
void InitResource(void);
void InitFBO(void);

float Globals::totalTime=0;

int Init ( ESContext *esContext )
{
	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );

	InitResource();
	InitObjects();
	InitFBO();
	
	return 0;
}

void Draw ( ESContext *esContext )
{
	//FBO
	// set rendering destination to FBO
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	CCamera reflectCam;
	reflectCam.pos = objectList[3].pos;
	reflectCam.rot = objectList[3].rot;
	reflectCam.rot.x = -reflectCam.rot.x;
	//Matrix maWVPReflect = objectList[i].maWorld * myCamera.maView * myCamera.maProjection;
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	for(int i=3; i<4; i++)
	{
		glUseProgram(objectList[i].m_Shaders->program);

		//transfer Vertex steam to Shader (using VBO)
		glBindBuffer(GL_ARRAY_BUFFER, objectList[i].m_Model->m_vertexBuffer);
		
		if(objectList[i].m_Shaders->positionAttribute != -1)
		{
			glEnableVertexAttribArray(objectList[i].m_Shaders->positionAttribute);
			glVertexAttribPointer(objectList[i].m_Shaders->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), POSITION_OFFSET);
		}

		if(objectList[i].m_Shaders->uvAttribute != -1)
		{
			glEnableVertexAttribArray(objectList[i].m_Shaders->uvAttribute);
			glVertexAttribPointer(objectList[i].m_Shaders->uvAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), TEXCOORD_OFFSET);
		}

		if(objectList[i].m_Shaders->normalAttribute != -1)
		{
			glEnableVertexAttribArray(objectList[i].m_Shaders->normalAttribute);
			glVertexAttribPointer(objectList[i].m_Shaders->normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NORMAL_OFFSET);
		}

		if(objectList[i].m_Shaders->binormalAttribute != -1)
		{
			glEnableVertexAttribArray(objectList[i].m_Shaders->binormalAttribute);
			glVertexAttribPointer(objectList[i].m_Shaders->binormalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BINORMAL_OFFSET);
		}

		if(objectList[i].m_Shaders->tangentAttribute != -1)
		{
			glEnableVertexAttribArray(objectList[i].m_Shaders->tangentAttribute);
			glVertexAttribPointer(objectList[i].m_Shaders->tangentAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), TANGENT_OFFSET);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//transfer matrix to Shader
		if(objectList[i].m_Shaders->maWorldUniform != -1)
		{
			glUniformMatrix4fv(objectList[i].m_Shaders->maWorldUniform, 1, GL_FALSE, (GLfloat*)objectList[i].maWorld.m);
		}

		if(objectList[i].m_Shaders->wvpUniform != -1)
		{
			glUniformMatrix4fv(objectList[i].m_Shaders->wvpUniform, 1, GL_FALSE, (GLfloat*)maWVP[i].m);
		}

		//transfer textureCube to Shader
		int textureUnit = 0;
		//textureCube
		if(objectList[i].m_Shaders->textureCubeUniform != -1)
		{
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_CUBE_MAP, objectList[i].m_TextureCube->m_TextureID);
			glUniform1i(objectList[i].m_Shaders->textureCubeUniform, textureUnit);

			textureUnit++;
		}

		//texture2D
		for(int j=0; j<objectList[i].m_NoTexture2D; j++)
		{
			if(objectList[i].m_Shaders->textureUniform[j] != -1)
			{
				glActiveTexture(GL_TEXTURE0 + textureUnit);
				glBindTexture(GL_TEXTURE_2D, objectList[i].m_TextureList[j].m_TextureID);
				glUniform1i(objectList[i].m_Shaders->textureUniform[j], textureUnit);

				textureUnit++;
			}
		}

		//transfer position of Camera to Shader
		if(objectList[i].m_Shaders->cameraPosUniform != -1)
		{
			glUniform3fv(objectList[i].m_Shaders->cameraPosUniform, 1, &myCamera.pos.x);
		}

		//transfer light's info to Shader
		if(objectList[i].m_Shaders->lightPosUniform != -1)
		{
			glUniform3fv(objectList[i].m_Shaders->lightPosUniform, 1, &light.lightPos.x);
		}
		if(objectList[i].m_Shaders->lightColorUniform != -1)
		{
			glUniform4fv(objectList[i].m_Shaders->lightColorUniform, 1, &light.lightColor.x);
		}

		//transfer fresnel power to Shader for water effect
		if(objectList[i].m_Shaders->fresnelPowerUniform != -1)
		{
			glUniform1f(objectList[i].m_Shaders->fresnelPowerUniform, fresnelPower);
		}

		//time
		if(objectList[i].m_Shaders->u_time!=-1)
		{
			glUniform1f(objectList[i].m_Shaders->u_time,Globals::totalTime);
		}
		//u_dMax
		if(objectList[i].m_Shaders->u_dMax!=-1)
		{
			glUniform1f(objectList[i].m_Shaders->u_dMax,0.05);
		}
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//using IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectList[i].m_Model->m_indexBuffer);

		glDrawElements(GL_TRIANGLES, objectList[i].m_Model->m_iNumIndices, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisable(GL_DEPTH_TEST);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 960, 720);
	for(int i=0; i<NUMBER_OBJECT; i++)
	{
		glUseProgram(objectList[i].m_Shaders->program);

		//transfer Vertex steam to Shader (using VBO)
		glBindBuffer(GL_ARRAY_BUFFER, objectList[i].m_Model->m_vertexBuffer);
		
		if(objectList[i].m_Shaders->positionAttribute != -1)
		{
			glEnableVertexAttribArray(objectList[i].m_Shaders->positionAttribute);
			glVertexAttribPointer(objectList[i].m_Shaders->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), POSITION_OFFSET);
		}

		if(objectList[i].m_Shaders->uvAttribute != -1)
		{
			glEnableVertexAttribArray(objectList[i].m_Shaders->uvAttribute);
			glVertexAttribPointer(objectList[i].m_Shaders->uvAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), TEXCOORD_OFFSET);
		}

		if(objectList[i].m_Shaders->normalAttribute != -1)
		{
			glEnableVertexAttribArray(objectList[i].m_Shaders->normalAttribute);
			glVertexAttribPointer(objectList[i].m_Shaders->normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NORMAL_OFFSET);
		}

		if(objectList[i].m_Shaders->binormalAttribute != -1)
		{
			glEnableVertexAttribArray(objectList[i].m_Shaders->binormalAttribute);
			glVertexAttribPointer(objectList[i].m_Shaders->binormalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BINORMAL_OFFSET);
		}

		if(objectList[i].m_Shaders->tangentAttribute != -1)
		{
			glEnableVertexAttribArray(objectList[i].m_Shaders->tangentAttribute);
			glVertexAttribPointer(objectList[i].m_Shaders->tangentAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), TANGENT_OFFSET);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//transfer matrix to Shader
		if(objectList[i].m_Shaders->maWorldUniform != -1)
		{
			glUniformMatrix4fv(objectList[i].m_Shaders->maWorldUniform, 1, GL_FALSE, (GLfloat*)objectList[i].maWorld.m);
		}

		if(objectList[i].m_Shaders->wvpUniform != -1)
		{
			glUniformMatrix4fv(objectList[i].m_Shaders->wvpUniform, 1, GL_FALSE, (GLfloat*)maWVP[i].m);
		}

		//transfer textureCube to Shader
		int textureUnit = 0;
		//textureCube
		if(objectList[i].m_Shaders->textureCubeUniform != -1)
		{
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_CUBE_MAP, objectList[i].m_TextureCube->m_TextureID);
			glUniform1i(objectList[i].m_Shaders->textureCubeUniform, textureUnit);

			textureUnit++;
		}

		//texture2D
		for(int j=0; j<objectList[i].m_NoTexture2D; j++)
		{
			if(objectList[i].m_Shaders->textureUniform[j] != -1)
			{
				glActiveTexture(GL_TEXTURE0 + textureUnit);
				glBindTexture(GL_TEXTURE_2D, objectList[i].m_TextureList[j].m_TextureID);
				glUniform1i(objectList[i].m_Shaders->textureUniform[j], textureUnit);

				textureUnit++;
			}
		}

		//transfer position of Camera to Shader
		if(objectList[i].m_Shaders->cameraPosUniform != -1)
		{
			glUniform3fv(objectList[i].m_Shaders->cameraPosUniform, 1, &myCamera.pos.x);
		}

		//transfer light's info to Shader
		if(objectList[i].m_Shaders->lightPosUniform != -1)
		{
			glUniform3fv(objectList[i].m_Shaders->lightPosUniform, 1, &light.lightPos.x);
		}
		if(objectList[i].m_Shaders->lightColorUniform != -1)
		{
			glUniform4fv(objectList[i].m_Shaders->lightColorUniform, 1, &light.lightColor.x);
		}

		//transfer fresnel power to Shader for water effect
		if(objectList[i].m_Shaders->fresnelPowerUniform != -1)
		{
			glUniform1f(objectList[i].m_Shaders->fresnelPowerUniform, fresnelPower);
		}

		//time
		if(objectList[i].m_Shaders->u_time!=-1)
		{
			glUniform1f(objectList[i].m_Shaders->u_time,Globals::totalTime);
		}
		//u_dMax
		if(objectList[i].m_Shaders->u_dMax!=-1)
		{
			glUniform1f(objectList[i].m_Shaders->u_dMax,0.05);
		}
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//using IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectList[i].m_Model->m_indexBuffer);

		glDrawElements(GL_TRIANGLES, objectList[i].m_Model->m_iNumIndices, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisable(GL_DEPTH_TEST);
	}

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{
	Globals::totalTime+=deltaTime;
	myCamera.Update(deltaTime);

	for(int i=0; i<NUMBER_OBJECT; i++)
	{
		objectList[i].Update(deltaTime);
		maWVP[i] = objectList[i].maWorld * myCamera.maView * myCamera.maProjection;
	}
	
	//control system
	if (WasKeyPressed(KEY_W))
	{
		//process when W key was pressed
		myCamera.MoveZ((-moveStepLenght)*deltaTime);
	}
	else if (WasKeyPressed(KEY_S))
	{
		myCamera.MoveZ(moveStepLenght*deltaTime);
	}
	else if (WasKeyPressed(KEY_A))
	{
		myCamera.MoveX((-moveStepLenght)*deltaTime);
	}
	else if (WasKeyPressed(KEY_D))
	{
		myCamera.MoveX(moveStepLenght*deltaTime);
	}
	else if (WasKeyPressed(KEY_UP))
	{
		myCamera.RotateX(rotateAngle*deltaTime);
	}
	else if (WasKeyPressed(KEY_DOWN))
	{
		myCamera.RotateX((-rotateAngle)*deltaTime);
	}
	else if (WasKeyPressed(KEY_RIGHT))
	{
		myCamera.RotateY((-rotateAngle)*deltaTime);
	}
	else if (WasKeyPressed(KEY_LEFT))
	{
		myCamera.RotateY(rotateAngle*deltaTime);
	}
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	if (bIsPressed)
	{
		switch(key)
		{
		case 'W':
			keyPressed |= KEY_W;
			break;
		case 'S':
			keyPressed |= KEY_S;
			break;
		case 'A':
			keyPressed |= KEY_A;
			break;
		case 'D':
			keyPressed |= KEY_D;
			break;
		case '%':
			keyPressed |= KEY_LEFT;
			break;
		case '\'':
			keyPressed |= KEY_RIGHT;
			break;
		case '&':
			keyPressed |= KEY_UP;
			break;
		case '(':
			keyPressed |= KEY_DOWN;
			break;
		}
	}
	else
	{
		switch(key)
		{
		case 'W': case 'w':
			keyPressed &= ~(KEY_W);
			break; 
		case 'S': case 's':
			keyPressed &= ~(KEY_S);
			break;
		case 'A': case 'a':
			keyPressed &= ~(KEY_A);
			break;
		case 'D': case 'd':
			keyPressed &= ~(KEY_D);
			break;
		case '%':
			keyPressed &= ~(KEY_LEFT);
			break;
		case '\'':
			keyPressed &= ~(KEY_RIGHT);
			break;
		case '&':
			keyPressed &= ~(KEY_UP);
			break;
		case '(':
			keyPressed &= ~(KEY_DOWN);
			break;
		case ' ':
			keyPressed &= ~(KEY_SPACEBAR);
			break;
		}
	}
}

void CleanUp()
{
	for(int i=0; i<NUMBER_OBJECT; i++)
	{
		SAFE_DEL_ARRAY(objectList[i].m_TextureList);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();

	printf("Press any key...\n");
	_getch();

	return 0;
}

bool WasKeyPressed(short keyCode)
{
	return ((keyPressed & keyCode) == keyCode);
}

void InitResource()
{
	modelList[0].LoadToBuffer("../Resources/Models/SkyBox.nfg");
	modelList[1].LoadToBuffer("../Resources/Models/Terrain.nfg");
	modelList[2].LoadToBuffer("../Resources/Models/Bila.nfg");
	modelList[3].LoadToBuffer("../Resources/Models/water.nfg");

	shaderList[0].Init("../Resources/Shaders/SkyBox.vs", "../Resources/Shaders/SkyBox.fs");
	shaderList[1].Init("../Resources/Shaders/Terrain.vs", "../Resources/Shaders/Terrain.fs");
	shaderList[2].Init("../Resources/Shaders/NormalMapping.vs", "../Resources/Shaders/NormalMapping.fs");
	shaderList[3].Init("../Resources/Shaders/Water.vs", "../Resources/Shaders/Water.fs");

	char** textureCubePath = new char*[6];
	textureCubePath[0] = "../Resources/Textures/skybox_back.tga";
	textureCubePath[1] = "../Resources/Textures/skybox_front.tga";
	textureCubePath[2] = "../Resources/Textures/skybox_right.tga";
	textureCubePath[3] = "../Resources/Textures/skybox_left.tga";
	textureCubePath[4] = "../Resources/Textures/skybox_top.tga";
	textureCubePath[5] = "../Resources/Textures/skybox_bottom.tga";
	textureCube.CreateTexture(textureCubePath);

	textureList[0].CreateTexture("../Resources/Textures/heightmap.tga");
	textureList[1].CreateTexture("../Resources/Textures/blendMap.tga");
	textureList[2].CreateTexture("../Resources/Textures/Grass.tga");
	textureList[3].CreateTexture("../Resources/Textures/Rock.tga");
	textureList[4].CreateTexture("../Resources/Textures/Dirt.tga");
	textureList[5].CreateTexture("../Resources/Textures/Rock_Normal.tga");
	textureList[6].CreateTexture("../Resources/Textures/DisplacementMap.tga");
	//textureList[7].CreateTexture("../Resources/Textures/WaterBottom.tga");

	textureList[7].CreateTexture("../Resources/Textures/WaterNormal.tga");
	textureList[8].CreateTextureL("../Resources/Textures/Gradient.tga");
	textureList[9].CreateTextureL("../Resources/Textures/Pool.tga");
	textureList[10].CreateTextureL("../Resources/Textures/PoolNormal.tga");
	textureList[11].CreateTexture("../Resources/Textures/PoolNormal.tga");
}

void InitObjects()
{
	light.lightPos = Vector3(0.0f, 0.0f, 0.0f);
	light.lightColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	//Init beginning stage of camera
	myCamera.Init();
	myCamera.pos = Vector3(-50.0f, -5.0f, 100.0f);
	myCamera.rot = Vector3(-0.2f, 0.0f, 0.0f);
	myCamera.speed = 10.0f;	

	//Init beginning stage of objects
	//skybox
	objectList[0].m_Model = &modelList[0];
	objectList[0].m_Shaders = &shaderList[0];

	objectList[0].m_TextureCube = &textureCube;

	objectList[0].m_NoTexture2D = 0;
	objectList[0].m_TextureList = NULL;

	objectList[0].pos = Vector3(0.0f, 0.0f, 0.0f);
	objectList[0].rot = Vector3(0.0f, 0.0f, 0.0f);
	objectList[0].scale = Vector3(500.0f, 500.0f, 500.0f);

	//terrain
	objectList[1].m_Model = &modelList[1];
	objectList[1].m_Shaders = &shaderList[1];

	objectList[1].m_TextureCube = NULL;

	objectList[1].m_NoTexture2D = 6;
	objectList[1].m_TextureList = new CTexture[6];

	objectList[1].m_TextureList[0] = textureList[0];
	objectList[1].m_TextureList[1] = textureList[1];
	objectList[1].m_TextureList[2] = textureList[2];
	objectList[1].m_TextureList[3] = textureList[3];
	objectList[1].m_TextureList[4] = textureList[4];
	objectList[1].m_TextureList[5] = textureList[11];

	objectList[1].pos = Vector3(0.0f, -30.0f, 0.0f);
	objectList[1].rot = Vector3(0.0f, 0.0f, 0.0f);
	objectList[1].scale = Vector3(5.0f, 5.0f, 5.0f);

	//normal mapping
	objectList[2].m_Model = &modelList[2];
	objectList[2].m_Shaders = &shaderList[2];

	objectList[2].m_TextureCube = NULL;

	objectList[2].m_NoTexture2D = 2;
	objectList[2].m_TextureList = new CTexture[2];
	objectList[2].m_TextureList[0] = textureList[3];	//rock
	objectList[2].m_TextureList[1] = textureList[5];	//rock_normal

	objectList[2].pos = Vector3(-30.0f, -10.0f, 50.0f);
	objectList[2].rot = Vector3(0.0f, 0.0f, 0.0f);
	objectList[2].scale = Vector3(0.1f, 0.1f, 0.1f);

	//water
	objectList[3].m_Model = &modelList[3];
	objectList[3].m_Shaders = &shaderList[3];

	objectList[3].m_TextureCube = &textureCube;

	objectList[3].m_NoTexture2D = 7;
	objectList[3].m_TextureList = new CTexture[7];
	objectList[3].m_TextureList[0] = textureList[3];	//rock
	objectList[3].m_TextureList[1] = textureList[6];	//displacement	
	objectList[3].m_TextureList[2] = textureList[7];	//waternormal
	objectList[3].m_TextureList[3] = textureList[8];	//HeightMap
	objectList[3].m_TextureList[4] = textureList[9];	//Pool
	objectList[3].m_TextureList[5] = textureList[10];	//PoolNormal
	objectList[3].m_TextureList[6] = textureList[11];	//PoolNormal

	objectList[3].pos = Vector3(-50.0f, -20.0f, 50.0f);
	objectList[3].rot = Vector3(PI, PI/2, 0.0f);
	objectList[3].scale = Vector3(10.0f, 10.0f, 10.0f);
}

void InitFBO(void)
{
	// Create FrameBuffer
	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &mDepthTexFBO);
	glBindTexture(GL_TEXTURE_2D, mDepthTexFBO);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexFBO, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//cheat water
	//objectList[3].m_TextureList[6].m_TextureID = textureId;
	objectList[1].m_TextureList[5].m_TextureID = textureId;
}