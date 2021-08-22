#include "stdafx.h"


#include "../Code/Common.h"
#include "Replay.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "ReplayDlg.h"

extern BYTE Mmain_filedump(void *dest,char *fname); // Quick & Easy File->Data Dumper! returns 0 for file error. (i.e no dump)
extern signed long Mmain_filelen(char *fname); // Quick & Easy File->Length. - returns -1 for file error.

char DaTxT_Symbol(int n)
{
	if(n<26) return 'A'+n; // 0...25 = A to Z
	if(n<52) return 'a'+n-26; // 26..51 = a to z
	if(n<62) return '0'+n-52; // 52..61 = 0 to 9
	if(n==62) return ','; // 62 = , (Comma)
	if(n==63) return '.'; // 63 = . (Full Stop)
	return '?'; // Bad Symbol
}

int DaTxT_Number(char c)
{
	if(c>='A' && c<='Z') return c-'A'; // 0...25 = A to Z
	if(c>='a' && c<='z') return c-'a'+26; // 26..51 = a to z
	if(c>='0' && c<='9') return c-'0'+52; // 52..61 = 0 to 9
	if(c==',') return 62; // 62 = , (Comma)
	if(c=='.') return 63; // 63 = . (Full Stop)
	return -1; // Bad Symbol
}

char *DaTxT_4(char *ptr, BYTE *data)
{ // convert 4bytes (32-bits) into 6 Chars by 6 (36-bits)
	
	// AAAAAA
	// BBBBAA
	// CCBBBB
	// CCCCCC

	// First 24bits
	ptr[0] = DaTxT_Symbol(data[0]&63); // lo 6 of 0.
	ptr[1] = DaTxT_Symbol((data[0]>>6) | ((data[1]&15)<<2)); // up 2 of 0, + low 4 of 1.
	ptr[2] = DaTxT_Symbol((data[1]>>4) | ((data[2]&3)<<4));  // up 4 of 1, + low 2 of 2
	ptr[3] = DaTxT_Symbol(data[2]>>2); // up 6 of 3.

	// Last 8bits
	ptr[4] = DaTxT_Symbol(data[3]&63); // lo 6 of 4.
	ptr[5] = DaTxT_Symbol(data[3]>>6); // up 2 of 4.

	return &ptr[6];
}

BYTE *RAW_4(BYTE *ptr, DWORD *data)
{
	DWORD *ptr4;
	ptr4 = (DWORD *) ptr;
	*ptr4 = *data;

	return &ptr[4];
}

char *DaTxT_N4(char *ptr, BYTE *data)
{ // convert 6 Chars into 4 bytes (36-bits)

	// 11000000
	// 22221111
	// 33333322
	// 55444444

	data[0] = DaTxT_Number(ptr[0]) | (DaTxT_Number(ptr[1])<<6);
	data[1] = (DaTxT_Number(ptr[1])>>2) | (DaTxT_Number(ptr[2])<<4);
	data[2] = (DaTxT_Number(ptr[2])>>4) | (DaTxT_Number(ptr[3])<<2);
	data[3] = DaTxT_Number(ptr[4]) | (DaTxT_Number(ptr[5])<<6);

	return &ptr[6];
}

BYTE *RAW_N4(BYTE *ptr, DWORD *data)
{
	DWORD *ptr4;
	ptr4 = (DWORD *) ptr;
	*data = *ptr4;

	return &ptr[4];
}

DWORD DaTxT_CRC(char *str) // Generates a DaTxT CRC code.
{ // Used for producing new CRCS
	char *ptr;
	int length;
	int start;
	DWORD crc;
	crc = 0;
	ptr = str;
	length = 0;
	while(*ptr!=0)
	{
	if(*ptr==']') break;
	ptr++;
	length++;
	}

	start = 0;
	ptr = str;
	while(length>6)
	{
	if(*ptr=='[') start = 1;

	if(start) crc+=DaTxT_Number(*ptr);
	ptr++;
	length--;
	}
	return crc; // if length is too small this will return 0
}

