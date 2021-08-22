
#include "stdafx.h"
#include "GameApp.h"
#include "MyButton.h"
#include "GameAppView.h"





MyButton::MyButton()
{
	m_accessLevel = 3;
	m_timer = 0;
}


MyButton::~MyButton(void)
{

}

void MyButton::process(void)
{
	/*
	if(m_timer) 
	{
		m_timer--;

		if(m_timer == 0)
		{
			g_gameAppView->m_sayButton.SetBitmap(Skin_ButtonSayOff);
		}
	}
	*/
	
}


BEGIN_MESSAGE_MAP(MyButton, CButton)
   ON_WM_MOUSEMOVE()
   // ON_WM_MOUSEACTIVATE()
   
END_MESSAGE_MAP()



void MyButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	
	/*
	if(g_gameAppView->m_playerSockIdx != -1 && g_gameAppView->m_playerIdx >= 0)
	{
		if(m_player[g_gameAppView->m_playerIdx].level < m_accessLevel)
		{
			g_gameAppView->m_sayButton.SetBitmap(Skin_ButtonSayOn);
			m_timer = 30;
		}
	}
	else
	{
		
		if(point.x > 0 && point.y > 0 && point.x < 28 && point.y < 22)
		{
			g_gameAppView->m_sayButton.SetBitmap(Skin_ButtonSayOn);
		}
		else
		{
			g_gameAppView->m_sayButton.SetBitmap(Skin_ButtonSayOff);
		}
		//m_timer = 30;
		
	}
	*/
}


