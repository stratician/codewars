#ifndef __HTTPSOCKETEX_H__
#define __HTTPSOCKETEX_H__

#include "HTTPSocket.h"

// HTTPSocketEx.h : header file
//

class CURL;

class CHTTPSocketEx : public CHTTPSocket {
public:
int m_status;
int m_idx;
CURL *m_pCurl;
BOOL saveFlag;
CWnd* m_pParent;
int m_tag;
BOOL m_active;
public:
	CHTTPSocketEx();
	//~CHTTPSocketEx();
	void GetParent(CWnd* pParent);
protected:
	void OnSessionStart(int nErrorCode);
	void OnReceiveHeader(int nErrorCode);
	void OnUpdate(int nErrorCode);
	void OnReceiveFile(void *lpBuf, int nBufLen, int nErrorCode);
	void OnSessionEnd(int nErrorCode);
	void OnDeinit(int nErrorCode);	
};

#endif // __HTTPSOCKETEX_H__