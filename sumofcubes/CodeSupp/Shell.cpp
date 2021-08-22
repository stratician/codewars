
#include "Build.h"
#include "stdafx.h"

#include "Shell.h"

#include "GameAppDoc.h"
#include "GameAppView.h"
#include "SocketEx.h"
#include "../Code/db.h"

#ifndef MAIN_SERVER
#ifndef TEST_SERVER

#define PUBLIC_VERSION

#endif
#endif

/*
#define MSGCODE_TEXT			0x00	// Private: Text
#define MSGCODE_PROFILE			0x01	// Private:CProfile
#define MSGCODE_ALLPROFILES		0x02	// Private:SAll Profiles
#define MSGCODE_DELETEPROFILE	0x03	// Private:SDelete Profile
#define MSGCODE_SITREQUEST		0x04	// Private:CSit Down Request.
#define MSGCODE_SITORDER		0x05    // Private:SSit Down Order.
#define MSGCODE_STANDREQUEST	0x06	// Private:CStand Up Request.
#define MSGCODE_STANDORDER		0x07	// Private:SStand Up Request.
#define MSGCODE_SYSTEMTEXT		0x08	// Private: System Text
#define MSGCODE_SOCKETIDX		0x09	// Private: Socket Idx

#define MSGCODE_GAME			0x7f	// Private: Game message.

#define MSGMASK_CODE		0x7f	// Code Mask
#define MSGMASK_BROADCAST	0x80	// Broadcast Mask (auto broadcast)
*/

int g_dataSent;
int g_dataReceived;

BOOL g_socketlog = FALSE;
BOOL g_socketfirst = FALSE;
BOOL g_gamelog = FALSE;
int g_daysFreeTrial = 0;
BOOL g_guestMember = TRUE;

void CProfile::AddMsgProfile(CMsgProfile *pMsgProfile)
{
//	strcpy(data.nick, pMsgProfile->nick);
	memcpy(&data, pMsgProfile, sizeof(CMsgProfile));
}

char *CProfile::GetNick(void)
{
	return data.nick;
}

CMsgProfile *CProfile::GetData(void)
{
	return &data;
}

CComm::CComm(int max_sockets, int queue_size)
{
	int n;
	m_socketList = new CAsyncSocket *[max_sockets];
	m_socketList_n = max_sockets;
	m_socketsUsed_n = 0;

	for(n=0;n<m_socketList_n;n++)
	m_socketList[n] = NULL;

	m_pShell = NULL;

	//
	m_packetFrag = new unsigned char *[max_sockets];
	for(n=0;n<m_socketList_n;n++)
		m_packetFrag[n] = new unsigned char [256];
	
	m_packetFrag_len = new int [max_sockets];
	//
		
	lpBuf = new unsigned char *[queue_size];
	lpBufLen = new int[queue_size];
	lpBufI = new int[queue_size];
	lpBufOffset = new int [max_sockets];
	for(n=0;n<max_sockets;n++)
		lpBufOffset[n] = 9999; // should cause an error?? for debugging purposes

	buf_n = queue_size;
	write_ptr = read_ptr = 0;
	queueMessageCount = 0;

	m_bSendBusy = FALSE;

	m_SendCnt = 0;
	m_ReceiveCnt = 0;

	m_rPeerAddress = _T("Empty!"); // for Server use only!
	m_rPeerPort = 0xffff; // for Server use only!
/*
	for(int m=0; m<4; m++)
	{
		//
		m_packetIdx[m] = 0;	
		for(n=0; n<16; n++)
		{
			memset(m_packet[m][n], 0x00, 256); // clear 256byte buffers
			m_packet_len[m][n] = 0;
		}

		m_buildingPacket[m] = FALSE;
		m_buildingPacketSize[m] = 0;
		//
	}
	*/
}

CComm::~CComm()
{
	int n;
	// Flush Queue slots.
	while(read_ptr!=write_ptr)
	{
	delete lpBuf[read_ptr]; // delete memory.
	read_ptr++;
	if(read_ptr==buf_n) read_ptr=0;
	}

	delete lpBuf;
	delete lpBufLen;
	delete lpBufI;
	delete lpBufOffset;

	//
	if(m_packetFrag)
	{
		for(n=0;n<m_socketList_n;n++)
		if(m_packetFrag[n]) {
								delete m_packetFrag[n];
								m_packetFrag[n] = NULL;
							}


		delete m_packetFrag;
		m_packetFrag = NULL;
	}
	//

	if(m_packetFrag_len) delete m_packetFrag_len;

	if(m_socketList)
	{
		for(n=0;n<m_socketList_n;n++)
		if(m_socketList[n]) {
								delete m_socketList[n];
								m_socketList[n] = NULL;
							}

		delete m_socketList;
		m_socketList = NULL;
	}
}

int CComm::SendToQueue(void* lpBuf, int nBufLen, int i) // Sends to all attached sockets defaultly otherwise player i. [Queue]
{	
	
	int plrSockIdx = i;
	if(g_socketlog)
	{
		FILE *fptr;
		if(g_socketfirst) 
		{
			if(fptr = fopen("send.log","a+b"))
			{
				fprintf(fptr,"---");
				fclose(fptr);
			}

			if(fptr = fopen("receive.log","a+b"))
			{
				fprintf(fptr,"---");
				fclose(fptr);
			}
			g_socketfirst = FALSE;
		}
	
		if(fptr = fopen("send.log","a+b"))
		{
			fprintf(fptr,"[%d:",plrSockIdx);
			fwrite(lpBuf,nBufLen,1,fptr);
			fclose(fptr);
		}
	}


	if(i==-1) // All
	{
	for(i=0;i<m_socketList_n;i++)
	if(m_socketList[i])
		{
		((CAsyncSocketEx *)m_socketList[i])->SendToQueue(lpBuf, nBufLen);
		}
	}
	else if(i==-2) // Special All except server socket 0
	{
	for(i=1;i<m_socketList_n;i++)
	if(m_socketList[i])
		{
		((CAsyncSocketEx *)m_socketList[i])->SendToQueue(lpBuf, nBufLen);
		}
	}
	else { // Singular
	if(i<m_socketList_n)
	if(m_socketList[i])
		{
		((CAsyncSocketEx *)m_socketList[i])->SendToQueue(lpBuf, nBufLen);
		}
	}

	if(g_socketlog)
	{
		FILE *fptr;
		if(fptr = fopen("send.log","a+b"))
		{
			fprintf(fptr,"]");
			fclose(fptr);
		}
	}

	// trigger another send! NEW!
	SendFromQueue();

	TakeOffReceive();

	return 1;
}

int CComm::TakeOffReceive(void)
{
	int i;
	for(i=0;i<m_socketList_n;i++) // -[Extra onreceives]- //
	if(m_socketList[i])
	{
	
	((CAsyncSocketEx *)m_socketList[i])->TakeOffReceive();

	//while(((CAsyncSocketEx *)m_socketList[i])->Pump()); // takes all the shit off the receive buffer, and puts in packets on receive queue
	//while(((CAsyncSocketEx *)m_socketList[i])->ReceiveFromQueue()); // takes all the packets off the receive queue and sends to m_pComm.
	}
	return 0;
}

int CComm::SendFromQueue(void)
{
	///////////////////
	//return 0; // bypass the following code.
	///////////////////

	//if(read_ptr==write_ptr) return 0; // no message to send.
	
	int i;
	for(i=0;i<m_socketList_n;i++)
	if(m_socketList[i])
	((CAsyncSocketEx *)m_socketList[i])->SendFromQueue(i);//);

	/*
	BOOL success;
	success = Send(lpBuf[read_ptr], lpBufLen[read_ptr], lpBufI[read_ptr]); // send message off.
	if(success)
	{
		delete lpBuf[read_ptr]; // delete memory.
		read_ptr++;
		if(read_ptr==buf_n) read_ptr=0;
		queueMessageCount--;
	}

	*/
	return 1;
}

BOOL CComm::Send(void* lpBuf, int nBufLen, int i) // Sends to all attached sockets defaultly otherwise player i.
{
	int newMsg = 1;
	int n;
	int sizeSent;
	unsigned char *buf = (unsigned char *)lpBuf;
	int nBufLenTmp;

	// check for new message
	for(n=0;n<m_socketList_n;n++)
	if(m_socketList[n])	
	if(lpBufOffset[n]!=-1)
	{
		newMsg = 0;		
	}
		
	if(newMsg)
	{
		if(i==-1)
		{
			for(n=0;n<m_socketList_n;n++)
			if(m_socketList[n])	
			{
				lpBufOffset[n] = 0;
			}
		}
		else
		{
			if(i<m_socketList_n)
				if(m_socketList[i])	
					lpBufOffset[i] = 0;
		}
	}

	// continue sending message
	for(n=0;n<m_socketList_n;n++)
	if(m_socketList[n])	
	if(lpBufOffset[n]!=-1)
	{
		nBufLenTmp = nBufLen - lpBufOffset[n];
		sizeSent = ((CAsyncSocketEx *)m_socketList[n])->Send(&buf[lpBufOffset[n]], nBufLenTmp);
		if( sizeSent == SOCKET_ERROR )
		{
			
			ErrorMessage(m_pShell->m_pParentWnd, ((CAsyncSocketEx *)m_socketList[n])->SocketErrorString());
			ErrorMessage(m_pShell->m_pParentWnd, "SOCKET_ERROR! (Broadcast)");
			lpBufOffset[n] = -1;
		}
		else
		{
			lpBufOffset[n]+=sizeSent;
			if(lpBufOffset[n] == nBufLen)
				lpBufOffset[n] = -1;
		}
	}

	// check if finished message
	newMsg = 1;
	for(n=0;n<m_socketList_n;n++)
	if(m_socketList[n])	
	if(lpBufOffset[n]!=-1)
	{
		newMsg = 0;
	}

	if(newMsg)
		return TRUE;

	return FALSE;

#if 0
	//if(m_bSendBusy) return FALSE;
	//m_bSendBusy = TRUE;	// locks
/*
	m_SendCnt++;

	int n;
	if(i==-1)
	{
	//MessageBox(0,"Sending","--",MB_OK);
	for(n=0;n<m_socketList_n;n++) // Send to Attached Sockets.
	if(m_socketList[n])	
		{
			//m_socketList[n]->Send(lpBuf, nBufLen);
			if( m_socketList[n]->Send(lpBuf, nBufLen) == SOCKET_ERROR ) {
				MessageBox(0,"SOCKET_ERROR!","SOCKET_ERROR!",MB_OK);
				return;// FALSE;
			}
		}
	}
	else {
	if(i<m_socketList_n)
		if( m_socketList[i]->Send(lpBuf, nBufLen) == SOCKET_ERROR ) {
			MessageBox(0,"SOCKET_ERROR!","SOCKET_ERROR!",MB_OK);
			return;
			}
	}
*/
	//m_bSendBusy = FALSE; // unlocks
	//return TRUE;


	//if(m_bSendBusy) return FALSE;
	//m_bSendBusy = TRUE;	// locks
	int sizeSent;
	int offset;
	int nBufLenTmp;
	unsigned char *buf = (unsigned char *)lpBuf;

	m_SendCnt++;

	int n;
	if(i==-1)
	{
	//MessageBox(0,"Sending","--",MB_OK);
	for(n=0;n<m_socketList_n;n++) // Send to Attached Sockets.
	if(m_socketList[n])	
		{
			offset = 0;
			nBufLenTmp = nBufLen;
			while(nBufLenTmp)
			{	
				/*
				// TEST CODE
				sizeSent = m_socketList[n]->Send(&buf[offset], 10);
				offset += 10;
				MessageBox(0, "a","a", MB_OK);
				sizeSent = m_socketList[n]->Send(&buf[offset], 10);
				offset += 10;
				MessageBox(0, "aa","aa", MB_OK);
				sizeSent = m_socketList[n]->Send(&buf[offset], 235);
				sizeSent = 255;
				// ---------
				*/
				//sizeSent = m_socketList[n]->Send(&buf[offset], nBufLenTmp);
				sizeSent = ((CAsyncSocketEx *)m_socketList[n])->Send(&buf[offset], nBufLenTmp);
				if( sizeSent == SOCKET_ERROR )
				{
					ErrorMessage(m_pShell->m_pParentWnd, ((CAsyncSocketEx *)m_socketList[n])->SocketErrorString());
					ErrorMessage(m_pShell->m_pParentWnd, "SOCKET_ERROR! (Broadcast)");
					//MessageBox(0,((CAsyncSocketEx *)m_socketList[n])->SocketErrorString(),"SOCKET_ERROR! (Broadcast)",MB_OK);
					break;//return;// FALSE;
				}
				nBufLenTmp-=sizeSent;
				offset+=sizeSent;
			}
		}
	}
	else {
	if(i<m_socketList_n)
	if(m_socketList[i])
		{
			offset = 0;
			while(nBufLen)
			{
				/*
				// TEST CODE
				sizeSent = m_socketList[i]->Send(&buf[offset], 10);
				offset += 10;
				MessageBox(0, "b","b", MB_OK);
				sizeSent = m_socketList[i]->Send(&buf[offset], 10);
				offset += 10;
				MessageBox(0, "bb","bb", MB_OK);
				sizeSent = m_socketList[i]->Send(&buf[offset], 235);
				sizeSent = 255;
				// ---------
				*/
				//sizeSent = m_socketList[i]->Send(&buf[offset], nBufLen);
				sizeSent = ((CAsyncSocketEx *)m_socketList[i])->Send(&buf[offset], nBufLen);
				if( sizeSent == SOCKET_ERROR )
				{
					ErrorMessage(m_pShell->m_pParentWnd, ((CAsyncSocketEx *)m_socketList[i])->SocketErrorString());
					ErrorMessage(m_pShell->m_pParentWnd, "SOCKET_ERROR! (Single)");
					//MessageBox(0,((CAsyncSocketEx *)m_socketList[i])->SocketErrorString(),"SOCKET_ERROR! (Single)",MB_OK);
					return;
				}
				nBufLen-=sizeSent;
				offset+=sizeSent;		
			}
		}
	}

	//m_bSendBusy = FALSE; // unlocks
	//return TRUE;

#endif //0
	
}

