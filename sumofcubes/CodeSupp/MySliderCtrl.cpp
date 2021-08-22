// CodeSupp\MySliderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MySliderCtrl.h"
#include "GameAppView.h"


// CMySliderCtrl

IMPLEMENT_DYNAMIC(CMySliderCtrl, CSliderCtrl)

CMySliderCtrl::CMySliderCtrl()
{

	EnableAutomation();
}

CMySliderCtrl::~CMySliderCtrl()
{
}

void CMySliderCtrl::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CSliderCtrl::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMySliderCtrl, CSliderCtrl)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMySliderCtrl, CSliderCtrl)
END_DISPATCH_MAP()

// Note: we add support for IID_IMySliderCtrl to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {A88DD919-31A2-4FA1-84A9-ED2188AFA9A6}
static const IID IID_IMySliderCtrl =
{ 0xA88DD919, 0x31A2, 0x4FA1, { 0x84, 0xA9, 0xED, 0x21, 0x88, 0xAF, 0xA9, 0xA6 } };

BEGIN_INTERFACE_MAP(CMySliderCtrl, CSliderCtrl)
	INTERFACE_PART(CMySliderCtrl, IID_IMySliderCtrl, Dispatch)
END_INTERFACE_MAP()


// CMySliderCtrl message handlers




BOOL CMySliderCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	
	// We want to disable MouseWheel for our raisecueslider.. as this is annoying when we're adjusting the power and it affects raised cue.. can cause you to miss.
	if (g_gameAppView)
	{
		g_gameAppView->MouseWheel(zDelta);
	}
	return true;

	// return CSliderCtrl::OnMouseWheel(nFlags, zDelta, pt);
}


void CMySliderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CSliderCtrl::OnMouseMove(nFlags, point);
}
