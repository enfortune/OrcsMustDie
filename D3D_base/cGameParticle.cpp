#include "stdafx.h"
#include "cGameParticle.h"


cGameParticle::cGameParticle()
	: m_pVB(nullptr)
{
}


cGameParticle::~cGameParticle()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pTexture);
}


void cGameParticle::RemoveDeadParticles()
{
	std::list<ST_PARTICLEATTRIBUTE>::iterator iter;

	iter = m_listParticles.begin();

	while (iter != m_listParticles.end())
	{
		if (iter->bIsAlive == false)
		{
			iter = m_listParticles.erase(iter);
		}
		else iter++;
	}
}
bool cGameParticle::IsEmpty()
{
	return m_listParticles.empty();
}
bool cGameParticle::IsDead()
{
	std::list<ST_PARTICLEATTRIBUTE>::iterator iter;

	for (iter = m_listParticles.begin(); iter != m_listParticles.end(); iter++)
	{
		if (iter->bIsAlive == true) return false;
	}

	return true;
}

void cGameParticle::Setup(std::string sFileName)
{
	HRESULT hr;

	hr = g_pD3DDevice->CreateVertexBuffer(
		m_dwVBSize * sizeof(ST_PARTICLE),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		ST_PARTICLE::FVF,
		D3DPOOL_DEFAULT,
		&m_pVB,
		0);

	if (sFileName == "") m_pTexture = nullptr;
	else 
	{
		m_pTexture = g_pTextureManager->GetTexture(sFileName);
		m_pTexture->AddRef();
	}
}
void cGameParticle::Update(float fDelta)
{
	
}
void cGameParticle::Render()
{
	if (!m_listParticles.empty())
	{
		PreRender();

		if (m_pTexture) g_pD3DDevice->SetTexture(0, m_pTexture);
		g_pD3DDevice->SetFVF(ST_PARTICLE::FVF);
		g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PARTICLE));

		if (m_dwVBOffset >= m_dwVBSize)
			m_dwVBOffset = 0;

		ST_PARTICLE* p = nullptr;

		m_pVB->Lock(
			m_dwVBOffset * sizeof(ST_PARTICLE),
			m_dwVBBatchSize * sizeof(ST_PARTICLE),
			(void**) &p,
			m_dwVBOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD );

		DWORD nNumParticlesInBatch = 0;

		std::list<ST_PARTICLEATTRIBUTE>::iterator iter;

		for (iter = m_listParticles.begin(); iter != m_listParticles.end(); iter++)
		{
			if (iter->bIsAlive)
			{
				p->vPosition = iter->vPosition;
				p->dwColor = (D3DCOLOR)iter->stColor;
				p++;
				nNumParticlesInBatch++;

				if (nNumParticlesInBatch >= m_dwVBBatchSize)
				{
					m_pVB->Unlock();
					g_pD3DDevice->DrawPrimitive(
						D3DPT_POINTLIST, 
						m_dwVBOffset, 
						m_dwVBBatchSize);
					m_dwVBOffset += m_dwVBBatchSize;
					
					if (m_dwVBOffset >= m_dwVBSize)
						m_dwVBOffset = 0;
					 
					m_pVB->Lock(
						m_dwVBOffset * sizeof(ST_PARTICLE), 
						m_dwVBBatchSize * sizeof(ST_PARTICLE),
						(void**) &p,
						m_dwVBOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					nNumParticlesInBatch = 0;
				}
			}
		}

		m_pVB->Unlock();


		if (nNumParticlesInBatch)
		{
			g_pD3DDevice->DrawPrimitive(
				D3DPT_POINTLIST,
				m_dwVBOffset,
				nNumParticlesInBatch);
		}

		m_dwVBOffset += m_dwVBBatchSize;

		PostRender();
	}
}

void cGameParticle::PreRender()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	//g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE, FtoDw(m_fSize));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0001f));

	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
void cGameParticle::PostRender()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	//g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}
void cGameParticle::Reset()
{
	std::list<ST_PARTICLEATTRIBUTE>::iterator iter;
	
	for (iter = m_listParticles.begin(); iter != m_listParticles.end(); iter++)
	{
		ResetParticle(&(*iter));
	}
}
void cGameParticle::AddParticles()
{
	ST_PARTICLEATTRIBUTE stAttr;

	ResetParticle(&stAttr);

	m_listParticles.push_back(stAttr);
}