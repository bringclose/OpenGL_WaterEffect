#include "StdAfx.h"
#include "Object3D.h"

Object3D::Object3D(void)
{
	m_Shaders = NULL;
	m_Model = NULL;	
	m_TextureCube = NULL;

	m_NoTexture2D = 0;
	m_TextureList = NULL;

	pos = rot = scale = Vector3(0.0f, 0.0f, 0.0f);

	maTrans.SetIdentity();
	maScale.SetIdentity();
	
	maRotX.SetIdentity();
	maRotY.SetIdentity();
	maRotZ.SetIdentity();

	maRot = maRotZ * maRotX * maRotY;
	maWorld = maScale * maRot * maTrans;
}

Object3D::~Object3D(void)
{
}

void Object3D::Update(float deltaTime)
{
	maTrans.SetTranslation(pos);

	maRotX.SetRotationX(rot.x);
	maRotY.SetRotationY(rot.y);
	maRotZ.SetRotationZ(rot.z);
	maRot = maRotZ * maRotX * maRotY;

	maScale.SetScale(scale);

	maWorld = maScale * maRot * maTrans;
}