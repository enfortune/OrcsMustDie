#include "stdafx.h"
#include "cGameObject.h"


cGameObject::cGameObject()
	: m_ulRefCount(1)
{
}


cGameObject::~cGameObject()
{
}

void cGameObject::AddRef()
{
	m_ulRefCount++;
}

void cGameObject::Release()
{
	m_ulRefCount--;
	if (m_ulRefCount == 0)
	{
		delete this;
	}
}
