// TableButton.cpp: implementation of the CTableButton class.
//
//////////////////////////////////////////////////////////////////////

//#include "realsnooker.h"
#include "../CodeSupp/stdafx.h"
#include "TableButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTableButton::CTableButton()
{
	m_pParent = NULL;
	m_listCtrlIdx = 0; // Table #0 / Watch
	m_seated = FALSE;	// Defaultly no one is seated at this Button //
	m_enabled = TRUE;	// Default Button is Enabled //
	m_visible = TRUE;
}

CTableButton::~CTableButton()
{

}


// Here we create the Table Button Data
// Type 0: Lobby TV
// Type 1: Watch
// Type 2: Join P1
// Type 3: Join P2
// title: Caption for generated Button //
void CTableButton::Create(char type, char *title)
{
	m_type = type;
	m_title = new char[strlen(title)+1];
}


void CTableButton::Show(void)
{
	m_visible = TRUE;
}


void CTableButton::Hide(void)
{
	m_visible = FALSE;
}