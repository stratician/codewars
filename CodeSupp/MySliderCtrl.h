#pragma once


// CMySliderCtrl

class CMySliderCtrl : public CSliderCtrl
{
	DECLARE_DYNAMIC(CMySliderCtrl)

public:
	CMySliderCtrl();
	virtual ~CMySliderCtrl();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


