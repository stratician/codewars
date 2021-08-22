// clntsock.cpp : implementation of the CClientSocket class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "SocketEx.h"
#include "Shell.h"
#include "ShellEx.h"

//#define SOCKET_DEBUG

#include <time.h>
#include <stdlib.h>

#include "Build.h"
#include "stdafx.h"
#include "GameApp.h"
#include "MainFrm.h"
#include "GameAppDoc.h"
#include "GameAppView.h" // ????

BOOL g_Socket_bBusy = FALSE;

WORD SocketDataToCRC(void *lpBuf, int nBufLen, WORD ocrc)
{
	BYTE *buffer;
	WORD crc;
	int n;
	crc = ocrc; // continue on from previous message.
	buffer = (BYTE *) lpBuf;
	for(n=0;n<nBufLen;n++)
	{
	crc^=buffer[n]+0xa5+n;	 // xor byte value onto lower 8-bits
	crc = (crc<<1) | (crc>>15); // rotate left
	}

	return crc;
}

// ASYNC //

extern void AddLog(char *fname, char *str);

class CQueueData {
public:
CQueueData *m_pNext;
char *m_data;
int m_size;
int m_offset;
CQueueData(BOOL send, void *lpBuf, int nBufLen, WORD *ocrc);
~CQueueData();
};

CQueueData::CQueueData(BOOL send, void *lpBuf, int nBufLen, WORD *ocrc)
{
	BOOL bAdvanced;
	//bAdvanced = FALSE;
	bAdvanced = TRUE; // Make every packet Advanced - only advanced packets have CRC's! //
	if(send) // SEND MODE
	{
	// Allocate memory for queue item.
	m_size = nBufLen+HEADER_SIZE;
	if(m_size>=255) bAdvanced=TRUE;

	if(!bAdvanced) // Normal packet [1byteheader:data]
	{
	m_data = new char[m_size]; // [+] Upgrade to 4 byte header.
	m_offset = 0;
	memcpy(&m_data[HEADER_SIZE], lpBuf, nBufLen);
	// encode header.
	SocketHeader *socketHeaderPtr;
	socketHeaderPtr = (SocketHeader *) m_data;
	socketHeaderPtr->size = m_size;
	}
	else { // Advanced packet [1byteheader:advancedheader:data]
	m_size+=HEADER_SIZE2;
	m_data = new char[m_size]; // [+] Upgrade to 4 byte header.
	m_offset = 0;
	memcpy(&m_data[HEADER_SIZE+HEADER_SIZE2], lpBuf, nBufLen);

	SocketHeader *socketHeaderPtr;
	socketHeaderPtr = (SocketHeader *) m_data;
	socketHeaderPtr->size = 255; // advanced packet.
	
	SocketHeader2 *socketHeaderPtr2;
	socketHeaderPtr2 = (SocketHeader2 *) &m_data[1];
	socketHeaderPtr2->size = m_size;

	socketHeaderPtr2->crc = SocketDataToCRC(lpBuf, nBufLen,*ocrc);
	*ocrc = socketHeaderPtr2->crc;

	}
	
	// [CRC] socketHeaderPtr->crc = SocketDataToCRC(lpBuf, nBufLen);	
	//socketHeaderPtr->crc = SocketDataToCRC(lpBuf, nBufLen);
	}
	else { // RECEIVE MODE
	// Allocate memory for queue item.
	m_size = nBufLen;
	m_data = (char *) lpBuf;	// it's already dynamically allocated - just use that one!
	m_offset = 0;

	*ocrc = SocketDataToCRC(lpBuf, nBufLen,*ocrc);
	}
	// Set up links to default
	m_pNext = NULL;	
}

CQueueData::~CQueueData()
{
	if(m_data) delete m_data;
}

/////////////////////////////////////////////////////////////////////////////
// CClientSocket Construction

CAsyncSocketEx::CAsyncSocketEx()
{
	m_pShellEx = NULL;
	m_pComm = NULL;
	m_dataSent = 0;
	m_dataReceived = 0;
	m_type = 0;

	m_pNext = NULL;
	m_pNextR = NULL;
	m_mode = MODE_HEADER;
	m_buf = (char *) new SocketHeader;//NULL;
	m_size = sizeof(SocketHeader);
	m_offset = 0;

	m_state = 0;

	m_crcSend = SOCKETCRC_START;
	m_crcReceive = SOCKETCRC_START;

	m_bBusy = FALSE;
	g_Socket_bBusy = FALSE;

	m_crcEnable = TRUE;
	m_exitout = FALSE;
	m_status = FALSE;
}