/*void CComm::OnReceive(CAsyncSocketEx *pSocket) // Called whenever an attached socket receives a message. pSocket is message originator.
{
	//char buffer2[80];
	//char buffer[80];
	//int buffer_len;
	
	//buffer_len = pSocket->Receive(buffer, 80);
	//sprintf(buffer2,"%s:%d",buffer,buffer_len);
	//MessageBox(0,buffer2,"CComm",MB_OK);
////	AfxMessageBox("Redundant");
//	if(m_pShell) m_pShell->OnReceive(this, pSocket);
}*/

BOOL CComm::OnReceive(CAsyncSocketEx *pSocket, void* lpBuf, int nBufLen) // Whenever a COMPLETE Packet comes back
{
	BOOL status;
	//char buffer2[80];
	//char buffer[80];
	//int buffer_len;
	
	//buffer_len = pSocket->Receive(buffer, 80);
	//sprintf(buffer2,"%s:%d",buffer,buffer_len);
	//MessageBox(0,buffer2,"CComm",MB_OK);

	int plrSockIdx;

	plrSockIdx = GetSocketIndex(pSocket);	

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

	fprintf(fptr,"[%d:",plrSockIdx);
	fwrite(lpBuf,nBufLen,1,fptr);
	fclose(fptr);
	}

	status = FALSE;
	if(m_pShell) {
				status = m_pShell->OnReceive(this, pSocket, lpBuf, nBufLen);
				}

	if(g_socketlog)
	{
	FILE *fptr;
	fptr = fopen("receive.log","a+b");
	fprintf(fptr,":%d]",plrSockIdx);
	fclose(fptr);
	}
	return status;
}

void CComm::OnSend(CAsyncSocketEx *pSocket)  // Called whenever an attached Comm receives a message. pSocket is message originator.
{
	//char buffer2[80];
	//char buffer[80];
	//int buffer_len;
	
	//buffer_len = pSocket->Receive(buffer, 80);
	//sprintf(buffer2,"%s:%d",buffer,buffer_len);
	//MessageBox(0,buffer2,"CComm",MB_OK);
	SendFromQueue();
	//--
	//char buff[80];
	//sprintf(buff,"OnSend");
	//ErrorMessage(m_pShell->m_pParentWnd, buff);
	//--
if(m_pShell) m_pShell->OnSend(this, pSocket);
}

void CComm::OnClose(CAsyncSocket *pSocket) // Called upon Socket Closure.
{
	int n;
	//char buffer[80];
	//MessageBox(0, "123", "123", MB_OK);
	if(pSocket)
	for(n=0;n<m_socketList_n;n++)
	if(m_socketList[n]==pSocket)
	{
		//MessageBox(0, "outer", "outer", MB_OK);
		if(m_socketList[n])
		{
			delete m_socketList[n];
			m_socketList[n] = NULL;
			//sprintf(buffer,"[%d]",n);
			//MessageBox(0, "inner", "inner", MB_OK);
			break;
		}
	}
	if(m_pShell) m_pShell->OnClose(this, n);
	//MessageBox(0, "456", "456", MB_OK);
}

void CComm::OnOpen(CAsyncSocket *pSocket) // Called upon Socket Opening.
{
	if(m_pShell) m_pShell->OnOpen(this);
}

void CComm::AttachSocket(int idx, CAsyncSocketEx *pSocket)
{
	m_packetFrag_len[idx] = 0;
	
	m_socketList[idx] = pSocket;
	((CAsyncSocketEx *)m_socketList[idx])->m_status = TRUE;
	pSocket->m_pComm = this; // let socket know who's daddy.

	lpBufOffset[idx] = -1;
}

int CComm::AttachSocket(CAsyncSocketEx *pSocket)
{ // Attach to any available socket
	int n;
	for(n=0;n<m_socketList_n;n++)
	if(m_socketList[n]==NULL)
	{
	//m_socketList[n] = socket;
	AttachSocket(n,pSocket);
	return n;
	}
	return -1; // didn't manage to attach the socket.
}

int CComm::GetSocketIndex(CAsyncSocketEx *pSocket)
{
 	int n;
	for(n=0;n<m_socketList_n;n++)
	if(m_socketList[n])
	{
	if(m_socketList[n]==pSocket) return n;
	}
	return -1; // couldn't find index.

}

int CComm::CountSocketsUsed(void)
{ // Attach to any available socket
	int n;
	m_socketsUsed_n = 0;
	for(n=0;n<m_socketList_n;n++)
	if(m_socketList[n])
		m_socketsUsed_n++;

	return m_socketsUsed_n;
}

void CComm::Close(void)
{
	int n;
	
	for(n=0;n<m_socketList_n;n++)
	if(m_socketList[n])
		m_socketList[n]->Close();
}

BOOL CComm::GetPeerName(CString& rPeerAddress, UINT& rPeerPort, int i) // Gets Peer name of socket index - the safe way
{
	if(i>=0 && i<m_socketList_n) // 0 to socketlist-1
	if(m_socketList[i])	// check if socket still open.
		{
		return ((CAsyncSocketEx *)m_socketList[i])->GetPeerName(rPeerAddress, rPeerPort);
		}
	return FALSE;
}

BOOL CComm::Kick(int i) // Gets Peer name of socket index - the safe way
{
	if(i>=0 && i<m_socketList_n) // 0 to socketlist-1
	if(m_socketList[i])	// check if socket still open.
		{
		((CAsyncSocketEx *)mainShell->Server->m_socketList[i])->Kick();
		return TRUE;
		}
	return FALSE;
}

CSeat::CSeat(CWnd *pParentWnd, int x, int y)
{
	CRect rect;
	m_x = x;
	m_y = y;

	m_state = SEAT_EMPTY;
	m_pParentWnd = pParentWnd;
	rect.SetRect(0,0,40,24);
	rect.OffsetRect(x,y);

	m_button.PassParent(this);
	m_button.Create("Sit", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | WS_BORDER, rect, m_pParentWnd, 65000);
	m_button.SetWindowPos(&((CGameAppView *)m_pParentWnd)->wndTop, 0,0, 32, 32, SWP_NOACTIVATE | SWP_HIDEWINDOW);
}

void CSeat::Sit(int idx)
{
	m_state = SEAT_TAKEN;
	m_button.ModifyStyle(WS_VISIBLE,0); // Remove Visible Flag.
	m_idx = idx;
	//m_button.ModifyStyle(0, WS_DISABLED);
	//m_button.SetWindowText("Taken");
}

void CSeat::Stand(void)
{
	m_state = SEAT_EMPTY;
	m_button.ModifyStyle(0,WS_VISIBLE); // Set Visible Flag.
}

void CSeat::SetName(char *str)
{
	strcpy(m_name,str);
}

void CSeat::OnEvent(int id)
{
	//MessageBox(0,"?!?#@","O",MB_OK);
	m_pParent->OnEvent(m_id); // send our own id
}

void CSeat::OnDraw(CDC* pDC)
{
	RECT rect;
	//if(m_state==SEAT_EMPTY) // Only Draw Button if empty.
	m_button.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
	
	m_button.GetWindowRect(&rect);
	
	CPen pen(PS_SOLID, 0, RGB(42,221,1));//RGB(0,0,0));
	CBrush brush(RGB(42,221,1));//RGB(255,255,255));
	CBrush *oldBrush = pDC->SelectObject(&brush);
	CPen *oldPen = pDC->SelectObject(&pen);

	pDC->Rectangle(m_x+40+4, m_y, m_x+200,m_y+24); // P1

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
		
	if(m_state==SEAT_TAKEN)
	{
	//pDC->SetBkColor(RGB(255,255,225));
	pDC->TextOut(m_x+40+8, m_y+4, m_name);
	}
	//pDC->TextOut(50, 50, "Chobby");
}

void CSeat::OnDraw(CDC* pDC, int x, int y, int n)
{
	RECT rect;
	int offset;
	char buffer[81];
	offset = n * (160 + 40);

	CPen pen2(PS_NULL, 0, RGB(20,20,20));
	CBrush brush2(RGB(243,246,29));
	//CBrush brush2(RGB(0,0,0));
	CBrush *oldBrush2 = pDC->SelectObject(&brush2);
	CPen *oldPen2 = pDC->SelectObject(&pen2);
	pDC->Rectangle(40+n*(x-160-80)+160-offset, x*0.5f+8, 40+n*(x-160-80)+160-offset+40, x*0.5f+8+24); // P1
	pDC->SelectObject(oldBrush2);
	pDC->SelectObject(oldPen2);
	RectangleShaded(pDC,40+n*(x-160-80)+160-offset, -7 + x*0.5f+8, 40+n*(x-160-80)+160-offset+40, 9 + x*0.5f+8+24, RGB(243,246,29)); //P1

	if(m_state==SEAT_EMPTY) // Only Draw Button if empty.
	{
	m_button.SetWindowPos(NULL, 40+n*(x-160-80)+160-offset, x*0.5f+8, 40, 24, SWP_SHOWWINDOW);//SWP_NOREDRAW);//
	m_button.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
	}
	
	m_button.GetWindowRect(&rect);
	
	CPen pen(PS_SOLID, 0, RGB(20,20,20));
	CBrush brush(RGB(255,255,255));//RGB(25,25,115));//RGB(243,246,29));
	CBrush *oldBrush = pDC->SelectObject(&brush);
	CPen *oldPen = pDC->SelectObject(&pen);

	pDC->Rectangle(40+n*(x-160-80), x*0.5f+8, 40+n*(x-160-80)+160, x*0.5f+24+8); // P1
	
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);

	if(m_state==SEAT_TAKEN)
	{
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(RGB(255,255,225));//RGB(25,25,115));//RGB(243,246,29));//RGB(255,255,225));
	pDC->TextOut(40+n*(x-160-80)+8, x*0.5f+4+8, m_name);
	}
	//pDC->TextOut(50, 50, "Chobby");
		
	// display 1P / 2P
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(RGB(243,246,29));
	//pDC->SetBkColor(RGB(0,0,0));
	sprintf(buffer, "P%d", n+1);
	pDC->TextOut(0+n*(x-160+80+40)+8, x*0.5f+4+8, buffer);
	/*
	// display scores
	sprintf(buffer, "%d", ((CGameAppView *)m_pParentWnd)->m_snooker->m_score1P);
	pDC->TextOut(160+125+n*(x+(-160-125)*2), x*0.5f+4+8, buffer);
	*/
}				 

