#include "StdAfx.h"
#include "Texture.h"

CTexture::CTexture(void)
{
}

CTexture::~CTexture(void)
{
	SAFE_DEL_TEXTURE(m_TextureID);
}

void CTexture::CreateTexture(char* texture2DPath)
{
	int width,height,bpp;
	int format;
	char* imageData = LoadTGA(texture2DPath, &width, &height, &bpp);

	if(imageData == NULL)
	{
		esLogMessage("\n ERROR: can not load %s", texture2DPath);
		return;
	}

	if(bpp == 24)
		format = GL_RGB;
	else if(bpp == 32)
		format = GL_RGBA;
	else
		esLogMessage("\n unsupported image format");

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	
	SAFE_DEL_ARRAY(imageData);

	esLogMessage("\n");
	esLogMessage("\n Finished create 2D texture: %s", texture2DPath);
}
