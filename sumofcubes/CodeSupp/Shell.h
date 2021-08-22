#ifndef __SHELL_H__
#define __SHELL_H__

#include "CoreEx.h"
#include "ButtonEx.h"
#include "ListCtrlEx.h"
#include "SocketEx.h"

#define SHELL_SERVER	0
#define SHELL_CLIENT	1

#define	SEAT_DISABLED	0
#define SEAT_EMPTY		1
#define SEAT_TAKEN		2



#define MSGCODE_TEXT				0x00	// Private: Text
#define MSGCODE_PROFILE				0x01	// Private:CProfile
#define MSGCODE_ALLPROFILES			0x02	// Private:SAll Profiles
#define MSGCODE_DELETEPROFILE		0x03	// Private:SDelete Profile
#define MSGCODE_SITREQUEST			0x04	// Private:CSit Down Request.
#define MSGCODE_SITORDER			0x05    // Private:SSit Down Order.
#define MSGCODE_STANDREQUEST		0x06	// Private:CStand Up Request.
#define MSGCODE_STANDORDER			0x07	// Private:SStand Up Request.
#define MSGCODE_SYSTEMTEXT			0x08	// Private: System Text
#define MSGCODE_SOCKETIDX			0x09	// Private: Socket Idx
#define MSGCODE_ALLPROFILESCHUNK	0x0A	// Private:SAll Profiles, in chunks of 248bytes
#define MSGCODE_REPORTABUSE			0x0B	// Report abuse
#define MSGCODE_TEXTCOLOR			0x0C	// Private: Text Color

#define MSGCODE_MUTE				0x00	// Private: Text

#define MSGCODE_GAME			0x7f	// Private: Game message.

#define MSGMASK_CODE		0x7f	// Code Mask
#define MSGMASK_BROADCAST	0x80	// Broadcast Mask (auto broadcast)


/*
// for receiveing variable length data packets
typedef struct dataPacket_typ {
unsigned char *buf;
int length;
void *lpNextBuf;
} dataPacket, dataPacket_ptr;

class CPacket {
public:
	dataPacket *m_startPacket;
	dataPacket *m_endPacket;
	
	CPacket();
	~CPacket();
	dataPacket Add(const void *lpBuf, int length);


}

CPacket::CPacket()
{
	m_startPacket = NULL;		
	m_endPacket = NULL;
}

CPacket::~CPacket()
{
}

dataPacket *CPacket::Add(const void *lpBuf, int length)
{
	dataPacket *p = NULL;
	if(lpBuf)
	{
		p = new dataPacket;
		p->buf = new unsigned char[length];
		p->length = length;
		
		if(!m_startPacket) // very first packet to add
		{
			m_startPacket = p;
			m_endPacket = m_startPacket;
		}
		else // another packet to add
		{
			m_endPacket->lpNextBuff = p;
		}
	}
	
	return p;
}
*/
extern int g_dataSent;
extern int g_dataReceived;

typedef struct idxData_typ {
unsigned char buffer[2];
int idx;	
} idxData; // MSGCODE_SOCKETIDX

typedef struct {
unsigned char code;	
unsigned char idx;
char nick[32]; // 31 characters long max for nick.
} CMsgProfile; //MSGCODE_PROFILE

class CProfile { // all details. server must know this, however clients only need a little info.
public://CP
CMsgProfile data;
public:
void AddMsgProfile(CMsgProfile *pMsgProfile);
char *GetNick(void);
CMsgProfile *GetData(void);
};

class CAsyncSocketEx;
class CShell;