int CSeat::IsEmpty(void)
{
	if(m_state==SEAT_EMPTY) return 1;
	return 0;
}

BOOL CSeat::CheckFocus(CWnd* focusWnd)
{
	BOOL bFocus = 0;
	if(focusWnd == &m_button) bFocus = 1;
	return bFocus;
}


CShell::CShell()
{
	SetNick("Guest");
	m_seatList = NULL;
	m_profileList = NULL;
	m_CprofileList = NULL;
	m_pNameList = NULL;
	
	m_seatList_n = 0;
	m_profileList_n = 0;
	m_CprofileList_n = 0;

	m_idx = -1; // initially not assigned an index.
	m_seat = -1;
	m_cyc = 0;
	
	//m_ProfilesChunkBuffer[4096];
	m_ProfilesChunkPos = 0;

	/*
	//
	int n;
	//m_receiveBufferIdx = 0;	
	//m_receiveBuffer_len = 0;
	//for(n=0; n<8; n++)
	//	memset(m_receiveBuffer[n], 0x00, 2048);	// clear 2k buffers
		
	
	Server->m_packetIdx = 0;	
	for(n=0; n<16; n++)
	{
		memset(Server->m_packet[n], 0x00, 256); // clear 256byte buffers
		Server->m_packet_len[n] = 0;
	}

	Server->m_buildingPacket = FALSE;
	Server->m_buildingPacketSize = 0;
	*/
	
}

CShell::CShell(CWnd *pParentWnd, int players)
{
	Init(pParentWnd, players);
	SetNick("Guest");
	m_profileList = NULL;
	m_CprofileList = NULL;
	m_pNameList = NULL;
	
	m_seatList_n = 0;
	m_profileList_n = 0;
	m_CprofileList_n = 0;
	
	m_idx = -1;
	m_seat = -1;
	m_cyc = 0;

	//m_ProfilesChunkBuffer[4096];
	m_ProfilesChunkPos = 0;

	/*
	//
	int n;
	m_receiveBufferIdx = 0;	
	m_receiveBuffer_len = 0;
	for(n=0; n<8; n++)
		memset(m_receiveBuffer[n], 0x00, 2048);	// clear 2k buffers
		
	m_packetIdx = 0;	
	for(n=0; n<16; n++)
	{
		memset(m_packet[n], 0x00, 256); // clear 256byte buffers
		m_packet_len[n] = 0;
	}

	m_buildingPacket = FALSE;
	m_buildingPacketSize = 0;
	//
	*/
}

CShell::~CShell()
{
	int n;
	
	if(m_seatList) // If we have seats allocated, depends on Init(..) Server side.
	{
	for(n=0;n<m_seatList_n;n++) // Delete all Seats in list
	if(m_seatList[n]) delete m_seatList[n];
	delete m_seatList;	// delete list.
	}

	if(m_profileList) // If we have profile allocated, depends on AttachComm(..) Server side.
	{
	for(n=0;n<m_profileList_n;n++)
	if(m_profileList[n]) delete m_profileList[n];
	delete m_profileList;
	}

	if(m_CprofileList) // If we have profile allocated, depends on AttachComm(..) Server side.
	{
	for(n=0;n<m_CprofileList_n;n++)
	if(m_CprofileList[n]) delete m_CprofileList[n];
	delete m_CprofileList;
	}

	//if(m_stand)	delete m_stand;// ????
	//if(m_stand) m_stand = NULL; //CP
}

void CShell::Init(CWnd *pParentWnd, int players)
{
	int n;
	m_pParentWnd = pParentWnd;
	m_seatList = new CSeat* [players];
	m_seatList_n = players;
	for(n=0;n<m_seatList_n;n++)
	m_seatList[n] = NULL;
	
	m_stand = NULL;
}

void CShell::CreateSeat(int idx, int x, int y)
{
	AttachSeat(idx,new CSeat(m_pParentWnd,x,y));
}

void CShell::AttachSeat(int idx, CSeat *pSeat)
{	// Attaches Seat object, once attached CShell assumes responsibility for destruction.
	m_seatList[idx] = pSeat;
	pSeat->SetId(idx); // id is mapped according to index.
	pSeat->PassParent(this); // pass this to seat, so events can be called.
}

CSeat *CShell::DetachSeat(int idx)
{	// Detaches Seat object, so responsibility is returned to user (e.g. destruction)
	CSeat *pSeat;
	pSeat = m_seatList[idx];
	m_seatList[idx] = NULL;
	return pSeat;
}

CSeat *CShell::GetSeat(int idx)
{	// returns pointer to seat.
	return m_seatList[idx];
}

void CShell::SitRequest(int idx)
{	// Sends request to server to sit down.
	unsigned char buffer[2];

	buffer[0] = MSGCODE_SITREQUEST;
	buffer[1] = idx;

	Client->SendToQueue(buffer, 2);
}

void CShell::StandRequest(void)
{
	unsigned char buffer[1];

	buffer[0] = MSGCODE_STANDREQUEST;
	
	Client->SendToQueue(buffer, 1);
}

void CShell::OnReceiveSitRequest(int idx, unsigned char *pMsgData)
{
	char buffer[4];
	int seat_id;
	int n;
	seat_id = pMsgData[1];


	for(n=0;n<m_seatList_n;n++) // check all seats to see if this guy is still sitting down.
	if(m_seatList[n])
	if(!m_seatList[n]->IsEmpty())
	if(m_seatList[n]->m_idx==idx) return; // Already sat down!!

	if(m_seatList[seat_id]) // check the seat exists. we don't want crashing.
		{	
		if(m_seatList[seat_id]->IsEmpty()) 
			{
			m_seatList[seat_id]->Sit(idx);
			buffer[0] = MSGCODE_SITORDER;
			buffer[1] = idx;	// id of player who wishes to sit...
			buffer[2] = seat_id; // seat to sit in.
			Server->SendToQueue(buffer,3);
			}
		}
	//sprintf(buffer,"%d:Seat %d", idx, pMsgData[1]);
	//MessageBox(0,buffer,"--", MB_OK);
}

void CShell::OnCReceiveSitOrder(unsigned char *pMsgData)
{
	int seat_id;
	int idx;
	idx = pMsgData[1]; // place player idx into seat_id.
	seat_id = pMsgData[2];

	if(m_idx == idx) m_seat = seat_id;

	m_seatList[seat_id]->SetName(m_profileList[idx]->GetNick());//"Bobby");
	m_seatList[seat_id]->Sit(idx);
	//m_pParentWnd->RedrawWindow();
//	CDC *pDC = ((CGameAppView *)m_pParentWnd)->GetDC();
//	OnDraw(pDC, ((CGameAppView *)m_pParentWnd)->m_cx, ((CGameAppView *)m_pParentWnd)->m_cy);
//	((CGameAppView *)m_pParentWnd)->ReleaseDC(pDC);
	((CGameAppView *)m_pParentWnd)->Invalidate(); // causes an ondraw message
	#ifndef MAIN_SERVER
	((CGameAppView *)m_pParentWnd)->m_snooker->m_seat = m_seat;
	#endif //!MAIN_SERVER
}

void CShell::OnReceiveStandRequest(int idx, unsigned char *pMsgData)
{
	int n;
	unsigned char buffer[4];
	for(n=0;n<m_seatList_n;n++) // Check all seats for this guy, and make him stand if he's there.
	if(m_seatList[n])
	if(!m_seatList[n]->IsEmpty())
	if(m_seatList[n]->m_idx==idx)
					{
					m_seatList[n]->Stand(); // make him stand on server.
					buffer[0] = MSGCODE_STANDORDER;
					buffer[1] = n;
					Server->SendToQueue(buffer,2);
					return;
					}
}

void CShell::OnCReceiveStandOrder(unsigned char *pMsgData)
{
	int seat_id;
	//int idx;
	//idx = pMsgData[1];
	seat_id = pMsgData[1];
	if(m_seat==seat_id) 
		{
		m_seat = -1; // if our player is standing then we're back to -1. not a player.
		}
	m_seatList[seat_id]->Stand();
	//m_pParentWnd->RedrawWindow();
//	CDC *pDC = ((CGameAppView *)m_pParentWnd)->GetDC();
//	OnDraw(pDC, ((CGameAppView *)m_pParentWnd)->m_cx, ((CGameAppView *)m_pParentWnd)->m_cy);
//	((CGameAppView *)m_pParentWnd)->ReleaseDC(pDC);
	((CGameAppView *)m_pParentWnd)->Invalidate(); // causes an ondraw message
	#ifndef MAIN_SERVER
	((CGameAppView *)m_pParentWnd)->m_snooker->m_seat = m_seat;
	#endif //!MAIN_SERVER
}

void CShell::CreateStand(int x,int y)
{
	CRect rect;
	m_stand	= new CButtonEx;
	rect.SetRect(0,0,100,24);
	rect.OffsetRect(x,y);
	
	m_stand->SetId(100);
	m_stand->PassParent(this);
	m_stand->Create("Stand Up", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | WS_BORDER, rect, m_pParentWnd, 65001);
}

void CShell::CreateStand(int idx, CButtonEx *button)
{
	m_stand = button;
		
	m_stand->SetId(idx);
	m_stand->PassParent(this);
}

void CShell::OnEvent(int id)
{

	if(id<m_seatList_n)
		{
			// handle sending of sit/stand request via mainShell
			int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(
																  ((CGameAppView *)m_pParentWnd)->m_playerSockIdx);

			if(plrIdx!=-1)
			if( m_player[plrIdx].table!=-1) // must be in table (not pracitce table!!)
			{
				int tbl = m_player[plrIdx].table;
				int enter = ((CGameAppView *)m_pParentWnd)->m_tableEnter; // 0 - Watch / 1 - 1P Join / 2 - 2P Join
				int sockIdx = ((CGameAppView *)m_pParentWnd)->m_playerSockIdx;
				
				if(!enter) // check if player is a watcher?
				{
					if(id == 0)	((CGameAppView *)m_pParentWnd)->SendPlayerSit(FALSE, sockIdx, -1, tbl, -1, -1);
					else if(id == 1) ((CGameAppView *)m_pParentWnd)->SendPlayerSit(FALSE, -1, sockIdx, tbl, -1, -1);
					
				}
			}
		}
	else 
	switch(id)
		{
		case 100: // Stand Up
				int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(
																	  ((CGameAppView *)m_pParentWnd)->m_playerSockIdx);
		
				if(plrIdx!=-1)					
				if( m_player[plrIdx].table!=-1) // must be in table (not pracitce table!!)
				{
					int enter = ((CGameAppView *)m_pParentWnd)->m_tableEnter; // 0 - Watch / 1 - 1P Join / 2 - 2P Join
					if(enter) // check if player is NOT a watcher?
					{
						int tbl = m_player[plrIdx].table;
						int sockIdx = ((CGameAppView *)m_pParentWnd)->m_playerSockIdx;
						((CGameAppView *)m_pParentWnd)->SendPlayerSit(FALSE, sockIdx, sockIdx, tbl, sockIdx, sockIdx);
					}
				}
		break;
		}

  
}

void CShell::AttachComm(int idx, CComm *pComm)
{
	pComm->m_pShell = this;
	if(idx==SHELL_SERVER) Server = pComm;
	else Client = pComm;
	
	if(idx==SHELL_SERVER) // If Server, then we should allocate Profile capacity.
		{
		/*
		m_profileList = new CProfile* [pComm->m_socketList_n];
		m_profileList_n = pComm->m_socketList_n;
		for(int n=0;n<m_profileList_n;n++)
		m_profileList[n] = NULL;
		*/
		AllocateProfile(pComm->m_socketList_n);
		}
}

void CShell::Send(const void* lpBuf, int nBufLen)
{

}

/*
m_packetIdx = 0;	
	for(n=0; n<16; n++)
	{
		memset(m_packet[n], 256); // clear 256byte buffers
		m_packet_len[n] = 0;
	}
	*/