DWORD DaTxT_GetCRCCode(char *str) // Returns value of what DaTxT CRCCode is saying.
{ // Just Retrieves CRC value in the DaTxT string. (i.e. last 6 chars)
	char *ptr;
	int length;
	DWORD crc;
	crc = 0;
	ptr = str;
	length = 0;
	while(*ptr!=0)
	{
	if(*ptr==']') break;
	ptr++;
	length++;
	}
	length-=6;

	if(length<0) return 0xffffffff; // if length is too small this will return 0xffffffff an impossible CRC value
		
	DaTxT_N4(&str[length], (BYTE *)&crc);
	return crc;
}

int DaTxT_Check(char *str)
{
	DWORD crc1;
	DWORD crc2;
	
	crc1 = DaTxT_CRC(str);
	crc2 = DaTxT_GetCRCCode(str);
	if(crc1!=crc2) return 1; // Failed CRC Check

	return 0; // Ok Passed All Tests.
}

// [CReplayStrike]
char *CReplayStrike::GenerateDaTxTCode(char *ptr)
{
	*ptr++ = '-'; // should be unused!!!
	return ptr;
}

// [CReplayStrike]
char *CReplayStrike::LoadDaTxTCode(char *ptr)
{
	ptr++;

	return ptr;
}

// [CReplayStrike]
void CReplayStrike::GenerateCode(BYTE *ptr, DWORD *size)
{
	*size = 0;
}

// [CReplayStrike]
BYTE *CReplayStrike::LoadCode(BYTE *ptr)
{
	ptr++;
	return ptr;
}



// [CReplayStrikeReRack]
char *CReplayStrikeReRack::GenerateDaTxTCode(char *ptr)
{
	DWORD v;

	*ptr++ = DaTxT_Symbol(m_type);
	
	*ptr++ = DaTxT_Symbol(m_reRackType);

	#ifdef THE_POOL_CLUB
	*ptr++ = DaTxT_Symbol(m_customTableType);
	#endif //THE_POOL_CLUB
	
	v = m_randseed;
	ptr = DaTxT_4(ptr, (BYTE *)&v);
	return ptr;
}

// [CReplayStrikeReRack]
char *CReplayStrikeReRack::LoadDaTxTCode(char *ptr)
{
	DWORD v;

	m_type = DaTxT_Number(*ptr++);
	m_reRackType = DaTxT_Number(*ptr++);
	
	#ifdef THE_POOL_CLUB
	m_customTableType = DaTxT_Number(*ptr++);
	#endif //THE_POOL_CLUB
		
	ptr = DaTxT_N4(ptr, (BYTE *)&v);
	m_randseed = v;
	return ptr;
}

// [CReplayStrike]
void CReplayStrikeReRack::GenerateCode(BYTE *ptr, DWORD *size)
{
	DWORD v;
	BYTE *start = ptr;


	*ptr++ = m_type;
	
	*ptr++ = m_reRackType;

	#ifdef THE_POOL_CLUB
	*ptr++ = m_customTableType;
	#endif //THE_POOL_CLUB

	v = m_randseed;
	ptr = RAW_4(ptr, (DWORD *)&v);
	*size = (int) (ptr - start);
}

// [CReplayStrike]
BYTE *CReplayStrikeReRack::LoadCode(BYTE *ptr)
{
	DWORD v;

	m_type = *ptr++;
	m_reRackType = *ptr++;

	#ifdef THE_POOL_CLUB
	m_customTableType = *ptr++;
	#endif //THE_POOL_CLUB
	
	ptr = RAW_N4(ptr, (DWORD *)&v);

	m_randseed = v;
	return ptr;
}


// [CReplayStrikeBall]
void CReplayStrikeBall::SetStart(float x,float y,float z)
{
	m_s[0] = x;
	m_s[1] = y;
	m_s[2] = z;
}

void CReplayStrikeBall::SetEnd(float x,float y,float z)
{
	m_e[0] = x;
	m_e[1] = y;
	m_e[2] = z;
}

void CReplayStrikeBall::SetCuetip(float x,float y,float z)
{
	m_cuetip[0] = x;
	m_cuetip[1] = y;
	m_cuetip[2] = z;
}

void CReplayStrikeBall::SetBallMousePos(float x,float y,float z)
{
	m_ballMousePos[0] = x;
	m_ballMousePos[1] = y;
	m_ballMousePos[2] = z;
}

