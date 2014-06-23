#include "StdAfx.h"
#include "TextureCube.h"

CTextureCube::CTextureCube(void)
{
}

CTextureCube::~CTextureCube(void)
{
	SAFE_DEL_TEXTURE(m_TextureID);
}
void CTextureCube::CreateTexture(char** textureCubePath)
{	
	int width,height,bpp;
	int format;
	char* imageData[6];
	
	imageData[0] = LoadTGA(textureCubePath[0], &width, &height, &bpp);
	imageData[1] = LoadTGA(textureCubePath[1], &width, &height, &bpp);
	imageData[2] = LoadTGA(textureCubePath[2], &width, &height, &bpp);
	imageData[3] = LoadTGA(textureCubePath[3], &width, &height, &bpp);
	imageData[4] = LoadTGA(textureCubePath[4], &width, &height, &bpp);
	imageData[5] = LoadTGA(textureCubePath[5], &width, &height, &bpp);

	if(bpp == 24)
		format = GL_RGB;
	else if(bpp == 32)
		format = GL_RGBA;
	else
		esLogMessage("\n unsupported image format");
	
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData[2]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData[3]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData[5]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData[4]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData[1]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData[0]);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	for(int i=0; i<6; i++)
		SAFE_DEL_ARRAY(imageData[i]);

	esLogMessage("\n");
	esLogMessage("\n Finished create Cube texture:", textureCubePath[0]);
	esLogMessage("\n \t %s", textureCubePath[0]);
	esLogMessage("\n \t %s", textureCubePath[1]);
	esLogMessage("\n \t %s", textureCubePath[2]);
	esLogMessage("\n \t %s", textureCubePath[3]);
	esLogMessage("\n \t %s", textureCubePath[4]);
	esLogMessage("\n \t %s", textureCubePath[5]);
}
