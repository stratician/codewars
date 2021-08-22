// MacFinderDlg.h : header file
//

#if !defined(AFX_MACFINDERDLG_H__E46973DD_3780_42CD_AB98_E9875C291365__INCLUDED_)
#define AFX_MACFINDERDLG_H__E46973DD_3780_42CD_AB98_E9875C291365__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMacFinderDlg dialog
//------------------------------------------------------
#define MAX_ADAPTER_DESCRIPTION_LENGTH  128 // arb.
#define MAX_ADAPTER_NAME_LENGTH         256 // arb.
#define MAX_ADAPTER_ADDRESS_LENGTH      8   // arb.
/*
#define DEFAULT_MINIMUM_ENTITIES        32  // arb.
#define MAX_HOSTNAME_LEN                128 // arb.
#define MAX_DOMAIN_NAME_LEN             128 // arb.
#define MAX_SCOPE_ID_LEN                256 // arb.*/
//------------------------------------------------------

//-------------------------------------------------
//Extracted from MSDN.
//-------------------------------------------------

//These declarations must be originally in Iptypes.h
//MSDN says that we must have Win XP to use them.
//I tested this on Win 98, Me, XP(sp1), 2000: It Works Fine.

	typedef struct {
		char String[4 * 4];
	} IP_ADDRESS_STRING2, *PIP_ADDRESS_STRING2, IP_MASK_STRING2, *PIP_MASK_STRING2;

	typedef struct _IP_ADDR_STRING2 {
		struct _IP_ADDR_STRING2* Next;
		IP_ADDRESS_STRING2 IpAddress;
		IP_MASK_STRING2 IpMask;
		DWORD Context;
	} IP_ADDR_STRING2, *PIP_ADDR_STRING2;

	typedef struct _IP_ADAPTER_INFO2 {
		struct _IP_ADAPTER_INFO2* Next;
		DWORD ComboIndex;
		char AdapterName[MAX_ADAPTER_NAME_LENGTH + 4];
		char Description[MAX_ADAPTER_DESCRIPTION_LENGTH + 4];
		UINT AddressLength;
		BYTE Address[MAX_ADAPTER_ADDRESS_LENGTH];
		DWORD Index;
		UINT Type;
		UINT DhcpEnabled;
		PIP_ADDR_STRING2 CurrentIpAddress;
		IP_ADDR_STRING2 IpAddressList;
		IP_ADDR_STRING2 GatewayList;
		IP_ADDR_STRING2 DhcpServer;
		BOOL HaveWins;
		IP_ADDR_STRING2 PrimaryWinsServer;
		IP_ADDR_STRING2 SecondaryWinsServer;
		time_t LeaseObtained;
		time_t LeaseExpires;
	} IP_ADAPTER_INFO2, *PIP_ADAPTER_INFO2;
//-------------------------------------------------
	//Try to access the function inside the dll
	typedef DWORD (WINAPI *lpfnGetAdaptersInfo)(
			PIP_ADAPTER_INFO pAdapterInfo,
			PULONG pOutBufLen
		);

	lpfnGetAdaptersInfo m_pGetAdaptersInfo;
	HMODULE hIpHlpApi;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACFINDERDLG_H__E46973DD_3780_42CD_AB98_E9875C291365__INCLUDED_)