char *CReplayStrikeBall::GenerateDaTxTCode(char *ptr)
{
	DWORD v;

	*ptr++ = DaTxT_Symbol(m_type);

	ptr = DaTxT_4(ptr, (BYTE *)&m_s[0]);
	ptr = DaTxT_4(ptr, (BYTE *)&m_s[1]);
	ptr = DaTxT_4(ptr, (BYTE *)&m_s[2]);
	ptr = DaTxT_4(ptr, (BYTE *)&m_e[0]);
	ptr = DaTxT_4(ptr, (BYTE *)&m_e[1]);
	ptr = DaTxT_4(ptr, (BYTE *)&m_e[2]);
	ptr = DaTxT_4(ptr, (BYTE *)&m_cuetip[0]);
	ptr = DaTxT_4(ptr, (BYTE *)&m_cuetip[1]);
	ptr = DaTxT_4(ptr, (BYTE *)&m_cuetip[2]);
	*ptr++ = DaTxT_Symbol(m_selectBall);
	ptr = DaTxT_4(ptr, (BYTE *)&m_strikeRnd);
	*ptr++ = DaTxT_Symbol(m_ballMouseOn);
	ptr = DaTxT_4(ptr, (BYTE *)&m_ballMousePos[0]);
	ptr = DaTxT_4(ptr, (BYTE *)&m_ballMousePos[1]);
	ptr = DaTxT_4(ptr, (BYTE *)&m_ballMousePos[2]);	
	*ptr++ = DaTxT_Symbol(m_nominateBall);
	*ptr++ = DaTxT_Symbol(m_testBallColour);
	
	return ptr;
}

// [CReplayStrikeBall]
char *CReplayStrikeBall::LoadDaTxTCode(char *ptr)
{
	m_type = DaTxT_Number(*ptr++);

	ptr = DaTxT_N4(ptr, (BYTE *)&m_s[0]);
	ptr = DaTxT_N4(ptr, (BYTE *)&m_s[1]);
	ptr = DaTxT_N4(ptr, (BYTE *)&m_s[2]);
	ptr = DaTxT_N4(ptr, (BYTE *)&m_e[0]);
	ptr = DaTxT_N4(ptr, (BYTE *)&m_e[1]);
	ptr = DaTxT_N4(ptr, (BYTE *)&m_e[2]);
	ptr = DaTxT_N4(ptr, (BYTE *)&m_cuetip[0]);
	ptr = DaTxT_N4(ptr, (BYTE *)&m_cuetip[1]);
	ptr = DaTxT_N4(ptr, (BYTE *)&m_cuetip[2]);
	m_selectBall = DaTxT_Number(*ptr++);
	ptr = DaTxT_N4(ptr, (BYTE *)&m_strikeRnd);
	m_ballMouseOn = DaTxT_Number(*ptr++);
	ptr = DaTxT_N4(ptr, (BYTE *)&m_ballMousePos[0]);
	ptr = DaTxT_N4(ptr, (BYTE *)&m_ballMousePos[1]);
	ptr = DaTxT_N4(ptr, (BYTE *)&m_ballMousePos[2]);	
	m_nominateBall = DaTxT_Number(*ptr++);
	m_testBallColour = DaTxT_Number(*ptr++);

	return ptr;
}

// [CReplayStrikeBall]
void CReplayStrikeBall::GenerateCode(BYTE *ptr, DWORD *size)
{
	BYTE *start = ptr;


	*ptr++ = m_type;

	ptr = RAW_4(ptr, (DWORD *)&m_s[0]);
	ptr = RAW_4(ptr, (DWORD *)&m_s[1]);
	ptr = RAW_4(ptr, (DWORD *)&m_s[2]);
	ptr = RAW_4(ptr, (DWORD *)&m_e[0]);
	ptr = RAW_4(ptr, (DWORD *)&m_e[1]);
	ptr = RAW_4(ptr, (DWORD *)&m_e[2]);
	ptr = RAW_4(ptr, (DWORD *)&m_cuetip[0]);
	ptr = RAW_4(ptr, (DWORD *)&m_cuetip[1]);
	ptr = RAW_4(ptr, (DWORD *)&m_cuetip[2]);
	*ptr++ = m_selectBall;
	ptr = RAW_4(ptr, (DWORD *)&m_strikeRnd);
	*ptr++ = m_ballMouseOn;
	ptr = RAW_4(ptr, (DWORD *)&m_ballMousePos[0]);
	ptr = RAW_4(ptr, (DWORD *)&m_ballMousePos[1]);
	ptr = RAW_4(ptr, (DWORD *)&m_ballMousePos[2]);	
	*ptr++ = m_nominateBall;
	*ptr++ = m_testBallColour;

	*size = (int) (ptr - start);
}

