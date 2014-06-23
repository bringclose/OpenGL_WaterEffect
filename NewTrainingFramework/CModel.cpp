#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include <tchar.h>
#include "CModel.h"


CModel::CModel(): m_aVertices(NULL), m_aIndices(NULL),
m_iNumVertices(0), m_iNumIndices(0),m_vertexBuffer(0), m_indexBuffer(0)
{
}

CModel::~CModel()
{
	SAFE_DEL_ARRAY(m_aVertices);
	SAFE_DEL_ARRAY(m_aIndices);
	SAFE_DEL_BUFFER(m_vertexBuffer);
	SAFE_DEL_BUFFER(m_indexBuffer);
}


void CModel::LoadToBuffer(char* modelPath)
{
	
	
	INT32 iBufferLength = 0;
	UINT8* byteBuffer = 0;
	FILE* file = fopen(modelPath, "rt");
	if(file != NULL)
	{
		//get file length
		fpos_t curpos;
		fgetpos(file, &curpos);
		fseek (file, 0, SEEK_END);
		UINT32 size = ftell (file);
		fseek(file, UINT32(curpos), SEEK_SET);

		iBufferLength = size;

		byteBuffer = new UINT8[iBufferLength];

		fread(byteBuffer, 1, iBufferLength, file);

		fclose(file);
	}
	else
	{
		esLogMessage("Can not read file %s\n", modelPath);
		return;
	}

	UINT32 offset = 0;
	UINT32 readByte = 0;

	int a = 0;
	sscanf((char*)byteBuffer + offset, "NrVertices: %d%n", &m_iNumVertices, &readByte);
	offset += readByte;
	
	m_aVertices = new Vertex[m_iNumVertices];

	INT32 i = 0;
	for(i = 0; i < m_iNumVertices; i++)
	{ 
		INT32 tmpID;
		sscanf((char*)byteBuffer + offset,"%d. %n", &tmpID, &readByte);
		offset += readByte;
		sscanf((char*)byteBuffer + offset,"pos:[%f, %f, %f]; %n", &m_aVertices[i].pos.x, &m_aVertices[i].pos.y, &m_aVertices[i].pos.z, &readByte);
		offset += readByte;
		sscanf((char*)byteBuffer + offset,"norm:[%f, %f, %f]; %n", &m_aVertices[i].normal.x, &m_aVertices[i].normal.y, &m_aVertices[i].normal.z, &readByte);
		offset += readByte;
		sscanf((char*)byteBuffer + offset,"binorm:[%f, %f, %f]; %n", &m_aVertices[i].binormal.x, &m_aVertices[i].binormal.y, &m_aVertices[i].binormal.z, &readByte);
		offset += readByte;
		sscanf((char*)byteBuffer + offset,"tgt:[%f, %f, %f]; %n", &m_aVertices[i].tangent.x, &m_aVertices[i].tangent.y, &m_aVertices[i].tangent.z, &readByte);
		offset += readByte;
		sscanf((char*)byteBuffer + offset,"uv:[%f, %f];%n", &m_aVertices[i].texcoord.x, &m_aVertices[i].texcoord.y, &readByte);
		offset += readByte;
	}

	//load m_aIndices
	sscanf((char*)byteBuffer + offset,"\n%n", &readByte); //need to skip 1 line here
	offset += readByte;
	sscanf((char*)byteBuffer + offset,"NrIndices: %d%n", &m_iNumIndices, &readByte);
	offset += readByte;
	m_aIndices = new UINT32[m_iNumIndices];
	m_iNumFaces = m_iNumIndices/3;

	for(i = 0; i < m_iNumFaces; i++)
	{ 
		INT32 tmpID;
		sscanf((char*)byteBuffer + offset,"%d.%n", &tmpID, &readByte);
		offset += readByte;
		sscanf((char*)byteBuffer + offset,"%d,%d,%d%n", &m_aIndices[i*3], &m_aIndices[i*3 + 1], &m_aIndices[i*3 + 2], &readByte);
		offset += readByte;
	
	}
	readByte = 0;
	SAFE_DEL_ARRAY(byteBuffer);
	

	//VBO
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,  sizeof(Vertex) * m_iNumVertices, m_aVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,  m_iNumIndices * sizeof(UINT32), m_aIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	esLogMessage("\n");
	esLogMessage("\n Finished load 3D model: %s", modelPath);
}



