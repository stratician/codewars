// SendEdit.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "SendEdit.h"
#include "GameAppView.h"
#include "ImageDataObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern HBITMAP Skin_ButtonSayOn;
extern HBITMAP Skin_ButtonSayOff;
extern HBITMAP Skin_ButtonSmiley;
extern HBITMAP Skin_ButtonSmiley2;






/////////////////////////////////////////////////////////////////////////////
// CSendEdit

CSendEdit::CSendEdit()
{
	m_message[0] = 0;
	m_buttonstate = FALSE;
}

CSendEdit::~CSendEdit()
{

}


BEGIN_MESSAGE_MAP(CSendEdit, CRichEditCtrl)//CEdit)
	//{{AFX_MSG_MAP(CSendEdit)
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_FONTCHANGE()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


int spos;
int	epos;
int offset;
int type;
int adjust;
char *s_gstr;


BOOL FIND_SMILEYS(void)
{
	int n,i,p,q;
	unsigned char found, got_a_smiley;
	


	got_a_smiley = 0;

	// Scan the strTemp for our Smiley List //
	for(n=offset;n<strlen(s_gstr);n++)
	{
		for(p=0;p<MAX_SMILIES;p++)
		{
			
			// Check if our Level is Sufficient
			if(g_gameAppView)
			{
				if(g_gameAppView->m_playerSockIdx != -1 && g_gameAppView->m_playerIdx >= 0) 
				{
					if(p >= m_player[g_gameAppView->m_playerIdx].level) break;
				}
				else
				{
					if(p > 0) break;
				}
			}
			

			// Check for Smiley Code //
			found=0;
			for(q=0;q<strlen(smiley_lst[p]);q++) 
			{	
				if((s_gstr[n+q] == smiley_lst[p][q]) ) found++;
					else if(s_gstr[n+q]>=65 && s_gstr[n+q]<=90) if(((s_gstr[n+q]+32) == smiley_lst[p][q]) ) found++;
			}
			
			if(found==strlen(smiley_lst[p])) 
			{
				got_a_smiley=1;
				break; // Found a Smiley Code so Break out //
			}
		}

		if(got_a_smiley)
		{
			// Found A Smiley //
			found=1;
			spos = n;
			epos = n+strlen(smiley_lst[p]);
			type = p;
			offset = epos;
			break;
		}
	}

	if(got_a_smiley) return TRUE;
		else return FALSE;
}





void CSendEdit::TextAdjust(void)
{
	CString strText;
	CString tText;
	int i;


	CHARFORMAT cf;
	GetSelectionCharFormat(cf);
	cf.dwMask = CFM_ITALIC | CFM_UNDERLINE | CFM_COLOR | CFM_SIZE;
	cf.dwEffects = 0;//CFE_BOLD;
	
	/*
	if(g_gfxDetails.bDefaultColorScheme == TRUE) cf.crTextColor = RGB(240,240,240);
		else cf.crTextColor = RGB(20,20,220);
	*/
	cf.crTextColor = g_gameAppView->m_TextColorChangerDlg.m_textColor;


	cf.yHeight = 215;

/*	PARAFORMAT pf;
	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_ALIGNMENT;
	pf.wAlignment = PFA_LEFT;
	SetParaFormat(pf);
*/
	SetSelectionCharFormat(cf);
	
	GetWindowText(strText);

	//////////////////////
#ifdef SMILEYS_ENABLED

	IRichEditOle	*m_pRichEditOle_SE;
	m_pRichEditOle_SE = GetIRichEditOle();
	if(m_pRichEditOle_SE != NULL)
	{	
		s_gstr = new char[strlen(strText)+1];
		strcpy(s_gstr, strText);

		offset = 0;
		adjust = 0;
		
		char buf[80];

		//for(i=0;i<64;i++)
		if(FIND_SMILEYS())
		{
			SetSel(spos-adjust, epos-adjust);
			
			if(CImageDataObject::InsertBitmap(m_pRichEditOle_SE, smiley[type], type))
			{
				adjust+=((epos-spos)-1);
			}
			else
			{
				int nBegin = GetTextLength();
				SetSel(nBegin, nBegin);
			}
		}

		if(s_gstr) delete s_gstr;
	}
	m_pRichEditOle_SE->Release();
#endif
  


	//if(m_pRichEditOle_SE) delete m_pRichEditOle_SE;
	/*
	// Add in a dummy (thin 1 pixel width black bmp) to center the test to the smileys height. 
	GetWindowText(strText);
	if(!strlen(strText))
		{
		m_pRichEditOle = GetIRichEditOle();
		ASSERT(m_pRichEditOle != NULL);
		// Only do if the m_pRichEditOle is Valid //
		if(m_pRichEditOle != NULL) CImageDataObject::InsertBitmap(m_pRichEditOle, smiley_empty);
		}
	*/
}