// [CReplayStrikeBall]
BYTE *CReplayStrikeBall::LoadCode(BYTE *ptr)
{
	DWORD v;

	m_type = *ptr++;

	ptr = RAW_N4(ptr, (DWORD *)&m_s[0]);
	ptr = RAW_N4(ptr, (DWORD *)&m_s[1]);
	ptr = RAW_N4(ptr, (DWORD *)&m_s[2]);
	ptr = RAW_N4(ptr, (DWORD *)&m_e[0]);
	ptr = RAW_N4(ptr, (DWORD *)&m_e[1]);
	ptr = RAW_N4(ptr, (DWORD *)&m_e[2]);
	ptr = RAW_N4(ptr, (DWORD *)&m_cuetip[0]);
	ptr = RAW_N4(ptr, (DWORD *)&m_cuetip[1]);
	ptr = RAW_N4(ptr, (DWORD *)&m_cuetip[2]);
	m_selectBall = *ptr++;
	ptr = RAW_N4(ptr, (DWORD *)&m_strikeRnd);
	m_ballMouseOn = *ptr++;
	ptr = RAW_N4(ptr, (DWORD *)&m_ballMousePos[0]);
	ptr = RAW_N4(ptr, (DWORD *)&m_ballMousePos[1]);
	ptr = RAW_N4(ptr, (DWORD *)&m_ballMousePos[2]);	
	m_nominateBall = *ptr++;
	m_testBallColour = *ptr++;

	return ptr;
}


// [CReplayStrikeBallPos]
char *CReplayStrikeBallPos::GenerateDaTxTCode(char *ptr)
{
	*ptr++ = DaTxT_Symbol(m_type);
	
	ptr = DaTxT_4(ptr, (BYTE *)&m_x);
	ptr = DaTxT_4(ptr, (BYTE *)&m_y);
	ptr = DaTxT_4(ptr, (BYTE *)&m_z);

	*ptr++ = DaTxT_Symbol(m_selectBall);
	return ptr;
}

// [CReplayStrikeBallPos]
char *CReplayStrikeBallPos::LoadDaTxTCode(char *ptr)
{
	m_type = DaTxT_Number(*ptr++);

	ptr = DaTxT_N4(ptr, (BYTE *)&m_x);
	ptr = DaTxT_N4(ptr, (BYTE *)&m_y);
	ptr = DaTxT_N4(ptr, (BYTE *)&m_z);

	m_selectBall = DaTxT_Number(*ptr++);
	return ptr;
}

// [CReplayStrikeBallPos]
void CReplayStrikeBallPos::GenerateCode(BYTE *ptr, DWORD *size)
{
	BYTE *start = ptr;


	*ptr++ = m_type;

	ptr = RAW_4(ptr, (DWORD *)&m_x);
	ptr = RAW_4(ptr, (DWORD *)&m_y);
	ptr = RAW_4(ptr, (DWORD *)&m_z);

	*size = (int) (ptr - start);
}

// [CReplayStrikeBallPos]
BYTE *CReplayStrikeBallPos::LoadCode(BYTE *ptr)
{
	DWORD v;

	m_type = *ptr++;

	ptr = RAW_N4(ptr, (DWORD *)&m_x);
	ptr = RAW_N4(ptr, (DWORD *)&m_y);
	ptr = RAW_N4(ptr, (DWORD *)&m_z);

	return ptr;
}

// [CReplayStrikeTime]
void CReplayStrikeTime::GenerateCode(BYTE *ptr, DWORD *size)
{
	BYTE *start = ptr;

	*ptr++ = m_type;

	ptr = RAW_4(ptr, (DWORD *)&m_time);

	*size = (int) (ptr - start);
}

