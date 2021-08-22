#include "stdafx.h"
#include "HTTPSocket.h"
#include "Helpers.h"
#include "MainFrm.h"

#include "HTTPSocketEx.h"

char *CHTTPSocket::OnConnectErrorString(int nErrorCode)
{
	switch(nErrorCode)
		{
	case WSAEADDRINUSE:return "WSAEADDRINUSE";
	case WSAEADDRNOTAVAIL:return "WSAEADDRNOTAVAIL";
	case WSAEAFNOSUPPORT:return "WSAEAFNOSUPPORT";
	case WSAECONNREFUSED:return "WSAECONNREFUSED";
	case WSAEDESTADDRREQ:return "WSAEDESTADDRREQ";
	case WSAEFAULT:return "WSAEFAULT";
	case WSAEINVAL:return "WSAEINVAL";
	case WSAEISCONN:return "WSAEISCONN";
	case WSAEMFILE:return "WSAEMFILE";
	case WSAENETUNREACH:return "WSAENETUNREACH";
	case WSAENOBUFS:return "WSAENOBUFS";
	case WSAENOTCONN:return "WSAENOTCONN";
	case WSAENOTSOCK:return "WSAENOTSOCK";
	case WSAETIMEDOUT:return "WSAETIMEDOUT";
// --------------- //
	case WSANOTINITIALISED:return "WSANOTINITIALISED";
	case WSAENETDOWN:return "WSAENETDOWN";
	case WSAEINPROGRESS:return "WSAEINPROGRESS";
	case WSAEPROTONOSUPPORT:return "WSAEPROTONOSUPPORT";
	case WSAEPROTOTYPE:return "WSAEPROTOTYPE";
	case WSAESOCKTNOSUPPORT:return "WSAESOCKTNOSUPPORT";
// --------------- //
	case WSAEACCES:return "WSAEACCES";
	case WSAEOPNOTSUPP:return "WSAEOPNOTSUPP";
	case WSAESHUTDOWN:return "WSAESHUTDOWN";
	case WSAEWOULDBLOCK:return "WSAEWOULDBLOCK";
	case WSAEMSGSIZE:return "WSAEMSGSIZE";
	case WSAECONNABORTED:return "WSAECONNABORTED";
	case WSAECONNRESET:return "WSAECONNRESET";
		}
	return "Undefined OnConnect Error";
}

CHTTPSocket::CHTTPSocket()
{
	CAsyncSocket();
}

CHTTPSocket::~CHTTPSocket()
{
	//~CAsyncSocket();
}

void CHTTPSocket::Init(char *base, int port)
{
	
	m_connected = FALSE;
	m_dataSent = 0;
	m_dataReceived = 0;
	m_dataPacket = NULL;

	m_base = NULL;
	m_saveFname = NULL;
	m_saveBuf = NULL;
	m_savePtr = NULL;
	
	m_base = new char[strlen(base)+1];
	strcpy(m_base, base);
	m_port = port;
	m_errorFlag = 0;
	m_fileRead=0;
	m_fileSize=-1;

	// Setup Receive Socket Stream ////////////////////////////////////////////////////////////////////////////////////

	m_sockStream = new CSockStream;
	s_headerSet = new CHTTPHeaderSet;
	ResetDigester();
}

void CHTTPSocket::DeInit()
{
	if(m_base) delete m_base;
	if(m_saveFname) delete m_saveFname;
	if(m_saveBuf) delete m_saveBuf;

	if(m_sockStream) delete m_sockStream;
	if(s_headerSet) delete s_headerSet;

	if(m_dataPacket) delete m_dataPacket;	


	m_base = NULL;
	m_saveFname = NULL;
	m_saveBuf = NULL;

	m_sockStream = NULL;
	s_headerSet = NULL;
	
	m_dataPacket = NULL;

	OnDeinit(0);
}

void webalizestring(char *buf, char *str)
{
	char *ptr;
	int n;
	ptr = buf;
	for(n=0;n<2048;n++)
	if(!str[n]) break;
	else
	{
	if(str[n]==' ')
		{
		int c;
		int a,b;
		c = (unsigned char) str[n];
		a = c>>4;
		b = c&0xf;
		if(a<10) a+='0';
		else a+='a'-10;
		if(b<10) b+='0';
		else b+='a'-10;

		*ptr++ = '%';
		*ptr++ = a;
		*ptr++ = b;
		}
	else *ptr++ = str[n];
	}
	*ptr = 0;
}

