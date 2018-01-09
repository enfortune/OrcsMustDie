#pragma once
class cGameObject
{
protected:
	ULONG m_ulRefCount;

public:
	cGameObject();
	virtual ~cGameObject();

	virtual void AddRef();
	virtual void Release();
};