// [CReplayStrikeTime]
BYTE *CReplayStrikeTime::LoadCode(BYTE *ptr)
{
	DWORD v;

	m_type = *ptr++;

	ptr = RAW_N4(ptr, (DWORD *)&m_time);

	return ptr;
}

// [CReplayStrikeState]
void CReplayStrikeState::GenerateCode(BYTE *ptr, DWORD *size)
{
	int n;
	BYTE *start = ptr;
	DWORD v;

	*ptr++ = m_type;

	for(n=0;n<64;n++)
	{
	ptr = RAW_4(ptr, (DWORD *)&m_ballX[n]);
	ptr = RAW_4(ptr, (DWORD *)&m_ballY[n]);
	}
	ptr = RAW_4(ptr, (DWORD *)&m_score1P);
	ptr = RAW_4(ptr, (DWORD *)&m_score2P);
	ptr = RAW_4(ptr, (DWORD *)&m_break1P);
	ptr = RAW_4(ptr, (DWORD *)&m_break2P);
	ptr = RAW_4(ptr, (DWORD *)&m_bTurn1P);
	ptr = RAW_4(ptr, (DWORD *)&m_bestScore);
	ptr = RAW_4(ptr, (DWORD *)&m_rednext);
	ptr = RAW_4(ptr, (DWORD *)&m_colourseq);

	#ifdef THE_POOL_CLUB
	for(n=0;n<16;n++) ptr = RAW_4(ptr, (DWORD *)&m_pottedSpots[n]);
	for(n=0;n<16;n++) ptr = RAW_4(ptr, (DWORD *)&m_pottedStripes[n]);
	#endif //THE_POOL_CLUB
	
	memset(ptr, 0, 30); // blank out array
	strcpy((char *)ptr, m_name1);
	ptr+=30;
	memset(ptr, 0, 30); // blank out array
	strcpy((char *)ptr, m_name2);
	ptr+=30;

	*size = (int) (ptr - start);
}

// [CReplayStrikeState]
BYTE *CReplayStrikeState::LoadCode(BYTE *ptr)
{
	int n;
	DWORD v;

	m_type = *ptr++;

	for(n=0;n<64;n++)
	{
	ptr = RAW_N4(ptr, (DWORD *)&m_ballX[n]);
	ptr = RAW_N4(ptr, (DWORD *)&m_ballY[n]);
	}
	ptr = RAW_N4(ptr, (DWORD *)&m_score1P);
	ptr = RAW_N4(ptr, (DWORD *)&m_score2P);
	ptr = RAW_N4(ptr, (DWORD *)&m_break1P);
	ptr = RAW_N4(ptr, (DWORD *)&m_break2P);
	ptr = RAW_N4(ptr, (DWORD *)&m_bTurn1P);
	ptr = RAW_N4(ptr, (DWORD *)&m_bestScore);
	ptr = RAW_N4(ptr, (DWORD *)&m_rednext);	
	ptr = RAW_N4(ptr, (DWORD *)&m_colourseq);

	#ifdef THE_POOL_CLUB
	for(n=0;n<16;n++) ptr = RAW_N4(ptr, (DWORD *)&m_pottedSpots[n]);
	for(n=0;n<16;n++) ptr = RAW_N4(ptr, (DWORD *)&m_pottedStripes[n]);
	#endif //THE_POOL_CLUB

	strcpy(m_name1, (char *)ptr);
	ptr+=30;
	strcpy(m_name2, (char *)ptr);
	ptr+=30;
	return ptr;
}

#define POOL_REPLAY_VERSION			1003
#define SNOOKER_REPLAY_VERSION		11

// [CReplay]
CReplay::CReplay(int max)
{
	// make a list of pointers to CReplayStrike
	m_strikelist_idx = 0;
	m_strikelist_max = max;
	m_strikelist = new CReplayStrike*[m_strikelist_max];
	m_strikelist_step = 0;
	//m_timeStart = CTime::GetCurrentTime();	// Set Event to go off at a specific time, defaultly present time.
	m_timeStart = m_timeStart.GetCurrentTime();	// Set Event to go off at a specific time, defaultly present time.
	m_pReplayDlg = NULL;
	m_bPlay = FALSE;
	m_bPeeked = TRUE;
	
	SetCurrentVersion();
}

