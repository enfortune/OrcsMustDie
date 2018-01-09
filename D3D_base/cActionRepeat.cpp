#include "stdafx.h"
#include "cActionRepeat.h"


cActionRepeat::cActionRepeat()
	: m_pAction(NULL)
{
}


cActionRepeat::~cActionRepeat()
{
	SAFE_DELETE(m_pAction);
}

void cActionRepeat::Start()
{
	if (m_pAction == NULL)
		return;

	m_pAction->SetDelegate(this);
	m_pAction->Start();
}

void cActionRepeat::Update()
{
	if (m_pAction == NULL)
		return;
	m_pAction->Update();
}

void cActionRepeat::OnActionFinish(cAction * pSender)
{
	if (m_pAction)
	{
		m_pAction->Start();
	}
}