CAsyncSocketEx::CAsyncSocketEx(CShellEx *pShellEx)
{
	// get parent class (for listening socket classes only!)
	//CAsyncSocketEx();
	//m_pShellEx = NULL;
	m_pComm = NULL;
	m_dataSent = 0;
	m_dataReceived = 0;
	m_type = 0;

	m_pShellEx = pShellEx;	

	m_pNext = NULL;
	m_pNextR = NULL;
	m_mode = MODE_HEADER;
	m_buf = (char *) new SocketHeader;//NULL;
	m_size = sizeof(SocketHeader);
	m_offset = 0;

	m_state = 0;

	m_crcSend = SOCKETCRC_START;
	m_crcReceive = SOCKETCRC_START;

	m_bBusy = FALSE;
	g_Socket_bBusy = FALSE;

	m_crcEnable = TRUE;
	m_exitout = FALSE;
	m_status = FALSE;
}

CAsyncSocketEx::~CAsyncSocketEx()
{
	if(m_buf) delete m_buf; // half way thru a receive.
	while(DeleteFromQueue(TRUE));
	while(DeleteFromQueue(FALSE));
}

int CAsyncSocketEx::SendToQueue(void* lpBuf, int nBufLen) // Puts Message on the socket's send queue.
{
	return PutOnQueue(TRUE, lpBuf, nBufLen);
}

int CAsyncSocketEx::ReceiveToQueue(void* lpBuf, int nBufLen) // Puts Message on the socket's receive queue.
{
	return PutOnQueue(FALSE, lpBuf, nBufLen);
}

int CAsyncSocketEx::PutOnQueue(BOOL send, void* lpBuf, int nBufLen) // Puts Message on a queue.
{
	CQueueData *pQueueData;
	int pos;
	pos = 0;

if(send)	// SEND QUEUE
{
	if(m_pNext==NULL)
	{
	m_pNext = new CQueueData(send, lpBuf, nBufLen,&m_crcSend);
	}
	else {
	pQueueData = m_pNext;
	pos++;
	while(pQueueData->m_pNext)
		{
		pQueueData = pQueueData->m_pNext;
		pos++;
		}
	// pQueueData = last item in queue.
	pQueueData->m_pNext = new CQueueData(send, lpBuf, nBufLen, &m_crcSend);
	}
} else { // RECEIVE QUEUE
	if(m_pNextR==NULL)
	m_pNextR = new CQueueData(send, lpBuf, nBufLen, &m_crcReceive);
	else {
	pQueueData = m_pNextR;
	pos++;
	while(pQueueData->m_pNext)
		{
		pQueueData = pQueueData->m_pNext;
		pos++;
		}
	// pQueueData = last item in queue.
	pQueueData->m_pNext = new CQueueData(send, lpBuf, nBufLen, &m_crcReceive);
	}
}
	return pos;
}

int CAsyncSocketEx::SendFromQueue(int i)//void)
{
	BOOL done;
	done = FALSE;

	if(m_pNext==NULL) return 0; // nothing on this queue
	int sizeSent;
	
	sizeSent = Send(&m_pNext->m_data[m_pNext->m_offset], m_pNext->m_size);

	/*if(m_pShellEx)
	{
	char buffer[80];
	sprintf(buffer,"%d (%d)",CheckQueue(),sizeSent);
	ErrorMessage(m_pShellEx->m_pParentWnd, buffer);
	}*/
	/*FILE *fptr;
	fptr = fopen("logit.txt","a+b");
	fprintf(fptr,"[");
	fwrite(&m_pNext->m_data[m_pNext->m_offset], sizeSent,1,fptr);
	fprintf(fptr,"]%d:%d (%d)\r\n",i,CheckQueue(),sizeSent);
	fclose(fptr);*/

	if( sizeSent == SOCKET_ERROR )
		{
			int err = GetLastError();
			//AddLog("LogConnect.txt", errbuf);
			if(err==WSAEWOULDBLOCK)
				{
				//if(m_pShellEx)
				//ErrorMessage(m_pShellEx->m_pParentWnd, "WSAEWOULDBLOCK");
				done = FALSE; // couldn't send due to would block.
				//done = TRUE; // allow for now!
				}
			else {
				if(m_pShellEx)
				{
				char *errbuf;
				errbuf = SocketErrorString();
				ErrorMessage(m_pShellEx->m_pParentWnd, errbuf);
				ErrorMessage(m_pShellEx->m_pParentWnd, "SOCKET_ERROR! (Broadcast)");
				}// else AddLog("LogConnect.txt", "<Socket Error NULL>");
				// take off queue
				done = TRUE;
				}

		//done = TRUE;
		}
	else {
	m_pNext->m_size-=sizeSent;
	m_pNext->m_offset+=sizeSent;

	if(!m_pNext->m_size) // Sent all Data
		{
		done = TRUE;
		}
		}


	if(done) // if queue item done, then delete it, and attach the next one in the list to the front!
	{
		DeleteFromQueue(TRUE);
	}
	return 1;
}