CReplay::~CReplay()
{
	while(m_strikelist_idx) // delete all strikes in replay.
	{
	m_strikelist_idx--;
	delete m_strikelist[m_strikelist_idx];
	}
	delete m_strikelist; // delete strike array.
}

void CReplay::SetReplayDlg(CReplayDlg *pReplayDlg)
{
	m_pReplayDlg = pReplayDlg;
}

void CReplay::ResetStrike(void)
{
	while(m_strikelist_idx) // delete all strikes in replay.
	{
	m_strikelist_idx--;
	delete m_strikelist[m_strikelist_idx];
	}
	//m_timeStart = CTime::GetCurrentTime();
	m_timeStart = m_timeStart.GetCurrentTime();
}

CReplayStrike *CReplay::AddTimeStamp(void)
{
	CReplayStrikeTime *pTime;
	if(!m_strikelist_idx) return NULL; // No point having a time stamp for the first strike.

	// creates a new strike object, and returns pointer to you to modify!
	if(m_strikelist_idx>=m_strikelist_max) return NULL; // can't add any more strikes!
	pTime = new CReplayStrikeTime;
	m_strikelist[m_strikelist_idx] = pTime;
	m_strikelist_idx++;

	CTime currentTime;
	CTimeSpan currentTimeSpan;
	//currentTime = CTime::GetCurrentTime();
	currentTime = currentTime.GetCurrentTime();

	currentTimeSpan = currentTime - m_timeStart; // current number of seconds since start.
	pTime->m_time = currentTimeSpan.GetTotalSeconds();
	pTime->m_type = STRIKETYPE_TIME;
	return m_strikelist[m_strikelist_idx-1];
}

CReplayStrike *CReplay::AddStrike(void)
{
	// creates a new strike object, and returns pointer to you to modify!
	if(m_strikelist_idx>=m_strikelist_max) return NULL; // can't add any more strikes!
	m_strikelist[m_strikelist_idx] = new CReplayStrike;
	m_strikelist_idx++;
	return m_strikelist[m_strikelist_idx-1];
}
BOOL CReplay::IsFull(void) // check before adding a strike.
{
	if(m_strikelist_idx>=m_strikelist_max-8) return TRUE; // Safety margin of 8 from end
	return FALSE;
}

void CReplay::AddStrike(CReplayStrike *strike)
{
	// creates a new strike object, and returns pointer to you to modify!
	if(m_strikelist_idx>=m_strikelist_max) return; // can't add any more strikes!
	m_strikelist[m_strikelist_idx] = strike;
	m_strikelist_idx++;
}

void CReplay::DeleteStrike(void)
{	// delete last strike in the replay list.
	if(m_strikelist_idx)
	{
	m_strikelist_idx--;
	delete m_strikelist[m_strikelist_idx];	
	}
}

CReplayStrike *CReplay::StepStrike(void) // return one strike.
{
	if(!m_strikelist_idx) return NULL; // nothing in the replay bank.
	if(m_strikelist_step<m_strikelist_idx) m_strikelist_step++;

	return m_strikelist[m_strikelist_step-1];
}

CReplayStrike *CReplay::BackStepStrike(void) // return one strike.
{
	if(!m_strikelist_idx) return NULL; // nothing in the replay bank.
	if(m_strikelist_step>0) m_strikelist_step--;

	return m_strikelist[m_strikelist_step];
}

CReplayStrike *CReplay::GetStrike(int idx) // return a specific strike.
{
	if(idx<m_strikelist_idx) return m_strikelist[idx];

	return NULL; // Error! strike unavailable!
}

void CReplay::GotoStrike(int idx) // jump to a specific strike
{
	if(idx<m_strikelist_idx) m_strikelist_step = idx;
}

void CReplay::SetCurrentVersion(void)
{
	#ifdef THE_POOL_CLUB
	m_version = POOL_REPLAY_VERSION;
	#else
	m_version = SNOOKER_REPLAY_VERSION;
	#endif
}