class CComm {
// Construction
public:
CComm(int max_sockets, int queue_size = 4);
~CComm();

// Attributes
public:
CShell *m_pShell;
CAsyncSocket **m_socketList;
int m_socketList_n;
int m_socketsUsed_n;

//
unsigned char **m_packetFrag;	// stores a partial packet for the next onReceive
int *m_packetFrag_len;			// keeps track of partial packet length
//

//
//int m_packetIdx[4];
//unsigned char m_packet[4][16][256];
//int m_packet_len[4][16];
//BOOL m_buildingPacket[4];
//int m_buildingPacketSize[4];
//


// Queue //

unsigned char **lpBuf; // array of 16 buffers (of varialbe size)
int *lpBufLen; // array of 16 ints
int *lpBufI; // array of 16 ints (indicates player to send to i.e. -1 is broadcast to all)
int *lpBufOffset;
int buf_n; // 16 then wrap-around
int write_ptr; // 0
int read_ptr; // 0
int queueMessageCount;

int SendToQueue(void* _lpBuf, int _nBufLen, int _i = -1); // Sends to all attached sockets defaultly otherwise player i. [Queue]
int TakeOffReceive(void);
int SendFromQueue(void);

BOOL m_bSendBusy;

int m_SendCnt;
int m_ReceiveCnt;

BOOL Send(void* lpBuf, int nBufLen, int i = -1);		// Sends to all attached sockets defaultly otherwise player i.
//virtual void OnReceive(CAsyncSocketEx *pSocket);		// Called upon Receiving a message from one of it's child Sockets.
virtual BOOL OnReceive(CAsyncSocketEx *pSocket, void* lpBuf, int nBufLen); // Whenever a COMPLETE Packet comes back
virtual void OnSend(CAsyncSocketEx *pSocket);			// Called upon ready to Send a message to one of it's child Sockets.
virtual void OnClose(CAsyncSocket *pSocket);			// Called upon Socket Closure.
virtual void OnOpen(CAsyncSocket *pSocket);				// Called upon Socket Opening.
void AttachSocket(int idx, CAsyncSocketEx *pSocket);	// Attach socket specifically
int AttachSocket(CAsyncSocketEx *pSocket);				// Attach socket to first avail.
int GetSocketIndex(CAsyncSocketEx *pSocket);
int CountSocketsUsed(void);
void Close(void);
BOOL GetPeerName(CString& rPeerAddress, UINT& rPeerPort, int i); // Gets Peer name of socket index
BOOL Kick(int i); // Gets Peer name of socket index - the safe way

// debug vars.
CString m_rPeerAddress; // for Server use only!
UINT m_rPeerPort; // for Server use only!
};

class CSeat : public CCoreEx {
CWnd *m_pParentWnd;
public://
int m_x,m_y;
int m_state;
CButtonEx m_button;
char m_name[32];
public:
int m_idx;
CSeat(CWnd *pParentWnd, int x, int y);
void Sit(int idx);
void Stand(void);
void SetName(char *str);
int IsEmpty(void);
BOOL CheckFocus(CWnd* focusWnd);

void OnEvent(int id); // Override.

void OnDraw(CDC* pDC);
void OnDraw(CDC* pDC, int x, int y, int n);
};