BOOL CShell::OnReceive(CComm *pComm, CAsyncSocketEx *pSocket, void* lpBuf, int nBufLen) // Whenever a COMPLETE Packet comes back
{

	if(g_bClose) return false;

	// This is the Big G!!!
	unsigned char *buffer;//[80];
	int buffer_len;
	unsigned char code;
	int plrSockIdx;
	int idx;

	buffer = (unsigned char *) lpBuf;
	buffer_len = nBufLen;
	plrSockIdx = pComm->GetSocketIndex(pSocket);	
	code = buffer[0];


	//if(!m_pParentWnd->wndTop) return false;

	int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(plrSockIdx);
	int plrTblIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetTableIdxFromPlayer(plrSockIdx);

			if((code&MSGMASK_CODE)== MSGCODE_GAME)
			{
			BOOL status;
			status = OnGameReceive(pComm, pSocket, buffer, buffer_len);
			pComm->m_ReceiveCnt++;
			return status;
			}

			if(pComm==Server) // if Server receives a text message, broadcast it out. - Text messages are ones with >=32 && 0.
			{				  // Special messages start with a code 1 or something.
			if(code&MSGMASK_BROADCAST)
				{
				Server->SendToQueue(buffer,buffer_len); // send out thru server slots. (broadcast message)
				}
			else 
			  { // process private message.
			switch(code&MSGMASK_CODE)
				{
				case MSGCODE_TEXT:	OnReceiveText(plrSockIdx, buffer, buffer_len, plrTblIdx);										
									break;
				case MSGCODE_TEXTCOLOR:OnReceiveTextColor(plrSockIdx, buffer, buffer_len, plrTblIdx);										
									break;
				case MSGCODE_PROFILE:
									OnReceiveProfile(plrSockIdx, (CMsgProfile *) buffer);
									break;
				case MSGCODE_SITREQUEST:
									OnReceiveSitRequest(plrSockIdx, buffer);
									break;
				case MSGCODE_STANDREQUEST:
									OnReceiveStandRequest(plrSockIdx, buffer);  
									break;
				
				case MSGCODE_SYSTEMTEXT:
									OnReceiveText(0xff, buffer, buffer_len);										
									break;
				case MSGCODE_REPORTABUSE:
									OnReceiveAbuse(buffer, buffer_len, plrIdx);
									break;
				} // end switch
			  } // end else
			} // end Server
			else if(pComm==Client)
			{ // if client receives a text message, then display it
			switch(code&MSGMASK_CODE)
				{
				case MSGCODE_TEXT:OnCReceiveText(buffer);
								  break;
				case MSGCODE_TEXTCOLOR:OnCReceiveTextColor(buffer);
								  break;
				case MSGCODE_PROFILE://ErrorMessage(m_pParentWnd, "MSGCODE_PROFILE: PRE");	
									OnCReceiveProfile((CMsgProfile *) buffer);
									//ErrorMessage(m_pParentWnd, "MSGCODE_PROFILE: POST");	
									break;
				case MSGCODE_ALLPROFILES://ErrorMessage(m_pParentWnd, "MSGCODE_ALLPROFILES: PRE");	
									OnCReceiveAllProfiles(buffer);
									//ErrorMessage(m_pParentWnd, "MSGCODE_ALLPROFILES: POST");
										 break;
				case MSGCODE_ALLPROFILESCHUNK:
									OnCReceiveAllProfilesChunk(buffer, buffer_len);
									break;
				case MSGCODE_DELETEPROFILE:	
											#ifndef MAIN_SERVER
											OnCReceiveDeleteProfile(buffer);
											#endif
											break;
				case MSGCODE_SITORDER:OnCReceiveSitOrder(buffer);
									  break;
				case MSGCODE_STANDORDER:OnCReceiveStandOrder(buffer);
									  break;
				case MSGCODE_SYSTEMTEXT:OnCReceiveText(buffer);
										break;
				case MSGCODE_SOCKETIDX:
										{
										//ErrorMessage(m_pParentWnd, "SOCKETIDX: PRE");
										// TODO: make this into function [+]
										idxData *idxdata;
										idxdata = (idxData_typ *) (void *)buffer;

										if(!idxdata->idx) break; // main server's own client (don't add this to player list)
										
										// save personal socket Idx (from main server's list)
										((CGameAppView *)m_pParentWnd)->m_playerSockIdx = idxdata->idx;

										//char buff[80];
										//sprintf(buff,"%d",idxdata->idx);
										//MessageBox(0,buff,"",MB_OK);
																		
										// get player's (client) local IP (LAN name)
										CString playerLocalIP;
										playerLocalIP = GetHostIPAddress(LOCAL_HOST_IP);
										
										Player plr;
										((CGameAppView *)m_pParentWnd)->m_lobby->ClearPlayer(&plr);
										//memset(&plr, 0, sizeof(Player_typ));
										if(((CGameAppView *)m_pParentWnd)->m_connectStyle==CONNECTSTYLE_NORMAL)
										{
										strcpy(plr.name, ((CGameAppView *)m_pParentWnd)->m_detailsDlg.m_name.GetBuffer(0));
										strcpy(plr.password, ((CGameAppView *)m_pParentWnd)->m_detailsDlg.m_password.GetBuffer(0));
										}
										else { // for creating new accounts/top ups.
										plr.name[0] = 0;
										plr.password[0] = 0;
										}
										

										plr.playerIP[0] = 0;
										strcpy(plr.playerLocalIP, playerLocalIP.GetBuffer(0));
										plr.playerPort = ((CGameAppView *)m_pParentWnd)->m_detailsSettingsDlg.m_myPort;
										plr.playerSocketIdx = idxdata->idx;
										// save socket index for use when/if needing to create a new account i.e. relogin
										((CGameAppView *)m_pParentWnd)->m_rememberPlayerSocketIdx = idxdata->idx;
										
										// sends crc aswell
										((CGameAppView *)m_pParentWnd)->SendPlayerDetailsConnect2(FALSE, &plr, -1, 0);
										}
										break;
				} // end switch
			} // end Client
		end:;	
		pComm->m_ReceiveCnt++;	
		return FALSE;
}

void CShell::OnSend(CComm *pComm, CAsyncSocketEx *pSocket) // Called whenever an attached Comm receives a message.
{// pComm: Comm group originator, pSocket: Actual Socket originator.
	   
}

void CShell::OnClose(CComm *pComm, int idx) // Called whenever an attached Comm closes
{// Note pSocket points to a former socket, that has been deleted. It serves only as reference.
	char buffer[80];
	int buffer_len;

	if(pComm==Client)
	{
		if(!m_bIsMainServer)
		{
			//MessageBox(0,"HERE!!!!CLOSURE PART for client!","",MB_OK);

			//if(!m_lobby->m_bInLobby) // If inside a game add one for extra!
			//	{
			//	((CGameAppView *)m_pParentWnd)->OnButtonExit(); // Kick out to lobby.
			//	}
			// Must be in lobby.
			//((CGameAppView *)m_pParentWnd)->OnButtonExit();


			//((CGameAppView *)m_pParentWnd)->OnSocketClose(this, pComm);
		}
		else
		{ 
			// You got disconnected from the main server. i.e. mainshell
			((CGameAppView *)m_pParentWnd)->ExitFromLobby(1);
		}

		//sprintf(buffer,"*** Disconnected From Server ***");
		//buffer_len = strlen(buffer)+1;
		//((CGameAppView *)m_pParentWnd)->Message(buffer,10); // personal message.
		
		//Server->Send(buffer,buffer_len); // THERE IS NO LONGER A SERVER TO RELAY MESSAGES, AND THIS MESSAGE IS GIVEN TO
										   // EVERYONE ANYWAY, SINCE THEY'RE ALL CONNECTED TO SERVER
		}
	else if(pComm==Server)
	{
		char buffer[256];
		char SendPLC = 0;

		sprintf(buffer,"OnClose(%d);",idx);
		AddLog("LogConnect.txt", buffer);
		// Clear Player's Name from Server's m_player list immediately //
	
		int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(idx);
		
		if(plrIdx != -1) 
		{
			// Reset Find Ladder Match status
			m_player[plrIdx].searchgamestatus = 0;
			m_player[plrIdx].textFormat = -2;

			 // Find out if we need to log this OnClose time //
			if(m_player[plrIdx].state&PLRSTATE_AUTHMASK || m_player[plrIdx].state2&PLRSTATE2_LOGTIME) 
			{
				// Inserts an entry into the logtime table for Logging Out //
			 	db_logtime(m_player[plrIdx].name, false);	
			}
		}

		


		
		//if(m_bIsMainServer) // main server only
		//{
		// send main server message that we've left the table (destroyed the game shell)
		//MessageBox(0,"Server - OnClose(...)","",MB_OK);
		if(((CGameAppView *)m_pParentWnd)->m_lobby->GetTableIdxFromPlayer(idx) != -1) // only do if at a table
		{
			//ErrorMessage(m_pParentWnd, "Onclose!!");
			// tell other player what happened, if involved in a game	
			int tblIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetTableIdxFromPlayer(idx);
			if(((CGameAppView *)m_pParentWnd)->m_lobby->GameStarted(idx))
			if(((CGameAppView *)m_pParentWnd)->m_lobby->PlayingAGame(idx))
			{	
				// This Code should always be done, if a game is in progress and a player loses connection //
				// If opponent is still at the Table, when a player disconnects, Update the Server and Opponent accordingly //
				// If opponent is also disconnected, then we need to End the Current Frame (gaveovermessage for all, and 
				// tell anyone at the table what has happened //
			

				// If this is a Double Match (UnRated), just end the Match if anyone disconnects from the game //
				if(m_table[tblIdx].doubles) ServerEndMatch(tblIdx);
				else
				{
					// Find Name of Opponent //
					int oppsocketIdx;
					char oppname[80];
					char Opponent_At_Table=0;
					
					if(m_player[plrIdx].playerSocketIdx == m_table[tblIdx].P1StartAtTable) strcpy(oppname, m_table[tblIdx].P2StartName);
						else strcpy(oppname, m_table[tblIdx].P1StartName);
					
					for(int n=0;n<MAX_LOBBY_PLAYERS;n++)
					if(m_player[n].status)
					if(!strcmpi(m_player[n].name, oppname))
					if(((CGameAppView *)m_pParentWnd)->m_lobby->GetTableIdxFromPlayer(m_player[n].playerSocketIdx) != -1) // only do if at a table	
					{
						Opponent_At_Table = 1;
						break;
					}

					if(Opponent_At_Table==1)
					{
						// Opponent is at the table still, do start Lost Connection code, to game can be continued //
						SendPLC = 1;
						ServerDoLostConnection(idx, m_player[plrIdx].name, tblIdx); // Server Code for Disconnected Players //
						((CGameAppView *)m_pParentWnd)->SendPlayerLostConnection(idx, m_player[plrIdx].name, tblIdx); // Opponent's code that is done for this Disconnection
					}
					else
					{
						// Opponent isn't at the table, so End this Frame / Rack and Notify any players of this at the table //
						ServerEndMatch(tblIdx);
					}
				}
			}

			((CGameAppView *)m_pParentWnd)->m_lobby->ExitTable(idx, FALSE, TRUE);		// Special, this will tell us not to Abort Match ///
			((CGameAppView *)m_pParentWnd)->SendPlayerExit(TRUE, idx, FALSE, TRUE);
			_skipExit:;
		}

		// Server should Delete Profile immediately, or there can be bugs when referring to this Closed socket (m_player)
		((CGameAppView *)m_pParentWnd)->m_lobby->RemovePlayer(idx);
		
		

		// If we didn't do a SendPLC, Update the clients, to Clear this profile - Not done immediately after OnClose //
		// If we did do SendPLC, then MSGCODE_DELETEPROFILE will get called up in the ReceivePLC code (at the Clients End at the correct time //
		// This is virtual, so no problem is called with m_PLCwait - if(LostConnectionReturned(TRUE))
		if(SendPLC == 0)
		{
			buffer[0] = MSGCODE_DELETEPROFILE;
			buffer[1] = idx;
			buffer_len = 2;
			Server->SendToQueue(buffer,buffer_len);		
		}
	}
}

void CShell::OnOpen(CComm *pComm)	// Called upon Socket Opening.
{
	//char buffer[80];
	//int buffer_len;

	if(pComm==Client)
		{
			//sprintf(buffer,"*** %s has joined the table ***", &m_nick);
			//buffer_len = strlen(buffer)+1;
			//((CGameAppView *)m_pParentWnd)->Message(buffer,10); // personal message
			
			/*
			#ifdef MAIN_SERVER
			SetNick("[Server]");
			SendProfile();
			#else
			// idle...
			#endif //MAIN_SERVER
			*/
		}
	else if(pComm==Server)
		{
			//sprintf(&buffer[1],"# Client Connected"); // personal server info, but we're letting everyone know via broadcast.
			//buffer[0] = MSGCODE_TEXT;
			//buffer_len = strlen(&buffer[1])+2;
			//Server->Send(buffer,buffer_len);
			//void CShell::BroadcastAllProfiles(void)
		}
}

