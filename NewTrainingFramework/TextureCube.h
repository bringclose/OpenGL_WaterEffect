#pragma once

#include "../Utilities/utilities.h"
#include "Globals.h"

class CTextureCube
{
public:
	CTextureCube(void);
	~CTextureCube(void);

	GLuint m_TextureID;

	void CreateTexture(char**);
};