void CHTTPSocket::MakeRequestPacket(char *fname, char *base, char *ref)
{
	m_dataPacket = new char[2048];
	char *ptr;

	ptr = m_dataPacket;

	sprintf(ptr,"GET /");//%s HTTP/1.1\r\n",fname);
	ptr+=strlen(ptr);
	webalizestring(ptr, fname);
	ptr+=strlen(ptr);
	sprintf(ptr," HTTP/1.1\r\n");
	ptr+=strlen(ptr);
	
	//sprintf(ptr,"GET /%s HTTP/1.1\r\n",fname);
	//ptr+=strlen(ptr);
	sprintf(ptr,"Accept: */*\r\n");
	ptr+=strlen(ptr);
	sprintf(ptr,"Referer: %s\r\n",ref);
	ptr+=strlen(ptr);
	//Accept-Language: en-us
	//Accept-Encoding: gzip, deflate
	sprintf(ptr,"User-Agent: %s\r\n", "MemirHTTP/1.0");
	ptr+=strlen(ptr);
	sprintf(ptr,"Host: %s\r\n", base);
	ptr+=strlen(ptr);
	sprintf(ptr,"Connection: Keep-Alive\r\n");
	ptr+=strlen(ptr);
	sprintf(ptr,"\r\n");
	ptr+=strlen(ptr);
	m_dataPacket_n = ptr-m_dataPacket;
	m_dataPacket_offset = 0;
	

	// Copy the data packet into a new datapacket buffer that is the perfect size! //
	ptr = m_dataPacket;
	m_dataPacket = new char[m_dataPacket_n+1];
	memcpy(m_dataPacket, ptr, m_dataPacket_n+1);

}




BOOL CHTTPSocket::Request(char *fname, char *ref, char *saveFname)
{
	if(saveFname)
	{
		m_saveFname = new char[strlen(saveFname)+1];
		strcpy(m_saveFname, saveFname);
		m_mode = CHTTPS_FILE;
	}
	else
	{
		m_mode = CHTTPS_MEMORY;
	}

	// Prepare Request ////////////////////////////////////////////////////////////////////////////////////////////////
	MakeRequestPacket(fname, m_base, ref);


	// This line needed to be added to stop the Assertion being false in Debug Mode //
	// Seems to have no negative effect on the socket communictaions //
	m_hSocket = INVALID_SOCKET;  //BAP_FIX //





	// Initiate Connection (ASYNC) ////////////////////////////////////////////////////////////////////////////////////
	if(!Create())
		{
		return FALSE;
		}


	if(!Connect(m_base, m_port))
		{
		if(GetLastError()==WSAEWOULDBLOCK);
		else{
			return FALSE;
			}
		}
	
	return TRUE;
}


void CHTTPSocket::OnConnect(int nErrorCode)
{

	if(nErrorCode) 
	{
		m_connected = FALSE;
		#ifndef MAIN_SERVER
			AfxMessageBox("Connection Failed. A firewall may be blocking iSnooker");
		#endif
		// AfxMessageBox(OnConnectErrorString(nErrorCode));
	}
	else m_connected = TRUE;
	OnSessionStart(nErrorCode);	
	if(nErrorCode) DeInit();
}

void CHTTPSocket::OnClose(int nErrorCode)
{
	m_connected = FALSE;
	OnSessionEnd(m_errorFlag);
}

void CHTTPSocket::OnReceive(int nErrorCode)
{
	if(m_connected)
	{
	while(Pump()); // Get all of the shit off the queue!
	} //
}

BOOL CHTTPSocket::Pump(void)
{
	int err;
	if(!m_connected) return FALSE;

	err = Receive(m_tmpbuf, CHTTPS_RECV_BUFFER);
	if(err==SOCKET_ERROR)
		{
		err = GetLastError();
		if(err==WSAEWOULDBLOCK);
		else {
			}
		return FALSE; // nothing else on the queue
		}

	if(!err) return FALSE; // nothing else on the queue.

	m_dataReceived+=err;

	OnUpdate(0);

	DigestRequest(m_tmpbuf, err); // gets request/headers/data
	
	return m_connected; // usually TRUE, until the end of the session!
}

