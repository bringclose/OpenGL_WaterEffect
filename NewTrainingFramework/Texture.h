#pragma once

#include "../Utilities/utilities.h"
#include "Globals.h"

class CTexture
{
public:
	CTexture(void);
	~CTexture(void);

	GLuint m_TextureID;

	void CreateTexture(char*);
};