int CAsyncSocketEx::ReceiveFromQueue(void)
{
	BOOL status;
	if(m_pNextR==NULL) return 0; // nothing on this queue
	
//	FILE *fptr = fopen("output.dat","a+b");
//	fwrite(m_pNextR->m_data, m_pNextR->m_size,1,fptr);
//	fwrite(":", 1,1,fptr);
//	fclose(fptr);

	// Ghost bug fixed. - a bug that allowed the same message to be heard twice if message took too long to be completely processed
	char *buf;
	int siz;
	buf = m_pNextR->m_data;
	siz = m_pNextR->m_size;
	m_pNextR->m_data = NULL; // prevents actual deletion of buffer
	DeleteFromQueue(FALSE);

	status = FALSE;
	if(m_pComm) {
				status = m_pComm->OnReceive(this, buf, siz);
				}
	delete buf;

	if(status) {	// Socket is destroyed/closed by now, so any other packets must be ignored, and not processed!
				return 0;
				}

	return 1;
}

BOOL CAsyncSocketEx::DeleteFromQueue(BOOL send)
{
	if(send)
	{
	if(m_pNext==NULL) return FALSE;
	CQueueData *pQueueData;
	pQueueData = m_pNext->m_pNext;
	delete m_pNext;
	m_pNext = pQueueData;
	}
	else {
	if(m_pNextR==NULL) return FALSE;
	CQueueData *pQueueData;
	pQueueData = (m_pNextR)->m_pNext;
	delete m_pNextR;
	m_pNextR = pQueueData;
	}

	return TRUE;
}

int CAsyncSocketEx::CheckQueue(void)
{
	int pos;
	CQueueData *pQueueData;
	pQueueData = m_pNext;
	pos = 0;
	while(pQueueData)
		{
		pQueueData = pQueueData->m_pNext;
		pos++;
		}
	return pos;
}

/////////////////////////////////////////////////////////////////////////////
// CClientSocket Operations
void CAsyncSocketEx::Kick(void)
{
	if(m_status == FALSE) return;
	Close();
	OnClose(0);
}

char *CAsyncSocketEx::SocketErrorString(void)
{
	switch(GetLastError())
		{
	case WSANOTINITIALISED:return "WSANOTINITIALISED";
	case WSAENETDOWN:return "WSAENETDOWN";
	case WSAEACCES:return "WSAEACCES";
	case WSAEINPROGRESS:return "WSAEINPROGRESS";
	case WSAEFAULT:return "WSAEFAULT";
	case WSAENETRESET:return "WSAENETRESET";
	case WSAENOBUFS:return "WSAENOBUFS";
	case WSAENOTCONN:return "WSAENOTCONN";
	case WSAENOTSOCK:return "WSAENOTSOCK";
	case WSAEOPNOTSUPP:return "WSAEOPNOTSUPP";
	case WSAESHUTDOWN:return "WSAESHUTDOWN";
	case WSAEWOULDBLOCK:return "WSAEWOULDBLOCK";
	case WSAEMSGSIZE:return "WSAEMSGSIZE";
	case WSAEINVAL:return "WSAEINVAL";
	case WSAECONNABORTED:return "WSAECONNABORTED";
	case WSAECONNRESET:return "WSAECONNRESET";
		}
	return "Undefined Socket Error";
}

int CAsyncSocketEx::Send( const void* lpBuf, int nBufLen, int nFlags)
{
	int err;
	err = CAsyncSocket::Send(lpBuf, nBufLen, nFlags);
	if(err!=SOCKET_ERROR)
	{
	m_dataSent+=err;
	g_dataSent+=err;
	}
	return err;
}

