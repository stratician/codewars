#ifndef __SHELLEX_H__
#define __SHELLEX_H__

#include "Shell.h"

class CShellEx : public CShell {
public:
	CShellEx(BOOL bIsMainServer = TRUE);
	~CShellEx();

	// attributes
	int m_clientSocketIdx;
	
	// Exteneded shell functionality
	CAsyncSocketEx *m_pSocketListen;
	CAsyncSocketEx *m_pSocketListenCheck;
	
	//BOOL DisplayConnection(void);
	//BOOL Connection(BOOL bIsHost, LPCTSTR lpszAddress, int port);
	//BOOL ConnectSocketC(LPCTSTR lpszHandle, LPCTSTR lpszAddress, UINT nPort);
	
	BOOL ConnectionASYNC(BOOL bIsHost, LPCTSTR lpszAddress, int port, int type);
	BOOL ConnectSocketCASYNC(LPCTSTR lpszHandle, LPCTSTR lpszAddress, UINT nPort, int type);

	//CString GetHostIPAddress(int addressListIndex);
	//enum{LOCAL_HOST_IP = 0, INTERNET_HOST_IP =1};

	// main server receive messeags
	void OnMainServerReceive(CComm *pComm, CAsyncSocketEx *pSocket, void *lpBuf, int nBufLen);
	
	// game server receive messages
	BOOL OnGameReceive(CComm *pComm, CAsyncSocketEx *pSocket, void *lpBuf, int nBufLen);
	//void OnGameReceivePlayerDetails(void *lpBuf,int plrSockIdx);
	BOOL OnGameReceivePlayerDetails2(void *lpBuf,int plrSockIdx, int nBufLen);
	//void OnMainReceive(CComm *pComm, CAsyncSocketEx *pSocket, void *lpBuf, int nBufLen);
	void ChatSend(CString& strText);
	void ChatSend(char *str); // Override the oldschool ChatSend (for interception of /code )

	void OnConnect(CAsyncSocketEx *pSocket, int nErrorCode);
};

#endif // __XSHELLEX_H__
