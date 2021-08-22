// Notes: Extension shell - handles listening socket processing	(used for accepting incoming client connections to server)
//						  - Main Server & Game Server processing

#include "Build.h"					  
#include "stdafx.h"

#include "ShellEx.h"

#include "GameAppView.h"
#include "GameSetup.h"
#include "SocketEx.h"
#include "../Code/lobby.h"
#include "../Code/db.h"
#include <direct.h>

#define MAX_NORMAL_PLAYERS_ONLINE	120//(MAX_LOBBY_PLAYERS - 10)//1

CShellEx::CShellEx(BOOL bIsMainServer)
{
	m_bIsMainServer = bIsMainServer;
	
	// Server's Listening socket (used for accepting incoming client connections)
	m_pSocketListen = new CAsyncSocketEx(this);
	m_pSocketListenCheck = 0;//new CAsyncSocketEx(this);
	
	m_clientSocketIdx = -1;
}

CShellEx::~CShellEx()
{
	if(m_pSocketListen) delete m_pSocketListen;
	if(m_pSocketListenCheck){
				 			delete m_pSocketListenCheck;
							m_pSocketListenCheck = NULL;
							}
}


BOOL CShellEx::ConnectionASYNC(BOOL bIsHost, LPCTSTR lpszAddress, int port, int type)
{
	if(bIsHost) // player creator
	{
		if(m_pSocketListen->Create(port)) // SERVER socket created
		{
			// let the server listen for clients
			if (m_pSocketListen->Listen())
			{
				// create a joiner for the server
				CString strHandle = _T("Creator");

				if(lpszAddress)
				{
					if (ConnectSocketCASYNC(strHandle, lpszAddress, port, type)) return TRUE;
				}
				else // so find this machines IP adress manually
				{
					CString str;
					str = GetHostIPAddress(LOCAL_HOST_IP);

					if( !str.GetLength() ) return FALSE; // couldn't find the local (host's) IP address
										
					if (ConnectSocketCASYNC(strHandle, str, port, type)) return TRUE;

				}

				return TRUE;


			}
		}
	}
	else // player joiner
	{
		// create a joiner
		CString strHandle = _T("Joiner");
		
		if(lpszAddress)
		{
			if (ConnectSocketCASYNC(strHandle, lpszAddress, port, type)) return TRUE;
		}
		else // so find this machines IP adress manually
		{
			CString str;
			str = GetHostIPAddress(LOCAL_HOST_IP);
			if( !str.GetLength() ) return FALSE; // couldn't find the local (host's) IP address
								
			if (ConnectSocketCASYNC(strHandle, str, port, type)) return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CShellEx::ConnectSocketCASYNC(LPCTSTR lpszHandle, LPCTSTR lpszAddress, UINT nPort, int type)
{

	// Client attributes
	CString m_strHandleC;
	CAsyncSocketEx* m_pSocketC;
	
	m_strHandleC = lpszHandle;
	
	m_pSocketC = new CAsyncSocketEx(this);
	m_pSocketC->m_type = type;
	
	if (!m_pSocketC->Create())
	{
		delete m_pSocketC;
		m_pSocketC = NULL;
		ErrorMessage(m_pParentWnd, "Failed to create a socket");
		return FALSE;
	}


	/*
	char buf[80];
	sprintf(buf, "lpszAddress: %s, nPort:%d", lpszAddress, nPort);
	AfxMessageBox(buf);
	*/
	
	if(!m_pSocketC->Connect(lpszAddress, nPort) )
		{
			/*
			switch(m_pSocketC->GetLastError())
			{
				case WSANOTINITIALISED: AfxMessageBox("WSANOTINITIALISED");	break;
				case WSAENETDOWN: AfxMessageBox("WSAENETDOWN");	break;
				case WSAEADDRINUSE: AfxMessageBox("WSAEADDRINUSE");	break;
				case WSAEINPROGRESS: AfxMessageBox("WSAEINPROGRESS");	break;
				case WSAEADDRNOTAVAIL: AfxMessageBox("WSAEADDRNOTAVAIL");	break;
				case WSAEAFNOSUPPORT: AfxMessageBox("WSAEAFNOSUPPORT");	break;
				case WSAECONNREFUSED: AfxMessageBox("WSAECONNREFUSED");	break;
				case WSAEDESTADDRREQ: AfxMessageBox("WSAEDESTADDRREQ");	break;
				case WSAEFAULT: AfxMessageBox("WSAEFAULT");	break;
				case WSAEINVAL: AfxMessageBox("WSAEINVAL");	break;
				case WSAEISCONN: AfxMessageBox("WSAEISCONN");	break;
				case WSAEMFILE: AfxMessageBox("WSAEMFILE");	break;
				case WSAENETUNREACH: AfxMessageBox("WSAENETUNREACH");	break;
				case WSAENOBUFS: AfxMessageBox("WSAENOBUFS");	break;
				case WSAENOTSOCK: AfxMessageBox("WSAENOTSOCK");	break;
				case WSAETIMEDOUT: AfxMessageBox("WSAETIMEDOUT");	break;
				case WSAEWOULDBLOCK: AfxMessageBox("WSAEWOULDBLOCK");	break;

			}
			*/

		if(m_pSocketC->GetLastError() != WSAEWOULDBLOCK) // ASYNC should return WSAEWOULDBLOCK
			{
			delete m_pSocketC;
			m_pSocketC = NULL;
			return FALSE;
			}
		}

	//m_clientSocketIdx = Client->AttachSocket(m_pSocketC);

	//m_pSocketC->OnOpen(); // Generate Open Message
		
	return TRUE; // Note: Doesn't mean successful connection (just means didn't do any other error. e.g. invalid str)
}

#define MSGGAME_SHOT					0		// xG
#define MSGGAME_RESET					1		// xG
#define MSGGAME_SIM_END					2		// xG
#define MSGGAME_BALL_POS				3		// xG
#define MSGGAME_SELECT_BALL				4		// xG
#define MSGGAME_MOUSE_MOVE				5		// xG
#define MSGGAME_CUE_BALL_TIP			6		// xG
#define MSGGAME_PLAYAGAIN				7		// xG
#define MSGGAME_BALLS					8		// xG
#define MSGGAME_SIM_START				9		// xG
#define MSGGAME_FOULALERT				10		// xG
#define MSGGAME_MISSRESET				11		// xG
#define MSGGAME_POT8BALLONBREAKRERACK	12		// xG
#define MSGGAME_BADBREAKRERACK			13		// xG
#define MSGGAME_UPDATEGAME				14		// xG
#define MSGGAME_RESET_BYSERVER			15		// xG
#define MSGGAME_ABORT_BYSERVER			16		// xG

#define MSGGAME_PLAYERDETAILS		50
#define MSGGAME_PLAYERJOIN			51
#define MSGGAME_PLAYEREXIT			52
#define MSGGAME_TABLEDETAILS		53			// Optimise this one!
#define MSGGAME_PLAYERSIT			54
#define MSGGAME_TABLEHOSTDETAILS	55
#define MSGGAME_PLAYERREJOIN		56
#define MSGGAME_LOBBYREADY			57
#define MSGGAME_PLAYERJOINFAILED	58
#define MSGGAME_LOGINFAILED			59
#define MSGGAME_STARTPRESSED	 	60
#define MSGGAME_GAMEOVER			61
#define MSGGAME_CONCEDEPRESSED		62
#define MSGGAME_RERACKPRESSED		63
#define MSGGAME_INVITEPRESSED		64
#define MSGGAME_BOOTPRESSED			65		// xG
#define MSGGAME_LOGINNEWACCOUNT		66
#define MSGGAME_SENDNEWACCOUNT		67
#define MSGGAME_ACCOUNTCREATED		68
#define MSGGAME_SERVERMESSAGE		69
#define MSGGAME_HOSTCHECK			70
#define MSGGAME_HOSTCHECKLISTENING	71
#define MSGGAME_HOSTCHECKBROADCAST	72
#define MSGGAME_KICK				73
#define MSGGAME_GAMEOVERFORALL		74
#define MSGGAME_EPD					75
#define MSGGAME_PLAYERID			76
#define MSGGAME_GAMEPING			77		// G (disabled the gameping routines OAP)
#define MSGGAME_CLEARTABLE			78
#define MSGGAME_HIGHBREAK			79
#define MSGGAME_ALLHIGHBREAKS		80
#define MSGGAME_TABLEHOSTCOMPLETE	81
#define MSGGAME_PLAYERCHANGEDATA	82
#define MSGGAME_TIMEUP				83
#define MSGGAME_PLAYERLOSTCONN		84
#define MSGGAME_STING				85
#define MSGGAME_CRASH				86
#define MSGGAME_PLAYERDETAILSC		87
#define MSGGAME_CUE_BALL_TIPC		88		// xG
#define MSGGAME_GENERICREQUEST		89		// For Generic Simple Requests [player data etc]
#define MSGGAME_MAXTIMEUP			90		// xG
#define MSGGAME_PLAYERTARGETDATA	91		// xG
#define MSGGAME_PLAYERTEXTFORMAT	92		// xG
#define MSGGAME_TMATCHTIMERLEFT		93
#define MSGGAME_SERVERDATA			94
#define MSGGAME_TEXTMSG				95
#define MSGGAME_SERVERTIME			96
#define MSGGAME_UNFINISHEDGAME		97
#define MSGGAME_UPDATEPLAYERBIO		98
#define MSGGAME_REQUESTPLAYERBIO	99
#define MSGGAME_RETURNPLAYERBIO		100
#define MSGGAME_TABLECHANGEDATA		101
#define MSGGAME_SERVERCONFIG		102
#define MSGGAME_PLAYERCONN			103
#define MSGGAME_TEMPERLOG			104
#define MSGGAME_CRASHTEMPER			105
#define MSGGAME_UPDATEHISTORY		106
#define MSGGAME_UPDATELOBBYTVCLIENT	107
#define MSGGAME_UPDATELOBBYTVSERVER	108
#define MSGGAME_REFRESHLEADERBOARDS	109
#define MSGGAME_ABUSELST			110

#define MSGGAME_REQUESTFILE			111
#define MSGGAME_RECEIVEFILE			112

#define MSGGAME_TEXTCOLORCHANGED	113
#define MSGGAME_UPDATEVCASH			114
#define MSGGAME_CHEATALERT			115


#define MSGGAME_FINDMATCH			120
#define MSGGAME_FINDMATCH_CANCEL	121

#define MSGGAME_LADDERRANKINGS		122



#define MSGGAME_OPEN				0xff // special reserved flag.


/////////////////////////////////////////////////////////////////////////////////////
/////////////// The MAIN Login Function /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

BOOL CShellEx::OnGameReceivePlayerDetails2(void *lpBuf,int plrSockIdx, int nBufLen)
{
		#ifdef MAIN_SERVER	
				playerDetailsSendDataConnect2 *sd;
				Player plr;
				char nameFile[80];
				char passwordFile[80];
				BOOL bGuest = FALSE;
				char guest_check[80];
				int dots = 0;
				int n = 0;
				int timetoexpire;
				int adminstate; // 0 = Admin, 1 = Senior Mod, 2 = Special Mod, 3 = Mod
				int ps = 0;
				char ssysid[16], smacid[16], sip[16];
				CString serverLocalIP;
				CString rPeerAddress;
				UINT rPeerPort;

				// Get External IP (rPeerAddress, rPeerPort) ///////////////////////////////////////////////
				Server->m_socketList[plrSockIdx]->GetPeerName(rPeerAddress, rPeerPort);

				
				// Get server's local IP (serverLocalIP) ///////////////////////////////////////////////////
				serverLocalIP = GetHostIPAddress(LOCAL_HOST_IP);

				if(sizeof(playerDetailsSendDataConnect2_typ)!=nBufLen)
				{
					char buffer[80];
					sprintf(buffer,"[%s]:(***Bad handshake/Old Version***) [%s]",rPeerAddress, g_build.version);
					AddLog("LogConnect.txt", buffer);

					((CGameAppView *)m_pParentWnd)->Message(buffer,10); // personal message	
						((CGameAppView *)m_pParentWnd)->SendLoginFailed(TRUE, 10, plrSockIdx); // incorrect version/bad crc
					return FALSE;
				}

				sd = (playerDetailsSendDataConnect2_typ *) lpBuf;

				
						
				
				

				// Load the Member Account from the Database ONCE, here, if possible //
				// The idea here is to cut down on the number of times we have to querry the MYSQL database //
				adminstate = 0;
				((CGameAppView *)m_pParentWnd)->m_lobby->ClearPlayer(&plr);
				
				strcpy(guest_check,sd->m_player2.name); // copy 'guest' to guest_check.
				guest_check[5] = 0; // "guest\0"

				// If a Guest Login (i.e. Username = 'guest' //
				if(!strcmpi(guest_check,"guest")) 
					{
					adminstate = 0; // Normal Accounts or Guests //
					}
				// Non Guest, so Lets Check if they have put in the Correct Username and Password //
				else{
					ps = LoadPlayerFileEx(sd->m_player2.name, &plr);
					}
				
				switch(plr.playertype)
					{
					case -1:// Normal Account
							break;
					case 0:adminstate = 2; // Admin
						   break;
					case 1:adminstate = 1; // Senior Mod
						   break;
					case 2:adminstate = 1; // Special Mod
						   break;
					case 3:adminstate = 1; // Mod
						   break;
					}
			
				// If Guest Only Lobby -> Turn all Non Admins / Mods into Guests //
				if(adminstate==0) if(g_guestonly) strcpy(sd->m_player2.name,"guest"); // FORCE GUEST LOGIN!
				/*
				// If not a Admin, Senior Mod, Special Mod or Mod, don't allow acces (Beta Testing) //
				if(adminstate == 0) 
					{
					AddLog("LogConnect.txt", "^Mods / Admins Only");
					((CGameAppView *)m_pParentWnd)->SendLoginFailed(TRUE, 15, plrSockIdx);
					return FALSE;					
					}
				*/
				// Maximum Lobby Entrants Check //
				if( ((CGameAppView *)m_pParentWnd)->m_lobby->CountPlayers()>=MAX_LOBBY_PLAYERS)
					{
					// If we will exceed the maximum number a lobby can support, Don't allow entry to anyone
					((CGameAppView *)m_pParentWnd)->SendLoginFailed(TRUE, 13, plrSockIdx); // lobby full.
					}

				if( ((CGameAppView *)m_pParentWnd)->m_lobby->CountPlayers()>=MAX_NORMAL_PLAYERS_ONLINE)
					{
					if(adminstate==0) // for non Mods / Admins, kick them off, we've reached the limit
						{
						((CGameAppView *)m_pParentWnd)->SendLoginFailed(TRUE, 13, plrSockIdx); // lobby full.
						}
					}

				// Update the User's Current Details (Name, Password, Port and IP Address)
				// strcpy(plr.name, sd->m_player2.name);
				// strcpy(plr.password, sd->m_player2.password);
				plr.playerPort = sd->m_player2.playerPort;
				strcpy(plr.playerLocalIP, sd->m_player2.playerLocalIP);

				// Version checking - i.e. makes sure the correct version is being used. ////////////////
				sd->m_versionstr[9] = 0; // Cap the string incase someone is trying to crash the server!
				if(!strcmpi(sd->m_versionstr, g_build.version)); // Ok
				else // Didn't match!
					{
					char buffer[MAX_CHARS];
					sprintf(buffer,"[%s]:user:%s pwd:%s (***VER Failed***) [%s]",sd->m_versionstr, sd->m_player2.name, sd->m_player2.password,g_build.version);
					AddLog("LogConnect.txt", buffer);
					((CGameAppView *)m_pParentWnd)->Message(buffer,10); // personal message	
					((CGameAppView *)m_pParentWnd)->SendLoginFailed(TRUE, 10, plrSockIdx); // incorrect version/bad crc
					return FALSE;
					}

				// CRC checking - i.e. makes sure no one has hacked the game /////////////////////////////
				if(!CrcCodeValid(sd->m_crc))
					{
					char buffer[MAX_CHARS];
					// CRC Failed
					sprintf(buffer,"[0x%lX]:user:%s pwd:%s (***CRC Failed***) [%s]",sd->m_crc, sd->m_player2.name, sd->m_player2.password,g_build.version);
					AddLog("LogConnect.txt", buffer);

					((CGameAppView *)m_pParentWnd)->Message(buffer,10); // personal message	
	
					((CGameAppView *)m_pParentWnd)->SendLoginFailed(TRUE, 9, plrSockIdx); // incorrect version/bad crc
					return FALSE;
					}
				else{
					
					
					char buffer[MAX_CHARS];
					sprintf(buffer,"[0x%lX](%s)<SID:%lx%04x><MID:%lx%lx>:user:%s pwd:%s [%s]",sd->m_crc,rPeerAddress,sd->m_sysID,sd->m_cpuID,sd->m_macID_U, sd->m_macID_L, sd->m_player2.name, sd->m_player2.password,g_build.version);
					AddLog("LogConnect.txt", buffer);

				
								
					// Check If the User is Walled, Denied or Blocked
					sprintf(ssysid, "%lx%04x",sd->m_sysID,sd->m_cpuID);
					sprintf(smacid, "%lx%lx",sd->m_macID_U, sd->m_macID_L);
					sprintf(sip, "%s",rPeerAddress);

					// If Walled //
					if(CheckWalledEx(ssysid))
						{
						sprintf(buffer,"^Walled");
						AddLog("LogConnect.txt", buffer);

						{
							char buffer[8];
							buffer[0] = 0x7f;	// MSGCODE_GAME Game message.
							buffer[1] = 73;		// MSGGAME_KICK
							buffer[2] = 2;		// Wall flag.
							buffer[3] = 0;		// Reason flag. - no reason for now, can't be assed to code a reason.

							(CAsyncSocketEx *)Server->SendToQueue(buffer, 4, plrSockIdx); // +1 for Ter, +1 for Code at beginning +1 for sender gap.
						}
					
		
						((CAsyncSocketEx *)Server->m_socketList[plrSockIdx])->Kick(); // Kick his ass off ;)
						return TRUE; // Socket is closed at this point, so no more socket related code can take effect!
						}
					
					// If Denied //
					if(CheckDeniedEx(smacid))
						{
						sprintf(buffer,"^Denied");
						AddLog("LogConnect.txt", buffer);

						{
							char buffer[8];
							buffer[0] = 0x7f;	// MSGCODE_GAME Game message.
							buffer[1] = 73;		// MSGGAME_KICK
							buffer[2] = 3;		// Denied flag. 
							buffer[3] = 0;		// Reason flag. - no reason for now, can't be assed to code a reason.

							(CAsyncSocketEx *)Server->SendToQueue(buffer, 4, plrSockIdx); // +1 for Ter, +1 for Code at beginning +1 for sender gap.
						}
					
		
						((CAsyncSocketEx *)Server->m_socketList[plrSockIdx])->Kick(); // Kick his ass off ;)
						return TRUE; // Socket is closed at this point, so no more socket related code can take effect!
						}

				
					// If Blocked //
					if(CheckBlockedEx(sip))
						{
						sprintf(buffer,"^Blocked");
						AddLog("LogConnect.txt", buffer);

						{
							char buffer[8];
							buffer[0] = 0x7f;	// MSGCODE_GAME Game message.
							buffer[1] = 73;		// MSGGAME_KICK
							buffer[2] = 4;		// Denied flag. (use this for now) 
							buffer[3] = 0;		// Reason flag. - no reason for now, can't be assed to code a reason.

							(CAsyncSocketEx *)Server->SendToQueue(buffer, 4, plrSockIdx); // +1 for Ter, +1 for Code at beginning +1 for sender gap.
						}
					
		
						((CAsyncSocketEx *)Server->m_socketList[plrSockIdx])->Kick(); // Kick his ass off ;)
						return TRUE; // Socket is closed at this point, so no more socket related code can take effect!
						}

					}

				// compare client's local IP against server's local IP (clients on same LAN as main server)
				do
				{
					if( serverLocalIP.GetAt(n) != rPeerAddress.GetAt(n) ) break;
					if( serverLocalIP.GetAt(n) == '.') dots++;
					n++;
				} while(dots!=3);

				// two strings are equal up to 3rd dot (so this client is on same LAN as main server)
				if(dots == 3) rPeerAddress = "LOCAL";

				strcpy(plr.playerIP, rPeerAddress.GetBuffer(0));
				plr.playerSocketIdx = plrSockIdx;
				plr.sysID = sd->m_sysID; // copy over sysID to player struct.
				plr.cpuID = sd->m_cpuID; // copy over cpuID to player struct.
				plr.macID_U = sd->m_macID_U; // copy over macID Upper 4 BYTES to player struct.
				plr.macID_L = sd->m_macID_L; // copy over macID Lower 4 BYTES to player struct.

				// Guest Login Checking /////////////////////////////////////////////////////////////////////				


				strcpy(guest_check,sd->m_player2.name); // copy 'guest' to guest_check.
				guest_check[5] = 0; // "guest\0"

				if(((CGameAppView *)m_pParentWnd)->m_lobbySettings->m_maxGuests>0)
				if(!strcmpi(guest_check,"guest")) 
					{

					/*
					if(g_membersonly==TRUE || g_memberstrialsonly==TRUE) // if it's members / memberstrials only then dont allow the guest name to work.
					{
					AddLog("LogConnect.txt", "^Guest Login Failed");
					((CGameAppView *)m_pParentWnd)->SendLoginFailed(TRUE, 15, plrSockIdx);
					return FALSE;					
					}
					*/

					NewAccountPlayer(&plr);
					plr.bIsGuest = TRUE;
					plr.rating = 0; 
					sprintf(plr.name,"guest%d",g_guestcount);
					
					g_guestcount++;
					if(g_guestcount>9999) g_guestcount = 1;
						{
						char buffer[32];
						sprintf(buffer,"^%s",plr.name);
						AddLog("LogConnect.txt", buffer);
						}
					
					CString rPA;
					UINT rPP;
					Server->m_socketList[plrSockIdx]->GetPeerName(rPA, rPP);


					// Check: has this Guest already logged in by System ID (Login Failed 2) /////////////////////
					if(((CGameAppView *)m_pParentWnd)->m_lobby->LoggedInAlreadySYSIDMACID(plr.name, plr.sysID, rPA.GetBuffer(0), plr.macID_U, plr.macID_L))
						{
						AddLog("LogConnect.txt", "^Logged in Already On This Computer (SYSID)");
						((CGameAppView *)m_pParentWnd)->SendLoginFailed(TRUE, 2, plrSockIdx);
						return FALSE;
						}
					
					goto mainLogin;  
					
					}
			
	
				// if GUEST ONLY MODE //

				BOOL ok;
				ok = FALSE;

				if(!plr.bIsGuest) // Non Guests //
				{			
				switch(plr.playertype)
					{
					case -1:break;// Normal Account
					case 0:ok = TRUE; // Admin
						   break;
					case 1:ok = TRUE; // Senior Mod
						   break;
					case 2:ok = TRUE; // Special Moderator (has Greeting, wall, deny commands)
						   break;
					case 3:ok = TRUE; // Mod
						   break;
					}
				}
				
				if(g_guestonly)
				{
					if(!plr.bIsGuest) // Non Guests //
					{
					if(!ok) {
							((CGameAppView *)m_pParentWnd)->SendLoginFailed(TRUE, 0, plrSockIdx); // Incorrect Login (For Now)
							return FALSE;
							}
					}
				}
				// ------------------ //


				// This is where we bring up the New Account Wizard, if the Username is Empty
				// which Create / Top-Up Account does //
				if(ps==0) //PLAYER_INVALID
					{
					if(!plr.name[0]) // if name field is empty then it's a create/topup newaccount
					((CGameAppView *)m_pParentWnd)->SendLoginNewAccount(TRUE, plrSockIdx);
					else { // otherwise it's an invalid name and therefore you have to reenter.
					AddLog("LogConnect.txt", "^Invalid");
					((CGameAppView *)m_pParentWnd)->SendLoginFailed(TRUE, 12, plrSockIdx);
						}
					return FALSE;
					}
				

				// Check: If password sent (sd...) is same as password in file (Login Failed 1)
				if(strcmpi(sd->m_player2.password, plr.password) != 0)
						{
						AddLog("LogConnect.txt", "^Password");
						((CGameAppView *)m_pParentWnd)->SendLoginFailed(TRUE, 1, plrSockIdx);
						return FALSE;
						}

				// Check: has this player already logged in? (Login Failed 2) ////////////////////////////////
				if(!ok) // if your not admin or mod, then you can't log in multiple times on same comp
					{
					CString rPA;
					UINT rPP;
					// Get External IP (rPeerAddress, rPeerPort) ///////////////////////////////////////////////
					Server->m_socketList[plrSockIdx]->GetPeerName(rPA, rPP);
					// Make Sure the Usernames are different or allowed
					// Only do if we are logging in with a different Username //
					// Code underneath, handles logging in with same username, kicks off old user 
					if(((CGameAppView *)m_pParentWnd)->m_lobby->LoggedInAlreadySYSIDMACID(plr.name, plr.sysID, rPA.GetBuffer(0), plr.macID_U, plr.macID_L))
							{
							AddLog("LogConnect.txt", "^Logged in Already On This Computer (SYSID)");
							((CGameAppView *)m_pParentWnd)->SendLoginFailed(TRUE, 2, plrSockIdx);
							return FALSE;
							}
					}

				if(ps&2) // PLAYER_BANNED
					{
					AddLog("LogConnect.txt", "^Banned");
					((CGameAppView *)m_pParentWnd)->SendLoginFailed(TRUE, 11, plrSockIdx);
					return FALSE;
					}


				// If this name is already logged in, then Kick Out the person logged in //
				// Tell them why they are being disconnected (unlike MSN)
				if(((CGameAppView *)m_pParentWnd)->m_lobby->LoggedInAlready(plr.name))
						{
						char buffer[80];
						AddLog("LogConnect.txt", "^Logged in Already");
						
						int o_sockIdx;
						// Lets Find the Socket Idx of the person still online //
						// Ignore our own SocketIdx in this search //
						o_sockIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->FindPlayerSockIdxi_Ignore(plr.name, plrSockIdx); // Get socket index of player you wish to message.noncase sensi.
						if(o_sockIdx==-1) // do other, full name e.g. "Freeda Reds" case-sensi string compare aswell.
						o_sockIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->FindPlayerSockIdx_Ignore(plr.name, plrSockIdx); // Get socket index of player you wish to message.noncase sensi.

						// If we found previously logged in SocketIdx, Kick it off //
						if(o_sockIdx!=-1) 
							{
							sprintf(buffer," old Socket kicked - [%d]", o_sockIdx);
							AddLog("LogConnect.txt", buffer);
								{
								char buffer[8];
								buffer[0] = 0x7f;	// MSGCODE_GAME Game message.
								buffer[1] = 73;		// MSGGAME_KICK
								buffer[2] = 5;		// Disconnected, Someone Else is Logging in with your Account
								buffer[3] = 0;		// Reason flag. - no reason for now, can't be assed to code a reason.
								(CAsyncSocketEx *)Server->SendToQueue(buffer, 4, o_sockIdx); // +1 for Ter, +1 for Code at beginning +1 for sender gap.
								}
							if(((CAsyncSocketEx *)Server->m_socketList[o_sockIdx]))	// check if socket still open.
								{
								((CAsyncSocketEx *)Server->m_socketList[o_sockIdx])->Kick(); // Kick his ass off ;)
								}
							sprintf(buffer," Done[%d]", o_sockIdx);
							AddLog("LogConnect.txt", buffer);
							
							// Don't Return, as we are only kicking the old socket //
							}
						
						}

			


				// Work out the plr.state
				switch(plr.playertype)
					{
					case -1:break;// Normal Account
					case 0:plr.state&=PLRSTATE_AUTHMASK^PLRSTATE_MASK;
						   plr.state|=PLRSTATE_ADMIN;
						   break;
					case 1:plr.state&=PLRSTATE_AUTHMASK^PLRSTATE_MASK;
						   plr.state|=PLRSTATE_LEADMODERATOR;
						   break;
					case 2:plr.state&=PLRSTATE_AUTHMASK^PLRSTATE_MASK;
						   plr.state|=PLRSTATE_MODERATOR;
						   break;
					case 3:plr.state&=PLRSTATE_AUTHMASK^PLRSTATE_MASK;
						   plr.state|=PLRSTATE_MODERATOR;
						   break;
					
					}

				if(sd->m_sysID==0 && plr.state&PLRSTATE_GUESTMEMBER) // dont let sysid-0 people log in with guest member accounts
				{
				// reject sysid 0's logging in on guest accounts.
				((CGameAppView *)m_pParentWnd)->SendLoginFailed(TRUE, 14, plrSockIdx);
				return FALSE;
				}
				
				// Let people with No MAC Address login for now. We have no idea how many people have these or not
				/*if(sd->m_macID_U==0 && sd->m_macID_L==0 && plr.state&PLRSTATE_GUESTMEMBER) // dont let sysid-0 people log in with guest member accounts
				{
				// reject sysid 0's logging in on guest accounts.
				((CGameAppView *)m_pParentWnd)->SendLoginFailed(TRUE, 14, plrSockIdx);
				return FALSE;
				}*/
				


				if(plr.state&PLRSTATE_GUESTMEMBER) {
												   if(g_daysFreeTrial==99) plr.bIsGuest = TRUE;
												   }

				if(plr.state&PLRSTATE_GUESTMEMBER) // Trialists.
				{
				if(g_disabletriallock==FALSE)
					{
					// sysID Check //
					if(plr.trialsysid!=0) // we have a trial code.
						{
						if(plr.trialsysid!=sd->m_sysID)
							{
							char tempbuf[256];
							sprintf(tempbuf,"^Trialist Sysid mismatch (Memberfile:%lx - Client:%lx)",plr.trialsysid,sd->m_sysID);
							AddLog("LogConnect.txt", tempbuf);
							((CGameAppView *)m_pParentWnd)->SendLoginFailed(TRUE, 14, plrSockIdx);
							return FALSE;
							}
						}
					// macID Check 
					/* 
					// System ID Check for Trial Mismatch is good enough 
					if(plr.trialmacid_u!=0 || plr.trialmacid_l!=0) // we have a trial MAC address.
						{
						if(sd->m_macID_U!=0 || sd->m_macID_L!=0) // we have a trial MAC address.
							{
							char mac1[80], mac2[80];
							sprintf(mac1, "%lx%lx", plr.trialmacid_u, plr.trialmacid_l);
							sprintf(mac2, "%lx%lx", sd->m_macID_U,sd->m_macID_L);
			
							//if(plr.trialmacid_u!=sd->m_macID_U || plr.trialmacid_l!=sd->m_macID_L)
							if(strcmpi(mac1, mac2))
								{
								char tempbuf[256];
								sprintf(tempbuf,"^Trialist MACid mismatch (Memberfile:%lx%lx - Client:%lx%lx)",plr.trialmacid_u, plr.trialmacid_l,sd->m_macID_U,sd->m_macID_L);
								AddLog("LogConnect.txt", tempbuf);
								((CGameAppView *)m_pParentWnd)->SendLoginFailed(TRUE, 14, plrSockIdx);
								return FALSE;
								}
							}
						}
					*/
									
					
					}
				}



				if(g_membersonly==TRUE) // refuse trialists.
				{
					if(plr.state&PLRSTATE_GUESTMEMBER) // Trialists.
					{
					((CGameAppView *)m_pParentWnd)->SendLoginFailed(TRUE, 15, plrSockIdx);
					return FALSE;					
					}
				}


				

				
				// If we have passed all the login checks, we should update the sysid, macid and ip on the account //
				if(CheckPasswordEx(sd->m_player2.name, sd->m_player2.password))
					{
					// Password Matches, so Update Login Details //
					UpdateSysMacIpEx(sd->m_player2.name, ssysid, smacid, sip);
					}

				///////////////////////////////////////////////////////
		mainLogin:
				timetoexpire = 0;//TimeToExpire(&plr);
				
				// For iSnooker only. We have an Expiry Date on accounts
				// if(g_build.gameType == GAMETYPE_SNOOKER) 
				if(plr.bIsGuest==FALSE ) // if it's a member account, check it's expiry.
				{
				timetoexpire = TimeToExpire(&plr);
				if(timetoexpire<=0)
					{
					((CGameAppView *)m_pParentWnd)->SendLoginNewAccount(TRUE, plrSockIdx,timetoexpire);
					return FALSE;
					}
				}

				/*
				// Make sure Account is activated before we'll login (currently only for iPool)
				if(g_build.gameType == GAMETYPE_POOL) 
				if(plr.bIsGuest==FALSE ) // if it's a member account, check it's expiry.
				{
					if(plr.activated == 0)
					{
						((CGameAppView *)m_pParentWnd)->SendLoginFailed(TRUE, 15, plrSockIdx); // lobby full.
						return FALSE;
					}
				}
				*/


				// If we're a Member, get our Ladder Rank
				if (!plr.bIsGuest)
				if (!(plr.state&PLRSTATE_GUESTMEMBER))
				{
					plr.ladderRank = GetLadderRank(plr.name, plr.ladderPoints);
				}


				plr.id = g_id_count;
				g_id_count++;

				// Set Socket state from handshaking to ready. (mainly for debugging reasons, sockets in handshaking mode need to be analyzed more!)
				if(Server)
				if(Server->m_socketList[plrSockIdx])
				((CAsyncSocketEx *)Server->m_socketList[plrSockIdx])->m_state = SOCKET_READY; // no longer handshaking


				// update server : we have m_sysID, keep a log of
				((CGameAppView *)m_pParentWnd)->m_lobby->AddPlayer(&plr);
				
				// Forward message onto clients.
				((CGameAppView *)m_pParentWnd)->SendPlayerDetailsCompact(TRUE, &plr);


				{
				char buffer[256];
				if(((CGameAppView *)m_pParentWnd)->m_greeting[0]!=0)
				((CGameAppView *)m_pParentWnd)->SendServerMessage(((CGameAppView *)m_pParentWnd)->m_greeting, plrSockIdx);
				

				if(plr.bIsGuest==TRUE)
					{
					#ifdef THE_POOL_CLUB
					sprintf(buffer,"You're using a guest account (%s), many features have been disabled.\r\nRegister your account at www.thepoolclub.com", plr.name);
					((CGameAppView *)m_pParentWnd)->SendServerMessage(buffer, plrSockIdx);
					#else
					sprintf(buffer,"You're using a guest account (%s), many features have been disabled.\r\nRegister your account at www.thesnookerclub.com", plr.name);
					((CGameAppView *)m_pParentWnd)->SendServerMessage(buffer, plrSockIdx);
					#endif
					}
				else{
					 

					 
					char s;
					s = ' ';
					if(timetoexpire!=1) s = 's';

					sprintf(buffer,"Your account will expire in %d day%c", timetoexpire,s);
					((CGameAppView *)m_pParentWnd)->SendServerMessage(buffer, plrSockIdx);
					 
					 
					 if((plr.state&PLRSTATE_AUTHMASK)==PLRSTATE_MODERATOR)
					 ((CGameAppView *)m_pParentWnd)->SendServerMessage("You have Moderator Powers", plrSockIdx);
					 else if((plr.state&PLRSTATE_AUTHMASK)==PLRSTATE_ADMIN)
					 ((CGameAppView *)m_pParentWnd)->SendServerMessage("You have Administrator Powers", plrSockIdx);
					 else if((plr.state&PLRSTATE_AUTHMASK)==PLRSTATE_LEADMODERATOR)
					 ((CGameAppView *)m_pParentWnd)->SendServerMessage("You have Lead Moderator Powers", plrSockIdx);


					 // Find out if we need to log this OnClose time //
					 if(plr.state&PLRSTATE_AUTHMASK || plr.state2&PLRSTATE2_LOGTIME) {
				 		// Inserts an entry into the logtime table for Logging In //
			 			db_logtime(plr.name, true);	
					 }


					 // Broadcast any upcoming events
					 ((CGameAppView*)m_pParentWnd)->ProcessEventBroadcasts(plrSockIdx);
				}

					
				}
	#endif // MAIN_SERVER
	return FALSE;
}

extern BOOL StingerSave(void);

BOOL CShellEx::OnGameReceive(CComm *pComm, CAsyncSocketEx *pSocket, void *lpBuf, int nBufLen)
{


	if(g_bClose) return false;

	unsigned char *buffer;
	int type;
	buffer = (unsigned char *) lpBuf;
	type = buffer[1];
	int plrSockIdx;

	plrSockIdx = pComm->GetSocketIndex(pSocket);
	int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(plrSockIdx);
	int plrTblIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetTableIdxFromPlayer(plrSockIdx);

	

	if(pComm==Server)
	{
		switch(type)
		{
			case MSGGAME_OPEN:
			// Client is open (i.e. updated of all sys stuff, just needs game info updates)
			// send game info updates to pSocket only!!
			break;

			// lobby operations
			case MSGGAME_PLAYERDETAILS:
				{
				//ErrorMessage(m_pParentWnd, "MSGGAME_PLAYERDETAILS: PRE");
				if(buffer[2]==1) return OnGameReceivePlayerDetails2(lpBuf,plrSockIdx, nBufLen); // More Efficient!
				else 
				{
				char buffer[80];
				sprintf(buffer,"(---VER Failed, Old Login style---) [%s]", g_build.version);
				AddLog("LogConnect.txt", buffer);

				((CGameAppView *)m_pParentWnd)->Message(buffer,10); // personal message	
				((CGameAppView *)m_pParentWnd)->SendLoginFailed(TRUE, 10, plrSockIdx); // incorrect version/bad crc						
				}
				//OnGameReceivePlayerDetails2(lpBuf);
				//OnGameReceivePlayerDetails2(lpBuf, plrSockIdx);
				//ErrorMessage(m_pParentWnd, "MSGGAME_PLAYERDETAILS: POST");
				}
			break;
			
			case MSGGAME_SENDNEWACCOUNT:
				{


				int exists = 0;
				int topup = 0; // defaultly no topup.
				int style;
				newAccountSendData *sd;
				sd = (newAccountSendData_typ *) lpBuf;

				
				style = sd->buffer[2];
				
				//CONNECTSTYLE_NORMAL - same as current (allows all methods)
				//CONNECTSTYLE_NEWACC - can only create new accounts - if account exists then fail!
				//CONNECTSTYLE_TOPUP - can't create new accounts, can only topup/revive - must be an existing account.

				char nameFile[256];
				/*
				sprintf(nameFile,"%sMembers/",g_basedir);
				//strcpy(nameFile, "Members/");
				strcat(nameFile, sd->m_name);
				strcat(nameFile, ".txt");
				*/

				// Guest Login Checking /////////////////////////////////////////////////////////////////////
				char guest_check[80];
				strcpy(guest_check,sd->m_name);
				guest_check[5] = 0; // "guest\0"

				if(!strcmpi(guest_check,"guest")) // no accounts with guest???? are allowed.
				{
				((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 1, plrSockIdx); // Account already exists
				goto endOfNewAccount;
				break;
				}

				
				// If the username contains the symbol =, doesn't allow it //
				// Don't allow extended character (chars beyond 127) //
				int i;
				for(i=0;i<strlen(sd->m_name);i++)
				if(sd->m_name[i] == 61 || sd->m_name[i] >= 128)
				{
				((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 7, plrSockIdx); // Membership code is invalid
					goto endOfNewAccount;
				break;

				

				}


#ifndef MYSQL_SYSTEM
				// [-1] Check if banned account already exists!!
				sprintf(nameFile,"%sMembers/%s.ban",g_basedir,sd->m_name);
				if(PlayerFileExists(nameFile)) 
				{
				((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 1, plrSockIdx); // Account already exists
				goto endOfNewAccount;
				}
				sprintf(nameFile,"%sMembers2/%s.ban",g_basedir,sd->m_name);
				if(PlayerFileExists(nameFile))
				{
				((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 1, plrSockIdx); // Account already exists
				goto endOfNewAccount;
				}

				sprintf(nameFile,"%sMembers/%s.txt",g_basedir,sd->m_name);
#endif

				// [0] Check if someone stupidly tries to put 'guest' for their account
				if(!strcmpi(sd->m_name,"guest")) 
					{
					((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 1, plrSockIdx); // Account already exists
					goto endOfNewAccount;
					}



				// [0] Check if referrer exists (if a player typed in one) (must be a member too)
				
				if(sd->m_mReferer[0] != 0)
				{
				Player plr;
				BOOL success = FALSE;
				success = LoadPlayerFileEx(sd->m_mReferer, &plr);
				if(!success || plr.playertype > 4)	// Trialists = 5, Guests = 6 //
					{
					((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 8, plrSockIdx); // Bad Referrer Name (not a Member)
					goto endOfNewAccount;
					}
				}

				// [1] Check if account already exists!!						 

//Moved Higher Up in Code
#ifdef MYSQL_SYSTEM
if(LoadPlayerFileEx(sd->m_name, NULL)>0) exists = 1;
else exists = 0;
#else
				if(PlayerFileExists(nameFile)) exists = 1;
				else {
					 sprintf(nameFile,"%sMembers2/%s.txt",g_basedir,sd->m_name);
					 if(PlayerFileExists(nameFile)) exists = 2;
					 }
				// 0 - doesn't exist
				// 1 - exists (Main Member)
				// 2 - exists (Guest Member)
#endif

				if(exists)
					{
					if(style==CONNECTSTYLE_NEWACC) // For 'new accounts' mode you can't do topups!
						{
						((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 1, plrSockIdx); // Account already exists
						goto endOfNewAccount;					
						}
					// [1.1] If password is same as account, then we'll top it up, instead of saying account already exists.
					// ^^ it's gotta be the same guy!!
					Player plr;
#ifdef MYSQL_SYSTEM
					LoadPlayerFileEx(sd->m_name, &plr); // just to get password.
#else
					LoadPlayerFile(nameFile, &plr); // just to get password.
#endif
					if(strcmpi(sd->m_password, plr.password) != 0)
						{ // invalid. password reject membership.
						((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 1, plrSockIdx); // Account already exists
						goto endOfNewAccount;
						}
					else{ // Correct password & account name it's a top up!
						topup=1;
						}
					}
				else {
					if(style==CONNECTSTYLE_TOPUP) // For 'top ups/expiry' mode you can't create new accounts!
						{
						((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 1, plrSockIdx); // Account already exists
						goto endOfNewAccount;					
						}
				}
				// [2] Check if Membership code is valid by sending code to 

				//((CGameAppView *)m_pParentWnd)->HTTPRequestMemberShipCode(sd->m_mCode);

				// [2] Check if Membership code is valid
				int type;
				type = MembershipCodeValid(sd->m_mCode);
				if(!type)
					{
					((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 2, plrSockIdx); // Membership code is invalid
					goto endOfNewAccount;
					}
				// [3] Membership code is valid, let's create account!!!
				// MCODE_SPECIAL/MCODE_1YEAR/MCODE_6MONTHS/MCODE_3MONTHS/MCODE_TRIAL
				
				if((type==MCODE_TRIAL || type==MCODE_GUESTMEMBER) && topup)
					{// Trial codes can only work for making accounts, not for topping up them (as that'll be messed up i.e. 7+7+7+7...).
					((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 2, plrSockIdx); // Membership code is invalid
					goto endOfNewAccount;
					}
				if(type==MCODE_TRIAL)
				{
				CString rPeerAddress;
				UINT rPeerPort;

				// Get External IP (rPeerAddress, rPeerPort) ///////////////////////////////////////////////
				pSocket->GetPeerName(rPeerAddress, rPeerPort);


				if(g_daysFreeTrial==99) // Infinite-Life GuestMember accounts, therefore u can create infinite, just takes time (cooldown)
					{
					// COOL DOWN SYSTEM //
					if( ((CGameAppView *)m_pParentWnd)->m_ipCoolDown->CheckIP(rPeerAddress.GetBuffer(0)) == FALSE)
						{ // Code Reached Limit.
						((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 4, plrSockIdx); // Membership code is invalid
						goto endOfNewAccount;
						}				
					}
				else // Limited GuestMember accounts, therefore we make it finite, one code per computer.
					{
					
					// ONCE ONLY SYSTEM //
					DWORD id;
					WORD cpuid;
					DWORD mac_u;
					WORD mac_l;
					id = sd->m_sysID;
					cpuid = sd->m_cpuID;
					mac_u = sd->m_macID_U;
					mac_l = sd->m_macID_L;

					
					// if(!id) goto allowitz; // Just incase a bunch of people have a messed up code or something!
					/*
					if(!id) // don't allow code 0 creations.
						{
						((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 5, plrSockIdx); // Membership code is invalid
						goto endOfNewAccount;
						}
					// Cache check first (less Harddrive Demanding) //
					if( ((CGameAppView *)m_pParentWnd)->m_syslist->CheckID(id) == FALSE) // Already there.
						{ // Code Reached Limit.
						((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 5, plrSockIdx); // Membership code is invalid
						goto endOfNewAccount;
						}
					// File check second //
					((CGameAppView *)m_pParentWnd)->m_syslist->Load();
					if( ((CGameAppView *)m_pParentWnd)->m_syslist->CheckID(id) == FALSE) // Already there.
						{ // Code Reached Limit.
						((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 5, plrSockIdx); // Membership code is invalid
						goto endOfNewAccount;
						}
					*/

		

					// If Impregnated files existed, then Don't let this user make another Trial account //
					//if(0)
					if(sd->MultiTrialist==1)
						{
						((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 5, plrSockIdx); // Membership code is invalid
							goto endOfNewAccount;
						}

					// If MAC Address Exists, Check this First for DEFENSE SYSTEM
					// MAC Address Check for Account Create
					if(mac_u && mac_l) // Check If MAC Address Exists
						{
						// If MAC Defense is on, then don't allow Account Creation if MAC address is already in our DataBank
						if(g_defensemac)
							{
							// Cache check first (less Harddrive Demanding) //
							if( ((CGameAppView *)m_pParentWnd)->m_maclist->CheckMAC(mac_u, mac_l) == FALSE) // Already there.
								{ // Code Reached Limit.
								((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 5, plrSockIdx); // Membership code is invalid
								goto endOfNewAccount;
								}
							// File check second //
							((CGameAppView *)m_pParentWnd)->m_maclist->Load();
							if( ((CGameAppView *)m_pParentWnd)->m_maclist->CheckMAC(mac_u, mac_l) == FALSE) // Already there.
								{ // Code Reached Limit.
								((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 5, plrSockIdx); // Membership code is invalid
								goto endOfNewAccount;
								}
							}
						// MAC Defense is disabled, so just Log the MAC ID, (SYSID), IP and Trial Name if MAC address is present in DataBank
						else{
							// Cache check first (less Harddrive Demanding) //
							if( ((CGameAppView *)m_pParentWnd)->m_maclist->CheckMAC(mac_u, mac_l) == FALSE) // Already there.
								{ // Code Reached Limit.
								((CGameAppView *)m_pParentWnd)->m_maclist->MACsysLog(sd->m_name, sd->m_password, sd->m_macID_U, sd->m_macID_L, sd->m_sysID, sd->m_cpuID);
								goto nextpart_mac;
								}
							// File check second //
							((CGameAppView *)m_pParentWnd)->m_maclist->Load();
							if( ((CGameAppView *)m_pParentWnd)->m_maclist->CheckMAC(mac_u, mac_l) == FALSE) // Already there.
								{ // Code Reached Limit.
								((CGameAppView *)m_pParentWnd)->m_maclist->MACsysLog(sd->m_name, sd->m_password, sd->m_macID_U, sd->m_macID_L, sd->m_sysID, sd->m_cpuID);
								}
							nextpart_mac:;
							}
					
						// Add MAC to list & save//
						((CGameAppView *)m_pParentWnd)->m_maclist->AddMAC(mac_u, mac_l);
						((CGameAppView *)m_pParentWnd)->m_maclist->Save();
						}

					if(!id) // don't allow code 0 creations.
						{
						((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 5, plrSockIdx); // Membership code is invalid
						goto endOfNewAccount;
						}
					else{ // USE SystemID + CPU ID
						// If SYS Defense is on, then don't allow Account Creation if SYS ID is already in our DataBank
						if(g_defensesys)
							{
							// Cache check first (less Harddrive Demanding) //
							if( ((CGameAppView *)m_pParentWnd)->m_syslist->CheckID(id, cpuid) == FALSE) // Already there.
								{ // Code Reached Limit.
								((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 5, plrSockIdx); // Membership code is invalid
								goto endOfNewAccount;
								}
							// File check second //
							((CGameAppView *)m_pParentWnd)->m_syslist->Load();
							if( ((CGameAppView *)m_pParentWnd)->m_syslist->CheckID(id, cpuid) == FALSE) // Already there.
								{ // Code Reached Limit.
								((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 5, plrSockIdx); // Membership code is invalid
								goto endOfNewAccount;
								}
							}
						// SYS Defense is disabled, so just Log the SYSID, (MACID), IP and Trial Name if MAC address is present in DataBank
						else{
							if( ((CGameAppView *)m_pParentWnd)->m_syslist->CheckID(id, cpuid) == FALSE) // Already there.
								{ // Code Reached Limit.
								((CGameAppView *)m_pParentWnd)->m_syslist->SYSmacLog(sd->m_name, sd->m_password, sd->m_sysID, sd->m_cpuID, sd->m_macID_U, sd->m_macID_L);
								goto nextpart_sys;
								}
							// File check second //
							((CGameAppView *)m_pParentWnd)->m_syslist->Load();
							if( ((CGameAppView *)m_pParentWnd)->m_syslist->CheckID(id, cpuid) == FALSE) // Already there.
								{ // Code Reached Limit.
								((CGameAppView *)m_pParentWnd)->m_syslist->SYSmacLog(sd->m_name, sd->m_password, sd->m_sysID, sd->m_cpuID, sd->m_macID_U, sd->m_macID_L);
								}
							nextpart_sys:;

							}	

						// Add ID to list & save//
						((CGameAppView *)m_pParentWnd)->m_syslist->AddID(id, cpuid);
						((CGameAppView *)m_pParentWnd)->m_syslist->Save();
						}

					allowitz:;
					
					}
				

				((CGameAppView *)m_pParentWnd)->m_ipCoolDown->AddIP(rPeerAddress.GetBuffer(0));
				}


				Player plr;

				
				if(!topup)
				{
					NewAccountPlayer(&plr,type);
					plr.trialsysid = sd->m_sysID; // copy code over.
					plr.trialcpuid = sd->m_cpuID; // copy code over.
					plr.trialmacid_u = sd->m_macID_U; // copy code over.
					plr.trialmacid_l = sd->m_macID_L; // copy code over.

					if(type==MCODE_TRIAL)
					{
						if(sd->m_mHearAbout[0]!=0) // check if the hearabout field has something in it.
						TopUpAccountPlayer(&plr, MCODE_TRIAL2);
						else TopUpAccountPlayer(&plr, type);
					}
					else TopUpAccountPlayer(&plr, type);

					plr.rating = 1200;  // You're just a Minor!
					plr.highestrating = 1200;
					plr.ladderPoints = 12000;
					plr.ladderWinsLosses = 0;
					plr.ladderHighBreak = 0;
					plr.cash = 300;		// Start with $3.00 Virtual Cash
					plr.realcash = 0;
					strcpy(plr.name, sd->m_name);
					strcpy(plr.password, sd->m_password);
				}
				else { // "Top Up Your dead/alive account"
#ifdef MYSQL_SYSTEM				
				LoadPlayerFileEx(sd->m_name, &plr);
#else
				LoadPlayerFile(nameFile, &plr);
#endif

				TopUpAccountPlayer(&plr, type);
				}

				if(!strcmpi(sd->m_mReferer, sd->m_name)); // same name don't credit self. this is for referers only.
				else 
					{
					Player refplr;
					char refnameFile[256];
					sprintf(refnameFile,"%sMembers/%s.txt",g_basedir,sd->m_mReferer);

#ifdef MYSQL_SYSTEM					
					if(LoadPlayerFileEx(sd->m_mReferer, &refplr)!=0)
#else				
					if(LoadPlayerFile(refnameFile, &refplr)!=0)
					
#endif 				
					// Don't Allow referal Code if we are a Trialist //
					if(refplr.playertype<5) // Must be Member to Credit Time to //
					{
					char path[256];
					char buffer[256];
					sprintf(path,"%sReferralLog.txt",g_basedir);				

					switch(type)
						{
						case MCODE_1YEAR:	// Add 1 Month and $12.00 Virtual Cash to referrer
											TopUpAccountPlayer(&refplr, MCODE_1MONTH);
											refplr.cash += 1200;
											sprintf(buffer,"Credited referer '%s' with 1 month & $12.00 Virtual Cash thanks to '%s'",sd->m_mReferer, sd->m_name);
											AddLog(path, buffer);
											break;
						case MCODE_6MONTHS:	// Add 14 days and $4.00 Virtual Cash to referrer
											TopUpAccountPlayerDays(&refplr, 14);
											refplr.cash += 600;
											sprintf(buffer,"Credited referer '%s' with 14 days + $6.00 Virtual Cash thanks to '%s'",sd->m_mReferer, sd->m_name);
											AddLog(path, buffer);
											break;
						case MCODE_3MONTHS:	// Add 5 days and $3.00 Virtual Cash to referrer
											TopUpAccountPlayerDays(&refplr, 5);
											refplr.cash += 300;
											sprintf(buffer,"Credited referer '%s' with 5 days + $3.00 Virtual Cash thanks to '%s'",sd->m_mReferer, sd->m_name);
											AddLog(path, buffer);
											break;

						case MCODE_1MONTH:	// Add $1.00 Virtual Cash to referrer
											refplr.cash += 100;
											sprintf(buffer,"Credited referer '%s' with $1.00 Virtual Cash thanks to '%s'",sd->m_mReferer, sd->m_name);
											AddLog(path, buffer);
											break;
						default:			// Add $0.50 Virtual Cash to referrer if Trial Account was created
											refplr.cash += 100;
											sprintf(buffer,"Credited referer '%s' with $1.00 Virtual Cash thanks to '%s'",sd->m_mReferer, sd->m_name);
											AddLog(path, buffer);
											break;
						}					
#ifdef MYSQL_SYSTEM	
					SavePlayerFileEx(sd->m_mReferer, &refplr);  // save member file.
#else
					SavePlayerFile(refnameFile, &refplr);  // save member file.
#endif
					}
				}

#ifdef MYSQL_SYSTEM
plr.state|=PLRSTATE_GUESTMEMBER;
//if(type==MCODE_TRIAL) plr.state|=PLRSTATE_GUESTMEMBER;
if(type!=MCODE_TRIAL) plr.state^=PLRSTATE_GUESTMEMBER; // disable guestmember if proper account
SavePlayerFileEx(sd->m_name, &plr);  // save member file.		// *BP* This is where the Trial Account is first created //

#else
				// [1] Check if account already exists!!
				//if(!exists) // New Account.
				//{
				if(type==MCODE_TRIAL)
					{
					sprintf(nameFile,"%sMembers2/%s.txt",g_basedir,sd->m_name);
					}
				else {
					 sprintf(nameFile,"%sMembers/%s.txt",g_basedir,sd->m_name);
					 }
				//}

				SavePlayerFile(nameFile, &plr);  // save member file.
#endif

#ifdef MYSQL_SYSTEM
if(topup)  // topping up
{
if(type!=MCODE_TRIAL) // full member
{
// [+] add code for changing trialist->proper member (it'll just be a bool flag)						
}
}
#else				
				//((CGameAppView *)m_pParentWnd)->UpdateMember(&plr); // don't Update Rankings table since it has no games on it.
				if(topup)  // topping up
				{
				if(type!=MCODE_TRIAL) // full member
					{
					if(exists==2) // guestmember
						{ // topping up account from guestmember to fullmember (i.e. delete old account)
						plr.state|=PLRSTATE_GUESTMEMBER; // enable flag - so it deletes the other file
						((CGameAppView *)m_pParentWnd)->DeletePlayerProfile(&plr,".txt");
						plr.state^=PLRSTATE_GUESTMEMBER; // disable flag, coz we don't want it anymore!
						}
					}
				}
#endif
				if(topup) ((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 3, plrSockIdx); // Succesfully topped up
				else ((CGameAppView *)m_pParentWnd)->SendAccountCreated(TRUE, 0, plrSockIdx); // Succesfully created!

					{
					char buffer[MAX_CHARS];
					((CGameAppView *)m_pParentWnd)->GetTime();

					if(topup)
					{
					sprintf(buffer,"[%s Topped-Up] %s (pwd:%s) <sysid:%lx%lx><macid:%lx%lx><email:%s><mc:%s> %s(%s) %s [%s]",g_mcode_type_lst[type], 
							plr.name, plr.password, sd->m_sysID, sd->m_cpuID, sd->m_macID_U, sd->m_macID_L, sd->m_email, sd->m_mCode,
							g_serialStruct.email,g_serialStruct.order_no,
							((CGameAppView *)m_pParentWnd)->m_stringShortTime,
							((CGameAppView *)m_pParentWnd)->m_stringShortDate);
					}
					else {
					sprintf(buffer,"[%s Created] %s (pwd:%s) <sysid:%lx%lx><macid:%lx%lx><email:%s><mc:%s> %s(%s) %s [%s]",g_mcode_type_lst[type],
							plr.name,plr.password, sd->m_sysID, sd->m_cpuID, sd->m_macID_U, sd->m_macID_L, sd->m_email, sd->m_mCode,
							g_serialStruct.email,g_serialStruct.order_no,
							((CGameAppView *)m_pParentWnd)->m_stringShortTime,
							((CGameAppView *)m_pParentWnd)->m_stringShortDate);
						}
					char path[256];
					sprintf(path,"%sNewAccountLog.txt",g_basedir);
					AddLog(path, buffer);

					// Update NewAccounts
				
					
					char datestr[20];
					sprintf(datestr, "%s", ((CGameAppView *)m_pParentWnd)->m_stringShortDate);
					// if a 1Month, 6Months, 3Months, or 1 Year account, log this in the Database //
					
					if(type==MCODE_1YEAR || type==MCODE_6MONTHS || type==MCODE_3MONTHS || type==MCODE_1MONTH)
					{
						NewAccountLogEx(plr.name, g_db_type_lst[type], sd->m_mCode, g_serialStruct.email, datestr);
					}
					else
					{
						// Add Trial Account to Database, using the E-mail entered in the NewAccountDlg //
						NewAccountLogEx(plr.name, g_db_type_lst[type], sd->m_mCode, sd->m_email, datestr);
					}


					
					if(!topup)
						{
						sprintf(buffer,"%s:%s", plr.name, sd->m_mHearAbout);
						sprintf(path,"%sHearAboutLog.txt",g_basedir);
						AddLog(path, buffer);
						}
					}
				endOfNewAccount:;
				}
			break;

			case MSGGAME_PLAYERJOIN:
				{
										

					playerJoinSendData *sd;
					sd = (playerJoinSendData_typ *) lpBuf;

					// Update this player's lobbytablewatch variable so they don't get MSGGAME_BALLS messages for Lobby TV when inside a table
					// Get plrIdx //
					int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(sd->m_playerSockIdx);
					if(plrIdx) m_player[plrIdx].lobbytablewatch = -1;

					// If this is not a Guest Lobby, Don't allow Guest to Sit, so force EnteryType to -1
					if(g_guestonly == false)
					if(plrIdx)
					{
						if(m_player[plrIdx].bIsGuest)
						{
							sd->m_enterType = -1;
						}
					}

					// if this table is empty - prevent others from hosting it
					if( ((CGameAppView *)m_pParentWnd)->m_lobby->EmptyTable(sd->m_tableIdx) )
					{
						((CGameAppView *)m_pParentWnd)->m_lobby->TakeTable(sd->m_tableIdx);

						// update server
						((CGameAppView *)m_pParentWnd)->m_lobby->JoinTable(sd->m_tableIdx,
																		   sd->m_enterType,
																		   sd->m_playerSockIdx,
																		   TRUE);

						// Forward message onto clients.
						((CGameAppView *)m_pParentWnd)->SendPlayerJoin(
						TRUE, sd->m_tableIdx, sd->m_enterType, sd->m_playerSockIdx, TRUE);
					}
					else
					{
						if(((CGameAppView *)m_pParentWnd)->m_lobby->JoinTableHostComplete(sd->m_tableIdx))
						{
							// update server
							((CGameAppView *)m_pParentWnd)->m_lobby->JoinTable(sd->m_tableIdx,
																			   sd->m_enterType,
																			   sd->m_playerSockIdx,
																			   FALSE);
							

							// We must also send the players (for the joiner) a list of all the playeridxs that are seated //
							// get current player indexes for each seat //

							if(m_table[sd->m_tableIdx].doubles)
								{
								// plrIdx1 //
								if(m_table[sd->m_tableIdx].player1SockIdx != -1) sd->m_tableseatsA = m_table[sd->m_tableIdx].player1SockIdx;
									else sd->m_tableseatsA = 0xFFFF;
								// plrIdx1b //
								if(m_table[sd->m_tableIdx].player1bSockIdx != -1) sd->m_tableseatsA |= (m_table[sd->m_tableIdx].player1bSockIdx)<<16;
									else sd->m_tableseatsA |= 0xFFFF0000;

								// plrIdx2 //
								if(m_table[sd->m_tableIdx].player2SockIdx != -1) sd->m_tableseatsB = m_table[sd->m_tableIdx].player2SockIdx;
									else sd->m_tableseatsB = 0xFFFF;
								// plrIdx2b //
								if(m_table[sd->m_tableIdx].player2bSockIdx != -1) sd->m_tableseatsB |= (m_table[sd->m_tableIdx].player2bSockIdx)<<16;
									else sd->m_tableseatsB |= 0xFFFF0000;

									
								}
							else{
								sd->m_tableseatsA = 0xFFFFFFFF;	// This data isn't required if we are not a Doubles Game since seat positions never change during the game for singles games //
								sd->m_tableseatsB = 0xFFFFFFFF;	// This data isn't required if we are not a Doubles Game since seat positions never change during the game for singles games //
								}


							// Forward message onto clients.
							((CGameAppView *)m_pParentWnd)->SendPlayerJoin(
							TRUE, sd->m_tableIdx, sd->m_enterType, sd->m_playerSockIdx, FALSE, -1, sd->m_tableseatsA, sd->m_tableseatsB);
						}
						else
						{
							((CGameAppView *)m_pParentWnd)->SendPlayerJoinFailed(TRUE, sd->m_playerSockIdx, -1, sd->m_playerSockIdx);
						}
						
					}					
					
				}
			break;

			case MSGGAME_PLAYERJOINFAILED:
				{
											
					// update server	
					playerJoinFailedSendData *sd;
					sd = (playerJoinFailedSendData_typ *) lpBuf;

					((CGameAppView *)m_pParentWnd)->m_lobby->JoinTableFailed(sd->m_joinerSockIdx, sd->m_hostSockIdx);

					Server->SendToQueue(lpBuf, nBufLen); // Forward message onto to clients.
					
				}
				break;

			case MSGGAME_PLAYEREXIT:
				{
					// update server
					playerExitSendData *sd;
					sd = (playerExitSendData_typ *) lpBuf;
					
					

					int tblIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetTableIdxFromPlayer(sd->m_playerSockIdx);
					BOOL seated = ((CGameAppView *)m_pParentWnd)->m_lobby->SeatedAtTableIdx(g_gameAppView->m_playerSockIdx, tblIdx);
					//int PlayerSeat = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerSeat(sd->m_playerSockIdx);

					// For people who are playing, don't let them leave unless game hasn't started //
					if(seated)
						{
						if(!m_table[tblIdx].bGameStarted)
							{
							((CGameAppView *)m_pParentWnd)->m_lobby->ExitTable(sd->m_playerSockIdx, sd->m_bTimedOutBoot, sd->m_bPLC);
							Server->SendToQueue(lpBuf, nBufLen); // Forward message onto to clients.
							((CGameAppView *)m_pParentWnd)->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);
							}

						}
					// For people who are not playing, let them Exit without always //
					else{
						((CGameAppView *)m_pParentWnd)->m_lobby->ExitTable(sd->m_playerSockIdx, sd->m_bTimedOutBoot, sd->m_bPLC);
						Server->SendToQueue(lpBuf, nBufLen); // Forward message onto to clients.
						((CGameAppView *)m_pParentWnd)->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);
						}
				}
				break;

			case MSGGAME_PLAYERSIT:
				{
					// update server
					playerSitSendData *sd;
					sd = (playerSitSendData_typ *) lpBuf;


					// If this is not a Guest Lobby, Don't allow Guest to Sit
					if(g_guestonly == false)
					if(plrIdx)
					{
						if(m_player[plrIdx].bIsGuest)
						{
							break;
						}
					}

					((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(sd->m_seat1SockIdx,
																		   sd->m_seat2SockIdx,
																		   sd->m_tableIdx,
																		   sd->m_seat1bSockIdx,
																		   sd->m_seat2bSockIdx);

					Server->SendToQueue(lpBuf, nBufLen); // Forward message onto to clients.
				}
				break;

			case MSGGAME_PLAYERREJOIN: // *TODO: should only be sending to people at the table (SendToQueue() currently does not support this)
				{
					// update server
					// ?????

					Server->SendToQueue(lpBuf, nBufLen); // Forward message onto to clients.

				}
				break;
			
			case MSGGAME_TABLEHOSTDETAILS:
				{

					// update server
					tableHostDetailsSendData *sd;
					sd = (tableHostDetailsSendData_typ *) lpBuf;
					
					((CGameAppView *)m_pParentWnd)->m_lobby->UpdateTableHostDetails(sd->m_cueError,sd->m_bRated, sd->m_type, sd->m_bMissRule, sd->m_playerSockIdx,
														sd->m_shotTime, sd->m_tableColour, sd->m_gameType, sd->m_addTimer, sd->m_doubles,
														sd->m_raceTo, sd->m_whoBreaks,
														sd->m_ballSize, sd->m_rackType, sd->m_tableType, sd->m_proTable,
														sd->m_tournamentTable, sd->m_fp1, sd->m_fp2, sd->m_score1, sd->m_score2, sd->m_bLadderMatch);

					Server->SendToQueue(lpBuf, nBufLen); // Forward message onto to clients.

				}
			break;




			// game operations
			case MSGGAME_SHOT:
				//////////////////////////////////////////////////////////
				// *NEW - Server to reset simEndBallsMsg flag
				if(plrTblIdx >= 0 && plrTblIdx < MAX_LOBBY_TABLES)
				{
					//m_table[plrTblIdx].bSimEndMsgBalls = FALSE;
					//m_table[plrTblIdx].nSimEndMsgBalls++;
					
					strikeBallSendData *sd;
					sd = (strikeBallSendData_typ *) lpBuf;
					sd->m_nSimEndMsgBalls = m_table[plrTblIdx].nSimEndMsgBalls;


					// Temp Bug Fix for NaN Bug...
					// For now, in the rare event of this bug, give 0 error for the shot...
					if (sd->m_strikeRnd != sd->m_strikeRnd) sd->m_strikeRnd = 0;
					if (sd->m_strikeMaxRnd != sd->m_strikeMaxRnd) sd->m_strikeMaxRnd = 0;

					// Make error 97% of total error
					sd->m_strikeRnd *= 0.97;
					sd->m_strikeMaxRnd *= 0.97;


					
					// LowErrorLog
					bool bLog = false;
					
					
					char buffer[256];
					
					// sprintf(buffer, "m_strikeRnd: %f", m_strikeRnd); 
					// ErrorMessage(m_parent, buffer); // de-rem this line to display this error!
	
	
					if(g_build.gameType == GAMETYPE_SNOOKER) 
					{
						if(m_table[plrTblIdx].cueError >= 1)
						{
							// For Snooker, log any error less than 0.0001
							if(sd->m_strikeRnd > -0.0001 && sd->m_strikeRnd < 0.0001)
							{
								bLog = true;
							}
						}
					}
					else
					{
						// For Pool, log any error less than 0.00003
											 
						if(sd->m_strikeRnd > -0.00003 && sd->m_strikeRnd < 0.00003)
						{
							bLog = true;
						}
					}

					if(bLog)
					{
						((CGameAppView *)m_pParentWnd)->GetTime();
						// Log the Random Error and the Players (with the player having their shot first.
						if(m_table[plrTblIdx].player1SockIdx == plrSockIdx)
						{
							sprintf(buffer, "%s %s - m_strikeRnd: %f - %s (in-play) vs %s", ((CGameAppView *)m_pParentWnd)->m_stringShortDate, ((CGameAppView *)m_pParentWnd)->m_stringShortTime, sd->m_strikeRnd, m_table[plrTblIdx].P1StartName, m_table[plrTblIdx].P2StartName);
						}
						else if(m_table[plrTblIdx].player2SockIdx == plrSockIdx)
						{
							sprintf(buffer, "%s %s - m_strikeRnd: %f - %s (in-play) vs %s", ((CGameAppView *)m_pParentWnd)->m_stringShortDate, ((CGameAppView *)m_pParentWnd)->m_stringShortTime, sd->m_strikeRnd, m_table[plrTblIdx].P2StartName, m_table[plrTblIdx].P1StartName);
						}
						else;

						AddLog("LogLowError.txt", buffer); // ### Log 

						// AfxMessageBox(buffer);
					}
					

					// Disable LobbyTV Show Show for Now //
					int n;
					for(n=0;n<MAX_LOBBY_PLAYERS;n++)
					if(m_player[n].status)
					{
						int tblIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetTableIdxFromPlayer(m_player[n].playerSocketIdx);
						if(tblIdx == -1)	// Only Do Following Code if this player is in the Lobby
						if(m_player[n].lobbytablewatch == plrTblIdx) 
						{
							Server->SendToQueue(lpBuf, nBufLen, m_player[n].playerSocketIdx);
						}
					}
				}

				//////////////////////////////////////////////////////////
			case MSGGAME_RESET:
			case MSGGAME_BALL_POS:
			case MSGGAME_SELECT_BALL:
			case MSGGAME_MOUSE_MOVE:
			case MSGGAME_CUE_BALL_TIP:
			case MSGGAME_PLAYAGAIN:	
			case MSGGAME_UPDATEGAME:	
			case MSGGAME_FOULALERT:
			case MSGGAME_MAXTIMEUP:
			case MSGGAME_MISSRESET:
			case MSGGAME_POT8BALLONBREAKRERACK:
			case MSGGAME_BADBREAKRERACK:
			case MSGGAME_PLAYERTARGETDATA:
			//case MSGGAME_CUE_BALL_TIPC: // Compact version! * ignore originator
				SendToQueueTable(Server, lpBuf, nBufLen, plrTblIdx);
				type = type;
			break;

			case MSGGAME_RESET_BYSERVER:
				SendToQueueTable(Server, lpBuf, nBufLen, buffer[6]); // buffer[6] is the tblIdx that server wants to reset the match for
			break;
			
			case MSGGAME_ABORT_BYSERVER:
				//plrTblIdx = buffer[6];
				((CGameAppView *)m_pParentWnd)->m_lobby->EndGameReceive(buffer[6]);	// Tell Server to End the Game //
				Server->SendToQueue(lpBuf, nBufLen); // Forward drop message back to clients.
				//SendToQueueTable(Server, lpBuf, nBufLen, buffer[6]); // buffer[6] is the tblIdx that server wants to reset the match for
			break;

			case MSGGAME_CUE_BALL_TIPC: // Compact version! * ignore originator i.e. person who's turn it is
				SendToQueueTableExceptOne(Server, lpBuf, nBufLen, plrTblIdx, plrIdx);
			break;
			
			// Server Sending itself a msg, that is now rebroadcasted to people at a table
			case MSGGAME_TMATCHTIMERLEFT:
				{
					TMatchTimerLeftSendData *sd;
					sd = (TMatchTimerLeftSendData_typ *) lpBuf;
					
					SendToQueueTable(Server, lpBuf, nBufLen, sd->m_tblIdx);
				}
			break;

			case MSGGAME_TEXTMSG:
				{
					TextMsgSendData *sd;
					sd = (TextMsgSendData_typ *) lpBuf;
					
					if(sd->m_plrSockIdx == -1) // send to all...
					{
						if(sd->m_tblIdx == -1) // now send to all...
							Server->SendToQueue(lpBuf, nBufLen); // Forward drop message back to clients.
						else // get server's own client to send it to itself, so that it can be rebroadcasted to all at specified table...
							SendToQueueTable(Server, lpBuf, nBufLen, sd->m_tblIdx);
					}
					else // send to an individual...
						Server->SendToQueue(lpBuf, nBufLen, sd->m_plrSockIdx); // Forward message back to client.
				}
			break;


			case MSGGAME_TEMPERLOG:
				{
					TemperSendData *sd;
					sd = (TemperSendData_typ *) lpBuf;
					// Get plrIdx //
					int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(sd->m_plrSockIdx);
					if(plrIdx)
						{
						char path[256];
						char buffer[256];
						sprintf(path,"%sLogTemper.txt",g_basedir);
						((CGameAppView *)m_pParentWnd)->GetTime();
						sprintf(buffer, "%s modifed files %s %s - <ip:%s> <sysid:%lx%lx> <macid:%lx%lx>", m_player[plrIdx].name, ((CGameAppView *)m_pParentWnd)->m_stringShortDate, ((CGameAppView *)m_pParentWnd)->m_stringShortTime, m_player[plrIdx].playerIP, m_player[plrIdx].sysID, m_player[plrIdx].cpuID, m_player[plrIdx].macID_U, m_player[plrIdx].macID_L);
						AddLog(path, buffer);
						}

					// Send a MSG to Client, to Crash Their Application //

					buffer[0] = 0x7f;	// MSGCODE_GAME Game message.
					buffer[1] = 105;	// MSGGAME_CRASH
					buffer[2] = 0;
					buffer[3] = 0;		// Reason flag.

					if(mainShell) mainShell->Server->SendToQueue(buffer, 4, sd->m_plrSockIdx); // +1 for Ter, +1 for Code at beginning +1 for sender gap.
					
				}
			break;


			

			// other
			case MSGGAME_STARTPRESSED:
			case MSGGAME_CONCEDEPRESSED:
			case MSGGAME_GAMEOVERFORALL:
			//case MSGGAME_PLAYERLOSTCONN:
			//case MSGGAME_UNFINISHEDGAME:
				Server->SendToQueue(lpBuf, nBufLen); // Forward drop message back to clients.
			break;

			case MSGGAME_RERACKPRESSED:
				// Update Server //
				/*
				ReRackPressedSendData *sd;
				sd = (ReRackPressedSendData_typ *) lpBuf;
				// We must update our stats for ReRacks here if accepted is true //
				if(sd->m_bAccepted)
					{
					// get member_id and current number of reracks //
					int plrIdx;
					int tblIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetTableIdxFromPlayer(sd->m_plrSockIdx);
					// Player 1 ReRacks //
					if(tblIdx != -1)
						{
						if(m_table[tblIdx].player1SockIdx != -1)
							{
							plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(m_table[tblIdx].player1SockIdx);
							m_player[plrIdx].reracks++;
							db_updatereracks(m_player[plrIdx].name, m_player[plrIdx].reracks);
							}
						// Player 2 ReRacks //
						if(m_table[tblIdx].player2SockIdx != -1)
							{
							plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(m_table[tblIdx].player2SockIdx);
							m_player[plrIdx].reracks++;
							db_updatereracks(m_player[plrIdx].name, m_player[plrIdx].reracks);
							}
						// Player 1b ReRacks //
						if(m_table[tblIdx].player1bSockIdx != -1)
							{
							plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(m_table[tblIdx].player1bSockIdx);
							m_player[plrIdx].reracks++;
							db_updatereracks(m_player[plrIdx].name, m_player[plrIdx].reracks);
							}
						// Player 2b ReRacks //
						if(m_table[tblIdx].player2bSockIdx != -1)
							{
							plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(m_table[tblIdx].player2bSockIdx);
							m_player[plrIdx].reracks++;
							db_updatereracks(m_player[plrIdx].name, m_player[plrIdx].reracks);
							}
						}
					}
				*/	
				Server->SendToQueue(lpBuf, nBufLen); // Forward drop message back to clients.
				break;

			case MSGGAME_UPDATEPLAYERBIO:
			{ // for server only
				PlayerBioSendData *sd;
				sd = (PlayerBioSendData_typ *) lpBuf;				

				((CGameAppView *)m_pParentWnd)->ReceivePlayerBioData(plrIdx, sd->m_cueError, sd->m_bioAverageShotTime, sd->m_bioAveragePosition, sd->m_bioAverageSafety, sd->m_bioAverageBreak);
			}
			break;

			case MSGGAME_REQUESTPLAYERBIO:
			{ // for server only
				RequestPlayerBioSendData *sd;
				sd = (RequestPlayerBioSendData_typ *) lpBuf;								

				//ErrorMessage(g_wnd, "AAA1");

				// remember requester's SockIdx for return msg later
				sd->m_returnPlrSockIdx = plrSockIdx;

				// send to player that we are requesting Bio. from
				if(sd->m_plrSockIdx!=-1)
				{
					//ErrorMessage(g_wnd, "AAA2");
					Server->SendToQueue(lpBuf, nBufLen, sd->m_plrSockIdx); // Forward message on to client.
				}
			}
			break;

			case MSGGAME_RETURNPLAYERBIO:
			{ // for server only
				//ErrorMessage(g_wnd, "BBB");

				ReturnPlayerBioSendData *sd;
				sd = (ReturnPlayerBioSendData_typ *) lpBuf;								

				// send to oringal requesting player
				Server->SendToQueue(lpBuf, nBufLen, sd->m_returnPlrSockIdx); // Forward message on to client.
			}
			break;
						
			
			case MSGGAME_PLAYERLOSTCONN:
			{
				PLCSendData *sd;
				sd = (PLCSendData_typ *) lpBuf;
				Server->SendToQueue(lpBuf, nBufLen); // Forward drop message back to clients.
				//SendToQueueTable(Server, lpBuf, nBufLen, sd->m_tblIdx); // Forward drop message back to clients. 				
		
			}
			break;

			case MSGGAME_PLAYERCONN:
			{
				// Player has rejoined..
				PCSendData *sd;
				sd = (PCSendData_typ *) lpBuf;

				int tblIdx = sd->m_tblIdx;
				// Tell everyone how many disconnections we have remaining...
				if(tblIdx != -1)
				{
					char buff[256];
					#ifdef THE_POOL_CLUB
					sprintf(buff, "Note: 3 Disconnections is a loss of a Rack!");
					#else
					sprintf(buff, "Note: 3 Disconnections is a loss of a Frame!");
					#endif
					((CGameAppView *)m_pParentWnd)->SendTextMsg(buff, 0, sd->m_playerSockIdx, -1);
					sprintf(buff, "%s has %d disconnection(s), %s has %d disconnection(s)", m_table[tblIdx].P1StartName, m_table[tblIdx].m_lostConn1P, m_table[tblIdx].P2StartName, m_table[tblIdx].m_lostConn2P);
					((CGameAppView *)m_pParentWnd)->SendTextMsg(buff, 0, sd->m_playerSockIdx, -1);
				}

				SendToQueueTable(Server, lpBuf, nBufLen, sd->m_tblIdx); // Forward the message back to Clients at the Table only //
				//Server->SendToQueue(lpBuf, nBufLen); // Forward drop message back to clients. 				
			}
			break;

			case MSGGAME_GAMEOVER:
			{
				// server to update ELO ratings! [i.e. override what we did in CLobby::WinnerDoGameOver(...)]
				GameOverSendData *sd;
				sd = (GameOverSendData_typ *) lpBuf;
					
				BOOL bFailedToLoadFiles = FALSE;
				


				// Don't Let Doubles Do this either //
				if(sd->m_bRated)
				{
					Player plr1;
					Player plr2;

					int RatingDeltaP1;
					int RatingDeltaP2;

								
					((CGameAppView *)m_pParentWnd)->m_lobby->ClearPlayer(&plr1);
					((CGameAppView *)m_pParentWnd)->m_lobby->ClearPlayer(&plr2);

					strcpy(plr1.name, sd->m_nameP1);
					strcpy(plr2.name, sd->m_nameP2);

					int loadedP1 = 1; // =1 trick to let guests thru
					int loadedP2 = 1;
					
					if(sd->m_ratingP1 > 0) loadedP1 = ((CGameAppView *)m_pParentWnd)->LoadPlayerProfile(&plr1, ".txt");
					if(sd->m_ratingP2 > 0) loadedP2 = ((CGameAppView *)m_pParentWnd)->LoadPlayerProfile(&plr2, ".txt");
							
					if(!loadedP1 || !loadedP2) bFailedToLoadFiles = TRUE;
					if(!bFailedToLoadFiles)
					{					
						// only do if both players are members / trialist
						if(sd->m_ratingP1 > 0 && sd->m_ratingP2 > 0)
						{
							sd->m_oldRating1P = plr1.rating;
							sd->m_oldRating2P = plr2.rating;	
															
							int oldLadderPointsP1 = plr1.ladderPoints;
							int oldLadderPointsP2 = plr2.ladderPoints;
							int oldLadderWinsLossesP1 = plr1.ladderWinsLosses;
							int oldLadderWinsLossesP2 = plr2.ladderWinsLosses;


							// Work Out Lobby Ratings / Tourney Ratings (if Rated Game)
							// (1+sd->m_bPlr1Won)  (Gives gameOver result) 1 (player1 lost) or 2 (player2 lost) //
							((CGameAppView *)m_pParentWnd)->m_lobby->CalcNewELORatings((1+sd->m_bPlr1Won), &plr1, &plr2);

							// Update Ladder Stats always. But double the rate, if a Ladder Match
							// if (m_table[sd->m_tblIdx].bLadderMatch)
							
							#ifndef THE_POOL_CLUB
							// iSnooker
							if (m_table[sd->m_tblIdx].cueError == 2)
							{
								// Pro (or 9-Ball if iPool)
								((CGameAppView *)m_pParentWnd)->m_lobby->CalcNewELOLadderPoints((1 + sd->m_bPlr1Won), sd->m_pHighBreakP1, sd->m_pHighBreakP2, true, &plr1, &plr2, m_table[sd->m_tblIdx].bLadderMatch);
							}
							else
							{
								// Semi Pro (of 8-Ball if iPool)
								((CGameAppView *)m_pParentWnd)->m_lobby->CalcNewELOLadderPoints((1 + sd->m_bPlr1Won), sd->m_HighBreakP1, sd->m_HighBreakP2, false, &plr1, &plr2, m_table[sd->m_tblIdx].bLadderMatch);
							}
							#else
							// iPool

							int runoutP1 = 0;
							int runoutP2 = 0;

							if (m_table[sd->m_tblIdx].gameType == 0)
							{
								// 8-Ball
								// Note Ladder Matches are currently always Pro Games
								if (sd->m_80sP1) runoutP1 = 1;
								if (sd->m_80sP2) runoutP2 = 1;
								((CGameAppView *)m_pParentWnd)->m_lobby->CalcNewELOLadderPoints((1 + sd->m_bPlr1Won), runoutP1, runoutP2, false, &plr1, &plr2, m_table[sd->m_tblIdx].bLadderMatch);
							}
							else 
							{
								// 9-Ball
								// Note Ladder Matches are currently always Pro Games
								if (sd->m_90sP1) runoutP1 = 1;
								if (sd->m_90sP2) runoutP2 = 1;
								((CGameAppView *)m_pParentWnd)->m_lobby->CalcNewELOLadderPoints((1 + sd->m_bPlr1Won), runoutP1, runoutP2, true, &plr1, &plr2, m_table[sd->m_tblIdx].bLadderMatch);
							}
							#endif		

														
							// Update Rating (Only If there's no Racks To Play //
							if(sd->m_bRacksStillToPlay)
							{
								plr1.rating = sd->m_oldRating1P;
								plr2.rating = sd->m_oldRating2P;

								plr1.ladderPoints = oldLadderPointsP1;
								plr2.ladderPoints = oldLadderPointsP2;
								plr1.ladderWinsLosses = oldLadderWinsLossesP1;
								plr2.ladderWinsLosses = oldLadderWinsLossesP2;
							}
					
							float level_progress_rfactorP1 = 1.0;
							float level_progress_rfactorP2 = 1.0;

							// Do all rating updates if for some other situations //
							if(sd->m_bInvalidGame || sd->m_ratingP1 == 0 || sd->m_ratingP2 == 0 || sd->m_bRacksStillToPlay)
							{
								// Do Nothing									
							}
							else
							{
								// update ELO rankings
								sd->m_ratingP1 = plr1.rating;
								sd->m_ratingP2 = plr2.rating;

								// Update Highest Rating
								if(plr1.rating > plr1.highestrating) plr1.highestrating = plr1.rating;
								if(plr2.rating > plr2.highestrating) plr2.highestrating = plr2.rating;

								// Update Highest Streak
								if (plr1.streak > plr1.highstreak) plr1.highstreak = plr1.streak;
								if (plr2.streak > plr2.highstreak) plr2.highstreak = plr2.streak;
							}

							// Update Cash //
								// All vCash and XP is multipled by Race to 
								//iSnooker gets $0.25 vCash for Win  //
								if(g_build.gameType == GAMETYPE_SNOOKER)
								{
									// Only Give Virtual Cash for 15 Reds or Billiards //
									if(m_table[sd->m_tblIdx].gameType == 0 || m_table[sd->m_tblIdx].gameType >= 4)
									{
									if(sd->m_bPlr1Won) 
										{
											plr1.cash += 45; // $0.45 for a win
											plr2.cash += 8; // $0.10 for a loss
										}
										else
										{
											plr2.cash += 45; // $0.45 for a win
											plr1.cash += 8; // $0.10 for a loss
										}
									}


									// If we made a high break then give extra level_progress points
									// We give 1.5x for Std, 2.0x for Pro and 2.5x for UK tables
							
									if(sd->m_HighBreakP1 > 50)
									{
										// So if we make a 100 break, then we're getting 1 + 1.5 = x2.5 factor
										level_progress_rfactorP1 = 1.0f + (float)(sd->m_HighBreakP1 - 50) * 0.01f;
									}
									else if(sd->m_pHighBreakP1 > 50)
									{
										// So if we make a 100 break on pro, then we're getting 1 + 2 = x3 factor
										level_progress_rfactorP1 = 1.0f + (float)(sd->m_pHighBreakP1 - 50) * 0.02f;
									}

									if(sd->m_HighBreakP2 > 50)
									{
										// So if we make a 100 break, then we're getting 1 + 1.5 = x2.5 factor
										level_progress_rfactorP2 = 1.0f + (float)(sd->m_HighBreakP2 - 50) * 0.01f;
									}
									else if(sd->m_pHighBreakP2 > 50)
									{
										// So if we make a 100 break on pro, then we're getting 1 + 2 = x3 factor
										level_progress_rfactorP2 = 1.0f + (float)(sd->m_pHighBreakP2 - 50) * 0.02f;
									}

								}	
								//iPool 
								else
								{
									// Only Give Virtual Cash for 8-Ball, 9-Ball & English Pool Normal / Pro //
									if(m_table[sd->m_tblIdx].tableType <= 2) // Std, Pro or UK Table
									if(m_table[sd->m_tblIdx].gameType <=1 || m_table[sd->m_tblIdx].gameType >= 6)  // 8-Ball or 9Ball, English Pool or Break
									{
										if(sd->m_bPlr1Won) 
										{
											plr1.cash += 3; // Gain $0.03 for a win
											plr2.cash += 1; // Gain $0.01 for a loss

											// Make sure we don't have negative money
											// if(plr2.cash < 0) plr2.cash = 0;

											// Give Extra vCash to Winner (P1) for RunOuts
											if(sd->m_50sP1)	plr1.cash += 1; // 8-Ball Std //
											else if(sd->m_80sP1) plr1.cash += 2; // 8-Ball Pro //
											else if(sd->m_p60sP1) plr1.cash += 3; // 8-Ball UK //
											else if(sd->m_60sP1) plr1.cash += 1; // 9-Ball Std //
											else if(sd->m_90sP1) plr1.cash += 2; // 9-Ball Pro //
											else if(sd->m_p70sP1) plr1.cash += 3; // 9-Ball UK //
											else if(sd->m_70sP1) plr1.cash += 1; // English Std //
											else if(sd->m_p50sP1) plr1.cash += 2; // English Pro //
											else if(sd->m_p80sP1) plr1.cash += 3; // English UK //

										}
										else
										{
											plr2.cash += 3; // Gain $0.03 for a win
											plr1.cash += 1; // Gain $0.01 for a loss

											// Make sure we don't have negative money
											// if(plr1.cash < 0) plr1.cash = 0;

											// Give Extra vCash to Winner (P2) for RunOuts
											if(sd->m_50sP2)	plr2.cash += 1; // 8-Ball Std //
											else if(sd->m_80sP2) plr2.cash += 2; // 8-Ball Pro //
											else if(sd->m_p60sP2) plr2.cash += 3; // 8-Ball UK //
											else if(sd->m_60sP2) plr2.cash += 1; // 9-Ball Std //
											else if(sd->m_90sP2) plr2.cash += 2; // 9-Ball Pro //
											else if(sd->m_p70sP2) plr2.cash += 3; // 9-Ball UK //
											else if(sd->m_70sP2) plr2.cash += 1; // English Std //
											else if(sd->m_p50sP2) plr2.cash += 2; // English Pro //
											else if(sd->m_p80sP2) plr2.cash += 3; // English UK //
										}
									}


									// If we made a Run-Out then give extra level_progress points
									// We give 1.5x for Std, 2.0x for Pro and 2.5x for UK tables
									
									if(sd->m_50sP1)	level_progress_rfactorP1 = 1.4;			// 8-Ball Std //
									else if(sd->m_80sP1) level_progress_rfactorP1 = 1.7;		// 8-Ball Pro //
									else if(sd->m_p60sP1) level_progress_rfactorP1 = 2.0;		// 8-Ball UK //
									else if(sd->m_60sP1) level_progress_rfactorP1 = 1.4;		// 9-Ball Std //
									else if(sd->m_90sP1) level_progress_rfactorP1 = 1.7;		// 9-Ball Pro //
									else if(sd->m_p70sP1) level_progress_rfactorP1 = 2.0;		// 9-Ball UK //
									else if(sd->m_70sP1) level_progress_rfactorP1 = 1.4;		// English Std //
									else if(sd->m_p50sP1) level_progress_rfactorP1 = 1.7;		// English Pro //
									else if(sd->m_p80sP1) level_progress_rfactorP1 = 2.0;		// English UK //

									if(sd->m_50sP2)	level_progress_rfactorP2 = 1.4;			// 8-Ball Std //
									else if(sd->m_80sP2) level_progress_rfactorP2 = 1.7;		// 8-Ball Pro //
									else if(sd->m_p60sP2) level_progress_rfactorP2 = 2.0;		// 8-Ball UK //
									else if(sd->m_60sP2) level_progress_rfactorP2 = 1.4;		// 9-Ball Std //
									else if(sd->m_90sP2) level_progress_rfactorP2 = 1.7;		// 9-Ball Pro //
									else if(sd->m_p70sP2) level_progress_rfactorP2 = 2.0;		// 9-Ball UK //
									else if(sd->m_70sP2) level_progress_rfactorP2 = 1.4;		// English Std //
									else if(sd->m_p50sP2) level_progress_rfactorP2 = 1.7;		// English Pro //
									else if(sd->m_p80sP2) level_progress_rfactorP2 = 2.0;		// English UK //
									

								}

								// Level Updating (if we're under level 10)
								
								if(sd->m_bPlr1Won)
								{
									plr1.level_progress += (35 * pow(plr1.level, 0.5)) * level_progress_rfactorP1;
									plr2.level_progress += (5 * pow(plr2.level, 0.5)) * level_progress_rfactorP2;
								}
								else
								{
									plr1.level_progress += (5 * pow(plr1.level, 0.5)) * level_progress_rfactorP1;
									plr2.level_progress += (35 * pow(plr2.level, 0.5)) * level_progress_rfactorP2;
								}


								// Adjust Level & Level Progress
								int level_progress_max;

			
								// Player 1 Level Adjust
								level_progress_max = (int) pow(plr1.level, 1.5)  * 100;
								while(plr1.level_progress >= level_progress_max)
								{
									plr1.level_progress -= level_progress_max;
									plr1.level++;

									// Update Level Progress Max now that we're at a new level
									level_progress_max = (int) pow(plr1.level, 1.5) * 100;
								}

								// Player 2 Level Adjust
								level_progress_max = (int) pow(plr2.level, 1.5)  * 100;
								while(plr2.level_progress >= level_progress_max)
								{
									plr2.level_progress -= level_progress_max;
									plr2.level++;

									// Update Level Progress Max now that we're at a new level
									level_progress_max = (int) pow(plr2.level, 1.5) * 100;
								}

						
								// Cap levels at 36
								// Cap Levels are 64 (new, added 3rd Jan 2017)
								int cap = 64;

								if(plr1.level > cap)
								{
									// If we exceed our level cap, then set level to this cap and level_progress to the max progress we can have without levelling
									plr1.level = cap;
									plr1.level_progress = -1 + (int) pow(cap, 1.5) * 100;
								}

								if(plr2.level > cap)
								{
									plr2.level = cap;
									plr2.level_progress =  -1 + (int) pow(cap, 1.5) * 100;
								}



							// Update the Rate of Run-Outs for 8-Ball, 9-Ball or English Pool //
							if(g_build.gameType == GAMETYPE_POOL)
							{
								// Std Table - 10% influence for making runout, 3% for not making a runout //
								// Pro Table RunOut - 10% influence for making runout, 2% for not making runout //
								// UK Table RunOut - 10% influence for making runout, 1% for not making a runout //

								float f100s;
								float fp90s;
								float fp100s;

								// Player 1 //
								f100s = plr1._100s;
								fp90s = plr1._p90s;
								fp100s = plr1._p100s;

								if(sd->m_50sP1)			f100s = (f100s * 0.90) + 100;	// 8-Ball Std //
								else if(sd->m_80sP1)	f100s = (f100s * 0.90) + 100;	// 8-Ball Pro //
								else if(sd->m_p60sP1)	f100s = (f100s * 0.90) + 100;	// 8-Ball UK //
								else if(sd->m_60sP1)	fp90s = (fp90s * 0.90) + 100;	// 9-Ball Std //
								else if(sd->m_90sP1)	fp90s = (fp90s * 0.90) + 100;	// 9-Ball Pro //
								else if(sd->m_p70sP1)	fp90s = (fp90s * 0.90) + 100;	// 9-Ball UK //
								else if(sd->m_70sP1)	fp100s = (fp100s * 0.90) + 100;	// English Std //
								else if(sd->m_p50sP1)	fp100s = (fp100s * 0.90) + 100;	// English Pro //
								else if(sd->m_p80sP1)	fp100s = (fp100s * 0.90) + 100;	// English UK //
								else
								{
									// No Run-Outs were made by Player 1, so we better Decay the appropriate Variable //
									if(m_table[sd->m_tblIdx].gameType == 0)	f100s *= g_GoldDecay8Ball;			// 8-Ball
									else if(m_table[sd->m_tblIdx].gameType == 1) fp90s *= g_GoldDecay9Ball;		// 9-Ball
									else if(m_table[sd->m_tblIdx].gameType == 7) fp100s *= g_GoldDecayEng;		// English
									else;
								}

								plr1._100s = (int)f100s;
								plr1._p90s = (int)fp90s;
								plr1._p100s = (int)fp100s;

								sd->m_100sP1 = plr1._100s;
								sd->m_p90sP1 = plr1._p90s;
								sd->m_p100sP1 = plr1._p100s;

								
								// Player 2 //
								f100s = plr2._100s;
								fp90s = plr2._p90s;
								fp100s = plr2._p100s;

								if(sd->m_50sP2)			f100s = (f100s * 0.90) + 100;	// 8-Ball Std //
								else if(sd->m_80sP2)	f100s = (f100s * 0.90) + 100;	// 8-Ball Pro //
								else if(sd->m_p60sP2)	f100s = (f100s * 0.90) + 100;	// 8-Ball UK //
								else if(sd->m_60sP2)	fp90s = (fp90s * 0.90) + 100;	// 9-Ball Std //
								else if(sd->m_90sP2)	fp90s = (fp90s * 0.90) + 100;	// 9-Ball Pro //
								else if(sd->m_p70sP2)	fp90s = (fp90s * 0.90) + 100;	// 9-Ball UK //
								else if(sd->m_70sP2)	fp100s = (fp100s * 0.90) + 100;	// English Std //
								else if(sd->m_p50sP2)	fp100s = (fp100s * 0.90) + 100;	// English Pro //
								else if(sd->m_p80sP2)	fp100s = (fp100s * 0.90) + 100;	// English UK //
								else
								{
									// No Run-Outs were made by Player 1, so we better Decay the appropriate Variable //
									if(m_table[sd->m_tblIdx].gameType == 0)	f100s *= g_GoldDecay8Ball;			// 8-Ball
									else if(m_table[sd->m_tblIdx].gameType == 1) fp90s *= g_GoldDecay9Ball;		// 9-Ball
									else if(m_table[sd->m_tblIdx].gameType == 7) fp100s *= g_GoldDecayEng;		// English
									else;
								}

								plr2._100s = (int)f100s;
								plr2._p90s = (int)fp90s;
								plr2._p100s = (int)fp100s;

								sd->m_100sP2 = plr2._100s;
								sd->m_p90sP2 = plr2._p90s;
								sd->m_p100sP2 = plr2._p100s;
							}



							// Update Stats 
							//P1
							sd->m_winsP1 = plr1.wins;
							sd->m_lossesP1 = plr1.losses;
							sd->m_streakP1 = plr1.streak;
							sd->m_highestratingP1 = plr1.highestrating;
							sd->m_cashP1 = plr1.cash;
							sd->m_reracksP1 = plr1.reracks;
							sd->m_ladderPointsP1 = plr1.ladderPoints;
							sd->m_ladderWinsLossesP1 = plr1.ladderWinsLosses;
							sd->m_ladderHighBreakP1 = plr1.ladderHighBreak;
							

				
							sd->m_p50sP1 += plr1._p50s, plr1._p50s = sd->m_p50sP1;
							sd->m_p60sP1 += plr1._p60s, plr1._p60s = sd->m_p60sP1;
							sd->m_p70sP1 += plr1._p70s, plr1._p70s = sd->m_p70sP1;
							sd->m_p80sP1 += plr1._p80s, plr1._p80s = sd->m_p80sP1;
							if(g_build.gameType == GAMETYPE_SNOOKER)
							{
								sd->m_p90sP1 += plr1._p90s, plr1._p90s = sd->m_p90sP1;
								sd->m_p100sP1+= plr1._p100s,plr1._p100s= sd->m_p100sP1;
							}
			
							
							// If the new break is greater than the profile break, update the profile break //
							// If not, then make sure the break in Memory is the same as the profile break so
							// when player's local stats are updated, it still has their highest break //
							if(sd->m_pHighBreakP1 > plr1.pHighBreak) plr1.pHighBreak = sd->m_pHighBreakP1;
								else sd->m_pHighBreakP1 = plr1.pHighBreak;

							sd->m_pMax147sP1 += plr1.pMax147s, plr1.pMax147s = sd->m_pMax147sP1;
		
							sd->m_50sP1	+= plr1._50s, plr1._50s = sd->m_50sP1;
							sd->m_60sP1	+= plr1._60s, plr1._60s = sd->m_60sP1;
							sd->m_70sP1	+= plr1._70s, plr1._70s = sd->m_70sP1;
							sd->m_80sP1	+= plr1._80s, plr1._80s = sd->m_80sP1;
							sd->m_90sP1	+= plr1._90s, plr1._90s = sd->m_90sP1;
							if(g_build.gameType == GAMETYPE_SNOOKER)
							{
								sd->m_100sP1+= plr1._100s,plr1._100s = sd->m_100sP1;
							}
	
							// If the new break is greater than the profile break, update the profile break //
							// If not, then make sure the break in Memory is the same as the profile break so
							// when player's local stats are updated, it still has their highest break //
							if(sd->m_HighBreakP1 > plr1.HighBreak) plr1.HighBreak = sd->m_HighBreakP1;
								else sd->m_HighBreakP1 = plr1.HighBreak;

							sd->m_Max147sP1	+= plr1.Max147s, plr1.Max147s = sd->m_Max147sP1;
							sd->m_bioAverageBreakP1 = plr1.bioAverageBreak;
							sd->m_pBioAverageBreakP1 = plr1.pBioAverageBreak;

							sd->m_levelP1 = plr1.level;
							sd->m_levelProgressP1 = plr1.level_progress;
							
							
							//P2
							sd->m_winsP2 = plr2.wins;
							sd->m_lossesP2 = plr2.losses;
							sd->m_streakP2 = plr2.streak;
							sd->m_highestratingP2 = plr2.highestrating;
							sd->m_cashP2 = plr2.cash;
							sd->m_reracksP2 = plr2.reracks;
							sd->m_ladderPointsP2 = plr2.ladderPoints;
							sd->m_ladderWinsLossesP2 = plr2.ladderWinsLosses;
							sd->m_ladderHighBreakP2 = plr2.ladderHighBreak;

							sd->m_p50sP2 += plr2._p50s, plr2._p50s = sd->m_p50sP2;
							sd->m_p60sP2 += plr2._p60s, plr2._p60s = sd->m_p60sP2;
							sd->m_p70sP2 += plr2._p70s, plr2._p70s = sd->m_p70sP2;
							sd->m_p80sP2 += plr2._p80s, plr2._p80s = sd->m_p80sP2;
							if(g_build.gameType == GAMETYPE_SNOOKER)
							{
								sd->m_p90sP2 += plr2._p90s, plr2._p90s = sd->m_p90sP2;
								sd->m_p100sP2 += plr2._p100s, plr2._p100s = sd->m_p100sP2;
							}
							
							// If the new break is greater than the profile break, update the profile break //
							// If not, then make sure the break in Memory is the same as the profile break so
							// when player's local stats are updated, it still has their highest break //
							if(sd->m_pHighBreakP2 > plr2.pHighBreak) plr2.pHighBreak = sd->m_pHighBreakP2;
								else sd->m_pHighBreakP2 = plr2.pHighBreak;

							sd->m_pMax147sP2 += plr2.pMax147s, plr2.pMax147s = sd->m_pMax147sP2;

							sd->m_50sP2	+= plr2._50s, plr2._50s = sd->m_50sP2;
							sd->m_60sP2	+= plr2._60s, plr2._60s = sd->m_60sP2;
							sd->m_70sP2	+= plr2._70s, plr2._70s = sd->m_70sP2;
							sd->m_80sP2	+= plr2._80s, plr2._80s = sd->m_80sP2;
							sd->m_90sP2	+= plr2._90s, plr2._90s = sd->m_90sP2;
							if(g_build.gameType == GAMETYPE_SNOOKER)
							{
								sd->m_100sP2+= plr2._100s, plr2._100s = sd->m_100sP2;
							}
							
							// If the new break is greater than the profile break, update the profile break //
							// If not, then make sure the break in Memory is the same as the profile break so
							// when player's local stats are updated, it still has their highest break //
							if(sd->m_HighBreakP2 > plr2.HighBreak) plr2.HighBreak = sd->m_HighBreakP2;
								else sd->m_HighBreakP2 = plr2.HighBreak;
							
							sd->m_Max147sP2	+= plr2.Max147s, plr2.Max147s = sd->m_Max147sP2;

							sd->m_bioAverageBreakP2 = plr2.bioAverageBreak;
							sd->m_pBioAverageBreakP2 = plr2.pBioAverageBreak;

							sd->m_levelP2 = plr2.level;
							sd->m_levelProgressP2 = plr2.level_progress;
							
							// Save Profiles //
							// At this point we should be fully updated //
							// Only Update if a Member vs Member Match or Trialist vs Trialist Games

							// Set our Ladder Ranks to old ladder rank by default
							sd->m_ladderRankP1 = -1;// plr1.ladderRank;
							sd->m_ladderRankP2 = -1;// plr2.ladderRank;
	
							// Note: must be a Member or Trialists if Loaded Profile..
							if (loadedP1)
							{
								((CGameAppView *)m_pParentWnd)->SavePlayerProfile(&plr1, ".txt");

								// Get our Ladder Ranks (if a Member Game)
								if (!plr1.bIsGuest)
								if (!(plr1.state&PLRSTATE_GUESTMEMBER))
								{
									// Get Ladder Rank of Player 1
									sd->m_ladderRankP1 = GetLadderRank(plr1.name, plr1.ladderPoints);
								}
							}
							if (loadedP2)
							{

								((CGameAppView *)m_pParentWnd)->SavePlayerProfile(&plr2, ".txt");
								
								// Get our Ladder Ranks (if a Member Game)
								if (!plr1.bIsGuest)
								if (!(plr2.state&PLRSTATE_GUESTMEMBER))
								{
									// Get Ladder Rank of Player  2
									sd->m_ladderRankP2 = GetLadderRank(plr2.name, plr2.ladderPoints);
								}
							}

							

						}
					}
				}
										
					if(!bFailedToLoadFiles) // dont forward msg, if we couldn't open the players' files
						Server->SendToQueue(lpBuf, nBufLen); // Forward drop message back to clients.

			}
			break;

		case MSGGAME_BALLS:
				{
					BallsSendData *sd;
					sd = (BallsSendData_typ *) lpBuf;
					char hacking = 0;
					char buf[256];
					int SockIdx1P, SockIdx1Pb;
					int SockIdx2P, SockIdx2Pb;
					int AOnline, BOnline;

	
					// Back Up the Table Number of the Sender for future use //
					// Handy for making sure the MSGGAME_BALL message isn't received by the wrong table //
					sd->buffer[2] = plrTblIdx;


					int i = sd->buffer[3];
					if(i == -1)
					{
						//////////////////////////////////////////////////////////
						// *NEW - check if this is the first simEnd msg for this shot
						if(sd->m_bSimEnd)
						{
							if(plrTblIdx >= 0 && plrTblIdx < MAX_LOBBY_TABLES)
							{

								// *NEW - server to remember tbl scores
								m_table[plrTblIdx].score1P = sd->m_score1P;
								m_table[plrTblIdx].score2P = sd->m_score2P;
								m_table[plrTblIdx].break1P = sd->m_break1P;
								m_table[plrTblIdx].break2P = sd->m_break2P;
								///////////////////////////////////////////

								if(sd->m_nSimEndMsgBalls == m_table[plrTblIdx].nSimEndMsgBalls)
								{
									// this is the first SendBalls SimEnd msg for this current shot, so dont take anymore!
									m_table[plrTblIdx].nSimEndMsgBalls++;
								}
								else
								{
									// If any of the above data does not match, then it's a hacking attempt //
									if(m_table[plrTblIdx].score1P != sd->m_score1P) hacking = 1;
									if(m_table[plrTblIdx].score2P != sd->m_score2P) hacking = 1;
									if(m_table[plrTblIdx].break1P != sd->m_break1P) hacking = 1;
									if(m_table[plrTblIdx].break2P != sd->m_break2P) hacking = 1;
									
									// If a hacking attempt, VOID the match and tell clients why //
									// Also log this in loghackers //
									if(hacking == 1) 
										{
										ServerEndMatch(plrTblIdx, 1, sd->m_score1P, sd->m_score2P, sd->m_break1P, sd->m_break2P);
										}
										
									// too slow! Already received this msg from 1P or 2P at table
									break;
								}

								
							}
						}

						

						// If Doubles and Turn Changes, then make next Team Swap Seats //
   						if(m_table[plrTblIdx].doubles)
						if(sd->m_bTurnChanged)
						if(sd->m_bSimEnd)
							{
							if(!sd->m_bTurn1P)
								{
								// Swap Team 1 - If both players are here //
								SockIdx1P = m_table[plrTblIdx].player1SockIdx;
								SockIdx1Pb = m_table[plrTblIdx].player1bSockIdx;
								if(SockIdx1P!=-1 && SockIdx1Pb!=-1)
									{
									((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(SockIdx1P, SockIdx1P, plrTblIdx, SockIdx1P, SockIdx1P);
									((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(SockIdx1Pb, SockIdx1Pb, plrTblIdx, SockIdx1Pb, SockIdx1Pb);
									((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(-1, -1, plrTblIdx, SockIdx1P, -1);
									((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(SockIdx1Pb, -1, plrTblIdx, -1, -1);
									}
								}
							else
								{
								// Swap Team 2 - If both players are here //
								SockIdx2P = m_table[plrTblIdx].player2SockIdx;
								SockIdx2Pb = m_table[plrTblIdx].player2bSockIdx;
								if(SockIdx2P!=-1 && SockIdx2Pb!=-1)
									{
									((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(SockIdx2P, SockIdx2P, plrTblIdx, SockIdx2P, SockIdx2P);
									((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(SockIdx2Pb, SockIdx2Pb, plrTblIdx, SockIdx2Pb, SockIdx2Pb);
									((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(-1, -1, plrTblIdx, -1, SockIdx2P);
									((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(-1, SockIdx2Pb, plrTblIdx, -1, -1);
									}
								}
							}


						//////////////////////////////////////////////////////////
						SendToQueueTable(Server, lpBuf, nBufLen, plrTblIdx);
						//ErrorMessage(((CGameAppView *)m_pParentWnd), "MSGGAME_BALLS (all)");

						// Check If there are any players in the Lobby watching this Table //
						// If so, then we need to send them the MSGGAME_BALLS message also //
						int n;
						int tblIdx;
						for(n=0;n<MAX_LOBBY_PLAYERS;n++)
						if(m_player[n].status)
							{
							tblIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetTableIdxFromPlayer(m_player[n].playerSocketIdx);
							if(tblIdx == -1)		// Only Do Following Code if we are in the Lobby
							if(m_player[n].lobbytablewatch == plrTblIdx) 
								{
								Server->SendToQueue(lpBuf, nBufLen, m_player[n].playerSocketIdx);
								}
							}
					}
					else
					{
						Server->SendToQueue(lpBuf, nBufLen, i);
					}
				}
			break;


			case MSGGAME_TIMEUP:
				{					
					TimeUpSendData *sd;
					sd = (TimeUpSendData_typ *) lpBuf;

					//((CGameAppView *)m_pParentWnd)->ReceivePlayerTimeUp(sd->m_shotTime, sd->m_bTimeUp);					

					int i = sd->buffer[3];
					if(i == -1)
					{
						SendToQueueTable(Server, lpBuf, nBufLen, plrTblIdx);
						//ErrorMessage(((CGameAppView *)m_pParentWnd), "MSGGAME_BALLS (all)");
					}
					else
					{
						Server->SendToQueue(lpBuf, nBufLen, i);
						//ErrorMessage(((CGameAppView *)m_pParentWnd), "MSGGAME_BALLS (i)");
					}
				}
			break;

			case MSGGAME_SIM_END: break; //not used as of 16 march 2004
				{
				// for host only.
				SimEndSendData *sd;
				sd = (SimEndSendData_typ *) lpBuf;
				
				if(sd->buffer[2]!=-1) sd->buffer[3] = 0; // FALSE
				SendToQueueTable(Server, lpBuf, nBufLen, plrTblIdx);
				
				}
			break;

			case MSGGAME_SIM_START:
				{
				// for host only.
				SimStartSendData *sd;
				sd = (SimStartSendData_typ *) lpBuf;
				
				//((CGameAppView *)m_pParentWnd)->m_lobby->HostSimStartReceive(sd->m_plrSockIdx);
								
//				Server->SendToQueue(lpBuf, nBufLen); // Forward simend message back to clients.
				SendToQueueTable(Server, lpBuf, nBufLen, plrTblIdx);
				}
			break;

			case MSGGAME_INVITEPRESSED:
				{
				// for host only.
				InvitePressedSendData *sd;
				sd = (InvitePressedSendData_typ *) lpBuf;

				// two modes:
				if(sd->m_bAccepted) // (mode 1) player has accepted an invitation
				{
					// if tourney lobby and this is seat1P/2P invite, check this player is allowed to be invited here
					#ifdef MAIN_SERVER
					if(g_bTournamentLobby)
					if(sd->m_enterType == 1 || sd->m_enterType == 2)
					{
						int invitedPlrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(sd->m_invitedPlrSockIdx);
						if(invitedPlrIdx!=-1)
						{
							int matchNo = g_tournament.GetMatchFromTableNo(sd->m_table);
							if(matchNo != -1)
							{
								if( (strcmpi(m_player[invitedPlrIdx].name, g_tournament.m_match[matchNo].m_name1P) == 0) ||
									(strcmpi(m_player[invitedPlrIdx].name, g_tournament.m_match[matchNo].m_name2P) == 0) )
								{
									if(g_tournament.m_match[matchNo].m_bMatchOver) break; // cancel sending this msg on!...										
									
									// if got this far, all is ok :) so carry on with msg...
								}
								else break; // cancel sending this msg on!...								
							}
							else break; // cancel sending this msg on!...							
						}
						else break;  // cancel sending this msg on!...
					}
					#endif//MAIN_SERVER

					// update server					
					((CGameAppView *)m_pParentWnd)->m_lobby->InvitationAcceptedReceive(sd->m_plrSockIdx, sd->m_invitedPlrSockIdx, sd->m_str, sd->m_initialHostTbl, sd->m_table, sd->m_enterType);

					Server->SendToQueue(lpBuf, nBufLen); // Forward message onto to clients.
				}
				else // (mode 2) host of table has invited a player
				{
					int invitedPlrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(sd->m_invitedPlrSockIdx);
					if(invitedPlrIdx!=-1)
					{
						if( strcmpi(m_player[invitedPlrIdx].name, sd->m_invitedName) == 0 ) // is our invited player still online?
						{
							// *NEW - server will send this invited client 'Games' & 'Reracks' host stats
							int hostPlrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(sd->m_plrSockIdx);							
							if(hostPlrIdx!=-1)
							{
								sd->m_reracks = m_player[hostPlrIdx].reracks;
								sd->m_totalGames = m_player[hostPlrIdx].wins + m_player[hostPlrIdx].losses;
							}
							else
							{
								sd->m_reracks = 0;
								sd->m_totalGames = 0;
							}

							Server->SendToQueue(lpBuf, nBufLen, sd->m_invitedPlrSockIdx); // Forward message to invited client only.
						}
					}
				}

				}
			break;

			case MSGGAME_BOOTPRESSED:
				{
					// for host only.					
					BootPressedSendData *sd;
					sd = (BootPressedSendData_typ *) lpBuf;

					int bPlrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(sd->m_bootedPlrSockIdx);
					if(bPlrIdx != -1)
					{
						int mod_type;
						int bmod_type;
						mod_type = m_player[plrIdx].playertype;	// Get the Mod Type
						bmod_type = m_player[bPlrIdx].playertype;	// Get the Mod Type
						
						if(bmod_type < mod_type)
						{
							((CGameAppView *)m_pParentWnd)->SendServerMessage("can't boot! - Player out-ranks you!", plrSockIdx);
						}
						else
						{
							SendToQueueTable(Server, lpBuf, nBufLen, plrTblIdx);
						}
					}
				}
			break;

			case MSGGAME_SERVERMESSAGE: // handle the '/' messages!
				{
				char *str;
				int len;
				str = (char *) lpBuf;
				str = &str[3];  // "XX/msg myfriend hey whats up" -> "msg myfreind hey whats up"		
				((CGameAppView *)m_pParentWnd)->HandleServerMessage(str, plrSockIdx);
				}
			break;

			case MSGGAME_HOSTCHECKLISTENING: // Client has told us that it has made a listening socket, we should try connecting!!!
				{
					int *port;
					port = (int *) &buffer[4];
					//*v = port
					//char buffer[MAX_CHARS];
					char ipaddr[MAX_CHARS];
					CString rPeerAddress;
					UINT rPeerPort;
					BOOL success;

					if(*port==0)
						{
						ErrorMessage(m_pParentWnd, "Client Failed to create listening socket");

						((CGameAppView *)m_pParentWnd)->SetCheckSockIdx(plrSockIdx);
						((CGameAppView *)m_pParentWnd)->SetCheckID(0);

						((CGameAppView *)m_pParentWnd)->m_hostcheckactive = -1; // reset hostchecker.
						
						}
					else 
					{
					// dangerous, if they disconnect halfway thru one of these host id checks.
					((CGameAppView *)m_pParentWnd)->SetCheckSockIdx(plrSockIdx);
					
					Server->m_socketList[plrSockIdx]->GetPeerName(rPeerAddress, rPeerPort);
					strcpy(ipaddr, rPeerAddress.GetBuffer(0));
					
					CString strHandle = _T("Joiner"); // unreq'd
					success = ConnectSocketCASYNC(strHandle, ipaddr, *port, ASYNCSOCK_TYPE_CHECK);
					if(success==FALSE)
						{
						((CGameAppView *)m_pParentWnd)->m_hostcheckactive = -1; // reset hostchecker.
						ErrorMessage(m_pParentWnd, "Failed to create host checking socket");				
						}
					}
				}
			break;
			case MSGGAME_KICK:
				{
				((CGameAppView *)m_pParentWnd)->ReceiveKickRequest(plrSockIdx);
				return TRUE; // Socket is closed at this point, so no more socket related code can take effect!
				}
			break;
			/*case MSGGAME_EPD:
				{
					// Server's job (whether game or main) is to reroute these messages to their destination.
					((CGameAppView *)m_pParentWnd)->m_lobby->ReRouteEPD(buffer, plrSockIdx);
				}
			break; */

			case MSGGAME_GAMEPING:
				{
				// for host only.
				GamePingSendData *sd;
				sd = (GamePingSendData_typ *) lpBuf;
				
				//Server->SendToQueue(lpBuf, nBufLen, sd->m_plrSockIdx); // Forward message to Pinger client only. [BUG! - cant do this, as "sd->m_bootedPlrSockIdx" is for mainShell only not gameShell (diff. order)]
				Server->SendToQueue(lpBuf, nBufLen); // send to all in this gameshell.
				}
			break;

			case MSGGAME_HIGHBREAK:
			{
				// for host only
				HighBreakSendData *sd;
				sd = (HighBreakSendData_typ *) lpBuf;
				
				BOOL bNewTopBreak = FALSE;

				// update high break table
				bNewTopBreak = ((CGameAppView *)m_pParentWnd)->m_lobby->HighBreakReceive(sd->m_plrSockIdx, sd->m_highBreak, sd->m_cueError, sd->m_high4);

				// send updated high-breaks to all
				if(bNewTopBreak)
					((CGameAppView *)m_pParentWnd)->SendAllHighBreaks(-1, 1, sd->m_cueError);
				else
					((CGameAppView *)m_pParentWnd)->SendAllHighBreaks(-1, 0);

			}
			break;

			case MSGGAME_UPDATEHISTORY:
			{
				
				HistoryReportData *sd;
				sd = (HistoryReportData *) lpBuf;
				db_updategamehistory(sd->myself, sd->opponent, sd->gameinfo, sd->time, sd->update_hid);
				
			}
			break;

			case MSGGAME_TABLEHOSTCOMPLETE:
			{
				// for host only
				TableHostCompleteSendData *sd;
				sd = (TableHostCompleteSendData_typ *) lpBuf;
								
				// update high break table
				((CGameAppView *)m_pParentWnd)->m_lobby->TableHostCompleteReceive(sd->m_tblIdx);
			}
			break;

			case MSGGAME_UPDATELOBBYTVSERVER:
			{
				// for host only
				LobbyWatchSendData *sd;
				sd = (LobbyWatchSendData_typ *) lpBuf;
								
				// update Server for Lobby TV data //
				// Get plrIdx //
				int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(sd->plrSockIdx);
				m_player[plrIdx].lobbytablewatch = sd->tblIdx;
			}
			break;

			case MSGGAME_GENERICREQUEST: // Handle Generic Request (GameAppView filter)
			{
				((CGameAppView *)m_pParentWnd)->ReceiveRequest((BYTE *) lpBuf,plrSockIdx);
			}
			break;
				
			case MSGGAME_REQUESTFILE:
			{
				// for host only
				RequestFile *sd;
				sd = (RequestFile_typ *) lpBuf;
				
				// Client is asking for a File to be sent back //
				g_gameAppView->ServerSendFile(sd->fname, plrSockIdx);
			}
			break;

			case MSGGAME_CHEATALERT:
			{
				// Server is informd of a cheater..
				CheatAlert *sd;
				sd = (CheatAlert_typ *) lpBuf;
				
				char smacid[16];
				char ssysid[16];
				char sip[16];
				char playerName[128];
				char buf[128];

				// Get player name, if exists still
				if(plrIdx != -1)
				{
					// Update Server's player array
					sprintf(playerName, "%s", m_player[plrIdx].name);
					sprintf(ssysid, "%lx%04x", m_player[plrIdx].sysID, m_player[plrIdx].cpuID);
					sprintf(smacid, "%lx%lx", m_player[plrIdx].macID_U, m_player[plrIdx].macID_L);
					sprintf(sip, "%s", m_player[plrIdx].playerLocalIP);
				}

				// Log this...
				sprintf(buf, "%s [%s] - %s is using [%s] - macid:%s, sysid:%s, ip:%s", g_gameAppView->m_stringShortTime, g_gameAppView->m_stringShortDate, playerName, sd->process, smacid, ssysid, sip);
				AddLog("cheaters.txt", buf);
			}
			break;


			// Server
			case MSGGAME_TEXTCOLORCHANGED:
			{
				
				TextColorChanged *sd;
				sd = (TextColorChanged_typ *) lpBuf;
				

				// Update this player's Text Color
				if(plrIdx != -1)
				{
					// Update Server's player array
					m_player[plrIdx].textColor = sd->textcolor;

					// Update Data Base
					db_textchanged(m_player[plrIdx].name, m_player[plrIdx].textFormat, m_player[plrIdx].textColor);
				}
			}
			break;

			// Server
			case MSGGAME_UPDATEVCASH:
			{
				// We want to update everyone in the lobby of this player's virtual cash
				UpdateVCashSendData *sd;
				sd = (UpdateVCashSendData_typ *) lpBuf;
				
				// Check if this player is online
				int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(sd->playerSockIdx);
				if(plrIdx != -1)
				{
					// Load this Player's Stats and return the vCash value
					Player plr;
					((CGameAppView *)m_pParentWnd)->m_lobby->ClearPlayer(&plr);
					
					strcpy(plr.name, m_player[plrIdx].name);
					if( ((CGameAppView *)m_pParentWnd)->LoadPlayerProfile(&plr, ".txt") )
					{
						sd->cash = plr.cash;

						Server->SendToQueue(lpBuf, nBufLen); // Forward drop message back to clients.
					}
				}

				
			}
			break;

			// Server
			case MSGGAME_FINDMATCH:
			{
				// When we receive this Message, we will update this player's Find Match status, so they will be considered
				// When Server Looks for a Match...

				// only if not Tournament Lobby
				if (g_bTournamentLobby == false)
				{
					FindMatchSendData *sd;
					sd = (FindMatchSendData_typ *)lpBuf;

					// Check if this player is online
					int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(plrSockIdx);
					if (plrIdx != -1)
					{
						// Check if we're already in a Ladder Match.. If so, set to Starting Match status
						if (g_tournament.InMatch(m_player[plrIdx].name))
						{
							// Tell Client that we're no longer searching for match
							g_gameAppView->SendClientFindMatchStatus(plrSockIdx, 2);
						}
						else
						{

							// Clear Search Game Status by Default
							m_player[plrIdx].searchgamestatus = 0;

							// Set search game status to 1, so server know we're looking for a game
							m_player[plrIdx].searchgamestatus |= 0x01;

							// Set the League Match State
							if (sd->optionRaceToTwo == 1) m_player[plrIdx].searchgamestatus |= (0x01 << 4);

							// Set the Pro Match State
							if (sd->optionPro == 1) m_player[plrIdx].searchgamestatus |= (0x01) << 5;

							// Set the Exact Match State
							if (sd->optionExactMatch == 1) m_player[plrIdx].searchgamestatus |= (0x01) << 6;

							// Set the Time that we started looking for a game 
							// SYSTEMTIME st;
							// GetLocalTime(&st);

							// Get our current time..
							long currentTime;
							CTime m_time;
							CTimeSpan diff;
							m_time = m_time.GetCurrentTime();
							CTime t1970(1970, 1, 1, 0, 0, 0);
							diff = m_time - t1970;
							currentTime = diff.GetTotalSeconds();

							// When we work out the total time, just use (Month*31 + Day) * 24

							// m_player[plrIdx].searchgametime = st.wMilliseconds;
							m_player[plrIdx].searchgametime = currentTime;

							int status = 1;	// Searching For Match
							status |= (sd->optionRaceToTwo << 4);
							status |= (sd->optionPro << 5);
							status |= (sd->optionExactMatch << 6);

							// Tell Client that we're in the Queue for getting a Match
							// g_gameAppView->SendClientFindMatchStatus(plrSockIdx, status);
						}
					}
				}
			}
			break;


			// Server
			case MSGGAME_FINDMATCH_CANCEL:
			{
				// Stop Searching for Game for this Player
				FindMatchCancelSendData *sd;
				sd = (FindMatchCancelSendData_typ *)lpBuf;

				// Check if this player is online
				int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(plrSockIdx);
				if (plrIdx != -1)
				{
					// Set search game status to 0
					m_player[plrIdx].searchgamestatus = 0;
				}
			}
			break;


			// Server
			case MSGGAME_LADDERRANKINGS:
			{
				// Get ladder rankings and send back to client who requested this.
				GetLadderRankingsData *sd;
				sd = (GetLadderRankingsData_typ *)lpBuf;

				// Check if this player is online
				int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(plrSockIdx);
				if (plrIdx != -1)
				{
					// Send client the ladder rankings...
					g_gameAppView->SendClientLadderRankings(GetLadderRankings(), plrSockIdx);
				}
			}
			break;


		} // end switch
	} 
	
	else if(pComm==Client) // All Clients receive these messages.
	{
		//if(type > 56) type = 99999;	
		switch(type)
		{

#ifndef MAIN_SERVER
			case MSGGAME_LADDERRANKINGS:
			{
				LadderRankingsData *sd;
				sd = (LadderRankingsData_typ *)lpBuf;

				// Check if this player is online
				int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(sd->playerSockIdx);
				if (plrIdx != -1)
				{
					// Bring up Ladder Rankings
					g_gameAppView->CreateLadderRankings(m_player[plrIdx].name, sd->rankData);
				}
				

			}
			break;
#endif

			// lobby operations
			case MSGGAME_SERVERDATA:
			{
				ServerSendData *sd;
				sd = (ServerSendData_typ *) lpBuf;

				
				
				((CGameAppView *)m_pParentWnd)->m_lobby->ServerDataReceive(sd->m_bTournament, sd->m_bOriginalVersion, sd->m_handle);
			}
			break;

			case MSGGAME_SERVERCONFIG:
			{
				ServerSendConfig *sd;
				sd = (ServerSendConfig_typ *) lpBuf;
				
				
				((CGameAppView *)m_pParentWnd)->ServerConfigReceive(sd);
				
			}
			break;
		
			case MSGGAME_LOBBYREADY:
			{
		
				char *v1 = (char *) &buffer[4];
				char *v2 = (char *) &buffer[4+60];
				char *v3 = (char *) &buffer[4+60*2];
				char *v4 = (char *) &buffer[4+60*3];
				char *v5 = (char *) &buffer[4+60*4];
				char *v6 = (char *) &buffer[4+60*4+90];


				g_lobbyMessageBoard = CString(v1);
				g_lobbyTournament = CString(v2);
				g_lobbyLeague = CString(v3);
				g_lobbyRegister = CString(v4);
				g_URLlobbyLeaderBoardA = CString(v5);
				g_URLlobbyLeaderBoardB = CString(v6);


				::ShowWindow(g_hwndDX, SW_HIDE);
				((CGameAppView *)m_pParentWnd)->m_lobby->LobbyReady();


				// Request advert.txt file from Server //
				g_gameAppView->ClientRequestFile("adverts.txt");

				// Request advert.txt file from Server //
				g_gameAppView->ClientRequestFile("news.txt");				

		
			}
			break;

			case MSGGAME_RECEIVEFILE:
			{
				ReceiveFile *sd;
				sd = (ReceiveFile_typ *) lpBuf;
				// Save File to Local Drive //
				FILE *fptr;

				char fname_full[128];
				sprintf(fname_full, "%s%s", g_build.savepath, sd->fname);

				// Make sure our adverts folder is present
				char gPath[128];
				sprintf(gPath, "%s%s", g_build.savepath, "adverts\\");
				_mkdir(gPath);

				fptr = fopen(fname_full, "wb");
				if(fptr)
				{
					for(int i=0;i<sd->fsize;i++)
					{
						fprintf(fptr, "%c", sd->data[i]);
					}
					fclose(fptr);

				}


				// If this was our 'news.txt' file, then we must update the News Loaded //
				if(strcmpi(sd->fname, "news.txt") == 0)
				{
					
					g_gameAppView->ShowNews();


					g_gameAppView->m_NewsDlg.LoadNews("news.txt");
				}

				// If this was our 'adverts.txt' file, then we need to check through this to see if there's new
				// images that we need to request from the server //

				if(strcmpi(sd->fname, "adverts.txt") == 0)
				{
					char fname_full[128];
					sprintf(fname_full, "%s%s", g_build.savepath, "adverts.txt");

					fptr = fopen(fname_full, "rb");
					if(fptr)
					{	
						char str[256];
						int total = 0;
						char fname_checked[32][64];
						char fname[64];

						for(int n=0;n<32;n++) fname_checked[n][0] = 0;

						while(fscanf(fptr, "%s", &str) != EOF)
						{
							// Find a new Advert //
							if(strcmpi(str, "[advert]") == 0)
							{
								// Check if this image file exists in "images/" folder //
								fscanf(fptr, "%s", &str);
								FILE *ifptr;
								
								sprintf(fname, "%s", str);
								for(int n=0;n<32;n++)
								if(fname_checked[n][0] == 0)
								{
									strcpy(fname_checked[n], fname);
									break;

								}
								else if(strcmpi(fname, fname_checked[n]) == 0) goto end;	// found a match so skip this //

								// Try our savepath first
								char fname_full[128];
								sprintf(fname_full, "%s%s", g_build.savepath, fname);
								ifptr = fopen(fname_full, "rb");

								
								if(ifptr == NULL)
								{
									// Try searching in our installed folder's adverts directory if nothing in the save path
									ifptr = fopen(fname, "rb");	
								}

								if(!ifptr)
								{
									// image isn't found, so we must request it from the server //
									g_gameAppView->ClientRequestFile(fname);

								}
								else fclose(ifptr);

								end:;

							}
							/*
							// Reload Advertisements Flag //
							else if(strcmpi(str, "[reload]") == 0)
							{
								// Reload Advertisements //
								g_gameAppView->DeleteAdvertisements();
								g_gameAppView->LoadAdvertisements();
							}
							*/
							else;

						}

						fclose(fptr);
					}

				}

				
			}

			break;

			// Client
			case MSGGAME_TEXTCOLORCHANGED:
			{
				/*
				TextColorChanged *sd;
				sd = (TextColorChanged_typ *) lpBuf;
				*/

				// Update this player's Text Color
				
			}
			break;

			// Client
			case MSGGAME_UPDATEVCASH:
			{
				// Update us for this player's vCash

				// We want to update everyone in the lobby of this player's virtual cash
				UpdateVCashSendData *sd;
				sd = (UpdateVCashSendData_typ *) lpBuf;
				
				// Check if this player is online
				int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(sd->playerSockIdx);
				if(plrIdx != -1)
				{
					m_player[plrIdx].cash = sd->cash;
				}
				
			}
			break;


			// Client
			case MSGGAME_FINDMATCH:
			{
				// When we receive this Message, we will update this player's Find Match status, so they will be considered
				// When Server Looks for a Match...

				FindMatchSendData *sd;
				sd = (FindMatchSendData_typ *)lpBuf;
				
				// Get our Player Index
				int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(sd->playerSockIdx);
				if (plrIdx != -1)
				{
					// Update Find Match Ctrl
					g_gameAppView->m_findMatchCtrl->UpdateStatus(sd->status);
				}
			}
			break;



			case MSGGAME_LOGINFAILED:
			{
				char failType = buffer[3];
				((CGameAppView *)m_pParentWnd)->LoginFailed(failType);
			}
			break;

			case MSGGAME_LOGINNEWACCOUNT:
			{
				int *v = (int *) &buffer[4];
				
				((CGameAppView *)m_pParentWnd)->LoginNewAccount(*v,buffer[2]);
			}
			break;


			case MSGGAME_ACCOUNTCREATED:
			{
				char failType = buffer[3];
				((CGameAppView *)m_pParentWnd)->AccountCreated(failType);
			}
			break;

			case MSGGAME_PLAYERDETAILS: // Should only be called when someone double clicks on a name in profile.
			{
				//ErrorMessage(m_pParentWnd, "MSGGAME_PLAYERDETAILS: PRE");
				// update clients
				#ifndef MAIN_SERVER



				playerDetailsSendData *sd;
				sd = (playerDetailsSendData_typ *) lpBuf;
				Player plr;
				char buf[256];
				
				

				
				memcpy(&plr, &sd->m_player, sizeof(Player_typ));
				
				// Safety check, to make sure our cue ranges from 0 - 7
				if(plr.cuetype < 0) 
				{
					plr.cuetype = 0;
				}
				if(plr.cuetype > 7) 
				{
					plr.cuetype = 0;
				}

				
				// Check If this player was on ignore //
				if(((CGameAppView *)m_pParentWnd)->m_ignorelst->CheckName(plr.name)) plr.bIgnore = TRUE;
					else plr.bIgnore = FALSE;

				
				((CGameAppView *)m_pParentWnd)->m_lobby->AddPlayer(&plr);


				// Ladder Stats Update
				((CGameAppView *)m_pParentWnd)->m_findMatchCtrl->UpdateLadderStats();
				
				// is this my player details?
				if(plr.playerSocketIdx==((CGameAppView *)m_pParentWnd)->m_playerSockIdx)
					{
					((CGameAppView *)m_pParentWnd)->m_playerId = plr.id;
					((CGameAppView *)m_pParentWnd)->m_playerIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->FindPlayerIdxFromId(plr.id);
					if(!sd->buffer[2]) // Special player just logged in case!
						{
							// Update our TextColorChanger
							g_gameAppView->m_TextColorChangerDlg.UpdateColor(plr.textColor);
							
							

							// Update Locally Stored Player History //
							// This helps when Updating History for a new game //
							// We can work out which history_id to update without having to do a querry first //
							for(int n=0;n<10;n++) 
								{
								g_history_data.time[n] = sd->m_history.time[n];
								g_history_data.gameinfo[n] = sd->m_history.gameinfo[n];
								g_history_data.history_id[n] = sd->m_history.history_id[n];
								strcpy(g_history_data.loser[n], sd->m_history.loser[n]);
								strcpy(g_history_data.winner[n], sd->m_history.winner[n]);
								}
						
							int myPlrSockIdx = sd->buffer[3];
							int myPlrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(myPlrSockIdx);	
							if(myPlrIdx != -1)
							{
								g_sbioAverageBreak = m_player[myPlrIdx].bioAverageBreak;
								g_sbioAverageShotTime = m_player[myPlrIdx].bioAverageShotTime;
								g_sbioAveragePosition = m_player[myPlrIdx].bioAveragePosition;
								g_sbioAverageSafety = m_player[myPlrIdx].bioAverageSafety;
								g_pbioAverageBreak = m_player[myPlrIdx].pBioAverageBreak;
								g_pbioAverageShotTime = m_player[myPlrIdx].pBioAverageShotTime;
								g_pbioAveragePosition = m_player[myPlrIdx].pBioAveragePosition;
								g_pbioAverageSafety = m_player[myPlrIdx].pBioAverageSafety;

								// overrrides - if this is the first time player has had bio. stats
								#ifdef THE_POOL_CLUB
								if(g_sbioAverageBreak < 1) g_sbioAverageBreak = 3;
								#else
								if(g_sbioAverageBreak < 1) g_sbioAverageBreak = 25;
								#endif					
								if(g_sbioAverageShotTime < 1) g_sbioAverageShotTime = 15;
								if(g_sbioAveragePosition < 1) g_sbioAveragePosition = 50;
								if(g_sbioAverageSafety < 1) g_sbioAverageSafety = 50;
								#ifdef THE_POOL_CLUB
								if(g_pbioAverageBreak < 1) g_pbioAverageBreak = 3;								
								#else
								if(g_pbioAverageBreak < 1) g_pbioAverageBreak = 25;
								#endif													
								if(g_pbioAverageShotTime < 1) g_pbioAverageShotTime = 15;
								if(g_pbioAveragePosition < 1) g_pbioAveragePosition = 50;
								if(g_pbioAverageSafety < 1) g_pbioAverageSafety = 50;
							}
							else
							{
								#ifdef THE_POOL_CLUB
								g_sbioAverageBreak = 3;
								#else
								g_sbioAverageBreak = 25;
								#endif
								g_sbioAverageShotTime = 15;
								g_sbioAveragePosition = 50;
								g_sbioAverageSafety = 50;
								#ifdef THE_POOL_CLUB
								g_pbioAverageBreak = 3;
								#else
								g_pbioAverageBreak = 25;
								#endif
								g_pbioAverageShotTime = 15;
								g_pbioAveragePosition = 50;
								g_pbioAverageSafety = 50;
							}
						}
					}
				
				if(sd->buffer[2]) // Special Bring Dialog up message.
				{
					int RplrSockIdx = sd->buffer[3];
					int RplrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(RplrSockIdx);
			
					if(RplrIdx != -1)
					{
						CGameAppView *m_pParent;
						m_pParent = (CGameAppView *)m_pParentWnd;

						char g_selected_text = (m_player[RplrIdx].textFormat>>4)&0xf;
						char g_selected_cue = 0;
						if(m_player[RplrIdx].cuetype==2) g_selected_cue = 1;
						else if(m_player[RplrIdx].cuetype==4) g_selected_cue = 2;
						else if(m_player[RplrIdx].cuetype==8) g_selected_cue = 3;
						else if(m_player[RplrIdx].cuetype==16) g_selected_cue = 4;
						else if(m_player[RplrIdx].cuetype==32) g_selected_cue = 5;
						else if(m_player[RplrIdx].cuetype==64) g_selected_cue = 6;
						else if(m_player[RplrIdx].cuetype==128) g_selected_cue = 7;



						if(g_build.gameType == GAMETYPE_SNOOKER)
						{
							// general
							((CGameAppView *)m_pParent)->m_statsDlg.m_name =  m_player[RplrIdx].name;
							((CGameAppView *)m_pParent)->m_statsDlg.m_rating = m_player[RplrIdx].rating;
							((CGameAppView *)m_pParent)->m_statsDlg.m_highestrating = m_player[RplrIdx].highestrating;


							// Get Ladder Wins, Losses & High Breaks
							int lLosses = (m_player[RplrIdx].ladderWinsLosses & 0xFFFF);
							int lWins = (m_player[RplrIdx].ladderWinsLosses & 0xFFFF0000) >> 16;
							int lHighBreak = (m_player[RplrIdx].ladderHighBreak & 0xFFFF);
							int lHighBreakPro = (m_player[RplrIdx].ladderHighBreak & 0xFFFF0000) >> 16;
							int gamesRemaining = (5 - (lWins + lLosses));
							if (gamesRemaining < 0) gamesRemaining = 0;

							// Ladder Rank & Points
							// Division (i.e. 0-10999: Bronze, 11000-13999: Silver, 14000-16999: Gold, 17000-20999: Platinum, 21000+ Diamond)
							int ladderPoints = m_player[RplrIdx].ladderPoints;
							char divisionStr[80];
							char pointsStr[80];
							char rankStr[80];
							
							
							// If we have played less than 5 games (change divisionStr too 'N Games needed')
							if (gamesRemaining > 0)
							{
								sprintf(divisionStr, "points & rank shown after %d ladder games", gamesRemaining);
								sprintf(pointsStr, "");
								sprintf(rankStr, "");
							}
							else
							{
								if (ladderPoints < 11000) sprintf(divisionStr, "Bronze Division");
								else if (ladderPoints < 14000) sprintf(divisionStr, "Silver Division");
								else if (ladderPoints < 17000) sprintf(divisionStr, "Gold Division");
								else if (ladderPoints < 21000) sprintf(divisionStr, "Platinum Division");
								else sprintf(divisionStr, "Diamond Division");

								sprintf(pointsStr, "%d", m_player[RplrIdx].ladderPoints);
								sprintf(rankStr, "%d", m_player[RplrIdx].ladderRank);
							}

							
							((CGameAppView *)m_pParent)->m_statsDlg.m_ladderRank = rankStr;
							((CGameAppView *)m_pParent)->m_statsDlg.m_ladderDivision = divisionStr;
							((CGameAppView *)m_pParent)->m_statsDlg.m_ladderPoints = pointsStr;

												
							
							((CGameAppView *)m_pParent)->m_statsDlg.m_ladderGames = lWins + lLosses;
							((CGameAppView *)m_pParent)->m_statsDlg.m_ladderWins = lWins;
							((CGameAppView *)m_pParent)->m_statsDlg.m_ladderLosses = lLosses;
							((CGameAppView *)m_pParent)->m_statsDlg.m_ladderHighBreak = lHighBreak;
							((CGameAppView *)m_pParent)->m_statsDlg.m_ladderHighBreakPro = lHighBreakPro;

																		
							if(m_player[RplrIdx].cash>=0) sprintf(buf, "$%ld.%02ld",m_player[RplrIdx].cash/100, (m_player[RplrIdx].cash)%100);
							else sprintf(buf, "$-%ld.%02ld",-(m_player[RplrIdx].cash)/100, (-m_player[RplrIdx].cash)%100);
							((CGameAppView *)m_pParent)->m_statsDlg.m_cash = _T(buf);
							
							((CGameAppView *)m_pParent)->m_statsDlg.m_wins = m_player[RplrIdx].wins;
							((CGameAppView *)m_pParent)->m_statsDlg.m_losses = m_player[RplrIdx].losses;
							((CGameAppView *)m_pParent)->m_statsDlg.m_games = m_player[RplrIdx].wins + m_player[RplrIdx].losses;
							((CGameAppView *)m_pParent)->m_statsDlg.m_streak = m_player[RplrIdx].streak;
							((CGameAppView *)m_pParent)->m_statsDlg.m_reracks = m_player[RplrIdx].reracks;

							if (m_player[RplrIdx].realcash >= 0) sprintf(buf, "$%ld.%02ld", m_player[RplrIdx].realcash / 100, (m_player[RplrIdx].realcash) % 100);
							else sprintf(buf, "$-%ld.%02ld", -(m_player[RplrIdx].realcash) / 100, (-m_player[RplrIdx].realcash) % 100);
							((CGameAppView *)m_pParent)->m_statsDlg.m_realCash = _T(buf);

							// breaks
							((CGameAppView *)m_pParent)->m_statsDlg.m_p50s = m_player[RplrIdx]._p50s;
							((CGameAppView *)m_pParent)->m_statsDlg.m_p60s = m_player[RplrIdx]._p60s;
							((CGameAppView *)m_pParent)->m_statsDlg.m_p70s = m_player[RplrIdx]._p70s;
							((CGameAppView *)m_pParent)->m_statsDlg.m_p80s = m_player[RplrIdx]._p80s;
							((CGameAppView *)m_pParent)->m_statsDlg.m_p90s = m_player[RplrIdx]._p90s;
							((CGameAppView *)m_pParent)->m_statsDlg.m_p100s = m_player[RplrIdx]._p100s;
							((CGameAppView *)m_pParent)->m_statsDlg.m_pHighBreak = m_player[RplrIdx].pHighBreak;
							((CGameAppView *)m_pParent)->m_statsDlg.m_pMax147s = m_player[RplrIdx].pMax147s;
							//////////////
							((CGameAppView *)m_pParent)->m_statsDlg.m_50s = m_player[RplrIdx]._50s;
							((CGameAppView *)m_pParent)->m_statsDlg.m_60s = m_player[RplrIdx]._60s;
							((CGameAppView *)m_pParent)->m_statsDlg.m_70s = m_player[RplrIdx]._70s;
							((CGameAppView *)m_pParent)->m_statsDlg.m_80s = m_player[RplrIdx]._80s;
							((CGameAppView *)m_pParent)->m_statsDlg.m_90s = m_player[RplrIdx]._90s;
							((CGameAppView *)m_pParent)->m_statsDlg.m_100s = m_player[RplrIdx]._100s;
							((CGameAppView *)m_pParent)->m_statsDlg.m_highBreak = m_player[RplrIdx].HighBreak;
							((CGameAppView *)m_pParent)->m_statsDlg.m_max147s = m_player[RplrIdx].Max147s;

							((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_breaksAv = ceil(10.0f*m_player[RplrIdx].bioAverageBreak)/10.0f;
							((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_posPlayAv = ceil(10.0f*m_player[RplrIdx].bioAveragePosition)/10.0f;
							((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_shotTimeAv = ceil(100.0f*m_player[RplrIdx].bioAverageShotTime)/100.0f;
							((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_safetyPlayAv = ceil(10.0f*m_player[RplrIdx].bioAverageSafety)/10.0f;
							//////////////
							((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_pBreaksAv = ceil(10.0f*m_player[RplrIdx].pBioAverageBreak)/10.0f;
							((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_pPosPlayAv = ceil(10.0f*m_player[RplrIdx].pBioAveragePosition)/10.0f;
							((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_pShotTimeAv = ceil(100.0f*m_player[RplrIdx].pBioAverageShotTime)/100.0f;
							((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_pSafetyPlayAv = ceil(10.0f*m_player[RplrIdx].pBioAverageSafety)/10.0f;

							char buf[256];

							/*
							// Show the Text Colors //
							// Get a List of Colours player has purchased //
							sprintf(buf, "Normal"); if(g_selected_text==0) strcat(buf, " (S)");
							if(m_player[RplrIdx].inventory&1) { strcat(buf, "\r\nBlue"); if(g_selected_text==1) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&2) { strcat(buf, "\r\nPeach"); if(g_selected_text==2) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&4) { strcat(buf, "\r\nPink"); if(g_selected_text==3) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&8) { strcat(buf, "\r\nMud"); if(g_selected_text==4) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&16) { strcat(buf, "\r\nRed"); if(g_selected_text==5) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&32) { strcat(buf, "\r\nYellow"); if(g_selected_text==6) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&64) { strcat(buf, "\r\nLtBlue"); if(g_selected_text==7) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&128) { strcat(buf, "\r\nGreen"); if(g_selected_text==8) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&256) { strcat(buf, "\r\nTan"); if(g_selected_text==9) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&512) { strcat(buf, "\r\nJade"); if(g_selected_text==10) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&1024) { strcat(buf, "\r\nRose"); if(g_selected_text==11) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&2048) { strcat(buf, "\r\nSoftGreen"); if(g_selected_text==12) strcat(buf, " (S)"); }
							((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_inventory = _T(buf);
							*/
							// Show the Cue Types //
							// Get a List of Cues player has purchased //
		
							sprintf(buf, "House");
							if(m_player[RplrIdx].inventory&(1<<17)) { strcat(buf, "\r\nBlack & White"); }
							if(m_player[RplrIdx].inventory&(1<<18)) { strcat(buf, "\r\nBlue Lightning"); } 
							if(m_player[RplrIdx].inventory&(1<<19)) { strcat(buf, "\r\nProton Pink"); }
							if(m_player[RplrIdx].inventory&(1<<20)) { strcat(buf, "\r\nPurple Chrome"); }
							if(m_player[RplrIdx].inventory&(1<<21)) { strcat(buf, "\r\nWood"); }
							if(m_player[RplrIdx].inventory&(1<<22)) { strcat(buf, "\r\nDungeon"); }
							if(m_player[RplrIdx].inventory&(1<<23)) { strcat(buf, "\r\nDark Knight"); }
							((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_cuetype = _T(buf);			


							// Tourney Results //
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_winner = m_player[RplrIdx].winner;
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_runnerUp = m_player[RplrIdx].runnerUp;
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_semiFinals = m_player[RplrIdx].semiFinals;
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_quarterFinals = m_player[RplrIdx].quarterFinals;
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_last16 = m_player[RplrIdx].last16;
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_last32 = m_player[RplrIdx].last32;


							// Ban Data (History) //
							int bandata = m_player[RplrIdx].bandata;
					
							int BansAbuse		= (bandata&0xF);				// Bits 0-3		= BansAbuse //
							int BansCheating	= (bandata&0xF0)>>4;			// Bits 4-7		= BansCheating //
							int BansSpamming	= (bandata&0xF00)>>8;			// Bits 8-11	= BansSpamming //
							int BansUnAuth		= (bandata&0xF000)>>12;			// Bits 12-15	= BansUnAuth //
							int BansMTrials		= (bandata&0xF0000)>>16;		// Bits 16-19	= BansMTrials //
							
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_BansAbuse = BansAbuse;
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_BansCheating = BansCheating; 
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_BansSpamming = BansSpamming;
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_BansUnAuth = BansUnAuth;
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_BansMTrials = BansMTrials;
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_BansTotal = BansAbuse + BansCheating + BansSpamming + BansMTrials + BansUnAuth;
						

							// ------------------------------------------- //
							((CGameAppView *)m_pParent)->m_statsChatDlg.DestroyWindow();
							//((CGameAppView *)m_pParent)->m_statsDlg.DestroyWindow();
							//((CGameAppView *)m_pParent)->m_statsDlg.Create(IDD_STATS);				
							//((CGameAppView *)m_pParent)->m_statsDlg.ShowWindow(SW_SHOW);
							((CGameAppView *)m_pParent)->m_statsTabCtrlDlg.DestroyWindow();
							((CGameAppView *)m_pParent)->m_statsTabCtrlDlg.Create(IDD_STATSTABCTRL);
							((CGameAppView *)m_pParent)->m_statsTabCtrlDlg.ShowWindow(SW_SHOW);
							((CGameAppView *)m_pParent)->PreviousWndEditSetFocus();
							
							// Update History //
							

							// GameInfo //
							
							// BIT 1	- Won or Lost	
							// BITS 2-5 - Game Type (16 possible game types)
							// BITS 6-7 - Cue Error (4 possible Values)
							// BIT 8	- Doubles
							// BIT 9	- Conceded
							// BIT 10	- Miss Rule / CallShots
							// Bits 11-16  (6 Bits Reserved for Later Use //
							// Bits 17-24 (Frame Score of Myself) //
							// Bits 25-32 (Frame Score of Opponent) //

							for(int n=0;n<10;n++)
								{	
								if(sd->m_history.time[n])
									{
									// Create Result String //
									char buf[256];
									// Win of Loss
									if((sd->m_history.gameinfo[n])&1) strcpy(buf, "Won ");
										else  strcpy(buf, "Lost ");
									
									// Time of Game (GMT) //
									//CTimeSpan timeofgame_s = CTimeSpan(sd->m_history.time[n]);
									CTime timeofgame;
									timeofgame = CTime(sd->m_history.time[n]);

									//CTime timeofgame = timeofgame_s.;
									//CTime timeofgame(1970, 1, 1, 0, 0, sd->m_history.time[n]);
									char timestr[64];
									sprintf(timestr, "%s %s - ", timeofgame.Format( "%H:%M:%S (GMT)" ), timeofgame.Format( "%b %d, %Y" )); 
									strcat(buf, timestr);


									// Result of Game //
									
									// Get Score of P1 & P2 //
									int scoreP1 = ((sd->m_history.gameinfo[n])&0xFF000000) >> 24; // 11111111000000000000000000000000 Bit Mask
									int scoreP2 = ((sd->m_history.gameinfo[n])&0x00FF0000) >> 16; // 00000000111111110000000000000000 Bit Mask

									char wscore[8], lscore[8];
									if(scoreP1 > scoreP2) 
										{
										sprintf(wscore, " (%d)", scoreP1);
										sprintf(lscore, " (%d) ", scoreP2);
										}
									else{
										sprintf(lscore, " (%d) ", scoreP1);
										sprintf(wscore, " (%d)", scoreP2);
										}

									strcat(buf, sd->m_history.winner[n]);
									strcat(buf, wscore);
									strcat(buf, " Vs");
									strcat(buf, lscore);
									strcat(buf, sd->m_history.loser[n]);
									
									// Build up GameInfo string //
									char table_info[128];
									int token = 0;
									strcpy(table_info , "  - ");
										


									// GameType //
									token = ((sd->m_history.gameinfo[n])&0x1e) >> 1; // 11110 Bit Mask
									switch(token)
										{
										case 0: strcat(table_info, "15Reds "); break;
										case 1: strcat(table_info, "10Reds "); break;
										case 2: strcat(table_info, "6Reds "); break;
										case 3: strcat(table_info, "ReSpot "); break;
										case 4: strcat(table_info, "Billiards (50 pts) "); break;
										case 5: strcat(table_info, "Billiards (100 pts) "); break;
										case 6: strcat(table_info, "Billiards (200 pts) "); break;
										}
									// Cue Error //
									token = ((sd->m_history.gameinfo[n])&0x60) >> 5;	// 1100000 Bit Mask
									if(token == 0) strcat(table_info, "(B)");
										else if(token == 1) strcat(table_info, "(S)");
										else strcat(table_info, "(P)");
									// Doubles //
									token = ((sd->m_history.gameinfo[n])&0x80) >> 7;	// 10000000 Bit Mask
									if(token == 1) strcat(table_info, "(Dbl)");
									// Miss Rule //
									token = ((sd->m_history.gameinfo[n])&0x200) >> 9;	// 1000000000 Bit Mask
									if(token == 1) strcat(table_info, "(M)");
									// Conceded //
									token = ((sd->m_history.gameinfo[n])&0x100) >> 8;	// 100000000 Bit Mask
									if(token == 1) strcat(table_info, "*C*");
									
										
									strcat(buf, table_info);


									
									((CGameAppView *)m_pParent)->m_tournamentsDlg.m_historyCtrl.AddString(buf);	
									}
								}

																
						}
						else // iPool
						{
							// general
							((CGameAppView *)m_pParent)->m_statsDlg2.m_name =  m_player[RplrIdx].name;
							((CGameAppView *)m_pParent)->m_statsDlg2.m_rating = m_player[RplrIdx].rating;
							((CGameAppView *)m_pParent)->m_statsDlg2.m_highestrating = m_player[RplrIdx].highestrating;
							
							// Get Ladder Wins, Losses & Run-outs for 8-Ball and 9-Ball Ladder Matches
							int lLosses = (m_player[RplrIdx].ladderWinsLosses & 0xFFFF);
							int lWins = (m_player[RplrIdx].ladderWinsLosses & 0xFFFF0000) >> 16;
							int lRunOuts8Ball = (m_player[RplrIdx].ladderHighBreak & 0xFFFF);
							int lRunOuts9Ball = (m_player[RplrIdx].ladderHighBreak & 0xFFFF0000) >> 16;
							int gamesRemaining = (5 - (lWins + lLosses));
							if (gamesRemaining < 0) gamesRemaining = 0;

							// Ladder Rank & Points
							// Division (i.e. 0-10999: Bronze, 11000-13999: Silver, 14000-16999: Gold, 17000-20999: Platinum, 21000+ Diamond)
							int ladderPoints = m_player[RplrIdx].ladderPoints;
							char divisionStr[80];
							char pointsStr[80];
							char rankStr[80];

							// If we have played less than 5 games (change divisionStr too 'N Games needed')
							
							
							if (gamesRemaining > 0)
							{
								sprintf(divisionStr, "points & rank shown after %d ladder games", gamesRemaining);
								sprintf(pointsStr, "");
								sprintf(rankStr, "");
							}
							else
							{
								if (ladderPoints < 11000) sprintf(divisionStr, "Bronze division");
								else if (ladderPoints < 14000) sprintf(divisionStr, "Silver division");
								else if (ladderPoints < 17000) sprintf(divisionStr, "Gold division");
								else if (ladderPoints < 21000) sprintf(divisionStr, "Platinum division");
								else sprintf(divisionStr, "Diamond division");

								sprintf(pointsStr, "%d", m_player[RplrIdx].ladderPoints);
								sprintf(rankStr, "%d", m_player[RplrIdx].ladderRank);
							}

							((CGameAppView *)m_pParent)->m_statsDlg2.m_ladderRank = rankStr;
							((CGameAppView *)m_pParent)->m_statsDlg2.m_ladderDivision = divisionStr;
							((CGameAppView *)m_pParent)->m_statsDlg2.m_ladderPoints = pointsStr;



							((CGameAppView *)m_pParent)->m_statsDlg2.m_ladderGames = lWins + lLosses;
							((CGameAppView *)m_pParent)->m_statsDlg2.m_ladderWins = lWins;
							((CGameAppView *)m_pParent)->m_statsDlg2.m_ladderLosses = lLosses;
							((CGameAppView *)m_pParent)->m_statsDlg2.m_ladderHighBreak = lRunOuts8Ball;
							((CGameAppView *)m_pParent)->m_statsDlg2.m_ladderHighBreakPro = lRunOuts9Ball;
												
							if(m_player[RplrIdx].cash>=0) sprintf(buf, "$%ld.%02ld",m_player[RplrIdx].cash/100, (m_player[RplrIdx].cash)%100);
							else sprintf(buf, "$-%ld.%02ld",-(m_player[RplrIdx].cash)/100, (-m_player[RplrIdx].cash)%100);
							
							((CGameAppView *)m_pParent)->m_statsDlg2.m_cash = _T(buf);


							if (m_player[RplrIdx].realcash >= 0) sprintf(buf, "$%ld.%02ld", m_player[RplrIdx].realcash / 100, (m_player[RplrIdx].realcash) % 100);
							else sprintf(buf, "$-%ld.%02ld", -(m_player[RplrIdx].realcash) / 100, (-m_player[RplrIdx].realcash) % 100);


							((CGameAppView *)m_pParent)->m_statsDlg2.m_realCash = _T(buf);

							
							((CGameAppView *)m_pParent)->m_statsDlg2.m_wins = m_player[RplrIdx].wins;
							((CGameAppView *)m_pParent)->m_statsDlg2.m_losses = m_player[RplrIdx].losses;
							((CGameAppView *)m_pParent)->m_statsDlg2.m_games = m_player[RplrIdx].wins + m_player[RplrIdx].losses;
							((CGameAppView *)m_pParent)->m_statsDlg2.m_streak = m_player[RplrIdx].streak;
							((CGameAppView *)m_pParent)->m_statsDlg2.m_reracks = m_player[RplrIdx].reracks;

							

							// breaks <=> 8-ball/9-ball/Enlighs Pool game stats
							((CGameAppView *)m_pParent)->m_statsDlg2.m_runOut8Std = m_player[RplrIdx]._50s;
							((CGameAppView *)m_pParent)->m_statsDlg2.m_runOut9Std = m_player[RplrIdx]._60s;
							((CGameAppView *)m_pParent)->m_statsDlg2.m_runOutEngStd = m_player[RplrIdx]._70s;
							
							((CGameAppView *)m_pParent)->m_statsDlg2.m_runOut8Pro = m_player[RplrIdx]._80s;
							((CGameAppView *)m_pParent)->m_statsDlg2.m_runOut9Pro = m_player[RplrIdx]._90s;
							((CGameAppView *)m_pParent)->m_statsDlg2.m_runOutEngPro = m_player[RplrIdx]._p50s;
							
							((CGameAppView *)m_pParent)->m_statsDlg2.m_runOut8UK = m_player[RplrIdx]._p60s;
							((CGameAppView *)m_pParent)->m_statsDlg2.m_runOut9UK = m_player[RplrIdx]._p70s;
							((CGameAppView *)m_pParent)->m_statsDlg2.m_runOutEngUK = m_player[RplrIdx]._p80s;

							((CGameAppView *)m_pParent)->m_statsDlg2.m_runOut8Rate = ((float)(m_player[RplrIdx]._100s))/1000;
							((CGameAppView *)m_pParent)->m_statsDlg2.m_runOut9Rate = ((float)(m_player[RplrIdx]._p90s))/1000;
							((CGameAppView *)m_pParent)->m_statsDlg2.m_runOutEngRate = ((float)(m_player[RplrIdx]._p100s))/1000;


							

							((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_breaksAv = ceil(10.0f*m_player[RplrIdx].bioAverageBreak)/10.0f; // change this for iPool
							((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_posPlayAv = ceil(10.0f*m_player[RplrIdx].bioAveragePosition)/10.0f;
							((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_shotTimeAv = ceil(100.0f*m_player[RplrIdx].bioAverageShotTime)/100.0f;
							((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_safetyPlayAv = ceil(10.0f*m_player[RplrIdx].bioAverageSafety)/10.0f;
							//////////////
							((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_pBreaksAv = ceil(10.0f*m_player[RplrIdx].pBioAverageBreak)/10.0f; // change this for iPool
							((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_pPosPlayAv = ceil(10.0f*m_player[RplrIdx].pBioAveragePosition)/10.0f;
							((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_pShotTimeAv = ceil(100.0f*m_player[RplrIdx].pBioAverageShotTime)/100.0f;
							((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_pSafetyPlayAv = ceil(10.0f*m_player[RplrIdx].pBioAverageSafety)/10.0f;

							char buf[256];
							/*
							// Show the Text Colors //
							// Get a List of Colours player's have purchased //
							sprintf(buf, "Normal"); if(g_selected_text==0) strcat(buf, " (S)");
							if(m_player[RplrIdx].inventory&1) { strcat(buf, "\r\nBlue"); if(g_selected_text==1) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&2) { strcat(buf, "\r\nPeach"); if(g_selected_text==2) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&4) { strcat(buf, "\r\nPink"); if(g_selected_text==3) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&8) { strcat(buf, "\r\nMud"); if(g_selected_text==4) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&16) { strcat(buf, "\r\nRed"); if(g_selected_text==5) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&32) { strcat(buf, "\r\nYellow"); if(g_selected_text==6) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&64) { strcat(buf, "\r\nLtBlue"); if(g_selected_text==7) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&128) { strcat(buf, "\r\nGreen"); if(g_selected_text==8) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&256) { strcat(buf, "\r\nTan"); if(g_selected_text==9) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&512) { strcat(buf, "\r\nJade"); if(g_selected_text==10) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&1024) { strcat(buf, "\r\nRose"); if(g_selected_text==11) strcat(buf, " (S)"); }
							if(m_player[RplrIdx].inventory&2048) { strcat(buf, "\r\nSoftGreen"); if(g_selected_text==12) strcat(buf, " (S)"); }
							((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_inventory = _T(buf);
							*/
							// Get a List of Cues player's have purchased //
							sprintf(buf, "House"); 
							if(m_player[RplrIdx].inventory&(1<<17)) { strcat(buf, "\r\nBlack & White"); }
							if(m_player[RplrIdx].inventory&(1<<18)) { strcat(buf, "\r\nBlue Lightning"); } 
							if(m_player[RplrIdx].inventory&(1<<19)) { strcat(buf, "\r\nProton Pink"); }
							if(m_player[RplrIdx].inventory&(1<<20)) { strcat(buf, "\r\nPurple Chrome"); }
							if(m_player[RplrIdx].inventory&(1<<21)) { strcat(buf, "\r\nWood"); }
							if(m_player[RplrIdx].inventory&(1<<22)) { strcat(buf, "\r\nDungeon"); }
							if(m_player[RplrIdx].inventory&(1<<23)) { strcat(buf, "\r\nDark Knight"); }
							
							((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_cuetype = _T(buf);	


							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_winner = m_player[RplrIdx].winner;
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_runnerUp = m_player[RplrIdx].runnerUp;
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_semiFinals = m_player[RplrIdx].semiFinals;
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_quarterFinals = m_player[RplrIdx].quarterFinals;
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_last16 = m_player[RplrIdx].last16;
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_last32 = m_player[RplrIdx].last32;
							
							// Ban Data (History) //
							int bandata = m_player[RplrIdx].bandata;
					
							int BansAbuse		= (bandata&0xF);				// Bits 0-3		= BansAbuse //
							int BansCheating	= (bandata&0xF0)>>4;			// Bits 4-7		= BansCheating //
							int BansSpamming	= (bandata&0xF00)>>8;			// Bits 8-11	= BansSpamming //
							int BansUnAuth		= (bandata&0xF000)>>12;			// Bits 12-15	= BansUnAuth //
							int BansMTrials		= (bandata&0xF0000)>>16;		// Bits 16-19	= BansMTrials //
							
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_BansAbuse = BansAbuse;
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_BansCheating = BansCheating; 
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_BansSpamming = BansSpamming;
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_BansUnAuth = BansUnAuth;
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_BansMTrials = BansMTrials;
							((CGameAppView *)m_pParent)->m_tournamentsDlg.m_BansTotal = BansAbuse + BansCheating + BansSpamming + BansMTrials + BansUnAuth;
						


							// ------------------------------------------- //
							((CGameAppView *)m_pParent)->m_statsChatDlg.DestroyWindow();
							//((CGameAppView *)m_pParent)->m_statsDlg2.DestroyWindow();
							//((CGameAppView *)m_pParent)->m_statsDlg2.Create(IDD_STATS2);
							//((CGameAppView *)m_pParent)->m_statsDlg2.ShowWindow(SW_SHOW);
							((CGameAppView *)m_pParent)->m_statsTabCtrlDlg.DestroyWindow();
							((CGameAppView *)m_pParent)->m_statsTabCtrlDlg.Create(IDD_STATSTABCTRL);
							((CGameAppView *)m_pParent)->m_statsTabCtrlDlg.ShowWindow(SW_SHOW);
							((CGameAppView *)m_pParent)->PreviousWndEditSetFocus();


							// Update History //
							

							// GameInfo //
							
							// BIT 1	- Won or Lost	
							// BITS 2-5 - Game Type (16 possible game types)
							// BITS 6-7 - Table Type (4 possible Values)
							// BIT 8	- Doubles
							// BIT 9	- Conceded
							// BIT 10	- Miss Rule / CallShots
							// Bits 11-16  (6 Bits Reserved for Later Use //
							// Bits 17-24 (Frame Score of Myself) //
							// Bits 25-32 (Frame Score of Opponent) //

							for(int n=0;n<10;n++)
								{	
								if(sd->m_history.time[n])
									{
									// Create Result String //
									char buf[256];
									// Win of Loss
									if((sd->m_history.gameinfo[n])&1) strcpy(buf, "Won ");
										else  strcpy(buf, "Lost ");
												
									// Time of Game (GMT) //
									CTime timeofgame;
									timeofgame = CTime(sd->m_history.time[n]);
									char timestr[64];
									sprintf(timestr, "%s %s - ", timeofgame.Format( "%H:%M:%S (GMT)" ), timeofgame.Format( "%b %d, %Y" )); 
									strcat(buf, timestr);


									// Build up GameInfo string //
									char table_info[128];
									int token = 0;
									strcpy(table_info , "  - ");

									// GameType //
									token = ((sd->m_history.gameinfo[n])&0x1e) >> 1; // 11110 Bit Mask
									switch(token)
										{
										case 0: strcat(table_info, "8-Ball "); break;
										case 1: strcat(table_info, "9-Ball "); break;
										case 2: strcat(table_info, "Rotation-Ball "); break;
										case 3: strcat(table_info, "Bowls "); break;
										case 4: strcat(table_info, "6-Ball "); break;
										case 5: strcat(table_info, "10-Ball "); break;
										case 6: strcat(table_info, "Break "); break;
										case 7: strcat(table_info, "UK Pool "); break;
										}



									// Result of Game //
									// Only add Score if Game is Break //

									char wscore[8], lscore[8];
									if(token == 6)
									{
										int scoreP1 = ((sd->m_history.gameinfo[n])&0xFF000000) >> 24; // 11111111000000000000000000000000 Bit Mask
										int scoreP2 = ((sd->m_history.gameinfo[n])&0x00FF0000) >> 16; // 00000000111111110000000000000000 Bit Mask

										
										
										if(scoreP1 > scoreP2) 
											{
											sprintf(wscore, " (%d)", scoreP1);
											sprintf(lscore, "(%d) ", scoreP2);
											}
										else{
											sprintf(lscore, " (%d)", scoreP1);
											sprintf(wscore, "(%d) ", scoreP2);
											}
									}
										

									strcat(buf, sd->m_history.winner[n]);
									if(token == 6) strcat(buf, wscore);
									strcat(buf, " Vs ");
									if(token == 6) strcat(buf, lscore);
									strcat(buf, sd->m_history.loser[n]);
									
									


									
									// Table Type //
									
									token = ((sd->m_history.gameinfo[n])&0x60) >> 5;	// 1100000 Bit Mask
									if(token == 0) strcat(table_info, "(Std)");
										else if(token == 1) strcat(table_info, "(Pro)");
										else if(token == 2) strcat(table_info, "(UK)");
										else strcat(table_info, "(Wild)");
									
									// Doubles //
									token = ((sd->m_history.gameinfo[n])&0x80) >> 7;	// 10000000 Bit Mask
									if(token == 1) strcat(table_info, "(Dbl)");
									// Miss Rule //
									token = ((sd->m_history.gameinfo[n])&0x200) >> 9;	// 1000000000 Bit Mask
									if(token == 1) strcat(table_info, "(CS)");
									// Conceded //
									token = ((sd->m_history.gameinfo[n])&0x100) >> 8;	// 100000000 Bit Mask
									if(token == 1) strcat(table_info, "*C*");
									
										
									strcat(buf, table_info);


									
									((CGameAppView *)m_pParent)->m_tournamentsDlg.m_historyCtrl.AddString(buf);	
									}
								}
											
						}
					}

				}
				

				//char buffer[80];
				//sprintf(buffer,"%s-[#%d]", plr.name, plr.id);
				//ErrorMessage(m_pParentWnd, buffer);
				
			
				#endif //MAIN_SERVER
				//ErrorMessage(m_pParentWnd, "MSGGAME_PLAYERDETAILS: POST");
			}
			break;

			case MSGGAME_PLAYERDETAILSC:
			{
	
				/*
				static int count = 0;
				count++;
				//AfxGetMainWnd()->DestroyWindow();
				if(count > 2) 
				{
				
					g_gameAppView->CloseApp();
					return false;
				
				}
				
				break;
				*/



				// update clients
				#ifndef MAIN_SERVER
				playerDetailsSendDataC *sd;
				sd = (playerDetailsSendDataC_typ *) lpBuf;
				Player plr;
				
				InitPlayer(&plr);
				
				plr.playerSocketIdx = sd->playerSocketIdx;
				plr.rating = sd->rating;
				////////////
				plr.Max147s = sd->Max147s;

				plr.pMax147s = sd->pMax147s;
				plr._100s = sd->_100s;
				plr._p100s = sd->_p100s;
				
				plr._50s = sd->_50s;
				plr._60s = sd->_60s;
				plr._70s = sd->_70s;
				plr._80s = sd->_80s;
				plr._90s = sd->_90s;
				
				plr._p50s = sd->_p50s;
				plr._p60s = sd->_p60s;
				plr._p70s = sd->_p70s;
				plr._p80s = sd->_p80s;

				plr.bioAverageBreak = sd->BioAverageBreak;
				plr.pBioAverageBreak = sd->pBioAverageBreak;


				plr.cuetype = sd->cuetype;

				plr.chalktype = sd->chalktype;
				
				// Safety check, to make sure our cue ranges from 0 - 7
				if(plr.cuetype < 0) 
				{
					plr.cuetype = 0;
				}
				if(plr.cuetype > 7) 
				{
					plr.cuetype = 0;
				}

				plr.playertype = sd->playertype;
				////////////
				plr.table = sd->table;
				plr.state = sd->buffer[2];
				plr.state2 = sd->state2;

				plr.bandata = sd->bandata;

				plr.level = sd->level;
				plr.level_progress = sd->level_progress;

				plr.cash = sd->cash;

				plr.ladderPoints = sd->ladderPoints;
				plr.ladderWinsLosses = sd->ladderWinsLosses;
				plr.ladderHighBreak = sd->ladderHighBreak;

				

				//plr.state = sd->state;
				plr.id = sd->id;
				BYTE flags;
				flags = sd->buffer[3]; // binvited/bisguest defaults to FALSE
				if(flags&1) plr.bInvited = TRUE;
				if(flags&2)	plr.bIsGuest = TRUE;
				//plr.bInvited = sd->bInvited;
				//plr.bIsGuest = sd->bIsGuest;
				strcpy(plr.name, sd->name);
				plr.password[0] = 0;
				plr.textFormat = sd->textFormat;

				// Check If this player was on ignore //
				if(((CGameAppView *)m_pParentWnd)->m_ignorelst->CheckName(plr.name)) plr.bIgnore = TRUE;
					else plr.bIgnore = FALSE;

	
				((CGameAppView *)m_pParentWnd)->m_lobby->AddPlayer(&plr);

		
						
				// is this my player details?
				if(plr.playerSocketIdx==((CGameAppView *)m_pParentWnd)->m_playerSockIdx)
				{
					((CGameAppView *)m_pParentWnd)->m_playerId = plr.id;
					((CGameAppView *)m_pParentWnd)->m_playerIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->FindPlayerIdxFromId(plr.id);

					// Update Cue Changer
					char buf[80];
					int cue_index = 0;
					
					

					((CGameAppView *)m_pParentWnd)->m_cueChanger->InitCue(plr.cuetype);

					DWORD chalkIdx = (plr.chalktype) & 0xff;

					((CGameAppView *)m_pParentWnd)->m_chalkChanger->InitChalk(chalkIdx);


					// Refresh our SmileySelect Dialog
					g_gameAppView->m_SmileySelectDlg.m_bRefresh = true;

				}
				//char buffer[80];
				//sprintf(buffer,"%s-[#%d]", plr.name, plr.id);
				//ErrorMessage(m_pParentWnd, buffer);

				
				#endif //MAIN_SERVER
				//ErrorMessage(m_pParentWnd, "MSGGAME_PLAYERDETAILS: POST");
			}
			break;

			case MSGGAME_PLAYERJOIN:
			{

				// update clients
				#ifndef MAIN_SERVER
				
				playerJoinSendData *sd;
				sd = (playerJoinSendData_typ *) lpBuf;
				
				// If I'm the joiner, then make sure we are using the Default View //
				// Also Update the Seat playersocketidx if we're a doubels game type //
				//if(((CGameAppView *)m_pParentWnd)->m_lobby->m_bInLobby)
				if(((CGameAppView *)m_pParentWnd)->m_playerSockIdx == sd->m_playerSockIdx)
					{
					//((CGameAppView *)m_pParentWnd)->m_snooker->m_3dtoggle = 3;
					//((CGameAppView *)m_pParentWnd)->m_snooker->PROCESS_CAMERA_TV(0, 1);	// Force the View to what we set //

					if(m_table[sd->m_tableIdx].doubles)
					{
						int plrTblIdx = sd->m_tableIdx;
						// Get player indexes for all seats //
						if(plrTblIdx != -1)
							{
							DWORD Sock1	= (sd->m_tableseatsA)&0xFFFF;
							DWORD Sock1b = (sd->m_tableseatsA)&0xFFFF0000;
							Sock1b = Sock1b >> 16;
							DWORD Sock2	= (sd->m_tableseatsB)&0xFFFF;
							DWORD Sock2b = (sd->m_tableseatsB)&0xFFFF0000;
							Sock2b = Sock2b >> 16;
							
							// Make sure both P1 and P1b Seats are empty, then add players seated in the right order (if any)
							int SockIdx1P = m_table[plrTblIdx].player1SockIdx;
							int SockIdx1Pb = m_table[plrTblIdx].player1bSockIdx;
							if(SockIdx1P != -1 || SockIdx1Pb != -1)
								{
								if(SockIdx1P != -1) ((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(SockIdx1P, SockIdx1P, plrTblIdx, SockIdx1P, SockIdx1P);
								if(SockIdx1Pb != -1) ((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(SockIdx1Pb, SockIdx1Pb, plrTblIdx, SockIdx1Pb, SockIdx1Pb);
								if(Sock1 != 65535) ((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(Sock1, -1, plrTblIdx, -1, -1);
								if(Sock1b != 65535) ((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(-1, -1, plrTblIdx, Sock1b, -1);
								}
							// If either P2 or P2b is occupied, we may need to swap the order //
							int SockIdx2P = m_table[plrTblIdx].player2SockIdx;
							int SockIdx2Pb = m_table[plrTblIdx].player2bSockIdx;
							if(SockIdx2P != -1 || SockIdx2Pb != -1)
								{
								if(SockIdx2P != -1) ((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(SockIdx2P, SockIdx2P, plrTblIdx, SockIdx2P, SockIdx2P);
								if(SockIdx2Pb != -1) ((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(SockIdx2Pb, SockIdx2Pb, plrTblIdx, SockIdx2Pb, SockIdx2Pb);
								if(Sock2 != 65535) ((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(-1, Sock2, plrTblIdx, -1, -1);
								if(Sock2b != 65535) ((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(-1, -1, plrTblIdx, -1, Sock2b);
								}
							
					
							}
						}
					}	

				((CGameAppView *)m_pParentWnd)->m_lobby->JoinTable(sd->m_tableIdx,
																   sd->m_enterType,
																   sd->m_playerSockIdx,
																   sd->m_bIsTableHost);
				// If we are watching This Table, then Update the Lobby TV //
				// Do this only if game is not started //
				if(m_table[sd->m_tableIdx].bGameStarted == 0) ((CGameAppView *)m_pParentWnd)->UpdateLobbyTableWatch(sd->m_tableIdx);
				#endif //MAIN_SERVER

			}
			break;
			
			case MSGGAME_PLAYERREJOIN:
			{

				playerReJoinSendData *sd;
				sd = (playerReJoinSendData_typ *) lpBuf;
				((CGameAppView *)m_pParentWnd)->m_lobby->ReJoinTable(sd->m_tableIdx);

			}
			break;			

			case MSGGAME_PLAYERJOINFAILED:
			{

				// update clients
				#ifndef MAIN_SERVER
				playerJoinFailedSendData *sd;
				sd = (playerJoinFailedSendData_typ *) lpBuf;

				((CGameAppView *)m_pParentWnd)->m_lobby->JoinTableFailed(sd->m_joinerSockIdx, sd->m_hostSockIdx);
				#endif //MAIN_SERVER

			}
			break;

			case MSGGAME_PLAYEREXIT:
			{

				// update clients
				#ifndef MAIN_SERVER

				

				playerExitSendData *sd;
				sd = (playerExitSendData_typ *) lpBuf;
				
				// If we are the one Exiting, then do the following Local Code also //
				if(((CGameAppView *)m_pParentWnd)->m_playerSockIdx == sd->m_playerSockIdx) 
					((CGameAppView *)m_pParentWnd)->ExitTable2Lobby();
				
				
				((CGameAppView *)m_pParentWnd)->m_lobby->ExitTable(sd->m_playerSockIdx, sd->m_bTimedOutBoot, sd->m_bPLC);
				
				// If we're in the Lobby, then we need to refresh the Table List / Player List //
				//if(((CGameAppView *)m_pParentWnd)->m_lobby->m_bInLobby) m_listTables.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);// | RDW_ALLCHILDREN);	
				//m_listPlayers.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);
	
				
				//((CGameAppView *)m_pParentWnd)->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);

				
				#endif //MAIN_SERVER

			}
			break;

			case MSGGAME_PLAYERSIT:
			{
				// update clients
				#ifndef MAIN_SERVER
				playerSitSendData *sd;
				sd = (playerSitSendData_typ *) lpBuf;

				((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(sd->m_seat1SockIdx,
																	   sd->m_seat2SockIdx,
																	   sd->m_tableIdx,
																	   sd->m_seat1bSockIdx,
																	   sd->m_seat2bSockIdx);

				// If we are watching This Table, then Update the Lobby TV //
				((CGameAppView *)m_pParentWnd)->UpdateLobbyTableWatch(sd->m_tableIdx);
				#endif //MAIN_SERVER

			}
			break;

			case MSGGAME_TABLEDETAILS:
			{

				tableDetailsSendData *sd;
				sd = (tableDetailsSendData_typ *) lpBuf;
				
				Table tbl;

				// Make sure playersSockIdx are all initialized to -1 //
				for(int n=0;n<MAX_TABLE_PLAYERS;n++) tbl.playersSockIdx[n] = -1;

				// Now when we memcpy over the data received, we'll only be updating playersSockIdx[n] if needed //
				int packetsize = sizeof(Table_typ);
				packetsize -= (MAX_TABLE_PLAYERS-sd->m_table.playersSockIdxN) * 4;
				
				memcpy(&tbl, &sd->m_table, packetsize);
				

				((CGameAppView *)m_pParentWnd)->m_lobby->AddTable(&tbl);

			}
			break;

			case MSGGAME_TABLEHOSTDETAILS:
			{

				// update clients
				#ifndef MAIN_SERVER
				tableHostDetailsSendData *sd;
				sd = (tableHostDetailsSendData_typ *) lpBuf;
				
				((CGameAppView *)m_pParentWnd)->m_lobby->UpdateTableHostDetails(sd->m_cueError,sd->m_bRated, sd->m_type, sd->m_bMissRule, sd->m_playerSockIdx,
															sd->m_shotTime, sd->m_tableColour, sd->m_gameType, sd->m_addTimer, sd->m_doubles,
															sd->m_raceTo, sd->m_whoBreaks,
															sd->m_ballSize, sd->m_rackType, sd->m_tableType, sd->m_proTable,
															sd->m_tournamentTable, sd->m_fp1, sd->m_fp2, sd->m_score1, sd->m_score2, sd->m_bLadderMatch);
				#endif //MAIN_SERVER

			}
			break;

			// game operations
			case MSGGAME_SHOT:
			{
			
			#ifndef MAIN_SERVER
			
			// ErrorMessage(g_wnd, "MSGGAME_SHOT - 1");

			// If we're in the Lobby and we received this message, then set the Cue-Ball Status
			// If we're in the Lobby, only proceed if g_LobbyTVInfo.bTurn1P is 0 or 1 (means we have received the MSGGAME_BALLS)
			
			BOOL bShowShotInLobbyTV = false;

			if (g_gfxDetails.bShowShotInLobbyTV)
			{
				int plrTblIdx = g_gameAppView->m_lobby->GetTableIdxFromPlayer(g_gameAppView->m_playerSockIdx);

				// If we received this message and we're watching the game, and in lobby, set bShowShotInLobbyTV to true
				if (plrTblIdx == -1) // If we're in the Lobby
				if (!g_gameAppView->m_lobby->m_bInTable) // If in the Lobby//
				{
					if (g_gameAppView->m_bLobbyMode != 2)
					{
						// If we are not watching a table ignore this and set our lobby tv to -1.. no table
						// 'Lobby TV' Button Click, so we need to update which Table We're watching //
						g_LobbyTVInfo.lobbytablewatch = -1;
						g_gameAppView->UpdateLobbyTableWatch(-1);
						// Update the Server Also //
						g_gameAppView->UpdateLobbyTVServer(g_gameAppView->m_playerSockIdx, -1);
						// ErrorMessage(g_wnd, "Cancel Lobby TV (table watch)");
					}
					else
					{

						if ((g_LobbyTVInfo.bTurn1P == 0) || (g_LobbyTVInfo.bTurn1P == 1))
						{
							bShowShotInLobbyTV = true;
						}
					}
				}
			}
			
			if (((CGameAppView *)m_pParentWnd)->m_lobby->m_bInTable || bShowShotInLobbyTV)
			{


				// ErrorMessage(g_wnd, "MSGGAME_SHOT - 2");

				strikeBallSendData *sd;
				sd = (strikeBallSendData_typ *) lpBuf;


				((CGameAppView *)m_pParentWnd)->m_snooker->m_bStrikeBallReplayDrawSL = 0;
				((CGameAppView *)m_pParentWnd)->m_snooker->StrikeBallReceive(sd->m_sPoint[0],sd->m_sPoint[1],sd->m_sPoint[2], 
																	  sd->m_tableMousePos[0],sd->m_tableMousePos[1],sd->m_tableMousePos[2],
																	  sd->m_cuetip,
																	  sd->m_selectBall,
																	  sd->m_strikeRnd,
																	  sd->m_strikeMaxRnd,
																	  sd->m_ballMouseOn, sd->m_ballMousePos, sd->m_nominateBall, sd->m_testBallColour, sd->m_crc, 0, sd->m_nSimEndMsgBalls);
			
			}

			#endif //!MAIN_SERVER
			}
			break;

			case MSGGAME_BALL_POS:
			{
			#ifndef MAIN_SERVER

			// BAPFIX //
			// Send this message, when the CueBall is selected, as Redudency, to ensure 
			// opponent and watchers see the correct Cueball position //
			// Should work //

			if (((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect >= 20 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 2 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 4) // *NEW - if still in sim ignore this msg
			{
				// save ballPosData for later use. while we carry on with our sim.
				memcpy(&g_ballPosDataBk, (ballPosSendData_typ *)lpBuf, sizeof(ballPosSendData_typ));
				//ErrorMessage(g_wnd, "memcpy ballPosData");
				break;
			}

			ballPosSendData *sd;
			sd = (ballPosSendData_typ *)lpBuf;
			((CGameAppView *)m_pParentWnd)->m_snooker->BallPos(sd->m_P, sd->m_selectBall);

			#endif //!MAIN_SERVER
			}
			break;

			case MSGGAME_SELECT_BALL:
			{
			#ifndef MAIN_SERVER

			if (((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect >= 20 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 2 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 4) // *NEW - if still in sim ignore this msg
			{
				// save selectBallData for later use. while we carry on with our sim.
				memcpy(&g_selectBallDataBk, (selectBallSendData_typ *)lpBuf, sizeof(selectBallSendData_typ));
				//ErrorMessage(g_wnd, "memcpy selectBallData");
				break;
			}

			selectBallSendData *sd;
			sd = (selectBallSendData_typ *)lpBuf;
			((CGameAppView *)m_pParentWnd)->m_snooker->SelectBallReceive(sd->m_cueBallSelect, sd->m_point, sd->w, sd->h, sd->m_selectBall);

			#endif //!MAIN_SERVER
			}
			break;

			case MSGGAME_MOUSE_MOVE:
			{
			#ifndef MAIN_SERVER

			if (((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect >= 20 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 2 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 4) // *NEW - if still in sim ignore this msg
			{
				// save mouseMoveData for later use. while we carry on with our sim.				
				memcpy(&g_mouseMoveDataBk, (mouseMoveSendData_typ *)lpBuf, sizeof(mouseMoveSendData_typ));
				//ErrorMessage(g_wnd, "memcpy mouseMoveData");
				break;
			}

			mouseMoveSendData *sd;
			sd = (mouseMoveSendData_typ *)lpBuf;
			((CGameAppView *)m_pParentWnd)->m_snooker->MouseMoveReceive(sd->x, sd->y, sd->w, sd->h, sd->strikeLine, sd->strikeDist);

			#endif //!MAIN_SERVER
			}
			break;

			case MSGGAME_CUE_BALL_TIP:
			{
			#ifndef MAIN_SERVER

			if (((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect >= 20 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 2 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 4) // *NEW - if still in sim ignore this msg
				break;

			CueBallTipSendData *sd;
			sd = (CueBallTipSendData_typ *)lpBuf;
			((CGameAppView *)m_pParentWnd)->m_snooker->CueBallTipReceive(sd->m_sx, sd->m_sy, sd->m_sz, sd->m_ex, sd->m_ey, sd->m_ez,
				sd->m_ballMouseOn, sd->m_ballMousePos, sd->m_cueTip,
				sd->m_nominateBall,
				sd->m_RaiseButt, sd->m_RaiseButtOld,
				sd->m_cueAngle);
			#endif //!MAIN_SERVER
			}
			break;

			case MSGGAME_CUE_BALL_TIPC:
			{
			#ifndef MAIN_SERVER			
			if (((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect >= 20 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 2 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 4) // *NEW - if still in sim ignore this msg
			{
				// save CueBallTipDataC for later use. while we carry on with our sim.
				memcpy(&g_CueBallTipDataCBk, (CueBallTipSendDataC_typ *)lpBuf, sizeof(CueBallTipSendDataC_typ));
				//ErrorMessage(g_wnd, "memcpy CueBallTipDataC");
				break;
			}

			((CGameAppView *)m_pParentWnd)->m_snooker->CueBallTipReceiveCompact((CueBallTipSendDataC_typ *)lpBuf);
			#endif //!MAIN_SERVER
			}
			break;


			case MSGGAME_ABORT_BYSERVER:
			{
			#ifndef MAIN_SERVER
			plrTblIdx = buffer[6];
			((CGameAppView *)m_pParentWnd)->m_lobby->EndGameReceive(plrTblIdx);	// Tell Server to End the Game //
			#endif
			}
			break;

			case MSGGAME_RESET_BYSERVER:
			{
			#ifndef MAIN_SERVER
			char buf[128];
			plrTblIdx = buffer[6];
			
			if(plrTblIdx >=0 && plrTblIdx <= MAX_LOBBY_TABLES)
				{
				m_table[plrTblIdx].bLockSeats = FALSE;

				((CGameAppView *)m_pParentWnd)->m_listTables.EnableItemButton(plrTblIdx*4+2, FALSE);
				((CGameAppView *)m_pParentWnd)->m_listTables.EnableItemButton(plrTblIdx*4+3, FALSE);

				// If Doubles, Make sure everyone is Stood Up //
				// Make sure we are Stood Up //
				int sockidx1 = m_table[plrTblIdx].player1SockIdx;
				int sockidx2 = m_table[plrTblIdx].player2SockIdx;
				int sockidx1b = m_table[plrTblIdx].player1bSockIdx;
				int sockidx2b = m_table[plrTblIdx].player2bSockIdx;

				if(sockidx1 != -1) ((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(sockidx1, sockidx1, plrTblIdx, sockidx1, sockidx1);
				if(sockidx2 != -1) ((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(sockidx2, sockidx2, plrTblIdx, sockidx2, sockidx2);
				if(sockidx1b != -1) ((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(sockidx1b, sockidx1b, plrTblIdx, sockidx1b, sockidx1b);
				if(sockidx2b != -1) ((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(sockidx2b, sockidx2b, plrTblIdx, sockidx2b, sockidx2b);



				m_table[plrTblIdx].bReRackRequest = FALSE;
				m_table[plrTblIdx].racksWon1P = 0;
				m_table[plrTblIdx].racksWon2P = 0;
				m_table[plrTblIdx].bGameStarted = FALSE;
				m_table[plrTblIdx].P1StartAtTable = -1;
				m_table[plrTblIdx].P2StartAtTable = -1;
				m_table[plrTblIdx].P1StartName[0] = 0;
				m_table[plrTblIdx].P2StartName[0] = 0;
				m_table[plrTblIdx].P1bStartAtTable = -1;
				m_table[plrTblIdx].P2bStartAtTable = -1;
				m_table[plrTblIdx].P1bStartName[0] = 0;
				m_table[plrTblIdx].P2bStartName[0] = 0;
				m_table[plrTblIdx].score1P = m_table[plrTblIdx].startScore1P;
				m_table[plrTblIdx].score2P = m_table[plrTblIdx].startScore2P;
				m_table[plrTblIdx].m_lostConn1P = 0;
				m_table[plrTblIdx].m_lostConn2P = 0;

				((CGameAppView *)m_pParentWnd)->m_standButtonEnabled = TRUE;



				char playerTurn = buffer[3];
				int *rndval = (int *)&buffer[4];
				((CGameAppView *)m_pParentWnd)->m_snooker->Init(RERACK_START, *rndval, playerTurn);
				}

			#endif //!MAIN_SERVER
			}
			break;
			
			
			case MSGGAME_RESET:
			{

			/*
			char buf[256];
			sprintf(buf, "MSGGAME_RESET: m_cueballSelect: %d", ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect);
			ErrorMessage(g_wnd, buf);
			*/

			#ifndef MAIN_SERVER
			if(((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect >= 20 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 2 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 4) // *NEW - if still in sim ignore this msg
			{
				// ErrorMessage(g_wnd, "save reset data for later use"); 
				// save reset data for later use. while we carry on with our sim.				
				memcpy(&g_ResetbufferBk, (char *) lpBuf, sizeof(char)*16);

				// clear out all other lagging backup data - since this msg would have cleared it anyway
				//memset(&g_ballsSendDataBk, 0, sizeof(BallsSendData_typ)); // leave this out! see [*(!)*]
				memset(&g_ballPosDataBk, 0, sizeof(ballPosSendData_typ));
				memset(&g_CueBallTipDataCBk, 0, sizeof(CueBallTipSendDataC_typ));
				memset(&g_selectBallDataBk, 0, sizeof(selectBallSendData_typ));
				memset(&g_mouseMoveDataBk, 0, sizeof(mouseMoveSendData_typ));
				memset(&g_PlayAgainbufferBk, 0, sizeof(char)*4);
				memset(&g_FoulAlertbufferBk, 0, sizeof(char)*4);
				memset(&g_MissResetbufferBk, 0, sizeof(char)*4);
				memset(&g_MaxTimeUpDataBk, 0, sizeof(MaxTimeUpSendData_typ));
				memset(&g_playerTargetDataBk, 0, sizeof(PlayerTargetSendData_typ));
				memset(&g_Pot8OnBreakRerackbufferBk, 0, sizeof(char)*4);
				memset(&g_BadBreakRerackbufferBk, 0, sizeof(char)*4);
				//ErrorMessage(g_wnd, "memcpy Resetbuffer");
				break;
			}

			char playerTurn = buffer[3];
			int *rndval = (int *)&buffer[4];

			((CGameAppView *)m_pParentWnd)->m_snooker->Init(((CGameAppView *)m_pParentWnd)->m_snooker->m_reRackType, *rndval, playerTurn);

			#endif //!MAIN_SERVER
			}
			break;

			case MSGGAME_PLAYAGAIN:
			#ifndef MAIN_SERVER

			if(((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect >= 20 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 2 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 4) // *NEW - if still in sim ignore this msg
			{
				// save playagain data for later use. while we carry on with our sim.				
				memcpy(&g_PlayAgainbufferBk, (char *) lpBuf, sizeof(char)*4);

				// clear out all other lagging backup data - since this msg would have cleared it anyway
				//memset(&g_ballsSendDataBk, 0, sizeof(BallsSendData_typ)); // leave this out! see [*(!)*]
				//memset(&g_ballPosDataBk, 0, sizeof(ballPosSendData_typ));
				memset(&g_CueBallTipDataCBk, 0, sizeof(CueBallTipSendDataC_typ));
				memset(&g_selectBallDataBk, 0, sizeof(selectBallSendData_typ));
				memset(&g_mouseMoveDataBk, 0, sizeof(mouseMoveSendData_typ));
				memset(&g_FoulAlertbufferBk, 0, sizeof(char)*4);
				memset(&g_MissResetbufferBk, 0, sizeof(char)*4);
				memset(&g_Pot8OnBreakRerackbufferBk, 0, sizeof(char)*4);
				memset(&g_BadBreakRerackbufferBk, 0, sizeof(char)*4);

				//ErrorMessage(g_wnd, "memcpy PlayAgainbuffer");
				break;
			}

			((CGameAppView *)m_pParentWnd)->m_snooker->PlayAgainReceive();

			#endif //!MAIN_SERVER
			break;

			case MSGGAME_UPDATEGAME:
			#ifndef MAIN_SERVER
				((CGameAppView *)m_pParentWnd)->m_snooker->UpdateGameReceive();
			#endif //!MAIN_SERVER
			break;


			case MSGGAME_MISSRESET:
			#ifndef MAIN_SERVER

			if(((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect >= 20 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 2 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 4) // *NEW - if still in sim ignore this msg
			{
				// save MissReset data for later use. while we carry on with our sim.				
				memcpy(&g_MissResetbufferBk, (char *) lpBuf, sizeof(char)*4);

				// clear out all other lagging backup data - since this msg would have cleared it anyway
				//memset(&g_ballsSendDataBk, 0, sizeof(BallsSendData_typ)); // leave this out! see [*(!)*]
				//memset(&g_ballPosDataBk, 0, sizeof(ballPosSendData_typ));
				memset(&g_CueBallTipDataCBk, 0, sizeof(CueBallTipSendDataC_typ));
				memset(&g_selectBallDataBk, 0, sizeof(selectBallSendData_typ));
				memset(&g_mouseMoveDataBk, 0, sizeof(mouseMoveSendData_typ));
				memset(&g_FoulAlertbufferBk, 0, sizeof(char)*4);
				//memset(&g_MissResetbufferBk, 0, sizeof(char)*4);
				memset(&g_PlayAgainbufferBk, 0, sizeof(char)*4);
				memset(&g_Pot8OnBreakRerackbufferBk, 0, sizeof(char)*4);
				memset(&g_BadBreakRerackbufferBk, 0, sizeof(char)*4);

				//ErrorMessage(g_wnd, "memcpy MissResetbuffer");
				break;
			}			

			((CGameAppView *)m_pParentWnd)->m_snooker->MissResetReceive();

			#endif //!MAIN_SERVER
			break;

			case MSGGAME_POT8BALLONBREAKRERACK:
			#ifndef MAIN_SERVER

			if(((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect >= 20 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 2 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 4) // *NEW - if still in sim ignore this msg
			{
				// save Pot8OnBreakRerack data for later use. while we carry on with our sim.
				memcpy(&g_Pot8OnBreakRerackbufferBk, (char *) lpBuf, sizeof(char)*4);
				
				// clear out all other lagging backup data - since this msg would have cleared it anyway
				//memset(&g_ballsSendDataBk, 0, sizeof(BallsSendData_typ)); // leave this out! see [*(!)*]
				//memset(&g_ballPosDataBk, 0, sizeof(ballPosSendData_typ));
				memset(&g_CueBallTipDataCBk, 0, sizeof(CueBallTipSendDataC_typ));
				memset(&g_selectBallDataBk, 0, sizeof(selectBallSendData_typ));
				memset(&g_mouseMoveDataBk, 0, sizeof(mouseMoveSendData_typ));
				memset(&g_FoulAlertbufferBk, 0, sizeof(char)*4);
				memset(&g_MissResetbufferBk, 0, sizeof(char)*4);
				memset(&g_PlayAgainbufferBk, 0, sizeof(char)*4);
				memset(&g_BadBreakRerackbufferBk, 0, sizeof(char)*4);

				//ErrorMessage(g_wnd, "memcpy Pot8OnBreakRerack");

				break;
			}

			((CGameAppView *)m_pParentWnd)->m_snooker->Pot8OnBreakRerackReceive(buffer[3]);

			#endif //!MAIN_SERVER
			break;

			case MSGGAME_BADBREAKRERACK:
			{
			#ifndef MAIN_SERVER

			if(((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect >= 20 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 2 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 4) // *NEW - if still in sim ignore this msg
			{
				// save BadBreakRerack data for later use. while we carry on with our sim.
				memcpy(&g_BadBreakRerackbufferBk, (char *) lpBuf, sizeof(char)*4);
				
				// clear out all other lagging backup data - since this msg would have cleared it anyway
				//memset(&g_ballsSendDataBk, 0, sizeof(BallsSendData_typ)); // leave this out! see [*(!)*]
				//memset(&g_ballPosDataBk, 0, sizeof(ballPosSendData_typ));
				memset(&g_CueBallTipDataCBk, 0, sizeof(CueBallTipSendDataC_typ));
				memset(&g_selectBallDataBk, 0, sizeof(selectBallSendData_typ));
				memset(&g_mouseMoveDataBk, 0, sizeof(mouseMoveSendData_typ));
				memset(&g_FoulAlertbufferBk, 0, sizeof(char)*4);
				memset(&g_MissResetbufferBk, 0, sizeof(char)*4);
				memset(&g_PlayAgainbufferBk, 0, sizeof(char)*4);
				memset(&g_BadBreakRerackbufferBk, 0, sizeof(char)*4);

				//ErrorMessage(g_wnd, "memcpy BadBreakRerack");

				break;
			}

			BOOL bOpponentToBreak = FALSE;
			if(buffer[3] == 1)
				bOpponentToBreak = TRUE;

			((CGameAppView *)m_pParentWnd)->m_snooker->BadBreakRerackReceive(bOpponentToBreak);

			#endif //!MAIN_SERVER
			}
			break;

			case MSGGAME_FOULALERT:
			#ifndef MAIN_SERVER

			if(((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect >= 20 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 2 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 4) // *NEW - if still in sim ignore this msg
			{
				// save FoulAlert data for later use. while we carry on with our sim.				
				memcpy(&g_FoulAlertbufferBk, (char *) lpBuf, sizeof(char)*4);

				//ErrorMessage(g_wnd, "memcpy FoulAlertbuffer");
				break;
			}

			((CGameAppView *)m_pParentWnd)->m_snooker->FoulAlertReceive((char)buffer[3]);

			#endif //!MAIN_SERVER
			break;

			case MSGGAME_TIMEUP:
			{
			#ifndef MAIN_SERVER

			TimeUpSendData *sd;
			sd = (TimeUpSendData_typ *) lpBuf;

			((CGameAppView *)m_pParentWnd)->ReceivePlayerTimeUp(sd->m_shotTime, sd->m_bTimeUp);

			#endif //!MAIN_SERVER
			}
			break;

			case MSGGAME_MAXTIMEUP:
			{
			#ifndef MAIN_SERVER

			if(((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect >= 20 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 2 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 4) // *NEW - if still in sim ignore this msg
			{
				// save MaxTimeUp data for later use. while we carry on with our sim.
				memcpy(&g_MaxTimeUpDataBk, (MaxTimeUpSendData_typ *) lpBuf, sizeof(MaxTimeUpSendData_typ));
				//ErrorMessage(g_wnd, "memcpy MaxTimeUpData");
				break;
			}

			// for clients at specific table only...
			MaxTimeUpSendData *sd;
			sd = (MaxTimeUpSendData_typ *) lpBuf;

			((CGameAppView *)m_pParentWnd)->ReceivePlayerMaxTimeUp(sd->m_playerSockIdx);

			#endif //!MAIN_SERVER
			}
			break;
			
			case MSGGAME_SIM_END: break; //not used as of 16 march 2004
			{
			#ifndef MAIN_SERVER

			SimEndSendData *sd;
			sd = (SimEndSendData_typ *) lpBuf;
			((CGameAppView *)m_pParentWnd)->m_snooker->SimEndReceive(sd->buffer[2], sd->buffer[3]);	
			//((CGameAppView *)m_pParentWnd)->m_snooker->SimEndReceive(sd->m_plrSockIdx, sd->m_bShotEnd);

				
			// Server tells us ALL clients have finished simulation
			//((CGameAppView *)m_pParentWnd)->m_snooker->m_SimEnd = TRUE;
			//((CGameAppView *)m_pParentWnd)->m_snooker->m_simEndCount++;
			
			#endif //!MAIN_SERVER
			}
			break;

			case MSGGAME_BALLS:
			{
				
				#ifndef MAIN_SERVER

				// receive data...
				BallsSendData *sd;
				sd = (BallsSendData_typ *) lpBuf;
				int SockIdx1P, SockIdx1Pb;
				int SockIdx2P, SockIdx2Pb;
				int AOnline, BOnline;


				int tblIdxSender = sd->buffer[2];

				///////////////////////////
				// If we're in the Table //
				///////////////////////////
				if(((CGameAppView *)m_pParentWnd)->m_lobby->m_bInTable)
					{
					int plrTblIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetTableIdxFromPlayer(((CGameAppView *)m_pParentWnd)->m_playerSockIdx);
					
					// Only receive if plrTblIdx matches the one that belongs to the MSGGAME_BALLS message //
					if(tblIdxSender != plrTblIdx) break;
					((CGameAppView *)m_pParentWnd)->SendBallsReceive(sd);

					if((g_build.gameType == GAMETYPE_SNOOKER) && (m_table[plrTblIdx].gameType == 4 || m_table[plrTblIdx].gameType == 5 || m_table[plrTblIdx].gameType == 6))
						{
						// Display Message on Table if a Cannon must be played //
						if(sd->m_poolPots1P == 10) 
							{
							char buf[80];
							sprintf(buf, "A 'Cannon' must be made for the Break to continue after this shot");
							((CGameAppView *)m_pParentWnd)->Message(buf, 40);
							}
						}

					//((CGameAppView *)m_pParentWnd)->SendBallsReceive(sd);
					

					// Debug /
					//char buf[80];
					//sprintf(buf, "g_bLastTurn:%d, sd->m_bTurn1P:%d", g_bLastTurn, sd->m_bTurn1P);
					///AfxMessageBox(buf);

					// If Doubles and Turn Changes, then make next Team Swap Seats //
   					if(m_table[plrTblIdx].doubles)
					if(sd->m_bTurnChanged)
					if(sd->m_bSimEnd)
						{
						if(!sd->m_bTurn1P)
							{
							// Swap Team 1 - If both players are here //
							SockIdx1P = m_table[plrTblIdx].player1SockIdx;
							SockIdx1Pb = m_table[plrTblIdx].player1bSockIdx;
							AOnline = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(SockIdx1P);
							BOnline = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(SockIdx1Pb);
							if(AOnline!=-1 && BOnline!=-1)
								{
								((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(SockIdx1P, SockIdx1P, plrTblIdx, SockIdx1P, SockIdx1P);
								((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(SockIdx1Pb, SockIdx1Pb, plrTblIdx, SockIdx1Pb, SockIdx1Pb);
								((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(-1, -1, plrTblIdx, SockIdx1P, -1);
								((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(SockIdx1Pb, -1, plrTblIdx, -1, -1);
								}
							}
						else
							{
							// Swap Team 2 - If both players are here //
							SockIdx2P = m_table[plrTblIdx].player2SockIdx;
							SockIdx2Pb = m_table[plrTblIdx].player2bSockIdx;
							AOnline = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(SockIdx2P);
							BOnline = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(SockIdx2Pb);
							if(AOnline!=-1 && BOnline!=-1)
								{
								((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(SockIdx2P, SockIdx2P, plrTblIdx, SockIdx2P, SockIdx2P);
								((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(SockIdx2Pb, SockIdx2Pb, plrTblIdx, SockIdx2Pb, SockIdx2Pb);
								((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(-1, -1, plrTblIdx, -1, SockIdx2P);
								((CGameAppView *)m_pParentWnd)->m_lobby->SitStandTable(-1, SockIdx2Pb, plrTblIdx, -1, -1);
								}
							}
						}
					
					}

				///////////////////////////
				// If we're in the Lobby //
				///////////////////////////
				else{	

					// Get table index //
					int i = tblIdxSender;
				

					// If this if the first time we are getting MSGGAME_BALLS message, make sure we have the correct rerack //
					if(g_LobbyTVInfo.bTurn1P == 12)	
						{
						if((g_build.gameType == GAMETYPE_SNOOKER) && (m_table[i].gameType == 4 || m_table[i].gameType == 5 || m_table[i].gameType == 6))
							{	
							((CGameAppView *)m_pParentWnd)->m_snooker->Init(RERACK_BILLIARDS);
							}
						else{ 
							((CGameAppView *)m_pParentWnd)->m_snooker->Init(RERACK_START);
							}
						}	

					if(g_build.gameType == GAMETYPE_SNOOKER) 
						{
						if(m_table[i].gameType == 4 || m_table[i].gameType == 5 || m_table[i].gameType == 6)
							{
							((CGameAppView *)m_pParentWnd)->m_snooker->m_customTable->m_type = CTTYPE_BILLIARDS;
							}
						else{
							((CGameAppView *)m_pParentWnd)->m_snooker->m_customTable->m_type = CTTYPE_SNOOKER;
							}
						}
					
					// Update Lobby TV Data //
					g_LobbyTVInfo.lobbytablewatch = i;
					g_LobbyTVInfo.score1P = sd->m_score1P;
					g_LobbyTVInfo.score2P = sd->m_score2P;
					g_LobbyTVInfo.break1P = sd->m_break1P;
					g_LobbyTVInfo.break2P = sd->m_break2P;
					
					
					if(m_table[i].doubles)
						{
						int plrIdxA;
						int plrIdxB;
						char tbufA[80];
						char tbufB[80];
						// Team 1 ///
						plrIdxA = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(m_table[i].player1SockIdx);
						plrIdxB = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(m_table[i].player1bSockIdx);
					
						if(plrIdxA != -1 && plrIdxB != -1) 
							{
							strcpy(tbufA, m_player[plrIdxA].name);
							if(tbufA[6] != 0)
								{
								tbufA[6] = '.';
								tbufA[7] = '.';
								tbufA[8] = 0;
								}
							strcpy(tbufB, m_player[plrIdxB].name);
							if(tbufB[6] != 0)
								{
								tbufB[6] = '.';
								tbufB[7] = '.';
								tbufB[8] = 0;
								}
							sprintf(g_LobbyTVInfo.name1P, "%s/%s", tbufA, tbufB);
							}
						else strcpy(g_LobbyTVInfo.name1P, "Team 1");

						// Team 2 //
						plrIdxA = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(m_table[i].player2SockIdx);
						plrIdxB = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(m_table[i].player2bSockIdx);
						
						if(plrIdxA != -1 && plrIdxB != -1) 
							{
							strcpy(tbufA, m_player[plrIdxA].name);
							if(tbufA[6] != 0)
								{
								tbufA[6] = '.';
								tbufA[7] = '.';
								tbufA[8] = 0;
								}
							strcpy(tbufB, m_player[plrIdxB].name);
							if(tbufB[6] != 0)
								{
								tbufB[6] = '.';
								tbufB[7] = '.';
								tbufB[8] = 0;
								}
							sprintf(g_LobbyTVInfo.name2P, "%s/%s", tbufA, tbufB);
							}
						else strcpy(g_LobbyTVInfo.name2P, "Team 2");
						}
					else{		
						int plrIdx;
						char tbuf[80];

						// Player 1
						plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(m_table[i].player1SockIdx);
						if(plrIdx != -1) 
							{
							strcpy(tbuf, m_player[plrIdx].name);
							if(tbuf[11] != 0)
								{
								tbuf[11] = '.';
								tbuf[12] = '.';
								tbuf[13] = 0;
								}
							strcpy(g_LobbyTVInfo.name1P, tbuf);
							}
						else strcpy(g_LobbyTVInfo.name1P, "Player 1");

						// Player 2
						plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(m_table[i].player2SockIdx);
						if(plrIdx != -1) 
							{
							strcpy(tbuf, m_player[plrIdx].name);
							if(tbuf[11] != 0)
								{
								tbuf[11] = '.';
								tbuf[12] = '.';
								tbuf[13] = 0;
								}
							strcpy(g_LobbyTVInfo.name2P, tbuf);
							}
						else strcpy(g_LobbyTVInfo.name2P, "Player 2");
						}

					g_LobbyTVInfo.bTurn1P = sd->m_bTurn1P;
	
						
					((CGameAppView *)m_pParentWnd)->SendBallsReceive(sd);
					}
					
			

				#endif //!MAIN_SERVER

			}
			break;

			case MSGGAME_STARTPRESSED:
			{
				

				// update clients
				StartPressedSendData *sd;
				sd = (StartPressedSendData_typ *) lpBuf;
				
				if(((CGameAppView *)m_pParentWnd)->m_lobby->PressedStartReceive(sd->m_plrSockIdx) == TRUE)
				{
					#ifndef MAIN_SERVER
					//((CGameAppView *)m_pParentWnd)->m_snooker->Init(((CGameAppView *)m_pParentWnd)->m_snooker->m_reRackType);
					if(g_build.gameType == GAMETYPE_SNOOKER)
					{
						if(((CGameAppView *)m_pParentWnd)->m_snooker->m_customTable->m_type == CTTYPE_RESPOTBLACK)
							((CGameAppView *)m_pParentWnd)->m_snooker->Init(RERACK_BLACK, sd->m_rndval);
							//((CGameAppView *)m_pParentWnd)->m_snooker->m_reRackType = RERACK_BLACK;
						else if(((CGameAppView *)m_pParentWnd)->m_snooker->m_customTable->m_type == CTTYPE_BILLIARDS)
						{
							// BILLIARDS
							((CGameAppView *)m_pParentWnd)->m_snooker->Init(RERACK_BILLIARDS, sd->m_rndval);
						}
						else
						{
							// SNOOKER, SNOOKER10, SNOOKER6
							((CGameAppView *)m_pParentWnd)->m_snooker->Init(RERACK_START, sd->m_rndval);
						}
					}
					if(g_build.gameType == GAMETYPE_POOL)
					{
						if(((CGameAppView *)m_pParentWnd)->m_snooker->m_customTable->m_type == CTTYPE_8BALL)
							((CGameAppView *)m_pParentWnd)->m_snooker->Init(RERACK_8BALL, sd->m_rndval);
						else if(((CGameAppView *)m_pParentWnd)->m_snooker->m_customTable->m_type == CTTYPE_9BALL)
							((CGameAppView *)m_pParentWnd)->m_snooker->Init(RERACK_9BALL, sd->m_rndval);
						else if(((CGameAppView *)m_pParentWnd)->m_snooker->m_customTable->m_type == CTTYPE_ROTATIONBALL)
							((CGameAppView *)m_pParentWnd)->m_snooker->Init(RERACK_ROTATIONBALL, sd->m_rndval);
						else if(((CGameAppView *)m_pParentWnd)->m_snooker->m_customTable->m_type == CTTYPE_BOWLS)
							((CGameAppView *)m_pParentWnd)->m_snooker->Init(RERACK_BOWLS, sd->m_rndval);
						else if(((CGameAppView *)m_pParentWnd)->m_snooker->m_customTable->m_type == CTTYPE_6BALL)
							((CGameAppView *)m_pParentWnd)->m_snooker->Init(RERACK_6BALL, sd->m_rndval);
						else if(((CGameAppView *)m_pParentWnd)->m_snooker->m_customTable->m_type == CTTYPE_10BALL)
							((CGameAppView *)m_pParentWnd)->m_snooker->Init(RERACK_10BALL, sd->m_rndval);
						else if(((CGameAppView *)m_pParentWnd)->m_snooker->m_customTable->m_type == CTTYPE_UKPOOL)
							((CGameAppView *)m_pParentWnd)->m_snooker->Init(RERACK_UKPOOL, sd->m_rndval);
						else if(((CGameAppView *)m_pParentWnd)->m_snooker->m_customTable->m_type == CTTYPE_BREAK)
							((CGameAppView *)m_pParentWnd)->m_snooker->Init(RERACK_BREAK, sd->m_rndval);
					}


					#endif //!MAIN_SERVER
				}			
			}
			break;

			case MSGGAME_GAMEOVER:
			{
				// update clients
				GameOverSendData *sd;
				sd = (GameOverSendData_typ *) lpBuf;


				Player plr1;
				Player plr2;
				
				// Regardless of being Rated or not, we still need the players current stats for Game Over Messages //
				
				strcpy(plr1.name, sd->m_nameP1);
				strcpy(plr2.name, sd->m_nameP2);

				plr1.rating = sd->m_ratingP1;
				plr1.wins = sd->m_winsP1;
				plr1.losses = sd->m_lossesP1;
				plr1.streak = sd->m_streakP1;
				plr1._p50s = sd->m_p50sP1;
				plr1._p60s = sd->m_p60sP1;
				plr1._p70s = sd->m_p70sP1;
				plr1._p80s = sd->m_p80sP1;
				plr1._p90s = sd->m_p90sP1;
				plr1._p100s = sd->m_p100sP1;
				plr1.pHighBreak = sd->m_pHighBreakP1;
				plr1.pMax147s = sd->m_pMax147sP1;
				plr1._50s = sd->m_50sP1;
				plr1._60s = sd->m_60sP1;
				plr1._70s = sd->m_70sP1;
				plr1._80s = sd->m_80sP1;
				plr1._90s = sd->m_90sP1;
				plr1._100s = sd->m_100sP1;
				plr1.HighBreak = sd->m_HighBreakP1;
				plr1.Max147s = sd->m_Max147sP1;
				plr1.pBioAverageShotTime = sd->m_pBioAverageShotTimeP1;
				plr1.pBioAveragePosition = sd->m_pBioAveragePositionP1;
				plr1.pBioAverageSafety = sd->m_pBioAverageSafetyP1;
				plr1.pBioAverageBreak = sd->m_pBioAverageBreakP1;
				plr1.bioAverageShotTime = sd->m_bioAverageShotTimeP1;
				plr1.bioAveragePosition = sd->m_bioAveragePositionP1;
				plr1.bioAverageSafety = sd->m_bioAverageSafetyP1;
				plr1.bioAverageBreak = sd->m_bioAverageBreakP1;
				plr1.cash = sd->m_cashP1;
				plr1.highestrating = sd->m_highestratingP1;
				plr1.reracks = sd->m_reracksP1;
				plr1.level = sd->m_levelP1;
				plr1.level_progress = sd->m_levelProgressP1;
				plr1.ladderPoints = sd->m_ladderPointsP1;
				plr1.ladderWinsLosses = sd->m_ladderWinsLossesP1;
				plr1.ladderHighBreak = sd->m_ladderHighBreakP1;
				plr1.ladderRank = sd->m_ladderRankP1;

				plr2.rating = sd->m_ratingP2;
				plr2.wins = sd->m_winsP2;
				plr2.losses = sd->m_lossesP2;
				plr2.streak = sd->m_streakP2;
				plr2._p50s = sd->m_p50sP2;
				plr2._p60s = sd->m_p60sP2;
				plr2._p70s = sd->m_p70sP2;
				plr2._p80s = sd->m_p80sP2;
				plr2._p90s = sd->m_p90sP2;
				plr2._p100s = sd->m_p100sP2;
				plr2.pHighBreak = sd->m_pHighBreakP2;
				plr2.pMax147s = sd->m_pMax147sP2;
				plr2._50s = sd->m_50sP2;
				plr2._60s = sd->m_60sP2;
				plr2._70s = sd->m_70sP2;
				plr2._80s = sd->m_80sP2;
				plr2._90s = sd->m_90sP2;
				plr2._100s = sd->m_100sP2;
				plr2.HighBreak = sd->m_HighBreakP2;
				plr2.Max147s = sd->m_Max147sP2;
				plr2.pBioAverageShotTime = sd->m_pBioAverageShotTimeP2;
				plr2.pBioAveragePosition = sd->m_pBioAveragePositionP2;
				plr2.pBioAverageSafety = sd->m_pBioAverageSafetyP2;
				plr2.pBioAverageBreak = sd->m_pBioAverageBreakP2;
				plr2.bioAverageShotTime = sd->m_bioAverageShotTimeP2;
				plr2.bioAveragePosition = sd->m_bioAveragePositionP2;
				plr2.bioAverageSafety = sd->m_bioAverageSafetyP2;
				plr2.bioAverageBreak = sd->m_bioAverageBreakP2;
				plr2.cash = sd->m_cashP2;
				plr2.highestrating = sd->m_highestratingP2;
				plr2.reracks = sd->m_reracksP2;
				plr2.level = sd->m_levelP2;
				plr2.level_progress = sd->m_levelProgressP2;
				plr2.ladderPoints = sd->m_ladderPointsP2;
				plr2.ladderWinsLosses = sd->m_ladderWinsLossesP2;
				plr2.ladderHighBreak = sd->m_ladderHighBreakP2;
				plr2.ladderRank = sd->m_ladderRankP2;
			
				((CGameAppView *)m_pParentWnd)->m_lobby->GameOverReceive(&plr1, &plr2,
												 sd->m_bPlr1Won, sd->m_bRated, sd->m_bInvalidGame, sd->m_tblIdx,
												 sd->m_oldRating1P, sd->m_oldRating2P, sd->m_bRacksStillToPlay, sd->m_EOGmsg);
			}
			break;

			case MSGGAME_CONCEDEPRESSED:
			{
				
				// note: via MainServer
				// update clients
				g_conceded_game = TRUE;
				ConcedePressedSendData *sd;
				sd = (ConcedePressedSendData_typ *) lpBuf;

				
				if(sd->m_plrSockIdx == -1 || sd->m_plrSockIdx == 0) // safety
					break;

				int plrTblIdx = sd->m_tableIdx;

				BOOL bAndExit = false;
				bAndExit = ((CGameAppView *)m_pParentWnd)->m_lobby->PressedConcedeReceive(sd->m_plrSockIdx, sd->m_bAndExit, plrTblIdx);
				
				if(plrTblIdx >=0 && plrTblIdx <= MAX_LOBBY_TABLES)
				{
					m_table[plrTblIdx].bLockSeats = TRUE;

					((CGameAppView *)m_pParentWnd)->m_listTables.EnableItemButton(plrTblIdx*4+2, FALSE);
					((CGameAppView *)m_pParentWnd)->m_listTables.EnableItemButton(plrTblIdx*4+3, FALSE);
				}
				
				// If Player has left the Table (forfeiting the match //
				// Set the StartName of the conceder to NULL //
				// This stops them from being invited back to the table or having any further part in the match they condeded //
				/*
				if(sd->m_bAndClose || bAndExit)
					{
					int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(sd->m_plrSockIdx);
					if(!strcmpi(m_player[plrIdx].name, m_table[plrTblIdx].P1StartName)) m_table[plrTblIdx].P1StartName[0] = 0;
						else  m_table[plrTblIdx].P2StartName[0] = 0;
					}
				*/

				if(sd->m_bAndClose)
				{
					// only player that is exiting
					if(sd->m_plrSockIdx ==  ((CGameAppView *)m_pParentWnd)->m_playerSockIdx)
						((CGameAppView *)m_pParentWnd)->CloseApp();
					else
						((CGameAppView *)m_pParentWnd)->m_lobby->ExitTable(sd->m_plrSockIdx);
				}
				else if(bAndExit)
				//if(bAndExit)
				{
					// only player that is exiting
					if(sd->m_plrSockIdx ==  ((CGameAppView *)m_pParentWnd)->m_playerSockIdx)
						((CGameAppView *)m_pParentWnd)->ExitTable2Lobby(); // <-- should be ok
					
					((CGameAppView *)m_pParentWnd)->m_lobby->ExitTable(sd->m_plrSockIdx);
				}
				else
				{
					//ErrorMessage(m_pParentWnd, "222c Else");
				}
			}
			break;

			case MSGGAME_RERACKPRESSED:
			{
				// note: via MainServer
				//#ifndef MAIN_SERVER <------ should we not be doing this, like above before??? (odin)
				// update clients
				ReRackPressedSendData *sd;
				sd = (ReRackPressedSendData_typ *) lpBuf;
				
				// two modes:
				if(sd->m_bAccepted) // (mode 1) player has accepted the request for a rerack
				{
					((CGameAppView *)m_pParentWnd)->m_lobby->ReRackAccpetedReceive(sd->m_plrSockIdx, sd->m_acceptType,
																				   sd->m_bAbortRaceTo);
				
					}
				else
				{
					((CGameAppView *)m_pParentWnd)->m_lobby->PressedReRackReceive(sd->m_plrSockIdx, sd->m_bAbortRaceTo);
				}
			}
			break;
			
			case MSGGAME_SIM_START:
				{
				#ifndef MAIN_SERVER
				// for client only.
				SimStartSendData *sd;
				sd = (SimStartSendData_typ *) lpBuf;
				
				((CGameAppView *)m_pParentWnd)->m_lobby->HostSimStartReceive(sd->m_plrSockIdx);
				#endif //MAIN_SERVER
				}
			break;

			case MSGGAME_INVITEPRESSED:
				{
				// for one client only.
				InvitePressedSendData *sd;
				sd = (InvitePressedSendData_typ *) lpBuf;
				
				// two modes:
				if(sd->m_bAccepted) // (mode 1) player has accepted an invitation
				{
					// update clients
					#ifndef MAIN_SERVER
					((CGameAppView *)m_pParentWnd)->m_lobby->InvitationAcceptedReceive(sd->m_plrSockIdx, sd->m_invitedPlrSockIdx, sd->m_str, sd->m_initialHostTbl, sd->m_table, sd->m_enterType);
					#endif //MAIN_SERVER
				}
				else // (mode 2) host of table has invited a player
				{
					// for invitee only...
					((CGameAppView *)m_pParentWnd)->m_lobby->PressedInviteReceive(sd->m_plrSockIdx, sd->m_invitedPlrSockIdx, sd->m_name, sd->m_initialHostTbl, sd->m_table, sd->m_enterType, sd->m_reracks, sd->m_totalGames);
				}

				}
			break;

			case MSGGAME_BOOTPRESSED:
			{
				#ifndef MAIN_SERVER
				// note: via GameServer
				// host of table has booted a player
				BootPressedSendData *sd;
				sd = (BootPressedSendData_typ *) lpBuf;
				((CGameAppView *)m_pParentWnd)->m_lobby->PressedBootReceive(sd->m_plrSockIdx, sd->m_bootedPlrSockIdx);
				#endif //MAIN_SERVER
			}
			break;

			case MSGGAME_HOSTCHECK:
			{
				Mmain_log("(HOSTCHECK) Listening Socket - 1");
				// Server has requested a host check!
				/*if(m_pSocketListenCheck) {
										delete m_pSocketListenCheck;
										m_pSocketListenCheck = NULL;
										}
				*/

				//ErrorMessage(m_pParentWnd, "Created");
				m_pSocketListenCheck = new CAsyncSocketEx(this);
				m_pSocketListenCheck->m_type = -1; // dummy listening - just used for initial connection then closed!

				int myPort = ((CGameAppView *)m_pParentWnd)->m_detailsSettingsDlg.m_myPort;
				BOOL success;

				Mmain_log("(HOSTCHECK) Listening Socket - 2");
				success = FALSE;				
				
				if(m_pSocketListenCheck->Create(myPort)) // Create listening check.
					{
					if(m_pSocketListenCheck->Listen())
						{
						// Listening initiated
						success = TRUE;
						} else	{
								char buffer[80];
								sprintf(buffer,"[%s-Listen]",OnConnectErrorString(m_pSocketListenCheck->GetLastError()));
								ErrorMessage(m_pParentWnd, buffer);
								}
					} else	{
							ErrorMessage(m_pParentWnd, OnConnectErrorString(m_pSocketListenCheck->GetLastError()));
							}
				if(success==FALSE) 
				{
					Mmain_log("(HOSTCHECK) Listening Socket - 3");
					ErrorMessage(m_pParentWnd, "Failed to Create Listening Socket");
				}
				
				Mmain_log("(HOSTCHECK) Listening Socket - 4");
				// Tell Server the port which we're listening on.
				if(success==FALSE) myPort=0; // Failed!
				((CGameAppView *)m_pParentWnd)->SendListenCheckListening(myPort, plrSockIdx);
			}
			break;

			case MSGGAME_HOSTCHECKBROADCAST:
			{
				BOOL id = buffer[3];
				int *v = (int *)&buffer[4];

				int plrIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerIdx(*v);
				m_player[plrIdx].bCanHost = id;
				m_player[plrIdx].bCanHostKnown = TRUE;

				

				
				((CGameAppView *)m_pParentWnd)->m_lobby->RefreshPlayerItem();

//				ErrorMessage(m_pParentWnd, "T");
//				if(*v==plrSockIdx) // this is me. - then delete the listening socket.
//					{
//					ErrorMessage(m_pParentWnd, "TT");
				/*
					if(m_pSocketListenCheck) {
											//ErrorMessage(m_pParentWnd, "Deleted");
											delete m_pSocketListenCheck;
											m_pSocketListenCheck = NULL;
											}
					*/
//					}
				//ErrorMessage(m_pParentWnd, "MSGGAME_HOSTCHECKBROADCAST");

				
				//g_extendedPlayerManagerLocal->SendPackage(g_extendedPlayerDataLocal, EPD_PERSONALLAYOUT, "ichidan");
			}
			break;
			case MSGGAME_KICK:
			{
				// buffer[2] = 0 (kick), 1 (ban)
				//if(!buffer[2]) ((CGameAppView *)m_pParentWnd)->m_kicked = 1;
				//else ((CGameAppView *)m_pParentWnd)->m_kicked = 2;
				((CGameAppView *)m_pParentWnd)->m_kicked = 1+buffer[2];

				if(buffer[3])  // Reason/Banned
				{
				char str[MAX_CHARS];
				strcpy(((CGameAppView *)m_pParentWnd)->m_kreason,
						(char *)&buffer[4]);

				switch(buffer[2])
					{
					case 0:sprintf(str,"You were kicked by a moderator for: %s", &buffer[4]);
						   break;
					case 1:sprintf(str,"You were banned by a moderator for: %s", &buffer[4]);
						   break;
					case 2:sprintf(str,"You have been walled by a moderator for: %s", &buffer[4]);
						   break;
				//	case 3:sprintf(str,"Your computer has been denied by a moderator for: %s", &buffer[4]);
				//		   break;

					}

				((CGameAppView *)m_pParentWnd)->Message(str, 40);
				}
				else {
					 ((CGameAppView *)m_pParentWnd)->m_kreason[0] = 0;
					 //if(!buffer[2])
					 //((CGameAppView *)m_pParentWnd)->Message("You were kicked by a moderator", 40);
					 //else ((CGameAppView *)m_pParentWnd)->Message("You were banned by a moderator", 40);
					#ifdef THE_POOL_CLUB
	 				switch(buffer[2])
						{
						case 0:((CGameAppView *)m_pParentWnd)->Message("You were kicked by a moderator", 40);
								break;
						case 1:((CGameAppView *)m_pParentWnd)->Message("You were banned by a moderator", 40);
								break;
						case 2: ((CGameAppView *)m_pParentWnd)->Message("You have been walled. Please contact support@thepoolclub.com", 40);
								break;
						case 3: ((CGameAppView *)m_pParentWnd)->Message("Your computer has been denied from iPool. Please contact support@thepoolclub.com", 40);
								break;
						case 4: ((CGameAppView *)m_pParentWnd)->Message("Your account is being accessed/you have been disconnected.\r\nIf you haven't allowed another person to use your account then please contact support@thepoolclub.com", 40);
								break;
						}
					#else
					switch(buffer[2])
						{
						case 0:((CGameAppView *)m_pParentWnd)->Message("You were kicked by a moderator", 40);
								break;
						case 1:((CGameAppView *)m_pParentWnd)->Message("You were banned by a moderator", 40);
								break;
						case 2: ((CGameAppView *)m_pParentWnd)->Message("You have been walled. Please contact support@thesnookerclub.com", 40);
								break;
						case 3: ((CGameAppView *)m_pParentWnd)->Message("Your computer has been denied from iSnooker. Please contact support@thesnookerclub.com", 40);
								break;
						case 4: ((CGameAppView *)m_pParentWnd)->Message("Your account is being accessed/you have been disconnected.\r\nIf you haven't allowed another person to use your account then please contact support@thesnookerclub.com", 40);
								break;
						}
					#endif
										 }
				((CGameAppView *)m_pParentWnd)->SendKickRequest();
			}
			break;

			case MSGGAME_GAMEOVERFORALL:
			{
				// update clients
				GameOverForAllSendData *sd;
				sd = (GameOverForAllSendData_typ *) lpBuf;

				((CGameAppView *)m_pParentWnd)->m_lobby->GameOverForAllReceive(sd->m_tblIdx, sd->m_gameOver, sd->m_bForceRacksEnd, sd->m_bMatchTimeUp);
			}
			break;

			/*case MSGGAME_EPD:
				{
					// Server's job (whether game or main) is to reroute these messages to their destination.
					((CGameAppView *)m_pParentWnd)->m_lobby->ReceiveEPD(buffer);
					
				}
			break;*/

/*			case MSGGAME_PLAYERID:
				{
					char str[80];
					int *v;
					int idx;
					v = (int *) &buffer[4];
					idx = ((CGameAppView *)m_pParentWnd)->m_lobby->FindPlayerIdxFromId(*v);

					if(idx==-1)
					sprintf(str,"??? (#%d)", *v);		
					else sprintf(str,"%s (#%d)", m_player[idx].name,*v);
					((CGameAppView *)m_pParentWnd)->Message(str, 40);
					
				}
			break;*/

			case MSGGAME_GAMEPING:
			{
				// note: via GameServer
				GamePingSendData *sd;
				sd = (GamePingSendData_typ *) lpBuf;
				((CGameAppView *)m_pParentWnd)->GamePingReceive(sd->m_plrSockIdx);
			}
			break;

			case MSGGAME_CLEARTABLE:
			{
				// note: via MainServer for all including server's own client
				ClearTableSendData *sd;
				sd = (ClearTableSendData_typ *) lpBuf;
				((CGameAppView *)m_pParentWnd)->m_lobby->ClearTableReceive(sd->m_tblIdx);
			}
			break;

			case MSGGAME_ALLHIGHBREAKS:
			{
				// note: via MainServer for all including server's own client
				AllHighBreaksSendData *sd;
				sd = (AllHighBreaksSendData_typ *) lpBuf;


				// If we are playing Billiards, then we need to set a flag //

				//int plrTblIdx = ((CGameAppView *)m_pParentWnd)->m_lobby->GetTableIdxFromPlayer(sd->m_plrSockIdx);
				

				// if we have a new high break on Semi-Pro
				//if(m_table[plrTblIdx].gameType != 4) // Can't be Billiards //
				if(sd->m_high4 == 1)
				if(sd->m_highBreak0 > 0)
				//if(sd->m_highBreak0 > ((CGameAppView *)m_pParentWnd)->m_sLobbyHighBreak[0])
				if(sd->m_cueError == 1)
				{
					#ifdef THE_POOL_CLUB
					char buffer[MAX_CHARS];
					sprintf(buffer, "ANNOUNCEMENT --> %s has the most Run-Outs today of '%d' on 8-Ball!", sd->m_highBreakName0, sd->m_highBreak0);
					((CGameAppView *)m_pParentWnd)->Message(buffer, 60); // personal message
					#else
					char buffer[MAX_CHARS];
					sprintf(buffer, "ANNOUNCEMENT --> %s has the highest break today of '%d' on Semi-Pro!", sd->m_highBreakName0, sd->m_highBreak0);
					((CGameAppView *)m_pParentWnd)->Message(buffer, 60); // personal message
					#endif
				}

				// if we have a new high break on Pro
				//if(m_table[plrTblIdx].gameType != 4) // Can't be Billiards //
				if(sd->m_high4 == 1)  
				if(sd->m_highPBreak0 > 0)
				//if(sd->m_highPBreak0 > ((CGameAppView *)m_pParentWnd)->m_sLobbyPHighBreak[0])
				if(sd->m_cueError == 2)
				{
					#ifdef THE_POOL_CLUB
					char buffer[MAX_CHARS];
					sprintf(buffer, "ANNOUNCEMENT --> %s has the most Run-Outs today of '%d' on 9-Ball!", sd->m_highPBreakName0, sd->m_highPBreak0);
					((CGameAppView *)m_pParentWnd)->Message(buffer, 60); // personal message
					#else
					char buffer[MAX_CHARS];
					sprintf(buffer, "ANNOUNCEMENT --> %s has the highest break today of '%d' on Pro!", sd->m_highPBreakName0, sd->m_highPBreak0);
					((CGameAppView *)m_pParentWnd)->Message(buffer, 60); // personal message
					#endif
				}

				

				// if we have a new high break on English Pool or Billiards
				if(sd->m_high4 == 1) 
				if(sd->m_highUKBreak0 > 0)
				//if(sd->m_highPBreak0 > ((CGameAppView *)m_pParentWnd)->m_sLobbyPHighBreak[0])
				if(sd->m_cueError == 3) 
				{
					#ifdef THE_POOL_CLUB
					char buffer[MAX_CHARS];
					sprintf(buffer, "ANNOUNCEMENT --> %s has the most Run-Outs today of '%d' on English Pool!", sd->m_highUKBreakName0, sd->m_highUKBreak0);
					((CGameAppView *)m_pParentWnd)->Message(buffer, 60); // personal message
					#else
					char buffer[MAX_CHARS];
					sprintf(buffer, "ANNOUNCEMENT --> %s has the highest break today of '%d' on Billiards!", sd->m_highUKBreakName0, sd->m_highUKBreak0);
					((CGameAppView *)m_pParentWnd)->Message(buffer, 60); // personal message
					#endif
				}




				// This Should Already be Updated (Client Side At Least, and not Vital on Server
				// update high-breaks table



				((CGameAppView *)m_pParentWnd)->m_sLobbyHighBreakName[0] = sd->m_highBreakName0;
				((CGameAppView *)m_pParentWnd)->m_sLobbyHighBreakName[1] = sd->m_highBreakName1;
				((CGameAppView *)m_pParentWnd)->m_sLobbyHighBreakName[2] = sd->m_highBreakName2;
				((CGameAppView *)m_pParentWnd)->m_sLobbyHighBreak[0] = sd->m_highBreak0;
				((CGameAppView *)m_pParentWnd)->m_sLobbyHighBreak[1] = sd->m_highBreak1;
				((CGameAppView *)m_pParentWnd)->m_sLobbyHighBreak[2] = sd->m_highBreak2;

				((CGameAppView *)m_pParentWnd)->m_sLobbyPHighBreakName[0] = sd->m_highPBreakName0;
				((CGameAppView *)m_pParentWnd)->m_sLobbyPHighBreakName[1] = sd->m_highPBreakName1;
				((CGameAppView *)m_pParentWnd)->m_sLobbyPHighBreakName[2] = sd->m_highPBreakName2;
				((CGameAppView *)m_pParentWnd)->m_sLobbyPHighBreak[0] = sd->m_highPBreak0;
				((CGameAppView *)m_pParentWnd)->m_sLobbyPHighBreak[1] = sd->m_highPBreak1;
				((CGameAppView *)m_pParentWnd)->m_sLobbyPHighBreak[2] = sd->m_highPBreak2;
				
				((CGameAppView *)m_pParentWnd)->m_sLobbyUKHighBreakName[0] = sd->m_highUKBreakName0;
				((CGameAppView *)m_pParentWnd)->m_sLobbyUKHighBreakName[1] = sd->m_highUKBreakName1;
				((CGameAppView *)m_pParentWnd)->m_sLobbyUKHighBreakName[2] = sd->m_highUKBreakName2;
				((CGameAppView *)m_pParentWnd)->m_sLobbyUKHighBreak[0] = sd->m_highUKBreak0;
				((CGameAppView *)m_pParentWnd)->m_sLobbyUKHighBreak[1] = sd->m_highUKBreak1;
				((CGameAppView *)m_pParentWnd)->m_sLobbyUKHighBreak[2] = sd->m_highUKBreak2;
				
				
				((CGameAppView *)m_pParentWnd)->m_bUpdateHighBreaks = TRUE;
			}
			break;

			case MSGGAME_PLAYERCHANGEDATA:
			{
				// note: via MainServer for all including server's own client
				((CGameAppView *)m_pParentWnd)->ReceivePlayerChangeData((char *)lpBuf,nBufLen);
			}
			break;

			case MSGGAME_TABLECHANGEDATA:
			{
				// note: via MainServer for all including server's own client
				((CGameAppView *)m_pParentWnd)->ReceiveTableChangeData((char *)lpBuf,nBufLen);
			}
			break;

			case MSGGAME_UPDATELOBBYTVCLIENT:
			{
				// for Client Only //
				// Update Table Watch Variables for this table //
				g_LobbyTVInfo.lobbytablewatch = buffer[2];
				g_gameAppView->UpdateLobbyTableWatch(buffer[2]);

			}
			break;

			case MSGGAME_REFRESHLEADERBOARDS:
			{
			#ifndef MAIN_SERVER
				// for Clients Only //
				// Tell Clients to Refresh the LeaderBoard Pages (if client is in lobby)//
				/*
				if(((CGameAppView *)m_pParentWnd)->m_lobby->m_bInTable == FALSE)
					{
					((CGameAppView *)m_pParentWnd)->m_htmlViewLeaderBoardA.Refresh2(REFRESH_COMPLETELY);
					((CGameAppView *)m_pParentWnd)->m_htmlViewLeaderBoardB.Refresh2(REFRESH_COMPLETELY);
					}
				*/
			#endif

			}
			break;

			case MSGGAME_ABUSELST:
			{
			#ifndef MAIN_SERVER
				// for Clients Only //
				// Tell Clients the Current List of Abusive Words //
				AbuseLstSendData *sd;
				sd = (AbuseLstSendData_typ *) lpBuf;
				// Copy over Abuse //
				g_AbuseLstData.abuse_lst_n = sd->abuse_lst_n;
				for(int i=0;i<g_AbuseLstData.abuse_lst_n;i++) strcpy(g_AbuseLstData.abuse_lst[i], sd->abuse_lst[i]);
			#endif

			}
			break;		


			case MSGGAME_REQUESTPLAYERBIO:
			{ // for client only
				RequestPlayerBioSendData *sd;
				sd = (RequestPlayerBioSendData_typ *) lpBuf;								

				((CGameAppView *)m_pParentWnd)->ReceiveRequestPlayerBio(sd->m_returnPlrSockIdx);
			}
			break;

			case MSGGAME_RETURNPLAYERBIO:
			{ // for client only
				ReturnPlayerBioSendData *sd;
				sd = (ReturnPlayerBioSendData_typ *) lpBuf;								
				((CGameAppView *)m_pParentWnd)->ReceiveReturnPlayerBio(sd);
			}
			break;

			case MSGGAME_PLAYERLOSTCONN:
				{ 
				// Clients Only Get Here //
							
				#ifndef MAIN_SERVER
				// note: sent directly from Server to Client OR Clients, except Server's own client!
				PLCSendData *sd;
				sd = (PLCSendData_typ *) lpBuf;
				((CGameAppView *)m_pParentWnd)->m_lobby->ReceivePlayerLostConnection(sd->m_playerSockIdx, sd->m_name, sd->m_tblIdx);
				#endif


			}
			break;
			case MSGGAME_PLAYERCONN:
			{
				// note: via MainServer for all including server's own client
				PCSendData *sd;
				sd = (PCSendData_typ *) lpBuf;
				((CGameAppView *)m_pParentWnd)->m_lobby->ReceivePlayerConnection(sd->m_playerSockIdx, sd->m_gameOver, sd->m_gameOverIWon, sd->m_playerSockIdx2);
				// This is important to update for the returning player, so Referee Code is processed after the first shot //
				
				// If I am the returning player, make sure we don't do Referee Code since we haven't selected the ball //
				if(sd->m_playerSockIdx == ((CGameAppView *)m_pParentWnd)->m_playerSockIdx)
				{
					if(sd->m_cueBallSelect != 0) ((CGameAppView *)m_pParentWnd)->m_snooker->m_bSeenSelectBall = FALSE;
				}
				//if(sd->m_cueBallSelect != 0) ((CGameAppView *)m_pParentWnd)->m_snooker->m_bSeenSelectBall = TRUE;
			}
			break;

			case MSGGAME_PLAYERTEXTFORMAT:
			{
				// note: via MainServer for all including server's own client
				TextFormatSendData *sd;
				sd = (TextFormatSendData_typ *) lpBuf;
				
				((CGameAppView *)m_pParentWnd)->m_lobby->ReceivePlayerTextFormat(sd->m_plrSockIdx, sd->m_name, sd->m_textFormat);
			}
			break;

			case MSGGAME_STING:
			{
				#ifndef MAIN_SERVER
				// do stinger code
				if(StingerSave())
				{

				}
				#endif //MAIN_SERVER
			}
			break;

			case MSGGAME_CRASH:
			{
				#ifndef MAIN_SERVER
				// do crash code eludes people who have been bad to think their app crashed for some reason other than dilberate crashing.
				while(1)
				{
				char buffer[80];				
								
				sprintf(buffer,"Can't find player '%s'",""); // blank name
				MessageBox(0,buffer,"Fatal Error",MB_OK);
				}
				#endif //MAIN_SERVER
			}
			break;

			case MSGGAME_CRASHTEMPER:
			{
				#ifndef MAIN_SERVER
				// do crash code eludes people who have been bad to think their app crashed for some reason other than dilberate crashing.
				while(1)
				{
				AfxMessageBox("Hacking!!!, Your Username, I.P and System ID has been Logged");
				}
				#endif //MAIN_SERVER
			}
			break;

			case MSGGAME_PLAYERTARGETDATA:
			{
				#ifndef MAIN_SERVER
				// note: via MainServer for all including server's own client
				
				if(((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect >= 20 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 2 || ((CGameAppView *)m_pParentWnd)->m_snooker->m_cueBallSelect == 4) // *NEW - if still in sim ignore this msg
				{
					// save playerTargetData for later use. while we carry on with our sim.
					memcpy(&g_playerTargetDataBk, (PlayerTargetSendData_typ *) lpBuf, sizeof(PlayerTargetSendData_typ));
					//ErrorMessage(g_wnd, "memcpy playerTargetData");
					break;
				}

				PlayerTargetSendData *sd;
				sd = (PlayerTargetSendData_typ *) lpBuf;				

				((CGameAppView *)m_pParentWnd)->m_snooker->ReceivePlayerTargetData(sd->m_targetPocket, sd->m_targetBall, sd->m_bTargetSuccess);
				#endif //MAIN_SERVER
			}
			break;

			case MSGGAME_TMATCHTIMERLEFT:
			{
				#ifndef MAIN_SERVER
				// note: sent directly from Server to Client OR Clients, except Server's own client!
				TMatchTimerLeftSendData *sd;
				sd = (TMatchTimerLeftSendData_typ *) lpBuf;

				((CGameAppView *)m_pParentWnd)->m_lobby->ReceiveTMatchTimerLeft(sd->m_timeRemaining, sd->m_type, sd->m_str, sd->m_plrSockIdx, sd->m_tblIdx);
				
				#endif //MAIN_SERVER
			}
			break;

			case MSGGAME_TEXTMSG:
			{
				// note: sent directly from Server to Client OR Clients, except Server's own client!
				TextMsgSendData *sd;
				sd = (TextMsgSendData_typ *) lpBuf;

				((CGameAppView *)m_pParentWnd)->m_lobby->ReceiveTextMsg(sd->m_str, sd->m_type, sd->m_plrSockIdx, sd->m_tblIdx, sd->m_lobbyAndTblIdx);
			}
			break;

			case MSGGAME_SERVERTIME:
			{

				serverTimeSendData *sd;
				sd = (serverTimeSendData_typ *) lpBuf;

				((CGameAppView *)m_pParentWnd)->ReceiveServerTime(sd);
			}
			break;

		} // end switch
	} // end else
	return FALSE;
}


void CShellEx::ChatSend(CString& strText)
{
	ChatSend(strText.GetBuffer(0));
}

void CShellEx::ChatSend(char *str)
{
	unsigned char buffer[MAX_SENDEDITCHARS];
	
	


	if(str[0]=='/')
	{
		sprintf((char *)&buffer[2],"%s",str); // Now clients only send their string.
		buffer[0] = MSGCODE_GAME;			// Goes to new (CShellEx)
		buffer[1] = MSGGAME_SERVERMESSAGE; // unused. (used as an id tag when server broadcasts out your message - server knows all id's)
		// buffer[2] is avail. for adding extra data.
	}
	else 
	{
		sprintf((char *)&buffer[5],"%s",str); // Now clients only send their string.

		buffer[0] = MSGCODE_TEXTCOLOR; // Goes to base (CShell)
		buffer[1] = 0; // unused. (used as an id tag when server broadcasts out your message - server knows all id's)

		// Include the Text Color
		int col = g_gameAppView->m_TextColorChangerDlg.m_textColor;
		buffer[2] = col&0xff;
		buffer[3] = (col>>8)&0xff;
		buffer[4] = (col>>16)&0xff;
	}


	
	Client->SendToQueue(buffer, strlen((char *)&buffer[5])+6); // +1 for Ter, +1 for Code at beginning +1 for sender gap + 3 for text color

//	CShell::ChatSend(str);
}

void CShellEx::OnConnect(CAsyncSocketEx *pSocket, int nErrorCode)
{
	((CGameAppView *)m_pParentWnd)->OnConnect(pSocket, nErrorCode);
}


