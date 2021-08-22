// clntsock.h : interface of the CClientSocket class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifndef __CSOCKETEX_H__
#define __CSOCKETEX_H__

#include <afxsock.h>

//#include "Shell.h"
//#include "ShellEx.h"


typedef struct {
BYTE size;
} SocketHeader;

typedef struct {
WORD size;
WORD crc;
} SocketHeader2;


#define HEADER_SIZE		sizeof(SocketHeader)		//1	// now 4, was 1
#define HEADER_SIZE2	sizeof(SocketHeader2)		//1	// now 4, was 1

#define MODE_HEADER		0
#define MODE_DATA		1
#define MODE_HEADER2	2
#define SOCKETCRC_START	0x00000000

#define	SOCKET_HANDSHAKE		1
#define SOCKET_READY			2

class CComm;
class CShellEx;
class CQueueData;

// ASYNC //
extern char *OnConnectErrorString(int nErrorCode);

class CAsyncSocketEx : public CAsyncSocket
{
	DECLARE_DYNAMIC(CAsyncSocketEx);
private:
WORD m_crcSend;
WORD m_crcReceive;
// Construction
public:
	CAsyncSocketEx();
	CAsyncSocketEx(CShellEx *pShellEx);
	~CAsyncSocketEx();

// Attributes
public:
CComm *m_pComm;
CShellEx *m_pShellEx;
int m_dataSent;
int m_dataReceived;
int m_type; // used for recogizing which socket it was u used to attempt to connect with.

int m_mode;
int m_size;
int m_offset;
char *m_buf;
DWORD m_state;
SocketHeader m_header;
SocketHeader2 m_header2;
BOOL m_crcOn;
WORD m_crc;
BOOL m_bBusy;
BOOL m_crcEnable;
BOOL m_exitout;
BOOL m_status;

CQueueData *m_pNext;	// Send Queue
CQueueData *m_pNextR;	// Receive Queue

// Operations
public:
	int SendToQueue(void* lpBuf, int nBufLen); // Puts a packet on the socket's send queue.
	int ReceiveToQueue(void* lpBuf, int nBufLen); // Puts a packet on the socket's receive queue.
	int PutOnQueue(BOOL send, void* lpBuf, int nBufLen); // Puts packet on a queue.
	int SendFromQueue(int i);//void);	// Takes bits of a packet off the queue, and sends thru the socket
	int ReceiveFromQueue(void); // Takes complete packets off the queue, and gives to the CShell to use.
	BOOL DeleteFromQueue(BOOL send);
	int CheckQueue(void);

	BOOL Pump(void);

	
	void Kick(void);
	char *SocketErrorString(void);
	int Send( const void* lpBuf, int nBufLen, int nFlags = 0 );
	int Receive( void* lpBuf, int nBufLen, int nFlags = 0 );

	void TakeOffReceive(void);
// Overridable callbacks
protected:
	virtual void OnAccept(int nErrorCode); // Listening sockets only
	
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
public:
	virtual void OnOpen(void); // Fake virtual, programmer calls this up, when he wants to say that a connection is firmly open.
							 // i.e. Connected, aswell as being attached to CComm object
	virtual void OnConnect( int nErrorCode );

// Implementation
public:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

#endif // __CSOCKETEX_H__