BOOL CShell::OnGameReceive(CComm *pComm, CAsyncSocketEx *pSocket, void *lpBuf, int nBufLen)
{
	return FALSE;
}

void CShell::OnMainReceive(CComm *pComm, CAsyncSocketEx *pSocket, void *lpBuf, int nBufLen)
{
}


void CShell::ChatSend(CString& strText)
{
	//ChatSend(strText.GetBuffer(0));
}

void CShell::ChatSend(char *str)
{
	/*
	unsigned char buffer[MAX_SENDEDITCHARS];

	sprintf((char *)&buffer[2],"%s",str); // Now clients only send their string.

	buffer[0] = MSGCODE_TEXT;
	buffer[1] = 0; // unused. (used as an id tag when server broadcasts out your message - server knows all id's)
	
	Client->SendToQueue(buffer, strlen((char *)&buffer[2])+3); // +1 for Ter, +1 for Code at beginning +1 for sender gap.
	*/
}

int g_hotnonecount = 0;

void CShell::OnAccept(CAsyncSocketEx *pSocket) // Called whenever a listening socket (Server) accepts a Client socket.
{
	CAsyncSocketEx* pSocketAccept = new CAsyncSocketEx();
	
	if (pSocket->Accept(*pSocketAccept))
	{

		/*{
			CString rPeerAddress;
			UINT rPeerPort;
			pSocketAccept->GetSockName(rPeerAddress, rPeerPort);
			char buff[80];
			sprintf(buff, "*** (Peer) IPAddress = %s", rPeerAddress);
			MessageBox(0,buff,"",MB_OK);
		}*/


		if(m_bIsMainServer)
		{
			CString rPeerAddress;
			UINT rPeerPort;
			char buffer[256];
			// ip
			pSocketAccept->m_state = SOCKET_HANDSHAKE;
			pSocketAccept->GetPeerName(rPeerAddress, rPeerPort);


			if(((CGameAppView *)m_pParentWnd)->m_IPConnectCoolDown) // make sure it's been created
			{
				int s;
				s = ((CGameAppView *)m_pParentWnd)->m_IPConnectCoolDown->Check(rPeerAddress.GetBuffer(0));
				if(s!=-1)
				{
				if(s!=-2) // if it isn't an uber too frequent one then say it's too hot!
				{
				if(g_hotnonecount)
					{
					sprintf(buffer,"IPTooHot(%s:%d) %d seconds [%d NC's];",rPeerAddress, rPeerPort, s, g_hotnonecount);
					AddLog("LogConnect.txt", buffer);
					}
					else 
					{
					sprintf(buffer,"IPTooHot(%s:%d) %d seconds;",rPeerAddress, rPeerPort, s);
					AddLog("LogConnect.txt", buffer);
					}
				g_hotnonecount = 0;
				} else {
				g_hotnonecount++;
				}

				pSocketAccept->Close();
				delete pSocketAccept;
				return;			
				}
			}

			
			idxData idxdata;
			idxdata.idx = Server->AttachSocket(pSocketAccept);
			idxdata.buffer[0] = MSGCODE_SOCKETIDX; // crash point (last message).

			sprintf(buffer,"OnAccept(%d,%s:%d);",idxdata.idx,rPeerAddress, rPeerPort);
			AddLog("LogConnect.txt", buffer);

#ifdef MAIN_SERVER
			// If we are the Server, then clean up the logtime table in our database //
			db_logtimecleanup();			
#endif

			if(idxdata.idx!=-1)
				Server->SendToQueue((void *)&idxdata, sizeof(idxData_typ), idxdata.idx);
			else
				delete pSocketAccept;
			//pSocketAccept->OnOpen(); // Generate Open Message
		}
		else
		{
			Server->AttachSocket(pSocketAccept);
			pSocketAccept->OnOpen(); // Generate Open Message
		}


		
	}
	else delete pSocketAccept;
}

// Profile Functions

void CShell::AllocateProfile(int players)
{
		// Server's Database
		m_profileList = new CProfile* [players];
		m_profileList_n = players;
		for(int n=0;n<m_profileList_n;n++)
		m_profileList[n] = NULL;

		// Client's Database.
		m_CprofileList = new CProfile* [players];
		m_CprofileList_n = players;
		for(int n=0;n<m_CprofileList_n;n++)
		m_CprofileList[n] = NULL;
}

int CShell::SearchForPlayerInSeat(int idx)
{
	int n;
	for(n=0;n<m_seatList_n;n++) // check all seats to see if this guy is sitting down.
	if(m_seatList[n])
	if(!m_seatList[n]->IsEmpty())
	if(m_seatList[n]->m_idx==idx) return n; // Yeah he's sitting down.
	return -1;
}


