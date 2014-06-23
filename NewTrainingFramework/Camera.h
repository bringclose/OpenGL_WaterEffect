#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "../Utilities/utilities.h"

class CCamera
{
public:
	CCamera(void);
	~CCamera(void);

	void Init(void);
	void Update(float);

	Vector3 pos;
	Vector3 rot;
	Vector3 scale;
	GLfloat speed;

	Matrix maTrans, maRotX, maRotY, maRotZ, maRot;
	Matrix maTransInverse, maRotInverse;
	Matrix maView;
	Matrix maProjection;

	void MoveX(GLfloat stepLenght);
	void MoveY(GLfloat stepLenght);
	void MoveZ(GLfloat stepLenght);
	void RotateX(GLfloat rotateAngle);
	void RotateY(GLfloat rotateAngle);
	void RotateZ(GLfloat rotateAngle);
};
#endif