void CHTTPSocket::OnSend(int nErrorCode)
{
	if(m_connected)
	{
		if(m_dataPacket)
		{
			int err;
			err = Send(m_dataPacket,m_dataPacket_n-m_dataPacket_offset);
			if(err!=SOCKET_ERROR)
			{
			m_dataSent+=err; // Log Data sent.

			m_dataPacket_offset+=err;
			if(m_dataPacket_offset==m_dataPacket_n)
				{
				delete m_dataPacket;
				m_dataPacket = NULL;		
				}

			OnUpdate(0);
			}
			else { // On Error, delete packet, end of transmission!
			delete m_dataPacket;
			m_dataPacket = NULL;
			}
		} // end if m_dataPacket
	}
}

void CHTTPSocket::ResetDigester(void)
{
	s_firstWrite = TRUE;
	s_headers = 0;
	s_state = 0;
	s_tmpstr[0] = 0;
	s_tmpstr_n = 0;
	s_datamode = FALSE;
	s_headerSet->ClearHeaders();
}

void CHTTPSocket::DigestRequest(char *pBuf, int siz)
{
	char c;
	if(!siz) return;
	if(s_datamode == TRUE)
	{
	DigestAllData(pBuf, siz); // Digest the rest of the stream as data.
	return;
	}

	m_sockStream->SetSource(pBuf, siz);
	
	while(m_sockStream->Parse(&c))
	{	
	// ---------- //
	if(s_state==0) // get string
	{
	s_tmpstr[s_tmpstr_n] = c;
	if(c==0x0d || c==0x0a) 
	{
	s_tmpstr[s_tmpstr_n] = 0;
	if(!s_tmpstr_n) // Blank Line (end of header)
		{
		
		if(c==0x0d)
			{
			s_state = 3; // Got nothing!
			}
		else {
		s_state = 2;
		}

		}
	else {
	// Add HEADER //
	s_headerSet->AddHeader(s_tmpstr);
	s_headers++;
	if(c==0x0d)
	{
	s_state = 1;
	} else {
	s_state = 0;
	s_tmpstr_n = 0;
	}

		}
	} else s_tmpstr_n++;
	} 
	else if(s_state==1) // get 0x0a. (for header)
	{
	s_state = 0;
	s_tmpstr_n = 0;
	}
	else if(s_state==2) // 
	{
		// Get that asshole byte back!!!
		m_sockStream->m_siz++;
		m_sockStream->m_pPtr--;

	s_state = 0;
	s_tmpstr_n = 0;
	s_datamode = TRUE;
	HandleRequest();
	if(m_errorFlag) return; 
	
	DigestAllData(m_sockStream->m_pPtr, m_sockStream->m_siz); // Digest the rest of the stream as data.
	// at this point all objects are deleted, so this is a crash fix!
	if(m_sockStream) m_sockStream->m_pPtr = m_sockStream->m_pEnd;	
	break;
	} else if(s_state==3) // get 0x0a
	{
	s_state = 2;
	}


	// ---------- //
	}
}

void CHTTPSocket::DigestAllData(char *pBuf, int siz)
{
	if(s_headerSet->m_chunked) 
	{
	DigestChunk(pBuf, siz);
	return;
	}
	DigestData(pBuf, siz);
	if(s_dataOffset==s_dataSize)
	{
	if(m_mode==CHTTPS_FILE) OnReceiveFile(NULL, s_dataSize, 0);
	else OnReceiveFile(m_saveBuf, s_dataSize, 0);
	Close();
	OnClose(0);
	DeInit();
	}
}

void CHTTPSocket::DigestData(char *pBuf, int siz)
{
	FILE *fptr;
	/*if(s_dataSize==-1) // Couldn't determine the size!
	{
	m_errorFlag|=HTTPSEGENERAL;
	Close();
	OnClose(0);
	DeInit();
	return; // we need to know a size for the memory version to work at all!!
	}*/
	m_fileRead+=siz;
	if(m_mode==CHTTPS_FILE)
	{		
		if(s_firstWrite) fptr = fopen(m_saveFname,"wb"), s_firstWrite=FALSE;
		else fptr = fopen(m_saveFname,"a+b");

		if(fptr != NULL)
		{
			fwrite(pBuf, siz, 1, fptr);
			fclose(fptr);
		}
	}
	else if(m_mode==CHTTPS_MEMORY)
	{		
		if(s_firstWrite)
		{
		s_firstWrite=FALSE;
		m_saveBuf = new char[s_dataSize];
		m_savePtr = m_saveBuf;
		}
		memcpy(m_savePtr, pBuf, siz);
		m_savePtr+=siz;
	}
	s_dataOffset+=siz;

	if(s_dataSize==-1) return; // don't bother having a size limit if we don't know the size yet!
}

