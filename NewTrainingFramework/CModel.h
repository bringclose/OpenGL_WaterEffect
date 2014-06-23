#ifndef __MODEL_H__
#define __MODEL_H__


#include "../Utilities/utilities.h"
#include "Vertex.h"


class CModel
{
public:
	// Constructor
	CModel();

	// Destructor
	virtual ~CModel();

	// Methods
	void LoadToBuffer(char* modelPath);

	// Propertices
	char m_modelPath[256];
	Vertex *m_aVertices;
	int m_iNumVertices;

	unsigned int *m_aIndices;
	int m_iNumIndices;
	int m_iNumFaces;

	//GLuint m_idVBO;
	GLuint m_vertexBuffer;
	GLuint m_indexBuffer;

	
	
};
#endif