#include "stdafx.h"
#include "HTTPSocketEx.h"
#include "GameAppView.h" // ????
#include <io.h>
#include <fcntl.h>

CHTTPSocketEx::CHTTPSocketEx()
{
	m_active = FALSE;
}

//CHTTPSocketEx::~CHTTPSocketEx()
//{
//
//}

void CHTTPSocketEx::GetParent(CWnd* pParent)
{
	m_pParent = pParent;
}


void CHTTPSocketEx::OnSessionStart(int nErrorCode)
{	// Start of Session (OnConnect)
	if(nErrorCode)
	{
	// Failed to connect! bad url or something.
	}
	((CGameAppView *)m_pParent)->HTTPSocketSessionStart(nErrorCode, this);
}

void CHTTPSocketEx::OnReceiveHeader(int nErrorCode)
{	// Header has been received, i.e. size of file etc.

}

void CHTTPSocketEx::OnUpdate(int nErrorCode)
{	// More data has been received, or something else i.e. for updating stats.
	//int m_fileRead;
	//int m_fileSize;
	//g_socketManager->UpdateInfo();
}

void CHTTPSocketEx::OnReceiveFile(void *lpBuf, int nBufLen, int nErrorCode)
{	// File has been received entirely - you assume responsibility of the lpBuf, i.e delete after use!
	// However, if you call the parent CHTTPSocket, then that takes responsibility.
}

int FileLength(char *fname)
{
	int len;
	int handle;
	int toread;
	handle = open(fname, O_RDONLY);
	if(handle==-1) return -1;
	len = filelength(handle);
	close(handle);
	if(len==-1) return -1;

	return len;
}

int ReadFilePlain(char *pBuf, int size, char *fname)
{
	FILE *fptr;
	int len;
	int handle;
	int toread;
	//char buffer[80];
	handle = open(fname, O_RDONLY);
	if(handle==-1) return -1;
	len = filelength(handle);
	close(handle);
	if(len==-1) return -1;

	toread = size;
	if(len<size) toread = len;

	fptr = fopen(fname, "rb");
	if(toread>0) fread(pBuf, toread, 1, fptr);
	fclose(fptr);
	return toread;
}

void CHTTPSocketEx::OnSessionEnd(int nErrorCode)
{	// End of OnClose
	
	if(nErrorCode)
	{

		if(nErrorCode&HTTPSE404) // File not Found
		{
		
		}
		//else AfxMessageBox("Error'ed");
		((CGameAppView *)m_pParent)->HTTPSocketSessionFailed(this);
	}
	else 
	{
		char *buf;
		int len;
		// AfxMessageBox(m_saveFname);
		if(m_saveFname)
		{
			len = FileLength(m_saveFname);//"socket.tmp");
		
			// sprintf(tbuf, "len: %d", len);
			// AfxMessageBox(tbuf);
			if(len!=-1)
			{
				buf = new char[len];
				ReadFilePlain(buf, len, m_saveFname);//"socket.tmp");
				unlink(m_saveFname);//"socket.tmp");
			
				((CGameAppView *)m_pParent)->HTTPSocketSessionEnd(buf, len, this);

				delete buf;
			} 
			else
			{
				((CGameAppView *)m_pParent)->HTTPSocketSessionFailed(this);
			}
		}
		else
		{
			if (m_saveBuf)
			{
				((CGameAppView *)m_pParent)->HTTPSocketSessionEnd(m_saveBuf, strlen(m_saveBuf), this);
			}
		}
	}
}

void CHTTPSocketEx::OnDeinit(int nErrorCode)
{	// End of OnDeinit

}