void CHTTPSocket::DigestChunk(char *pBuf, int siz)
{
	char c;
	int len;
	int left;
	m_sockStream->SetSource(pBuf, siz);
		
	while(m_sockStream->Parse(&c))
	{
	// ---------- //
	if(s_state==0) // get string
	{
	s_tmpstr[s_tmpstr_n] = c;
	if(c==0x0d) 
	{
	s_tmpstr[s_tmpstr_n] = 0;
	if(!s_tmpstr_n) // Blank Line (end of data chunk)
		{

		}
	else {
		// GetChunk Size //
		sscanf(s_tmpstr,"%lx",&s_dataSize);
		s_dataOffset = 0;

		s_state = 1;
		}
	} else s_tmpstr_n++;
	} 
	else if(s_state==1) // get 0x0a. (for header)
	{
	s_state = 2;
	s_tmpstr_n = 0;
	}
	else if(s_state==2) // get 0x0a (for nothing)
	{
		// Get that asshole byte back!!!
		m_sockStream->m_siz++;
		m_sockStream->m_pPtr--;

		if(s_dataSize)
		{
			len = m_sockStream->m_siz;	// bytes in this received packet.
			left = s_dataSize-s_dataOffset; // bytes left to makeup file.

			if(len>left) len = left; // if received packet is larger than bytes left, then only do bytes left!

			DigestData(m_sockStream->m_pPtr, len); // Digest the rest of the stream as data.

			m_sockStream->m_siz-=len;
			m_sockStream->m_pPtr+=len;
		}

		if(s_dataOffset==s_dataSize) // Finished streaming all data for this chunk. (not nesc. eof!)
		{
		s_state=3; // get the 0x0d/0x0a for the end of this line next!
		// allow it, assume thats all the data!
		}
	}
	else if(s_state==3) // get 0x0d (for the end of the line)
	{
	s_state=4;
	}
	else if(s_state==4) // get 0x0a (for the end of the line too)
	{
	s_state = 0; // get next chunk.
	s_tmpstr_n = 0;

	if(!s_dataSize) // Empty Chunk, therefore it was the last chunk!!!
		{
		if(m_mode==CHTTPS_FILE) OnReceiveFile(NULL, s_dataSize, 0);
		else OnReceiveFile(m_saveBuf, s_dataSize, 0);
		Close();
		OnClose(0);
		DeInit();
		break;
		}
	}

	// ---------- //
	}
}

void CHTTPSocket::HandleRequest(void)
{
	if(s_headerSet->m_contentLength!=-1)
	{
	s_dataSize = s_headerSet->m_contentLength;
	s_dataOffset = 0;
	} else s_dataSize = -1;

	m_fileRead=0;
	m_fileSize=s_dataSize; // -1 size means we honestly don't know!

	if(s_headerSet->m_code==404)
	{
	m_errorFlag = HTTPSE404;
		
	Close();
	OnClose(m_errorFlag);
	DeInit();

	return;
	}

	/*if(s_dataSize==-1) // Couldn't determine the size!
	{
	m_errorFlag|=HTTPSEGENERAL;
	}
	*/
	OnReceiveHeader(m_errorFlag);
}

void CHTTPSocket::AdoptMemory(void)
{	// Call this if you're gonna handle destruction of the m_saveBuf
	m_saveBuf = NULL;
}

void CHTTPSocket::OnSessionStart(int nErrorCode)
{	// Start of Session (OnConnect)

}

void CHTTPSocket::OnReceiveHeader(int nErrorCode)
{	// Header has been received, i.e. size of file etc.
	
}

void CHTTPSocket::OnUpdate(int nErrorCode)
{	// More data has been received, or something else i.e. for updating stats.
	
}

void CHTTPSocket::OnReceiveFile(void *lpBuf, int nBufLen, int nErrorCode)
{	// File has been received entirely - you assume responsibility of the lpBuf, i.e delete after use!
	// However, if you call the parent CHTTPSocket, then that takes responsibility.

}

void CHTTPSocket::OnSessionEnd(int nErrorCode)
{	// End of OnClose

}

void CHTTPSocket::OnDeinit(int nErrorCode)
{	// End of OnDeinit

}

void CHTTPSocket::Kick(int nErrorCode)
{	// End of OnDeinit
	Close();
	OnClose(nErrorCode);
}