BOOL CSendEdit::PreTranslateMessage(MSG *pMsg)
{
	return CRichEditCtrl::PreTranslateMessage(pMsg);
}


void CSendEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	if(!g_gameAppView) return;

	// TODO: Add your message handler code here and/or call default

	CString strText;
	
	// 0x0d = Return (Normal/Shift)
	// 0x0a = Return (Ctrl)

	

	if (nChar == 32)
	{
		// Check if we have a recent PM from someone ...
		if(g_RecentMessengerName != NULL)
		{
			// Check for /r (replace by /msg <username> if we had a previous PM from someone
			GetWindowText(strText);
			if(strText.GetLength())
			{
				if(strcmpi(strText, "/r") == 0)
				{
					SetSel(0, strText.GetLength());
					char tbuf[80];
					sprintf(tbuf, "/msg %s", g_RecentMessengerName);
					ReplaceSel(tbuf);
				}
			}
		}
		
		/*
		TotalTextLength = g_gameAppView->m_sendEdit.GetWindowTextLength();
		if(iTotalTextLength>110) break;
		SetSel(0, TotalTextLength);
		ReplaceSel(smiley_lst[data_idx[n]]);
		*/
	}


	if ((nChar != VK_RETURN && nChar != 0x0a) || (nRepCnt!=1))
	{
		//////////////////////
		
		CHARFORMAT cf;
		GetSelectionCharFormat(cf);
		cf.dwMask = CFM_ITALIC | CFM_UNDERLINE | CFM_COLOR | CFM_SIZE;
		cf.dwEffects = 0;//CFE_BOLD;
		
		/*
		if(g_gfxDetails.bDefaultColorScheme == TRUE) cf.crTextColor = RGB(240,240,240);
			else cf.crTextColor = RGB(20,20,220);
		*/

		cf.crTextColor = g_gameAppView->m_TextColorChangerDlg.m_textColor;

		cf.yHeight = 215;
/*
		PARAFORMAT pf;
		pf.cbSize = sizeof(PARAFORMAT);
		pf.dwMask = PFM_ALIGNMENT;
		pf.wAlignment = PFA_LEFT;
		SetParaFormat(pf);
*/
		SetSelectionCharFormat(cf);
		//////////////////////


		
		CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags); // was CEdit::
		
		// Replace any Smiley Codes by their Image //
		TextAdjust();
													   
		return;
	}
	
	else
	{
		GetWindowText(strText);
		// check if we have any text to send?
		if( !strText.GetLength() ) return;
		
		
		
		// Get a List of Smileys 
		IRichEditOle	*m_pRichEditOle_SE;
		m_pRichEditOle_SE = GetIRichEditOle();

		int n;
		int total = 0;
		int data_pos[128], data_idx[128];  // Maximum of 128 Objects //

		for(n=0;n<128;n++) 
			{
			data_pos[n] = 0;
			data_idx[n] = 0;
			}
		if (m_pRichEditOle_SE)
			{
			HRESULT hr = 0;

			//
			// Start by getting the total number of objects in the control.
			//
			int objectCount = m_pRichEditOle_SE->GetObjectCount();


			//
			// Loop through each object in the control and if active
			// deactivate, and if open, close.
			//
			for (int i = 0; i < objectCount; i++)
				{
				REOBJECT reObj;
				 ZeroMemory(&reObj, sizeof(REOBJECT));
				reObj.cbStruct  = sizeof(REOBJECT);

				//
				// Get the Nth object
				//
				hr = m_pRichEditOle_SE->GetObject(i,&reObj,REO_GETOBJ_POLEOBJ);
				if(SUCCEEDED(hr))
					{
					// Insert the Smiley Short Cut Code, back into string, so it's dispalyed in main Chat //
					data_pos[i] = reObj.cp;
					data_idx[i] = reObj.dwUser;
					total++;
					//
					// If active, deactivate.
					//
					if (reObj.dwFlags && REO_INPLACEACTIVE)
					  m_pRichEditOle_SE->InPlaceDeactivate();

					//
					// If the object is open, close it.
					//
					if(reObj.dwFlags&&REO_OPEN)
					  hr = reObj.poleobj->Close(OLECLOSE_NOSAVE);

					  reObj.poleobj->Release();
					}
				}
			m_pRichEditOle_SE->Release();
			//if(m_pRichEditOle_SE) delete m_pRichEditOle_SE;
			}

		
		// Replace the Smiley Objects, by their ASCII code, so they are processed correctly and then
		char tbuf[256];
		int iTotalTextLength;
		for(n=(total-1);n>=0;n--)
			{
			iTotalTextLength = g_gameAppView->m_sendEdit.GetWindowTextLength();
			if(iTotalTextLength>110) break;
			SetSel(data_pos[n], data_pos[n]+1);
			ReplaceSel(smiley_lst[data_idx[n]]);
			}
		
		GetWindowText(strText);
		// check if we have any text to send?
		if( !strText.GetLength() ) return;

		strcpy(m_message, strText);
		
			
		CWnd *wndParent = GetParent(); // CGameAppView is the the parent wnd of this child wnd
		
		if(!((CGameAppView *)wndParent)->OptionNotForGuest(((CGameAppView *)wndParent)->m_playerSockIdx, 666))
			((CGameAppView *)wndParent)->MessageSend(m_message);
		
		strText=_T("");
		SetWindowText(strText);
		SetCursor(((CGameAppView *)wndParent)->m_hCursor);

		/*
		// Add in a dummy (thin 1 pixel width black bmp) to center the test to the smileys height. 
		m_pRichEditOle = GetIRichEditOle();
		ASSERT(m_pRichEditOle != NULL);
		// Only do if the m_pRichEditOle is Valid //
		if(m_pRichEditOle != NULL) CImageDataObject::InsertBitmap(m_pRichEditOle, smiley_empty);
		*/	
	
	}
}


