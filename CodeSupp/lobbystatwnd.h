// lobbystatwnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLobbyStatWnd window

#define LSWMODE_HOUR	0
#define LSWMODE_MINUTE	1

class CLobbyStat;

class CLobbyStatWnd : public CWnd
{
// Construction
public:
	CLobbyStatWnd();

// Attributes
public:
	CLobbyStat *m_pParent;
	BOOL m_bActive;
	DWORD m_mode;

// Operations
public:
	void Appear(void);
	void Disappear(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLobbyStatWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLobbyStatWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLobbyStatWnd)
	afx_msg void OnPaint();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