int CAsyncSocketEx::Receive( void* lpBuf, int nBufLen, int nFlags)
{
	int err;
	err = CAsyncSocket::Receive( lpBuf, nBufLen, nFlags);
	if(err!=SOCKET_ERROR)
	{
	m_dataReceived+=err;
	g_dataReceived+=err;
	}
	return err;
}

/////////////////////////////////////////////////////////////////////////////
// CClientSocket Overridable callbacks

void CAsyncSocketEx::OnAccept(int nErrorCode)// Listening sockets only
{
	//FILE *fptr;
	CAsyncSocket::OnAccept(nErrorCode);
	//g_gameAppView->GetTime();
	//fptr = fopen("logitz.txt","a+b");
	//fprintf(fptr,"Accept: %s:", g_gameAppView->m_stringShortTime);
	//fclose(fptr);

	//m_pDoc->ProcessPendingAccept();
	//if(m_pComm) m_pComm->OnAccept(this);

	//ErrorMessage(m_pShellEx->m_pParentWnd, "Accepting");

	//char buffer[256];
	//sprintf(buffer,"OnConnect, Error Code:%d (Type: %d)", nErrorCode, m_type);
	//ErrorMessage(g_gameAppView, buffer);


	if(m_type==-1) // -1 equals a dummy connect
	{
	CAsyncSocketEx* pSocketAccept = new CAsyncSocketEx();
	pSocketAccept->m_type = -1; // dummy socket (not attached to anything, just let it die!)
		if(Accept(*pSocketAccept))
		{

		}
		else delete pSocketAccept;

	delete this; // close the listening socket.
	}
	else m_pShellEx->OnAccept(this);
}

void CAsyncSocketEx::OnReceive(int nErrorCode)
{ // Called on Server upon receiving a message from a client!
	//char buffer2[80];
	//char buffer[80];
	//int buffer_len;
	CAsyncSocket::OnReceive(nErrorCode);

	//buffer_len = Receive(buffer, 80);
	//sprintf(buffer2,"%s:%d",buffer,buffer_len);
	//MessageBox(0,buffer2,"SocketEx",MB_OK);

	TakeOffReceive();
}

void CAsyncSocketEx::TakeOffReceive(void)
{
	//if(m_bBusy) return;
	if(g_Socket_bBusy) return;
	m_bBusy = TRUE;
	g_Socket_bBusy = TRUE;
	// !!! This is the code moder thing! - swap rem to make sure.

	while(Pump()); // takes all the shit off the receive buffer, and puts in packets on receive queue

	if(m_exitout)
	{
	Close();
	OnClose(0);
	return;
	}

	while(ReceiveFromQueue()); // takes all the packets off the receive queue and sends to m_pComm.
	m_bBusy = FALSE;
	g_Socket_bBusy = FALSE;
}

void SLog(char *str, int siz)
{
	if(g_socketlog)
	{
	FILE *fptr;
	if(g_socketfirst) 
	{
		fptr = fopen("send.log","a+b");
		fprintf(fptr,"---");
		fclose(fptr);
		fptr = fopen("receive.log","a+b");
		fprintf(fptr,"---");
		fclose(fptr);
		g_socketfirst = FALSE;
	}
	fptr = fopen("send.log","a+b");
	fwrite(str,siz,1,fptr);
	fclose(fptr);
	}
}

void RLog(char *str, int siz)
{
	if(g_socketlog)
	{
	FILE *fptr;
	if(g_socketfirst) 
	{
		fptr = fopen("send.log","a+b");
		fprintf(fptr,"---");
		fclose(fptr);
		fptr = fopen("receive.log","a+b");
		fprintf(fptr,"---");
		fclose(fptr);
		g_socketfirst = FALSE;
	}
	fptr = fopen("receive.log","a+b");
	fwrite(str,siz,1,fptr);
	fclose(fptr);
	}
}


void HACK_LOG(char *rPeerAddress, int rPeerPort, char *msg)
{
#ifdef MAIN_SERVER 
	char buf[256];
	sprintf(buf, "%s:%d (%s)", rPeerAddress, rPeerPort, msg);
	AddLog("LogHackers.txt", buf);
#endif
}


