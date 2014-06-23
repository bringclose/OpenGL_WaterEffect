#pragma once

#include "../Utilities/utilities.h"
#include "Shaders.h"
#include "Globals.h"
#include "CModel.h"
#include "Texture.h"
#include "TextureCube.h"

class Object3D
{
public:
	Object3D(void);
	~Object3D(void);

	void Update(float);

	Shaders* m_Shaders;
	CModel* m_Model;	
	CTextureCube* m_TextureCube;

	GLuint m_NoTexture2D;
	CTexture* m_TextureList;

	Vector3 pos, rot, scale;

	Matrix  maWorld;
	Matrix maTrans, maScale, maRotX, maRotY, maRotZ, maRot;
};
