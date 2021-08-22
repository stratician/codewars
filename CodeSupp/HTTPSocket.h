#ifndef __HTTPSOCKET_H__
#define __HTTPSOCKET_H__

#include <afxsock.h>

// HTTPSocket.h : header file
//

// Error Codes //
#define HTTPSEGENERAL		1
#define HTTPSE404			2

#define CHTTPS_FILE			0
#define CHTTPS_MEMORY		1

#define CHTTPS_RECV_BUFFER		2048//256 //2048

class CSockStream;
class CHTTPHeaderSet;

class CHTTPSocket : public CAsyncSocket {
private:
BOOL m_connected;
char m_tmpbuf[CHTTPS_RECV_BUFFER];
DWORD m_errorFlag;

char *m_base;
int m_port;

int m_dataSent;	// Logs
int m_dataReceived;

char *m_dataPacket;	// Send Packet.
int m_dataPacket_n;
int m_dataPacket_offset;

int m_mode;

CSockStream *m_sockStream; // Receive Socket Stream.

// State Machine (used for the digester) //
BOOL s_firstWrite;
int s_state;
char s_tmpstr[256];
int s_tmpstr_n;
int s_dataSize;
int s_dataOffset;
BOOL s_datamode;

void ResetDigester(void);
void DigestRequest(char *pBuf, int siz);
void DigestAllData(char *pBuf, int siz);
void DigestData(char *pBuf, int siz);
void DigestChunk(char *pBuf, int siz);
void HandleRequest(void);

char *OnConnectErrorString(int nErrorCode);
void MakeRequestPacket(char *fname = "", char *base = "", char *ref = "");
BOOL Pump(void);
public:
CHTTPSocket();
virtual ~CHTTPSocket();
void Init(char *base, int port = 80);
void DeInit();
BOOL Request(char *fname, char *ref, char *saveFname = NULL);
void AdoptMemory(void);

public:
// State Machine (used for the digester) Public members //
int s_headers;
CHTTPHeaderSet *s_headerSet;
int m_fileRead;
int m_fileSize;
char *m_saveFname;	// For saving to a File.
char *m_saveBuf; // For saving to memory
char *m_savePtr;
void Kick(int nErrorCode = 0);
protected:
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
protected:
	virtual void OnSessionStart(int nErrorCode);
	virtual void OnReceiveHeader(int nErrorCode);
	virtual void OnUpdate(int nErrorCode);
	virtual void OnReceiveFile(void *lpBuf, int nBufLen, int nErrorCode);
	virtual void OnSessionEnd(int nErrorCode);
	virtual void OnDeinit(int nErrorCode);
};

#endif // __HTTPSOCKET_H__
