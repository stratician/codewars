#include "stdafx.h"

#include "CoreEx.h"

CCoreEx::CCoreEx()
{
m_pParent = NULL;
}

void CCoreEx::SetId(int id)
{
	m_id = id;
}

void CCoreEx::PassParent(CCoreEx *pParent)
{
	m_pParent = pParent;
}

void CCoreEx::OnEvent(int id)
{
	// idle //
	//MessageBox(0,"<-->","ok",MB_OK);
}