void CReplay::VersionSettings(BOOL bReplayVersion)
{
	DWORD version = m_version;


	//version = 1;

	#ifndef THE_POOL_CLUB // iSnooker
		if(!bReplayVersion)
		{
			version = SNOOKER_REPLAY_VERSION;
		}
		
		// This Global Version variable will be useful in other parts of the code //
		
		switch(version)
		{
			case 0:
				// Update physics switches back to old physics - iSnooker ver. <= 1.6.7c
				g_bBallRollOff = FALSE;
				g_bBallJustHitEnergy = FALSE;
				g_bNewParallelCushionLoss = FALSE;
				g_bTextDoubleError = FALSE;
				g_bDetectionO = FALSE;
				g_iPhysicsType = 0;

				break;
			case 1:			
				// Old Detection Method //
				g_bBallRollOff = TRUE;
				g_bBallJustHitEnergy = TRUE;
				g_bNewParallelCushionLoss = FALSE;
				g_bTextDoubleError = FALSE;
				g_bDetectionO = FALSE;
				g_iPhysicsType = 0;
				break;
			case 2:
			case 3:
				// Use the new Detect Side (Obstacle Course Detection method)
				g_bBallRollOff = TRUE;
				g_bBallJustHitEnergy = TRUE;
				g_bNewParallelCushionLoss = FALSE;
				g_bTextDoubleError = TRUE;
				g_bDetectionO = TRUE;
				g_iPhysicsType = 0;
			case 4:
			case 5:
				g_iPhysicsType = 0;
				break;
			case 6:
				g_iPhysicsType = 1;
				break;
			case 7:
			case 8:
				g_iPhysicsType = 2;
				break;
			case 9:
				g_iPhysicsType = 3;
				break;
			case 10:
				g_iPhysicsType = 4;
				break;
			case 11:
				g_iPhysicsType = 5;
				break;

		}
	#else // iPool
		if(!bReplayVersion)
		{
			version = POOL_REPLAY_VERSION;			
		}
		
		switch(version)
		{
			case 0:
				// Update physics switches back to old physics - iSnooker ver. <= 1.6.7c
				g_bBallRollOff = FALSE;
				//g_bBallJustHitEnergy = FALSE;
				g_bNewParallelCushionLoss = FALSE;
				g_bDetectionO = FALSE;
				g_iPhysicsType = 0;
				break;
			case 1001:			
				g_bBallRollOff = TRUE;
				//g_bBallJustHitEnergy = TRUE;
				g_bNewParallelCushionLoss = TRUE;
				g_bDetectionO = FALSE;
				g_iPhysicsType = 0;
				break;
			case 1002:			
				g_bBallRollOff = TRUE;
				//g_bBallJustHitEnergy = TRUE;
				g_bNewParallelCushionLoss = TRUE;
				g_bDetectionO = TRUE;
				g_iPhysicsType = 0;
				break;
			case 1003:
				g_bBallRollOff = TRUE;
				//g_bBallJustHitEnergy = TRUE;
				g_bNewParallelCushionLoss = TRUE;
				g_bDetectionO = TRUE;
				g_iPhysicsType = 5;
				break;

		}	
	#endif

		g_gameAppView->m_snooker->UpdatePhysics(g_iPhysicsType);
		
}

void CReplay::SaveRaw(char *fname)
{
	BYTE cbuf[1024]; // max size of data per packet.
	DWORD csiz;
	int siz;
	int n;
	DWORD crc;
	DWORD version;
	FILE *fptr;

	fptr = fopen(fname, "wb");	
	if(fptr == NULL)
	{
		return;
	}
	
	#ifndef THE_POOL_CLUB // iSnooker
		version = SNOOKER_REPLAY_VERSION;
	#else // iPool
		version = POOL_REPLAY_VERSION;
	#endif
	fwrite(&version,4,1,fptr);

	for(n=0;n<m_strikelist_idx;n++) // Go thru all commands.XP_end[0] = pPlr1->level_progress;
	{
	GetStrike(n)->GenerateCode(cbuf, &csiz);
	fwrite(cbuf,csiz,1,fptr);
	}
	fclose(fptr);
}