class CShell : public CCoreEx {
public://
CWnd *m_pParentWnd;
BOOL m_bIsMainServer;

CSeat **m_seatList;
int m_seatList_n;
CButtonEx *m_stand;

char m_ProfilesChunkBuffer[4096];
int m_ProfilesChunkPos;

//
//int m_receiveBufferIdx;
//unsigned char m_receiveBuffer[8][/*256*/2048]; // 2k receive buffer.
//int m_receiveBuffer_len;

//int m_packetIdx;
//unsigned char m_packet[16][256];
//int m_packet_len[16];
//BOOL m_buildingPacket;
//int m_buildingPacketSize;
//

public:
CShell();
CShell(CWnd *pParentWnd, int players);
~CShell();
void Init(CWnd *pParentWnd, int players);

// Seat Functions
void CreateSeat(int idx, int x, int y);
void AttachSeat(int idx, CSeat *pSeat);
CSeat *DetachSeat(int idx);
CSeat *GetSeat(int idx);

void SitRequest(int idx);
void StandRequest(void);
void OnReceiveSitRequest(int idx, unsigned char *pMsgData);
void OnCReceiveSitOrder(unsigned char *pMsgData);
void OnReceiveStandRequest(int idx, unsigned char *pMsgData);
void OnCReceiveStandOrder(unsigned char *pMsgData);
// Stand Functions
void CreateStand(int x,int y);
void CreateStand(int idx, CButtonEx *button);

// Communication Functions
CComm *Server;
CComm *Client;
void AttachComm(int idx, CComm *pComm);
void Send(const void* lpBuf, int nBufLen);
void OnReceiveX(CComm *pComm, CAsyncSocketEx *pSocket); // Called whenever an attached Comm receives a message.
BOOL OnReceive(CComm *pComm, CAsyncSocketEx *pSocket, void* lpBuf, int nBufLen); // Whenever a COMPLETE Packet comes back
void OnSend(CComm *pComm, CAsyncSocketEx *pSocket); // Called whenever an attached Comm receives a message.
void OnClose(CComm *pComm, int idx); // Called whenever an attached Comm closes
void OnOpen(CComm *pComm);	// Called upon Socket Opening.
virtual BOOL OnGameReceive(CComm *pComm, CAsyncSocketEx *pSocket, void *lpBuf, int nBufLen);
virtual void OnMainReceive(CComm *pComm, CAsyncSocketEx *pSocket, void *lpBuf, int nBufLen);

void ChatSend(CString& strText);	// Called for sending normal chat messages
void ChatSend(char *str);
void OnAccept(CAsyncSocketEx *pSocket); // Called whenever a listening socket (Server) accepts a Client socket.

// Profile Functions
CProfile **m_profileList;	// server profile list.
int m_profileList_n;
CProfile **m_CprofileList;	// client profile list.
int m_CprofileList_n;
char m_nick[80];			// Your Nick
int m_idx;					// Unique Index.
int m_seat;					// Seat Index.
void AllocateProfile(int players);
void BroadcastProfileAllButOne(CProfile *pProfile, int idx);
int SearchForPlayerInSeat(int idx);
void BroadcastAllProfilesToOne(int idx);
void ShowNameList(void);
void SetNick(CString& strText);
void SetNick(char *str);

void SendProfile(void);
void OnReceiveProfile(int idx, CMsgProfile *pMsgProfile);
void OnReceiveAbuse(unsigned char *buffer, int buffer_len, int plrIdx = -1);
void OnReceiveText(int idx, unsigned char *pMsgData, int len, int tblIdx = -1);
void OnReceiveTextColor(int idx, unsigned char *pMsgData, int len, int tblIdx = -1);
void LogText(FILE *fptr, unsigned char *pMsgData, int tblIdx = -1);
void LogTextColor(FILE *fptr, unsigned char *pMsgData, int tblIdx = -1);

void OnCReceiveText(unsigned char *pMsgData);
void OnCReceiveTextColor(unsigned char *pMsgData);
void OnCReceiveProfile(CMsgProfile *pMsgProfile);
void OnCReceiveAllProfiles(unsigned char *buffer);
void OnCReceiveAllProfilesChunk(unsigned char *buffer, int size);
void OnCReceiveDeleteProfile(unsigned char *buffer);

// NameList Functions
//CListBox *m_pNameList;
CListCtrlEx *m_pNameList;
void AttachNameList(CListCtrlEx *pNameList);//CListBox *pNameList);
void UpdateNameList(void);

void OnEvent(int id); // Override.

int m_cyc;
void Process(void);
void OnDraw(CDC* pDC);
void OnDraw(CDC* pDC, int x, int y);
BOOL FindFocus(CWnd *focusWnd);
CString GetHostIPAddress(int addressListIndex);
void ServerDoLostConnection(int playerSockIdx, char name[22], int tblIdx);
void ServerEndMatch(int tblIdx, char type = 0, int m_score1P = 0, int m_score2P = 0, int m_break1P = 0, int m_break2P = 0);
enum{LOCAL_HOST_IP = 0, INTERNET_HOST_IP =1};
};

extern BOOL g_socketlog;
extern BOOL g_socketfirst;
extern BOOL g_gamelog;
extern int g_daysFreeTrial;
extern BOOL g_guestMember;

#endif // __SHELL_H__

