// ListCtrlEx.h : header file
//

#ifndef __LISTCTRLEX_H__
#define __LISTCTRLEX_H__

#include "../Code/Common.h"
#include "HeaderCtrlEx.h"
//#include "ButtonListCtrlEx.h"
#include "../Code/TableButton.h"



/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx window

#define MAX_TABLEITEMS			MAX_LOBBY_TABLES //50//40
#define MAX_TABLEBUTTONS		(MAX_TABLEITEMS * 4)
#define MAX_COLUMNS				10

class CListCtrlEx : public CListCtrl
{	
// Construction
public:
	CListCtrlEx();

// Attributes
public:
	CWnd *m_pParent;
	int m_totalColumns;
	int m_totalItems;
	int m_maxItems;
	int m_selected;
	int m_selectedold;
	CDC* m_pDC;
	CDC m_dcMemory;
	CDC m_dcbb;
	BOOL just_selected;
	char m_firsttime;
	char m_ebkfirst;
	CRect m_selectedrect;
	CRect m_selectedoldrect;
	BOOL m_bAllowColumnSizing;
	CFont m_NewHeaderFont;
	CHeaderCtrlEx m_HeaderCtrl;
	CDC dcMem_buf;
	BOOL m_bBackground;		// If this is True, we will use a Bitmap Background, False uses color (59,19,0)
	unsigned int m_flags;
	enum {GRIDHORZ = 0x01, 
		  GRIDVERT = 0x02, 
		  GRID1GAP = 0x04, 
		  GRID2GAP = 0x08, 
		  GRIDSPLIT = 0x10, 
		  HIGHLIGHTROWDISABLED = 0x20,
		  BUTTONS = 0x40,
		  TABLE = 0x80,
		  TEXTCOLOR = 0x100,
		  LOBBYSELECT = 0x200,
	};
	COLORREF m_crGridColor1st;
	COLORREF m_crGridColor2nd;
	COLORREF m_crTextColor;
	COLORREF m_crTextBkColor;
		
	// child windows
	CTableButton **m_buttonLst;
	int m_totalButtons;
	
	int m_columnHeight;
	
	int m_gridSplitOffsetStartX;
	int m_gridSplitOffsetEndX;
	char *m_splitText[MAX_TABLEITEMS];
	int *m_columnWidth;
	HICON m_TableIcon;
	HICON m_TableDimIcon;
	HICON m_WatchIcon;
	HICON m_PlayIcon;
	int m_lastSelectedNItem;	
	char m_lastSelectedName[22];

	CScrollBar m_scrollBar;
	CRect m_itemRect;

	int m_iSubItem;
	BOOL m_bToggleName;
	BOOL m_bToggleRating;
	BOOL m_bToggleTable;

// Operations
public:
	void PassParent(CWnd *pParent);
	void SetMaxItems(int maxItems);
	int CreateColumn(int n, char *str, int cx);
	int CreateItem(int n, CString *str);
	int CreateItemLabel(int n, CString *str);
	int AddSubItemText(int columnN, int subItemN, CString *str);
	void AddSplitText(int n, char *str);
	int CreateButtons(int totalButtons, BOOL bSplit = 0);
	void DestroyButtons(void);
	int DrawItemButton(CDC *m_pDC, int n, char *str, int nItem, int nSubItem, BOOL bSplit = 0, BOOL bShow = TRUE);
	int HideItemButton(int n);
	int ShowItemButton(int n, BOOL bState = TRUE);
	void EnableItemButton(int n, BOOL bState);
	void AllowColumnSizing(BOOL allowColumnSizing);
	void CreateTableItem(int n, CString *str, char *options);
	void DeleteTableItem(int n);
	void DeleteAllTableItems(void);
	CPoint GetGridBottomRight(void);
	void HideAllButtons(void);


	void SetGridFlags(unsigned int flags);
	void SetGridColor(BOOL n, COLORREF cr);
	void SetGridSplitOffsetX(int startX, int endX);

	virtual void CListCtrlEx::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );

	void TriggerOnMeasureItem(UINT nID);
	void SetColumnHeight(int cy);
	void SetColumnFontHeight(float factor = 1.0f);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEx)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlEx)
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif // __LISTCTRLEX_H__