bool CReplay::LoadRaw(char *fname)
{
	int len;
	BYTE *buf;
	BYTE *ptr;
	DWORD version;
	CReplayStrike *tempstrike;

	len = Mmain_filelen(fname);
	if(len==-1) return false; // Failed to load file
	buf = new BYTE[len];
	Mmain_filedump(buf, fname);
	ptr = buf;

	ptr = RAW_N4(ptr, (DWORD *)&version);
	
	// replay version checking
	m_version = version;

	#ifndef THE_POOL_CLUB // iSnooker
		if(version == 0);
		else if(version == 1);
		else if(version == 2);
		else if(version == 3);
		else if(version == 4);
		else if(version == 5);
		else if(version == 6);
		else if(version == 7);
		else if(version == 8);
		else if(version == 9);
		else if(version == SNOOKER_REPLAY_VERSION);
		else
		{
			AfxMessageBox("Invalid Replay File.");
			return false;
		}
	#else // iPool
		if(version == 0);
		else if(version == 1001);
		else if(version == 1002);
		else if(version == POOL_REPLAY_VERSION);
		else
		{
			AfxMessageBox("Invalid Replay File.");
			return false;
		}	
	#endif
	
	ResetStrike();

	while(ptr!=&buf[len])
	{
	switch(*ptr)
		{
		case STRIKETYPE_BALL:tempstrike = new CReplayStrikeBall;
							 ptr = ((CReplayStrikeBall *)tempstrike)->LoadCode(ptr);
							 AddStrike(tempstrike);						 
							 break;

		case STRIKETYPE_RERACK:tempstrike = new CReplayStrikeReRack;
							 ptr = ((CReplayStrikeReRack *)tempstrike)->LoadCode(ptr);
							 AddStrike(tempstrike);
							 break;

		case STRIKETYPE_BALLPOS:tempstrike = new CReplayStrikeBallPos;
							 ptr = ((CReplayStrikeBallPos *)tempstrike)->LoadCode(ptr);
							 AddStrike(tempstrike);
							 break;

		case STRIKETYPE_TIME:tempstrike = new CReplayStrikeTime;
							 ptr = ((CReplayStrikeTime *)tempstrike)->LoadCode(ptr);
							 AddStrike(tempstrike);
							 break;

		case STRIKETYPE_STATE:tempstrike = new CReplayStrikeState;
							 ptr = ((CReplayStrikeState *)tempstrike)->LoadCode(ptr);
							 AddStrike(tempstrike);
							 break;

		default:ptr++;
				AfxMessageBox("Unknown Tag");
				break;
		}
	}
	

	delete buf;

	return true;
}

void CReplay::HandleTime(CReplayStrikeTime *strike)
{
	if(m_pReplayDlg)
	{
	char buffer[80];
	sprintf(buffer,"%d.00s",strike->m_time);
	m_pReplayDlg->UpdateDisplay(buffer);
	}
}

void CReplay::ZoomPosition(int pos)
{
	CReplayStrike *pStrike;
	while(pos)
	{
	pStrike = GetStrike(pos);
	if(pStrike==NULL) return;
	GotoStrike(pos);
	if(pStrike->m_type==STRIKETYPE_RERACK || pStrike->m_type==STRIKETYPE_STATE)
		{
		g_gameAppView->HandleReplayCommand(pStrike);
		return;
		}
	pos--;
	}
}

void CReplay::PeekNextState(void)
{
	if(GetStrike(0)==NULL) return; // [+] Should Gray this button out
	// REPLAY: Step Forward /////////////////////////////////////////////////////////////////
	int old_step = m_strikelist_step;
	CReplayStrike *strike;

	while(m_strikelist_step<m_strikelist_idx)
	{
		strike = StepStrike();		
		if(strike->m_type==STRIKETYPE_STATE) 
		{
			m_strikelist_step = old_step;
			
			g_gameAppView->m_snooker->StateReplay((CReplayStrikeState *)strike);

			break;
		}
	} 	

	m_bPeeked = TRUE;

	// ///////////////////////////////////////////////////////////////////////////////
}


void CReplay::Process(void)
{
	if(m_bPlay) // replay play mode
	if(g_gameAppView->m_snooker->m_bPracticeMode) // in practice mode
	{
		if(g_gameAppView->m_snooker->m_bShotInProgress); // if in simulation mode
		else
		{
			m_pReplayDlg->PlayStep();
		}
	}
	
	// for PeekNextState()...
	if(!m_bPeeked)
	{
		if(g_gameAppView->m_snooker->m_bShotInProgress); // if in simulation mode
		else
			PeekNextState();		
	}
	
}
