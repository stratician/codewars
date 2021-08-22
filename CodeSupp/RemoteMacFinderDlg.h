// RemoteMacFinderDlg.h : header file
//

#if !defined(AFX_REMOTEMACFINDERDLG_H__779B1F02_F08F_4862_80BF_D7B92C333530__INCLUDED_)
#define AFX_REMOTEMACFINDERDLG_H__779B1F02_F08F_4862_80BF_D7B92C333530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAXLEN_IFDESCR 256
#define MAXLEN_PHYSADDR 8
/////////////////////////////////////////////////////////////////////////////
// CRemoteMacFinderDlg dialog

class CRemoteMacFinderDlg : public CDialog
{
// Construction
public:
	CString FindRemoteMAC(CString IP);
	CRemoteMacFinderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRemoteMacFinderDlg)
	enum { IDD = IDD_REMOTEMACFINDER_DIALOG };
	CIPAddressCtrl	m_ipAddress;
	CString	m_hostName;
	CString	m_foundHost;
	CString	m_foundIP;
	CString	m_foundMac;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoteMacFinderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRemoteMacFinderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnFind();
	afx_msg void OnByName();
	afx_msg void OnByIp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL ValidateInput();
//----------------------------------------------
	typedef struct _MIB_IPNETROW
	{
		DWORD        dwIndex;
		DWORD        dwPhysAddrLen;
		BYTE        bPhysAddr[MAXLEN_PHYSADDR];
		DWORD        dwAddr;
		DWORD        dwType;
	} MIB_IPNETROW, *PMIB_IPNETROW;

	#define    MIB_IPNET_TYPE_OTHER        1
	#define    MIB_IPNET_TYPE_INVALID        2
	#define    MIB_IPNET_TYPE_DYNAMIC        3
	#define    MIB_IPNET_TYPE_STATIC        4

	typedef struct _MIB_IPNETTABLE
	{
		DWORD             dwNumEntries;
		MIB_IPNETROW      table[1];
	} MIB_IPNETTABLE, *PMIB_IPNETTABLE;


	//Try to access the function inside the dll
	typedef DWORD (WINAPI *lpfnGetIpNetTable)(
			PMIB_IPNETTABLE pIpNetTable,
			PULONG pdwSize,
			BOOL bOrder
		);

	lpfnGetIpNetTable m_pGetIpNetTable;
	HMODULE hIpHlpApi;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTEMACFINDERDLG_H__779B1F02_F08F_4862_80BF_D7B92C333530__INCLUDED_)
