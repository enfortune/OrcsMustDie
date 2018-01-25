#pragma once
#include "cGameParticleEnumList.h"

typedef struct ST_PARTICLE
{
	D3DXVECTOR3 vPosition;
	D3DCOLOR	dwColor;
	
	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE /*| D3DFVF_PSIZE*/ };
}PARTICLE;

typedef struct ST_PARTICLEATTRIBUTE
{
	D3DXVECTOR3 vPosition;
	D3DXVECTOR3 vVelocity;
	D3DXVECTOR3 vAcceleration;
	float		fLifeTime;
	float		fAge;
	D3DXCOLOR	stColor;
	D3DXCOLOR	stColorFade;
	bool		bIsAlive;

}PARTICLEATTRIBUTE;

class cGameParticle
{
protected:
	D3DXVECTOR3	m_vOrigin;
	D3DXVECTOR3 m_vBound_000;
	D3DXVECTOR3 m_vBound_111;
	float m_fEmitRate; // 파티클이 초당 추가되는 비율
	float m_fSize;
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	std::list<ST_PARTICLEATTRIBUTE> m_listParticles;
	int m_nMaxParticles;

	DWORD m_dwVBSize;
	DWORD m_dwVBOffset;
	DWORD m_dwVBBatchSize;
	

protected:
	virtual void RemoveDeadParticles();
public:
	cGameParticle();
	virtual ~cGameParticle();

	bool IsEmpty();
	bool IsDead();

	virtual void Setup(std::string sFileName);
	virtual void Update(float fDelta);
	virtual void Render();
	
	virtual void PreRender();
	virtual void PostRender();
	virtual void Reset();
	virtual void ResetParticle(ST_PARTICLEATTRIBUTE* pAttr) = 0;
	virtual void AddParticles();
};