void CSendEdit::UpdateTextColor(void)
{
	// Make sure the entire text selection has the color of our TextColorChanger 
	SetSel(0, -1);
	CHARFORMAT cf;
	GetSelectionCharFormat(cf);
	cf.dwMask = CFM_ITALIC | CFM_UNDERLINE | CFM_COLOR | CFM_SIZE;
	cf.dwEffects = 0;
	cf.crTextColor = g_gameAppView->m_TextColorChangerDlg.m_textColor;
	cf.yHeight = 215;
	SetSelectionCharFormat(cf);
	SetSel(GetTextLength(), GetTextLength());

	SetFocus();
}



void CSendEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CRichEditCtrl::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	
	// save this window as current previous focus
	//g_previousWndEditFocus = (CWnd *)this;
	//ErrorMessage(GetParent(), "g_previousWndEditFocus = m_sendEdit");
}

void CSendEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CRichEditCtrl::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	g_previousWndNo = 0;

}









void CSendEdit::OnFontChange() 
{
	CRichEditCtrl::OnFontChange();
	
	// TODO: Add your message handler code here


}

BOOL CSendEdit::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	

	return CRichEditCtrl::OnNotify(wParam, lParam, pResult);
}

void CSendEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	



	CString strText;
	// If no text in the string, set the button to inactive
	GetWindowText(strText);

	CRichEditCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CSendEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CString strText;
	// If no text in the string, set the button to inactive
	GetWindowText(strText);


	CRichEditCtrl::OnLButtonDown(nFlags, point);
}

void CSendEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	

// TODO: Add your message handler code here and/or call default
	CString strText;
	// If no text in the string, set the button to inactive
	GetWindowText(strText);

	if(nChar == VK_RETURN) return; // Prevents the annoying windows Vista SFX 
 
	
	CRichEditCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSendEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CString strText;
	// If no text in the string, set the button to inactive
	GetWindowText(strText);


	
	CRichEditCtrl::OnLButtonUp(nFlags, point);
}



/*
// We must check if the Power of the Shot needs to be updated, as this Message can be called when the shot is Set
BOOL CSendEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
#ifndef MAIN_SERVER

	if(!g_gameAppView) return 0;

	// If we're in Camear Mode, do appropriate code //
	if(g_pCameraMode)
	{
		// Get Unit Vector in direction we're looking at //

		g_pCameraFree->_distancev -= zDelta * 0.1f;
		g_gameAppView->m_bUpdateTable = true;
	}


	// If we have set our shot
	if(g_gameAppView->m_snooker->m_cueBallSelect != 5) return 0;

	float percent = g_gameAppView->m_snooker->m_cuePowerPercentage;
	if(zDelta>0)
		{
		if(percent < 100) percent+=1;
		}
	else{
		if(percent > 0) percent-=1;
		}
	g_gameAppView->UpdatePower((float)(percent/100));

#endif
	return 0; //CView::OnMouseWheel(nFlags, zDelta, pt);

}
*/