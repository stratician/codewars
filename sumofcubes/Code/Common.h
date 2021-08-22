///////////////////////////////////////////////////////////////////////////////////////////////////
// (Common.h)
//

#ifndef __COMMON_H__
#define __COMMON_H__

#define GAME_ITTR					11.0f//11.0f//16.0f//16.0f	// multiple processes per frame

#define SQUAREROOT3					1.7320508

#define SQR(n)						((n)*(n))

// flags
#define SetFlag(var, flags)			((var) |= (flags))
#define ClearFlag(var, flags)		((var) &= (~(flags)))
#define ToggleFlag(var, flags)		((var) ^= (flags))
#define TestFlag(var, flags)		((var)&(flags))

typedef unsigned int Flags;

#define	GAMEAPPVIEW					( (CGameAppView *)m_parent )
#define	GAMEAPPVIEW2				( (CGameAppView *)m_pParent->m_parent )

#define WORLD_SCALE_X				3.45f
#define WORLD_SCALE_Y				3.45f
#define WORLD_OFFSET_X				591.0f
#define WORLD_OFFSET_Y				297.0f
#define WORLD_X(x)					( (x-WORLD_OFFSET_X)*WORLD_SCALE_X )
#define WORLD_Y(y)					( (y-WORLD_OFFSET_Y)*WORLD_SCALE_Y )

//Note: RGB(r, g, b) already exists as standard windows macro
#define RGBA(r, g ,b, a)  ( (DWORD) ((BYTE) (r)) | (DWORD) (((BYTE) (g))<<8) | (DWORD) (((BYTE) (b))<<16) | (DWORD) (((BYTE) (a))<<24))
//Note: GetRValue(...), GetGValue(...), GetBValue(...) already exist as standard windows macros
#define GetAValue(rgba)   ((BYTE) ((rgba) >> 24)) 

#define FLOAT_1000000				1000000.0f
#define FIXFLOAT(f)				(f)
//#define FIXFLOAT(f)					(    (  float( ( ((long)((f) * FLOAT_1000000)) >> 4 ) << 4 )  ) / FLOAT_1000000    )
#define VFIXFLOAT(V)				CVector( FIXFLOAT(V.x), FIXFLOAT(V.y), FIXFLOAT(V.z) )

#define CUEERROR_DEFAULTON		// rem this is u want the semi-pro level to be off defaultly
//#define SHOW_SIDESPIN			// debug stats [rem this!!!]
//#define MATCH_TABLE
//#define SHAKE_LINE			// debug stats [rem this!!!]
//#define SHOW_ERRORVALUE		// debug stats [rem this!!!]
#define CLEAR_DECLINE_INVITES
#define LOAD_QMARK
#define LOAD_NEXTBALLLIST

#define MAX_CHARS				512 // this is maximum message length that we display

#define MAX_LOBBY_PLAYERS		128//160//32

#define MAX_TABLE_PLAYERS		32//64 [Tourney Lobbies] //16

#define SEND_QUEUE_SIZE			48//16

#define MAX_LOBBY_TABLES		50

/*
#define NET_FTP_URL "ftp.thesnookerclub.com"
#define NET_FTP_USER "thesnook"
#define NET_FTP_PASS "haj5kJC"
#define NET_FTP_DIR "mainwebsite_html"
*/

#define NET_HTTP_URL "http://www.thesnookerclub.com"

/*
#define NET_FTP_URL "ftp.memirsoftware.com"
#define NET_FTP_USER "memirsoftware"
#define NET_FTP_PASS "sakura1"
#define NET_FTP_DIR "www"

#define NET_HTTP_URL "http://www.memirsoftware.com"
*/

#define DAYSTOKEEPACCOUNT				60	// Hold accounts till 60days after they've expired

extern float g_ballRadius;
extern float g_ballRadiusFactor;

extern void TextOutEllipsis(CDC *pDC, int x, int y, const CString& str, int cx);
extern void ErrorMessage(CWnd *pParent, char *errorMessage); // Note: use this function as a replacement to MessageBox(...

// [CExaminer] //

class CExaminer {
char *m_buf;
int m_len;
int m_offset;
char *m_tmpstr;
public:
CExaminer(char *buf, int len);
~CExaminer();
void Restart(void);
CExaminer *Examine(char *start, char *end);
CExaminer *ExamineEntire(char *start, char *end);
char *GetString(void);
int GetLength(void);
int GetInt(void);
};

#endif // __COMMON_H__