BOOL CAsyncSocketEx::Pump(void)
{
	int len;

	
	
	len = Receive(&m_buf[m_offset], m_size); // Get Size data.
	if(len==SOCKET_ERROR)
	{
	len = GetLastError();
	if(len==WSAEWOULDBLOCK)
	{
//		if(m_pShellEx)
//			ErrorMessage(m_pComm->m_pShell->m_pParentWnd, "WOULDBLOCK!");
	}
	else {
			if(m_pComm)
			if(m_pComm->m_pShell)
			if(m_pComm->m_pShell->m_pParentWnd)
			{
			char buffer[80];
			//sprintf(buffer,"B:%s",OnConnectErrorString(len));
			if(m_mode==MODE_HEADER)
			sprintf(buffer,"Header:%s(%d,%d)",SocketErrorString(),m_offset,m_size);
			else if(m_mode==MODE_DATA)
			sprintf(buffer,"Data:%s(%d,%d)",SocketErrorString(),m_offset,m_size);
			
			if(m_pShellEx)
			ErrorMessage(m_pComm->m_pShell->m_pParentWnd, buffer);
			}
		}
	return FALSE;
	}


/*

	------------------------------------------------
	------------------------------------------------


	------------------------------------------------
	------------------------------------------------
	

	------------------------------------------------
	------------------------------------------------



	// Log Data packets coming in - can be multipackets //
	FILE *fptr;
	fptr = fopen("newrec.txt","a+b");
	fprintf(fptr,"[(%d):",len);
	fwrite(&m_buf[m_offset], len,1,fptr);
	fprintf(fptr,"]");
	fclose(fptr);

	// ------------------------------------------------ //
	// ------------------------------------------------ //
	// ------------------------------------------------ //

						//  //
		
	// ------------------------------------------------ //
	// ------------------------------------------------ //
	// ------------------------------------------------ //
*/


//	if(m_pShellEx)
//	{
//		char buffer[80];
//		sprintf(buffer, "%d bytes", len);
//		ErrorMessage(m_pComm->m_pShell->m_pParentWnd, buffer);
//	}

	if(!len) return FALSE; // no more data on queue

	m_offset+=len;
	m_size-=len;
	if(!m_size) // read all of packet!
		{
		switch(m_mode)
			{
			case MODE_HEADER:
							{
							SocketHeader *socketHeaderPtr;
							socketHeaderPtr = (SocketHeader *) m_buf;
							memcpy(&m_header, socketHeaderPtr, HEADER_SIZE);
							if(socketHeaderPtr->size==255) // Advanced packet system
							{
							delete m_buf;
							m_size = HEADER_SIZE2;
							m_offset = 0;
							m_buf = (char *) new SocketHeader2;
							m_mode = MODE_HEADER2;
							break;
							}


							m_size = socketHeaderPtr->size;
							m_crcOn = FALSE;

							//if(m_size<=HEADER_SIZE) {
							if(1) {

										CString rPeerAddress;
										UINT rPeerPort;
										GetPeerName(rPeerAddress, rPeerPort);


										  char buffer[80];
										  sprintf(buffer,"Bad Size Packet:%d",m_size);
										  HACK_LOG(rPeerAddress.GetBuffer(0), rPeerPort, buffer); 
										  g_gameAppView->m_IPConnectCoolDown->Check(rPeerAddress.GetBuffer(0),8*3600);

										  if(m_pShellEx)
										  ErrorMessage(m_pComm->m_pShell->m_pParentWnd, buffer);
										  //Close();
										  //OnClose(0);
										  m_exitout = TRUE;
										  return FALSE;
										  }
							delete m_buf;

							m_size-=HEADER_SIZE;	// since size includes itself	
#ifdef SOCKET_DEBUG
	{
	char buffer[80];
	sprintf(buffer,"Receive Packet:%dbytes",m_size);

	if(m_pShellEx)
	ErrorMessage(m_pComm->m_pShell->m_pParentWnd, buffer);
	}
#endif
							m_offset = 0;
							m_buf = new char[m_size];				 
							m_mode = MODE_DATA;
							}
							 break;
			case MODE_HEADER2:
							{
							SocketHeader2 *socketHeaderPtr2;
							socketHeaderPtr2 = (SocketHeader2 *) m_buf;
							memcpy(&m_header2, socketHeaderPtr2, HEADER_SIZE2);

							m_size = socketHeaderPtr2->size;
							m_crc = socketHeaderPtr2->crc;
							m_crcOn = TRUE;
							//= SocketDataToCRC(lpBuf, nBufLen,0);

							m_size-=HEADER_SIZE+HEADER_SIZE2;	// remove both headers.
#ifdef SOCKET_DEBUG
	{
	char buffer[80];
	sprintf(buffer,"Receive Advanced Packet:%dbytes",m_size);

	if(m_pShellEx)
	ErrorMessage(m_pComm->m_pShell->m_pParentWnd, buffer);
	}
#endif
							
							if(m_size<=1) {
									
									CString rPeerAddress;
									UINT rPeerPort;
									GetPeerName(rPeerAddress, rPeerPort);

									char buffer[80];
									sprintf(buffer,"Bad Adv.Size Packet:%d",m_size);
									HACK_LOG(rPeerAddress.GetBuffer(0), rPeerPort, buffer);
									g_gameAppView->m_IPConnectCoolDown->Check(rPeerAddress.GetBuffer(0),8*3600);

									if(m_pShellEx)
									ErrorMessage(m_pComm->m_pShell->m_pParentWnd, buffer);

									m_exitout = TRUE;
									return FALSE;
							}								

							delete m_buf; // added'04
							m_offset = 0;
							m_buf = new char[m_size];				 
							m_mode = MODE_DATA;
							}
							 break;
			case MODE_DATA:	// note m_offset should now be equal to the original size value
							// [CRC] m_header.crc <=> SocketDataToCRC(m_buf, m_offset);
							WORD crc;

							crc = SocketDataToCRC(m_buf, m_offset, m_crcReceive);

							if(m_crcOn && m_crcEnable)
							{
								if(crc!=m_crc)
								{

								CString rPeerAddress;
								UINT rPeerPort;
								GetPeerName(rPeerAddress, rPeerPort);


								char buffer[80];
								sprintf(buffer,"Error, Packet CRC Failure");
								HACK_LOG(rPeerAddress.GetBuffer(0), rPeerPort, buffer);
								g_gameAppView->m_IPConnectCoolDown->Check(rPeerAddress.GetBuffer(0),20);
						
								if(m_pShellEx)
								ErrorMessage(m_pComm->m_pShell->m_pParentWnd, buffer);
								//Close();
								//OnClose(0);
								m_exitout = TRUE;
								return FALSE;
								} /*else {
								if(m_pShellEx)
								ErrorMessage(m_pComm->m_pShell->m_pParentWnd, buffer);
								}*/
							}

							ReceiveToQueue(m_buf, m_offset);	// Add complete packet to Receive Queue.
							//m_buf = NULL;
							//delete m_buf; // Don't need to delete m_buf! - coz CQueueData takes ownership of it!
							m_size = sizeof(SocketHeader);
							m_offset = 0;
							m_buf = (char *) new SocketHeader;//NULL;
							m_mode = MODE_HEADER;
							break;
			} // end switch
		} // end if
	return TRUE;
}