void CShell::BroadcastAllProfilesToOne(int idx)
{
	char g_buffer[4096];
	char *str;
	int n;
	//int i;
	int len;
	str = (char *) g_buffer;//g_receiveBuffer;
	*str = MSGCODE_ALLPROFILES;
	str++;
	*str = idx; // pass thru player index, so they know!!
	str++;
	
	for(n=0;n<m_profileList_n;n++)
	if(m_profileList[n])
	{
	//*str = '0'+n;
	//str++;
		*str = n;
		str++;
		*str = SearchForPlayerInSeat(n); // Pass over the seat index.
		str++;
	strcpy(str, m_profileList[n]->GetNick());
	while(*str!=0) str++;
	//*str = '.';
	*str=0;
	str++;
	}
	*str = -1; // terminate.
	str++;
	len = (int) str- (int) g_buffer;//g_receiveBuffer;	


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// send all profiles data in small chunks of 248bytes (to adhere to the 254bytes packet size constraint)
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	char buffer[248]; // [1=msgtype][3=padd][4=length][chunk=240]
	int size;
	int offset = 0;
	int length = len;
	int *v = (int *)&buffer[4];
	
	while(length > 0)
	{		
		if(length > 240)
			size = 240;
		else
			size = length;

		buffer[0] = MSGCODE_ALLPROFILESCHUNK;
		*v = len;
		
		memcpy(&buffer[8], &g_buffer[offset], size);
		Server->SendToQueue(buffer, 8+size, idx);
		
		length -= size;
		offset += size;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////


							//	Server->m_socketList[idx]->Send(g_buffer/*g_receiveBuffer*/,len);
//	Server->SendToQueue(g_buffer, len, idx);
							//str = (char *) g_receiveBuffer;
							//sprintf(str, "<%d>", len);
							//((CGameAppView *)m_pParentWnd)->Message(str);//(char *) g_receiveBuffer); // personal message	
}

void CShell::BroadcastProfileAllButOne(CProfile *pProfile, int idx)
{ // Server does this broadcasting.
	//unsigned char buffer[80];
	//CSocket **m_socketList;
	//int m_socketList_n;
	int n;
	for(n=0;n<Server->m_socketList_n;n++) // send it too all
	if(Server->m_socketList[n]) // [make sure the socket is there]
	if(n!=idx) // but idx.
		{
		//Server->m_socketList[n]->Send(pProfile->GetData(),sizeof(CMsgProfile));
		Server->SendToQueue(pProfile->GetData(), sizeof(CMsgProfile), n);
		}
	//Server->Send(pProfile->GetData(),sizeof(CMsgProfile));
}

void CShell::ShowNameList(void)
{
	int n;
	/*
	for(n=0;n<m_CprofileList_n;n++)
	if(m_CprofileList[n])
	{
	((CGameAppView *)m_pParentWnd)->Message(m_CprofileList[n]->GetNick());
	}
	*/
	for(n=0;n<m_profileList_n;n++)
	if(m_profileList[n])
	{
	((CGameAppView *)m_pParentWnd)->Message(m_profileList[n]->GetNick());
	} 
}

void CShell::SetNick(CString& strText)
{
	strcpy(m_nick,strText.GetBuffer(0));
}

void CShell::SetNick(char *str)
{
	strcpy(m_nick,str);
}

void CShell::SendProfile(void)
{
	CMsgProfile msgprofile;

	msgprofile.code = MSGCODE_PROFILE;
	strcpy(msgprofile.nick, m_nick);

	Client->SendToQueue(&msgprofile, sizeof(CMsgProfile));
}

void CShell::OnReceiveProfile(int idx, CMsgProfile *pMsgProfile)
{ // Server Handles this  idx = socket index where profile came from.
		//char buffer[80];
		//int n;
//		sprintf(buffer,"< (%d:Profile:%s)",idx,pMsgProfile->nick);
		
		
		if(m_profileList==NULL) {
								ErrorMessage(m_pParentWnd, "Can't Add Profile (Server/Host)");
								//MessageBox(0,"Can't Add Profile","Error",MB_OK);
								return;
								}

		if(m_profileList[idx]) delete m_profileList[idx]; // delete previous profile stored in this slot.

		pMsgProfile->idx = idx; // set index.
		m_profileList[idx] = new CProfile();
		m_profileList[idx]->AddMsgProfile(pMsgProfile);
//		((CGameAppView *)m_pParentWnd)->Message(buffer); // personal message	
		//BroadcastAllProfiles(); // Now broadcast the new profile line up to all clients.	

		
		BroadcastProfileAllButOne(m_profileList[idx],idx); // Tell everyone about the new profile, so they can add it to their current
											  // lists. Tell everyone but one (socket idx), coz socket idx needs to know the big update!!
		BroadcastAllProfilesToOne(idx); // Now broadcast the new profile line up to all clients.
}

void CShell::OnReceiveAbuse(unsigned char *buffer, int buffer_len, int plrIdx)
{
#ifdef MAIN_SERVER
	FILE *fptr;
	fptr = fopen("LogAbuse.txt","a+b");
	char tableinfo[80];

	// Find out if this Abuse was reported at a Table and if so, whether it was Private or Public //
	int tblIdx = m_player[plrIdx].table;

	if( tblIdx == -1) sprintf(tableinfo, "[Lobby]");
	else{
		if(!m_table[tblIdx].type) sprintf(tableinfo, "[Table %d (Public)]", tblIdx+1);	// Public Table 
			else  sprintf(tableinfo, "[Table %d (Public)]", tblIdx+1);	// Private Table 
		}


	if(buffer[1]==0) // Reset - that means show the player index.
	{
		((CGameAppView *)m_pParentWnd)->GetTime();
		fprintf(fptr,"### %s - Abuse Reported by '%s' at %s on %s:\r\n", 
		tableinfo,
		m_player[plrIdx].name,
		((CGameAppView *)m_pParentWnd)->m_stringFullTime,
		((CGameAppView *)m_pParentWnd)->m_stringShortDate); // composite log contains every thing so need the tblidx							

	}
	fprintf(fptr,"%s\r\n", (char *) &buffer[2]);

	fclose(fptr);
#endif
}

void CShell::OnReceiveTextColor(int idx, unsigned char *pMsgData, int len, int tblIdx)
{ // Server Handles this
	pMsgData[1] = idx; // set message owner to idx. This is the only alteration we make to the message.
						// we can also descrimate messages according to sender.

	int plrIdx;

	plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(idx);
	if(plrIdx!=-1)
	{
	//if(m_player[plrIdx].state2&PLRSTATE2_MUTE) return; // messsages from this player have been muted!
		if(m_player[plrIdx].state2&PLRSTATE2_MUTE)
		{
		unsigned char buffer[256];
		sprintf((char *)&buffer[5],"Can't send messages, you have been muted!");
		buffer[0] = MSGCODE_TEXT;
		buffer[1] = 0xfd; // unused. (used as an id tag when server broadcasts out your message - server knows all id's)

		if(mainShell) mainShell->Server->SendToQueue(buffer, strlen((char *)&buffer[5])+6, idx); // +1 for Ter, +1 for Code at beginning +1 for sender gap + 3 for text color.
		return;
		}
	}
	

	#ifdef MAIN_SERVER
	if(tblIdx!=-1 && g_config.m_logTableChat)
	{
	char buffer[80];
	sprintf(buffer,"LogTable%d.txt",tblIdx+1);
	FILE *fptr;
	fptr = fopen(buffer,"a+b");
	LogTextColor(fptr, pMsgData);
	fclose(fptr);


	sprintf(buffer,"LogTableAll.txt"); // composite log contains every thing so need the tblidx
	fptr = fopen(buffer,"a+b");
	LogTextColor(fptr, pMsgData, tblIdx);
	fclose(fptr);	
	}
	#endif
	SendToQueueTableX(Server, pMsgData, len, tblIdx);
}

void CShell::OnReceiveText(int idx, unsigned char *pMsgData, int len, int tblIdx)
{ // Server Handles this
	pMsgData[1] = idx; // set message owner to idx. This is the only alteration we make to the message.
						// we can also descrimate messages according to sender.

	int plrIdx;

	plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(idx);
	if(plrIdx!=-1)
	{
	//if(m_player[plrIdx].state2&PLRSTATE2_MUTE) return; // messsages from this player have been muted!
		if(m_player[plrIdx].state2&PLRSTATE2_MUTE)
		{
		unsigned char buffer[256];
		sprintf((char *)&buffer[2],"Can't send messages, you have been muted!");
		buffer[0] = MSGCODE_TEXT;
		buffer[1] = 0xfd; // unused. (used as an id tag when server broadcasts out your message - server knows all id's)

		if(mainShell) mainShell->Server->SendToQueue(buffer, strlen((char *)&buffer[2])+3, idx); // +1 for Ter, +1 for Code at beginning +1 for sender gap.
		return;
		}
	}
	

	#ifdef MAIN_SERVER
	if(tblIdx!=-1 && g_config.m_logTableChat)
	{
	char buffer[80];
	sprintf(buffer,"LogTable%d.txt",tblIdx+1);
	FILE *fptr;
	fptr = fopen(buffer,"a+b");
	LogText(fptr, pMsgData);
	fclose(fptr);


	sprintf(buffer,"LogTableAll.txt"); // composite log contains every thing so need the tblidx
	fptr = fopen(buffer,"a+b");
	LogText(fptr, pMsgData, tblIdx);
	fclose(fptr);	
	}
	#endif
	SendToQueueTableX(Server, pMsgData, len, tblIdx);
}

void CShell::LogText(FILE *fptr, unsigned char *pMsgData, int tblIdx)
{
	char buffer[256];
	int idx;

	buffer[0] = 0;

	idx = pMsgData[1];
	if(idx==0xff || idx==0xfe || idx==0xfd) sprintf(buffer,"%s",(char *) &pMsgData[2]); // 0xff id is no id.
	else 
	{
		if(idx == 0) // server's 'internal' child socket idx
		{
			sprintf(buffer,"%s: %s", g_serverHandle.GetBuffer(0), (char *) &pMsgData[2]);
		}
		else // all other 'external' clients to server
		{
			int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(idx);
			if(plrIdx!=-1)
			{
				if(tblIdx!=-1) sprintf(buffer,"[%d]%s: %s",tblIdx+1, m_player[plrIdx].name,(char *) &pMsgData[2]);
				else sprintf(buffer,"%s: %s",m_player[plrIdx].name,(char *) &pMsgData[2]);
			}
			else return;
		}
	}
	fprintf(fptr,"%s\r\n",buffer);
}

void CShell::LogTextColor(FILE *fptr, unsigned char *pMsgData, int tblIdx)
{
	char buffer[256];
	int idx;

	buffer[0] = 0;

	idx = pMsgData[1];
	if(idx==0xff || idx==0xfe || idx==0xfd) sprintf(buffer,"%s",(char *) &pMsgData[5]); // 0xff id is no id.
	else 
	{
		if(idx == 0) // server's 'internal' child socket idx
		{
			sprintf(buffer,"%s: %s", g_serverHandle.GetBuffer(0), (char *) &pMsgData[5]);
		}
		else // all other 'external' clients to server
		{
			int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(idx);
			if(plrIdx!=-1)
			{
				if(tblIdx!=-1) sprintf(buffer,"[%d]%s: %s",tblIdx+1, m_player[plrIdx].name,(char *) &pMsgData[5]);
				else sprintf(buffer,"%s: %s",m_player[plrIdx].name,(char *) &pMsgData[5]);
			}
			else return;
		}
	}
	fprintf(fptr,"%s\r\n",buffer);
}

void ExtractName(char *dest, char *src)
{
	char *str = &src[5];
	for(int n=0;n<80;n++)
	{
		dest[n] = str[n];
		if(dest[n] == ':')
			dest[n] = 0;

		if(!dest[n])
			return;
	}

	dest[0] = 0;
}

// Client //
void CShell::OnCReceiveText(unsigned char *pMsgData)
{ // C

	// don't display server text when in a table (i.e. in a game server)
	// display sent text
	//char buffer[256];
	char *buffer;
	char labelName[80]; // max name length 30 chars + 1 for ':'
	int idx;
	BOOL bIsAdmin = FALSE;
	BOOL bIsMod = FALSE;
	BOOL bIsMember = FALSE;
	int len;

	len = strlen((char *) &pMsgData[2]);
	len+=256;
	buffer = new char[len];

	idx = pMsgData[1];
	
	


	// handle ignore if send is not a Moderator / Admin
	
	if(idx==0xff || idx==0xfc)
	{
		char tmpStr[80];
		sscanf((char *) &pMsgData[3], "%s", tmpStr);
		if( !strcmpi("From", tmpStr) )
		{
			ExtractName(tmpStr, (char *) &pMsgData[3]);

			/*
			#define MODTYPE_NONE			-1	// Anyone who ain't on the moderator list.
			#define MODTYPE_ADMIN			0	// 
			#define MODTYPE_LEADMODERATOR	1	// *
			#define MODTYPE_SMODERATOR		2	// ^
			#define MODTYPE_MODERATOR		3
			*/

			int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->FindPlayerIdx(tmpStr);
			if(plrIdx!=-1)
			if(m_player[plrIdx].playertype < 0 || m_player[plrIdx].playertype > 3)
			if(m_player[plrIdx].bIgnore) // if the sender has been ignored, unless they aren't a Moderator
			{
				delete buffer;
				return;
			}
		}

		/*
		else if( !strcmpi("To", tmpStr) )
		{
			ExtractName(tmpStr, (char *) &pMsgData[1]);

			int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->FindPlayerIdx(tmpStr);
			if(plrIdx!=-1)
			// if(m_player[plrIdx].playertype < 0 || m_player[plrIdx].playertype > 3)
			if(m_player[plrIdx].bIgnore) // if the sender has been ignored
			{
				delete buffer;
				// Displayed Ignored Message
				((CGameAppView *)m_pParentWnd)->Message("You are ignored by this player\n", -2);	
				return;
			}
		}
		*/ 
	}

	int senderLevel = 99;
	if(idx==0xfc) 
	{
		// Get level of Sender
		senderLevel = pMsgData[2];
		sprintf(buffer,"%s",(char *) &pMsgData[3]); // 0xfc id is includes level

		char tmpStr[80];
		sscanf((char *) &pMsgData[3], "%s", tmpStr);
		if( !strcmpi("From", tmpStr) )
		{
			ExtractName(tmpStr, (char *) &pMsgData[3]);
			// Lets Store the Sender's Username in our g_RecentMessengerName Buffer
			g_RecentMessengerName = new char[strlen(tmpStr)+1];
			strcpy(g_RecentMessengerName, tmpStr);
		}
	}
	else if(idx==0xff || idx==0xfe || idx==0xfd) sprintf(buffer,"%s",(char *) &pMsgData[2]); // 0xff id is no id.
	else 
	{
		if(idx == 0) // server's 'internal' child socket idx
		{
			sprintf(buffer,"%s: %s",g_serverHandle.GetBuffer(0), (char *) &pMsgData[2]);
		}
		else // all other 'external' clients to server
		{
			int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(idx);
			if(plrIdx!=-1)
			{
				//sprintf(buffer,"%s: %s",m_player[plrIdx].name,(char *) &pMsgData[2]);
				
				sprintf(labelName,"-> ");
				sprintf(buffer,"%s: %s",m_player[plrIdx].name,(char *) &pMsgData[2]);

				//sprintf(labelName, "%s:",m_player[plrIdx].name);
				//sprintf(buffer," %s",(char *) &pMsgData[2]);
				
				//find if Admin player
				bIsAdmin = FALSE;
				if( (m_player[plrIdx].state&PLRSTATE_AUTHMASK) == PLRSTATE_ADMIN )
				{
					bIsAdmin = TRUE;
				}

				//find if moderator player
				bIsMod = FALSE;
				if( (m_player[plrIdx].state&PLRSTATE_AUTHMASK) == PLRSTATE_MODERATOR ||
					(m_player[plrIdx].state&PLRSTATE_AUTHMASK) == PLRSTATE_LEADMODERATOR)
				{
					bIsMod = TRUE;
				}

				//find if Member player
				bIsMember = FALSE;
				if( !bIsMod && !bIsAdmin && !(m_player[plrIdx].state&PLRSTATE_GUESTMEMBER) )
					{
					bIsMember = TRUE;
					}


			}
			else {
				 delete buffer;
				 return;
				 }
		}
		
	}

	if( ((CGameAppView *)m_pParentWnd)->m_lobby->m_bInLobby && idx == 0 ) // main server messages
	{
		sprintf(labelName,"-> ");
		((CGameAppView *)m_pParentWnd)->Message(labelName, -1);
		((CGameAppView *)m_pParentWnd)->Message(buffer,20);
	}
	else if(idx==0xff) // system messages
		((CGameAppView *)m_pParentWnd)->Message(buffer,10);
	else if(idx==0xfe) // system messages that can get you from anywhere!!! i.e. /msg stuff
	{
		sprintf(labelName,"-> ");
		((CGameAppView *)m_pParentWnd)->Message(labelName, -2);		
		((CGameAppView *)m_pParentWnd)->Message(buffer,30);
	}
	else if(idx==0xfd) // system messages that can get you from anywhere!!! e.g. error stuff
		((CGameAppView *)m_pParentWnd)->Message(buffer,40);
	else if(idx==0xfc) // /msg that includes level
		((CGameAppView *)m_pParentWnd)->Message(buffer,40,-1, 14803425, senderLevel);
	else  // player messages
	{
		// check if we should display this players chat text?
		int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(idx);
		if(plrIdx!=-1)
		if(!m_player[plrIdx].bIgnore)
		{
			//if(bIsAdmin) // Admin only
			if(bIsAdmin || bIsMod || bIsMember) // Admin & moderators only
			{
				//((CGameAppView *)m_pParentWnd)->Message(buffer, 0, idx);
				((CGameAppView *)m_pParentWnd)->Message(labelName, -1, idx);
				((CGameAppView *)m_pParentWnd)->Message(buffer, 0, idx);
			}
			else // rated/guest players (everyone else)
			{
				//((CGameAppView *)m_pParentWnd)->Message(buffer);			
				((CGameAppView *)m_pParentWnd)->Message(labelName, -1);
				((CGameAppView *)m_pParentWnd)->Message(buffer);
			}
		}		
	}

	delete buffer;
}

void CShell::OnCReceiveTextColor(unsigned char *pMsgData)
{ // C

	// don't display server text when in a table (i.e. in a game server)
	// display sent text
	//char buffer[256];
	char *buffer;
	char labelName[80]; // max name length 30 chars + 1 for ':'
	int idx;
	BOOL bIsAdmin = FALSE;
	BOOL bIsMod = FALSE;
	BOOL bIsMember = FALSE;
	BOOL bIsTrialist = FALSE;
	int len;

	len = strlen((char *) &pMsgData[5]);
	len+=256;
	buffer = new char[len];

	idx = pMsgData[1];
	
	// Get Text Color
	int col;
	col = pMsgData[2];
	col |= ((int)(pMsgData[3]))<<8;
	col |= ((int)(pMsgData[4]))<<16;




	/*
	#define MODTYPE_NONE			-1	// Anyone who ain't on the moderator list.
	#define MODTYPE_ADMIN			0	// 
	#define MODTYPE_LEADMODERATOR	1	// *
	#define MODTYPE_SMODERATOR		2	// ^
	#define MODTYPE_MODERATOR		3
	*/


	// handle ignore player /msg
	
	int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(idx);
	if(plrIdx!=-1)
	if(m_player[plrIdx].playertype < 0 || m_player[plrIdx].playertype > 3)
	if(m_player[plrIdx].bIgnore) // if the inviter has been ignored
	{
		delete buffer;
		return;
	}
	
	

	if(idx==0xff || idx==0xfe || idx==0xfd) sprintf(buffer,"%s",(char *) &pMsgData[5]); // 0xff id is no id.
	else 
	{
		if(idx == 0) // server's 'internal' child socket idx
		{
			sprintf(buffer,"%s: %s",g_serverHandle.GetBuffer(0), (char *) &pMsgData[5]);
		}
		else // all other 'external' clients to server
		{
			int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(idx);
			if(plrIdx!=-1)
			{
				//sprintf(buffer,"%s: %s",m_player[plrIdx].name,(char *) &pMsgData[2]);
				
				sprintf(labelName,"-> ");
				sprintf(buffer,"%s: %s",m_player[plrIdx].name,(char *) &pMsgData[5]);

				//sprintf(labelName, "%s:",m_player[plrIdx].name);
				//sprintf(buffer," %s",(char *) &pMsgData[2]);
				
				//find if Admin player
				bIsAdmin = FALSE;
				if( (m_player[plrIdx].state&PLRSTATE_AUTHMASK) == PLRSTATE_ADMIN )
				{
					bIsAdmin = TRUE;
				}

				//find if moderator player
				bIsMod = FALSE;
				if( (m_player[plrIdx].state&PLRSTATE_AUTHMASK) == PLRSTATE_MODERATOR ||
					(m_player[plrIdx].state&PLRSTATE_AUTHMASK) == PLRSTATE_LEADMODERATOR)
				{
					bIsMod = TRUE;
				}
			
				bIsTrialist = m_player[plrIdx].state&PLRSTATE_GUESTMEMBER;

				//find if Member player
				bIsMember = FALSE;
				if( !bIsMod && !bIsAdmin && !bIsTrialist )
				{
					bIsMember = TRUE;
				}


			}
			else {
				 delete buffer;
				 return;
				 }
		}
		
	}

	if( ((CGameAppView *)m_pParentWnd)->m_lobby->m_bInLobby && idx == 0 ) // main server messages
	{
		sprintf(labelName,"-> ");
		((CGameAppView *)m_pParentWnd)->Message(labelName, -1);
		((CGameAppView *)m_pParentWnd)->Message(buffer, 20);
	}
	else if(idx==0xff) // system messages
		((CGameAppView *)m_pParentWnd)->Message(buffer, 10);
	else if(idx==0xfe) // system messages that can get you from anywhere!!! i.e. /msg stuff
	{
		sprintf(labelName,"-> ");
		((CGameAppView *)m_pParentWnd)->Message(labelName, -2);		
		((CGameAppView *)m_pParentWnd)->Message(buffer,30);
	}
	else if(idx==0xfd) // system messages that can get you from anywhere!!! e.g. error stuff
		((CGameAppView *)m_pParentWnd)->Message(buffer,40);
	else  // player messages
	{
		// check if we should display this players chat text?
		int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(idx);
		if(plrIdx!=-1)
		{
			//if(bIsAdmin) // Admin only
			if(bIsAdmin || bIsMod || bIsMember || bIsTrialist) // Admin & moderators only
			{
				//((CGameAppView *)m_pParentWnd)->Message(buffer, 0, idx);
				((CGameAppView *)m_pParentWnd)->Message(labelName, -1, idx);
				((CGameAppView *)m_pParentWnd)->Message(buffer, 0, idx, col);
			}
			else // rated/guest players (everyone else)
			{
				//((CGameAppView *)m_pParentWnd)->Message(buffer);			
				((CGameAppView *)m_pParentWnd)->Message(labelName, -1);
				((CGameAppView *)m_pParentWnd)->Message(buffer, 0, -1, col);
			}
		}		
	}

	delete buffer;
}


void CShell::OnCReceiveProfile(CMsgProfile *pMsgProfile)
{ // C

		//char buffer[80];
		//int n;
		int idx;
		char *timestr;

		// Get (Local) time
		((CGameAppView *)m_pParentWnd)->GetTime();
		timestr = ((CGameAppView *)m_pParentWnd)->m_stringFullTime.GetBuffer(0);

		idx = pMsgProfile->idx;
//		sprintf(buffer,"< (%d:Profile:%s:)",pMsgProfile->idx, pMsgProfile->nick);

		if(m_profileList==NULL) {
								ErrorMessage(m_pParentWnd, "Can't Add Profile (Client)");
								//MessageBox(0,"Can't Add Profile","Error",MB_OK);
								return;
								}

		if(m_profileList[idx]) delete m_profileList[idx]; // delete previous profile stored in this slot.

		pMsgProfile->idx = idx; // set index.
		m_profileList[idx] = new CProfile();
		m_profileList[idx]->AddMsgProfile(pMsgProfile);

//		((CGameAppView *)m_pParentWnd)->Message(buffer); // personal message	
		UpdateNameList();

	#ifdef MAIN_SERVER
		// display text for player joining
		char _buffer[MAX_CHARS];
		//if(((CGameAppView *)m_pParentWnd)->m_lobby->m_bInLobby)

		//sprintf(_buffer,"*** %s has joined the lobby ***", m_profileList[idx]->GetNick());
		sprintf(_buffer,"*** %s joined the lobby at %s ***", m_profileList[idx]->GetNick(),timestr);		
		
		//else
		//	sprintf(_buffer,"*** %s has joined the table ***", m_profileList[idx]->GetNick());

		if(idx == 0)
			((CGameAppView *)m_pParentWnd)->Message(_buffer,20); // personal message
		else 
			((CGameAppView *)m_pParentWnd)->Message(_buffer,10); // personal message
	#else
		// display text for player joining
		char _buffer[MAX_CHARS];
		if(((CGameAppView *)m_pParentWnd)->m_lobby->m_bInLobby)
		{
			if(idx == ((CGameAppView *)m_pParentWnd)->m_playerSockIdx)
			{
				//sprintf(_buffer,"*** %s has joined the lobby ***", m_profileList[idx]->GetNick());
				sprintf(_buffer,"*** %s joined the lobby at %s ***", m_profileList[idx]->GetNick(),timestr);
				((CGameAppView *)m_pParentWnd)->Message(_buffer,10); // personal message
			}
		}
		else // player inside a table, so just check for other players joining this table
		{
			if(!m_bIsMainServer)
			{
				//MessageBox(0, "1", "1", MB_OK);
				//sprintf(_buffer,"*** %s has joined the table ***", m_profileList[idx]->GetNick());	
				sprintf(_buffer,"*** %s has joined the table ***", m_profileList[idx]->GetNick());	
				((CGameAppView *)m_pParentWnd)->Message(_buffer,10); // personal message
			}
		}
		
	#endif //MAIN_SERVER
}

void CShell::OnCReceiveAllProfiles(unsigned char *buffer)
{ // C
		
	//char buff[80];
	//sprintf(buff,"*** %s has left the table ***", &m_profileList[idx]->data.nick);
	//MessageBox(0, buff, "", MB_OK);	
	//MessageBox(0, "1", "2", MB_OK);		

	char *str;
	int idx;
	int seat_id;
	unsigned char datapacket[4];
	int n;

	char *timestr;

	// Get (Local) time
	((CGameAppView *)m_pParentWnd)->GetTime();
	timestr = ((CGameAppView *)m_pParentWnd)->m_stringFullTime.GetBuffer(0);

		
	m_idx = buffer[1];
	//char _buffer[80];
	//sprintf(_buffer,"You're #%d",m_idx);
	//MessageBox(0,_buffer,"-",MB_OK);
	//((CGameAppView *)m_pParentWnd)->Message(_buffer); // personal message
		
	str = (char *) &buffer[2];
	 
	for(n=0;n<m_profileList_n;n++) // NOTE: not an entity scan, just using this max const as a safety loop.
	{
	idx = *str;
	str++;
	seat_id = *str;
	str++;

	m_profileList[idx] = new CProfile();
	strcpy(m_profileList[idx]->GetNick(), str);
	m_profileList[idx]->GetData()->idx = idx;

	if(seat_id!=-1)
	{
	m_seatList[seat_id]->SetName(m_profileList[idx]->GetNick());
	m_seatList[seat_id]->Sit(idx);
	m_pParentWnd->RedrawWindow();
	}

	//m_profileList[idx]->AddMsgProfile(pMsgProfile);
	//sprintf(_buffer,"%d:(%s)",idx,str);
	//((CGameAppView *)m_pParentWnd)->Message(_buffer); // personal message	
	
	while(*str!=0) str++;
	str++;
	if(*str==-1) break; // finished.
	}

	// Let Server know that we're in the game properly, and would like a little info of what's been happening.
	// From here on it's handled by the game custom code.
	datapacket[0] = MSGCODE_GAME;
	datapacket[1] = 0xff; // Open flag.
	datapacket[2] = 0;
	datapacket[3] = 0;
	
	Client->SendToQueue(datapacket,4);

	UpdateNameList();
	m_pParentWnd->RedrawWindow();
	
//	// display text for player joining table
//	char _buffer[80];
//	sprintf(_buffer,"*** %s has joined the table ***", m_profileList[m_idx]->GetNick());
//	((CGameAppView *)m_pParentWnd)->Message(_buffer,10); // personal message

	#ifdef MAIN_SERVER
		// display text for player joining
		char _buffer[MAX_CHARS];
		//if(((CGameAppView *)m_pParentWnd)->m_lobby->m_bInLobby)
		//sprintf(_buffer,"*** %s has joined the lobby ***", m_profileList[idx]->GetNick());
		sprintf(_buffer,"*** %s joined the lobby at %s ***", m_profileList[idx]->GetNick(),timestr);
		//else
		//	sprintf(_buffer,"*** %s has joined the table ***", m_profileList[idx]->GetNick());

		if(idx == 0)
			((CGameAppView *)m_pParentWnd)->Message(_buffer,20); // personal message
		else 
			((CGameAppView *)m_pParentWnd)->Message(_buffer,10); // personal message
	#else
		// display text for player joining
		char _buffer[MAX_CHARS];
		if(((CGameAppView *)m_pParentWnd)->m_lobby->m_bInLobby)
		{
			//if(idx == ((CGameAppView *)m_pParentWnd)->m_playerSockIdx) // this line was causing problems displaying the message??
			{
				//sprintf(_buffer,"*** %s has joined the lobby ***", m_profileList[idx]->GetNick());
				//sprintf(_buffer,"*** %s joined the lobby at %s ***", m_profileList[idx]->GetNick(),timestr);
				sprintf(_buffer,"*** %s joined the lobby at %s ***", m_player[0].name, timestr);
				((CGameAppView *)m_pParentWnd)->Message(_buffer,10); // personal message
			}
		}
		else // player inside a table, so just check for other players joining this table
		{
			if(!m_bIsMainServer)
			{
				//MessageBox(0, "2", "2", MB_OK);
				//sprintf(_buffer,"*** %s has joined the table ***", m_profileList[idx]->GetNick());	
				sprintf(_buffer,"*** %s has joined the table ***", m_player[0].name);	
				((CGameAppView *)m_pParentWnd)->Message(_buffer,10); // personal message
			}
		}
		
	#endif //MAIN_SERVER
}	


void CShell::OnCReceiveAllProfilesChunk(unsigned char *buffer, int size)
{
	//char buffer[248]; // [1=msgtype][3=padd][4=length][chunk=240]
	int *v = (int *)&buffer[4];
		
	memcpy(&m_ProfilesChunkBuffer[m_ProfilesChunkPos], &buffer[8], size-8);
	
	m_ProfilesChunkPos += (size-8);
	
	// check if we have the whole msg!!
	if(m_ProfilesChunkPos == *v)
	{
		m_ProfilesChunkPos = 0;
		OnCReceiveAllProfiles((unsigned char *)m_ProfilesChunkBuffer);
	}
}



void CShell::OnCReceiveDeleteProfile(unsigned char *buffer)
{ // C
	int idx;
	int n;
	idx = buffer[1];
		
	// player has left main server (lobby)
	if(m_bIsMainServer) ((CGameAppView *)m_pParentWnd)->m_lobby->RemovePlayer(idx);

	/*
	// display text messages & update profile list
	if(m_profileList[idx]) 
	{
		char buff[80];
		
		// lobby or game, which one?
		if(((CGameAppView *)m_pParentWnd)->m_lobby->m_bInLobby)
		{
			//sprintf(buff,"*** %s has left the lobby ***", &m_profileList[idx]->data.nick);
			//((CGameAppView *)m_pParentWnd)->Message(buff,10); // personal message.
		}
		else
		{
			if(!m_bIsMainServer) {
				sprintf(buff,"*** %s has left the table ***", &m_profileList[idx]->data.nick);
				((CGameAppView *)m_pParentWnd)->Message(buff,10); // personal message.
			}
		}
		
		delete m_profileList[idx];
		m_profileList[idx] = NULL;
	}

	UpdateNameList();
	for(n=0;n<m_seatList_n;n++) // Check all seats for this guy, and make him stand if he's there.
	if(m_seatList[n])
	if(!m_seatList[n]->IsEmpty())
	if(m_seatList[n]->m_idx==idx)
					{
					m_seatList[n]->Stand(); // make him stand on server.
					break;
					}
	*/
	//m_pParentWnd->RedrawWindow(); // this causes flicker at tables when ppl leave the lobby!
}

void CShell::AttachNameList(CListCtrlEx *pNameList)//CListBox *pNameList)
{
	m_pNameList = pNameList;
}

void CShell::UpdateNameList(void)
{
	int n;
	if(m_pNameList)
	{
		//m_pNameList->ResetContent();
		m_pNameList->DeleteAllItems();
		m_pNameList->m_totalItems = 0;
		for(n=0;n<m_profileList_n;n++) // NOTE: not an entity scan, just using this max const as a safety loop.
		if(m_profileList[n])
		{
			//m_pNameList->AddString(m_profileList[n]->GetNick());
			//m_pNameList->InsertItem(i, m_profileList[n]->GetNick());
			CString str;
			str.Format("%s",m_profileList[n]->GetNick());
			
			int plrRating = -1;
			int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->FindPlayerIdx(str);
			if(plrIdx != -1)
				plrRating = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerRating(plrIdx);
							
			CString items[2];
			items[0] = str;
			if(plrRating != -1) items[1].Format("%d", plrRating);
			else items[1].Format("");
			m_pNameList->CreateItem(m_pNameList->m_totalItems, items);
		}
		//((CGameAppView *)m_pParentWnd)->m_inviteDlg.RefreshPlayerItem();
		//((CGameAppView *)m_pParentWnd)->m_bootDlg.RefreshPlayerItem();
	}
}

void CShell::Process(void)
{
	/*
	if(!(m_cyc&0x07)) {
						Server->SendFromQueue();
						Client->SendFromQueue();
						}
	m_cyc++;
	*/

//	if(!(m_cyc%10)) {
							
						//while(Server->SendFromQueue());
						//while(Client->SendFromQueue());
						


						Server->SendFromQueue();
						Client->SendFromQueue();
						Server->TakeOffReceive();
						Client->TakeOffReceive();

	

//						}


	m_cyc++;
}

void CShell::OnDraw(CDC* pDC)
{
	// Draw Seats
	for(int n=0;n<m_seatList_n;n++)
	if(m_seatList[n])
	m_seatList[n]->OnDraw(pDC);
	// Draw Stand
	//if(m_stand)
	m_stand->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}

void CShell::OnDraw(CDC* pDC, int x, int y)
{
	// Draw Seats
	for(int n=0;n<m_seatList_n;n++)
	if(m_seatList[n])
	{
		m_seatList[n]->OnDraw(pDC, x, y, n);		
	}
	// Draw Stand
	//if(m_stand)
	m_stand->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}

BOOL CShell::FindFocus(CWnd *focusWnd) // checks seats and stand button for window focus
{
	BOOL bFocus = 0;
	// Seats
	for(int n=0;n<m_seatList_n;n++)
	if(m_seatList[n])
	if(m_seatList[n]->CheckFocus(focusWnd)) bFocus = 1;
	
	// Stand
	if(m_stand)
	if(m_stand == focusWnd) bFocus = 1;	

	return bFocus;
}

CString CShell::GetHostIPAddress(int addressListIndex)
{
	char hostName[80];
	CString szIPAddress;
	PHOSTENT hostinfo;

	if( gethostname(hostName, sizeof(hostName)) == 0)
	{
		if((hostinfo = gethostbyname(hostName)) != NULL)
		{
			szIPAddress = inet_ntoa (*(struct in_addr *)hostinfo->h_addr_list[addressListIndex]);
			//szIPAddress = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
		}
	}

	return szIPAddress;
}


void CShell::ServerDoLostConnection(int playerSockIdx, char name[22], int tblIdx)
{
	if(tblIdx == -1) return;

	// For now, ignore Doubles Player Lost connection
	if(m_table[tblIdx].doubles) return;

	// Server will keep track of how many times this player has disconnected
	if(strcmpi(m_table[tblIdx].P1StartName, name) == 0)	m_table[tblIdx].m_lostConn1P++;
	else if(strcmpi(m_table[tblIdx].P2StartName, name) == 0) m_table[tblIdx].m_lostConn2P++;
			
	char buff[256];
	#ifdef THE_POOL_CLUB
	sprintf(buff, "Note: 3 Disconnections is a loss of a Rack!");
	#else
	sprintf(buff, "Note: 3 Disconnections is a loss of a Frame!");
	#endif
	((CGameAppView *)m_pParentWnd)->SendTextMsg(buff, 0, -1, tblIdx);
	sprintf(buff, "%s has %d disconnection(s), %s has %d disconnection(s)", m_table[tblIdx].P1StartName, m_table[tblIdx].m_lostConn1P, m_table[tblIdx].P2StartName, m_table[tblIdx].m_lostConn2P);
	((CGameAppView *)m_pParentWnd)->SendTextMsg(buff, 0, -1, tblIdx);

	// Check if we have disconnected a 3rd time. If so, this is a loss of Frame
	int plrSocketIdx;
	if(m_table[tblIdx].m_lostConn1P >= 3)
	{
		plrSocketIdx = m_table[tblIdx].player1SockIdx;
		// Force player to Concede. This allows other player to continue his/her break until they finish //
		((CGameAppView *)m_pParentWnd)->SendConcedePressed(FALSE, plrSocketIdx, 0, 0, tblIdx); // network game only!
		((CGameAppView *)m_pParentWnd)->m_lobby->PressedConcedeReceive(plrSocketIdx, false, tblIdx);
	}
	else if(m_table[tblIdx].m_lostConn2P >= 3)
	{
		plrSocketIdx = m_table[tblIdx].player2SockIdx;
		// Force player to Concede. This allows other player to continue his/her break until they finish //
		((CGameAppView *)m_pParentWnd)->SendConcedePressed(FALSE, plrSocketIdx, 0, 0, tblIdx); // network game only!
		((CGameAppView *)m_pParentWnd)->m_lobby->PressedConcedeReceive(plrSocketIdx, false, tblIdx);
	}
		



	// block seats, so nobody else can use them
	m_table[tblIdx].bLockSeats = TRUE; 

	// Disable Lobby table buttons also, so no one can join without an 'Auto Invite'
	((CGameAppView *)m_pParentWnd)->m_listTables.EnableItemButton(tblIdx*4+2, FALSE);
	((CGameAppView *)m_pParentWnd)->m_listTables.EnableItemButton(tblIdx*4+3, FALSE);

	// Set our Table Lost connection Flag
	TableLostConnectionFlag[tblIdx] = 1;

	

	
}





void CShell::ServerEndMatch(int tblIdx, char type, int m_score1P, int m_score2P, int m_break1P, int m_break2P)
{
	if(tblIdx == -1) return;

	// if we haven't started a frame
	if(!m_table[tblIdx].bGameStarted) return;


	if(m_table[tblIdx].doubles)
		{
		// Tell Server & Clients to End the Game //
		char buffer[16];
		buffer[0] = 0x7f; // MSGCODE_GAME Game message.
		buffer[1] = 16;	  // MSGGAME_ABORT_BYSERVER
		buffer[2] = 0;
		buffer[4] = 0; 
		int *v = (int *)&buffer[4];
		*v = rand();
		buffer[6] = tblIdx;

		if(mainShell) mainShell->Client->SendToQueue(buffer, 16);

		return;
		}


	char buf[256];
	if(type == 0) // default: Server Reset due to disconnection  //
		{
		// Send a Message to all players at the Table, Notifying them that the Game/Match has been reset as
		// both players have disconnected, no points have been won / lost //
		if(g_bTournamentLobby)
			{
			#ifdef THE_POOL_CLUB 
				sprintf(buf, "Both Players (%s and %s) have disconnected. This rack is now void and will be reset. Both players will receive a new invite for the remainder of the match", m_table[tblIdx].P1StartName, m_table[tblIdx].P2StartName);
			#else
				sprintf(buf, "Both Players (%s and %s) have disconnected. This frame is now void and will be reset. Both players will receive a new invite for the remainder of the match", m_table[tblIdx].P1StartName, m_table[tblIdx].P2StartName);
			#endif
			}
		else{
			#ifdef THE_POOL_CLUB
				if(m_table[tblIdx].doubles) sprintf(buf, "A player has left the Table. This rack is now void and will be reset.");
					else sprintf(buf, "Both Players (%s and %s) have disconnected. This rack is now void and will be reset.", m_table[tblIdx].P1StartName, m_table[tblIdx].P2StartName);
			#else
				if(m_table[tblIdx].doubles) sprintf(buf, "A player has left the Table. This rack is now void and will be reset.");
					else sprintf(buf, "Both Players (%s and %s) have disconnected. This frame is now void and will be reset.", m_table[tblIdx].P1StartName, m_table[tblIdx].P2StartName);
			#endif
			}
		}
	else if(type = 1) // Server Reset due to Hacking //
		{
		sprintf(buf, "%s %d (brk %d) Vs (brk %d) %d %s", m_table[tblIdx].P1StartName, m_score1P, m_break1P, m_break2P, m_score2P, m_table[tblIdx].P2StartName);
		AddLog("LogHackers.txt", buf);	
		
		sprintf(buf, "Hacking Attempt!!! This match (%s vs %s) has been Aborted and will be Investigated by Admin.", m_table[tblIdx].P1StartName, m_table[tblIdx].P2StartName);
		
		}


	((CGameAppView *)m_pParentWnd)->SendTextMsg(buf, 0, -1, tblIdx);

	m_table[tblIdx].bLockSeats = FALSE;
	
	((CGameAppView *)m_pParentWnd)->m_listTables.EnableItemButton(tblIdx*4+2, TRUE);
	((CGameAppView *)m_pParentWnd)->m_listTables.EnableItemButton(tblIdx*4+3, TRUE);

	m_table[tblIdx].bReRackRequest = FALSE;
	m_table[tblIdx].racksWon1P = 0;
	m_table[tblIdx].racksWon2P = 0;

	m_table[tblIdx].bGameStarted = FALSE;

	m_table[tblIdx].P1StartAtTable = -1;
	m_table[tblIdx].P1StartName[0] = 0;
	m_table[tblIdx].P2StartAtTable = -1;
	m_table[tblIdx].P2StartName[0] = 0;

		
	m_table[tblIdx].score1P = m_table[tblIdx].startScore1P;
	m_table[tblIdx].score2P = m_table[tblIdx].startScore2P;
	m_table[tblIdx].bCanRerack = TRUE;

	m_table[tblIdx].m_lostConn1P = 0;
	m_table[tblIdx].m_lostConn2P = 0;

	// Deduct some points from both players also USARYU //
	
	// If A Tournament Match //
	if(g_bTournamentLobby)
		{
		// Clear The Match, and Make Table clear in 5 mins, so it can be re-used //
		int matchNo = g_tournament.GetMatchFromTableNo(tblIdx);
		if(matchNo != -1)
			{
			g_tournament.m_match[matchNo].Clear();
			}
		}

	// Send a Message to the Server's Client for MSGGAME_RESET, for anyone at the table //
	char buffer[16];
	buffer[0] = 0x7f; // MSGCODE_GAME Game message.
	buffer[1] = 15;	  // MSGGAME_RESET_BYSERVER
	buffer[2] = 0;
	buffer[4] = 0; 
	int *v = (int *)&buffer[4];
	*v = rand();
	buffer[6] = tblIdx;

	if(mainShell) mainShell->Client->SendToQueue(buffer, 16);
}


