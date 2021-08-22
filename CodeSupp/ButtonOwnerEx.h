// ButtonOwnerEx.h : header file
//

#ifndef __BUTTONOWNEREX_H__
#define __BUTTONOWNEREX_H__

/////////////////////////////////////////////////////////////////////////////
// CButtonOwnerEx window

#define	BUTTONOWNEREX_CENTER	-1

class CButtonOwnerEx : public CButton
{
// Construction
public:
	CButtonOwnerEx();

// Attributes
public:
	//	Positioning
	BOOL		m_bUseOffset;
	CPoint		m_pointImage;
	CPoint		m_pointText;
	int			m_nImageOffsetFromBorder;
	int			m_nTextOffsetFromImage;

	//	Image
	HICON		m_hIcon;
	HBITMAP		m_hBitmap;
	HBITMAP		m_hBitmapDisabled;
	int			m_nImageWidth, m_nImageHeight;

	//	Color Tab
	char		m_bColorTab;				
	COLORREF	m_crColorTab;


	//	State
	BOOL		m_bDefault;
	UINT		m_nOldAction;
	UINT		m_nOldState;

	// Text
	CString m_strText;

// Operations
public:
	//	Positioning
	int		SetImageOffset( int nPixels ); 
	int		SetTextOffset( int nPixels );
	CPoint	SetImagePos( CPoint p );
	CPoint	SetTextPos( CPoint p );

	// Text
	CString SetText( CString s );

	//	Image
	BOOL	SetIcon( UINT nID, int nWidth, int nHeight );
	//BOOL	SetBitmap( UINT nID, int nWidth, int nHeight );
	BOOL	SetMaskedBitmap( UINT nID, int nWidth, int nHeight, COLORREF crTransparentMask );
	BOOL	HasImage() { return (BOOL)( m_hIcon != 0  | m_hBitmap != 0 ); }

	//	Color Tab
	void	SetColorTab(COLORREF crTab);

	//	State
	BOOL	SetDefaultButton( BOOL bState = TRUE );

private:
	BOOL	SetBitmapCommon( UINT nID, int nWidth, int nHeight, COLORREF crTransparentMask, BOOL bUseMask );
	void	CheckPointForCentering( CPoint &p, int nWidth, int nHeight );
	void	Redraw();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonOwnerEx)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CButtonOwnerEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CButtonOwnerEx)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif //__BUTTONOWNEREX_H__