void CAsyncSocketEx::OnSend(int nErrorCode)
{ // Called on Server upon receiving a message from a client!
	CAsyncSocket::OnSend(nErrorCode);
	if(m_pComm) m_pComm->OnSend(this);
}

void CAsyncSocketEx::OnClose( int nErrorCode )
{
	//FILE *fptr;
	//fptr = fopen("logitz.txt","a+b");
	//fprintf(fptr,"Closed\r\n");
	//fclose(fptr);
	m_status = FALSE;

	if(m_pComm) {
				m_pComm->OnClose(this);
				}
	else {
		 delete this;
		 }
}

void CAsyncSocketEx::OnOpen(void) // Fake virtual, programmer calls this up, when he wants to say that a connection is firmly open.
{							 // i.e. Connected, aswell as being attached to CComm object
	// m_status is set to TRUE in CComm::AttachSocket(int idx, CAsyncSocketEx *pSocket)
	if(m_pComm) m_pComm->OnOpen(this);
}

//WSAECONNREFUSED
char *OnConnectErrorString(int nErrorCode)
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

//extern CGameAppView *g_gameAppView;

void CAsyncSocketEx::OnConnect( int nErrorCode )
{
	if(m_pShellEx)
	{
	m_pShellEx->OnConnect(this, nErrorCode);
	}
	/*if(!nErrorCode) // Ok Connected
	{
	if(m_pComm) m_pComm->OnClose(this);
	} */
}


/////////////////////////////////////////////////////////////////////////////
// CSocket Implementation

#ifdef _DEBUG
void CAsyncSocketEx::AssertValid() const
{
	CAsyncSocket::AssertValid();
}

void CAsyncSocketEx::Dump(CDumpContext& dc) const
{
	CAsyncSocket::Dump(dc);
}
#endif //_DEBUG

IMPLEMENT_DYNAMIC(CAsyncSocketEx, CAsyncSocket)

