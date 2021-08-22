// TableButton.h: interface for the CTableButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLEBUTTON_H__656B663B_3247_4A98_BB35_4E68B7B3272A__INCLUDED_)
#define AFX_TABLEBUTTON_H__656B663B_3247_4A98_BB35_4E68B7B3272A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTableButton  
{
public:
	CTableButton();
	virtual ~CTableButton();

	CWnd *m_pParent;
	int m_listCtrlIdx;
	BOOL m_seated;
	BOOL m_enabled;
	char m_type;
	char *m_title;
	BOOL m_visible;

public:
	void Create(char type, char *title);
	void Show(void);
	void Hide(void);

};

#endif // !defined(AFX_TABLEBUTTON_H__656B663B_3247_4A98_BB35_4E68B7B3272A__INCLUDED_)
