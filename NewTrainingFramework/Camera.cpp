#include "StdAfx.h"
#include "Camera.h"
#include "Globals.h"

CCamera::CCamera(void)
{
}

CCamera::~CCamera(void)
{
}

void CCamera::Init(void)
{
	maTrans.SetIdentity();
	maRotX.SetIdentity();
	maRotY.SetIdentity();
	maRotZ.SetIdentity();
	maRot.SetIdentity();

	maView.SetIdentity();
	maProjection.SetIdentity();
}

void CCamera::Update(float deltaTime)
{
	maRotX.SetRotationX(rot.x);
	maRotY.SetRotationY(rot.y);
	maRotZ.SetRotationZ(rot.z);

	maRot = maRotZ * maRotX * maRotY;
	maTrans.SetTranslation(pos);

	//inverse rotation matrix
	maRotInverse.m[0][0] = maRot.m[0][0];
	maRotInverse.m[0][1] = maRot.m[1][0];
	maRotInverse.m[0][2] = maRot.m[2][0];
	maRotInverse.m[0][3] = maRot.m[3][0];

	maRotInverse.m[1][0] = maRot.m[0][1];
	maRotInverse.m[1][1] = maRot.m[1][1];
	maRotInverse.m[1][2] = maRot.m[2][1];
	maRotInverse.m[1][3] = maRot.m[3][1];

	maRotInverse.m[2][0] = maRot.m[0][2];
	maRotInverse.m[2][1] = maRot.m[1][2];
	maRotInverse.m[2][2] = maRot.m[2][2];
	maRotInverse.m[2][3] = maRot.m[3][2];

	maRotInverse.m[3][0] = maRot.m[0][3];
	maRotInverse.m[3][1] = maRot.m[1][3];
	maRotInverse.m[3][2] = maRot.m[2][3];
	maRotInverse.m[3][3] = maRot.m[3][3];

	//inverse translation matrix
	maTransInverse = maTrans.SetTranslation(-pos);

	maView = maTransInverse * maRotInverse;

	maProjection.SetPerspective(FOV, ASPECT, NEAR_PLANE, FAR_PLANE);
}

void CCamera::MoveX(GLfloat stepLenght)
{
	pos.x += stepLenght;
}
void CCamera::MoveY(GLfloat stepLenght)
{
	pos.y += stepLenght;
}
void CCamera::MoveZ(GLfloat stepLenght)
{
	pos.z += stepLenght;
}
void CCamera::RotateX(GLfloat rotateAngle)
{
	rot.x += rotateAngle;
}
void CCamera::RotateY(GLfloat rotateAngle)
{
	rot.y += rotateAngle;
}
void CCamera::RotateZ(GLfloat rotateAngle)
{
	rot.z += rotateAngle;
}