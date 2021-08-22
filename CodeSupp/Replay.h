///////////////////////////////////////////////////////////////////////////////////////////////////
// (Replay.h)
//

#ifndef __REPLAY_H__
#define __REPLAY_H__

#include "../CodeSupp/build.h"

#define STRIKETYPE_BALL		0
#define STRIKETYPE_RERACK	1
#define STRIKETYPE_BALLPOS	2
#define STRIKETYPE_TIME		3
#define STRIKETYPE_STATE	4

int DaTxT_Check(char *str);

class CReplayStrike {
public:
DWORD m_type;
virtual char *GenerateDaTxTCode(char *ptr);
virtual char *LoadDaTxTCode(char *ptr);
virtual void GenerateCode(BYTE *ptr, DWORD *size); // Generate code equivalent of Strike
virtual BYTE *LoadCode(BYTE *ptr); // Generate code equivalent of Strike
};

class CReplayStrikeReRack : public CReplayStrike {
public:
int m_reRackType;
int m_randseed;

#ifdef THE_POOL_CLUB
DWORD m_customTableType;
#endif //THE_POOL_CLUB

char *GenerateDaTxTCode(char *ptr);
char *LoadDaTxTCode(char *ptr);
void GenerateCode(BYTE *ptr, DWORD *size); // Generate code equivalent of Strike
BYTE *LoadCode(BYTE *ptr); // Generate code equivalent of Strike
};

class CReplayStrikeBall : public CReplayStrike {
public:
float m_s[3]; // sx/sy/sz
float m_e[3]; // ex/ey/ez
float m_cuetip[3];
int m_selectBall;
float m_strikeRnd;
BOOL m_ballMouseOn;
float m_ballMousePos[3];
int m_nominateBall;
int m_testBallColour;

void SetStart(float x,float y,float z);
void SetEnd(float x,float y,float z);
void SetCuetip(float x,float y,float z);
void SetBallMousePos(float x,float y,float z);
char *GenerateDaTxTCode(char *ptr);
char *LoadDaTxTCode(char *ptr);
void GenerateCode(BYTE *ptr, DWORD *size); // Generate code equivalent of Strike
BYTE *LoadCode(BYTE *ptr); // Generate code equivalent of Strike
};

class CReplayStrikeBallPos : public CReplayStrike {
public:
float m_x,m_y,m_z;
int m_selectBall;

char *GenerateDaTxTCode(char *ptr);
char *LoadDaTxTCode(char *ptr);
void GenerateCode(BYTE *ptr, DWORD *size); // Generate code equivalent of Strike
BYTE *LoadCode(BYTE *ptr); // Generate code equivalent of Strike
};

class CReplayStrikeTime : public CReplayStrike {
public:
int m_time; // time in seconds from start of replay.

void GenerateCode(BYTE *ptr, DWORD *size); // Generate code equivalent of Strike
BYTE *LoadCode(BYTE *ptr); // Generate code equivalent of Strike
};

class CReplayStrikeState : public CReplayStrike {
public:
//__fix f_ballX[64]; // 128 * 8 = 1k per state.
//__fix f_ballY[64];
float m_ballX[64];
float m_ballY[64];
int m_score1P;
int m_score2P;
int m_break1P;
int m_break2P;
int m_bTurn1P;
int m_bestScore;
int m_rednext;
int m_colourseq;
char m_name1[22];
char m_name2[22];

#ifdef THE_POOL_CLUB
int m_pottedSpots[16];
int m_pottedStripes[16];
#endif //THE_POOL_CLUB

void GenerateCode(BYTE *ptr, DWORD *size); // Generate code equivalent of Strike
BYTE *LoadCode(BYTE *ptr); // Generate code equivalent of Strike
};

// Call AddStrike (below), then write to object returned.

class CReplayDlg;

class CReplay {
public:
CReplayStrike **m_strikelist;
int m_strikelist_max;	// Absoloute maximum number of steps that can be created in replay.
int m_strikelist_idx;	// number of steps in replay / current step being written.
int m_strikelist_step; // step/play index for playing back replay
CTime m_timeStart;
CReplayDlg *m_pReplayDlg;
BOOL m_bPlay;
BOOL m_bPeeked;
DWORD m_version;

CReplay(int max);
~CReplay();
void SetReplayDlg(CReplayDlg *pReplayDlg);
void ResetStrike(void);	// reset strike list (i.e. for starting a new replay)
CReplayStrike *AddTimeStamp(void);
CReplayStrike *AddStrike(void); // add a new strike. (write to the object it returns)
BOOL IsFull(void);
void AddStrike(CReplayStrike *strike); // add a new strike. method #2 (ask colin about this if you don't know)

void DeleteStrike(void);	// delete last strike in the list
CReplayStrike *StepStrike(void); // return one strike.
CReplayStrike *BackStepStrike(void); // return one strike.
CReplayStrike *GetStrike(int idx); // return a specific strike.
void GotoStrike(int idx); // jump to a specific strike
//char *GenerateDaTxT(void);	// Generate DaTxT based on strikes in replay!
//void RemoveCrap(char *str); // Gets rid of spaces e.g. "colin is an ace coder" -> "colinisanacecoder"
//void LoadDaTxT(char *str);	// Load DaTxT!
void SetCurrentVersion(void);
void VersionSettings(BOOL bReplayVersion = TRUE);
void SaveRaw(char *fname); // Save Raw format (to file)
bool LoadRaw(char *fname);
void HandleTime(CReplayStrikeTime *strike);
void ZoomPosition(int pos);
void PeekNextState(void);
void Process(void);

};

// Use for generating replay: ResetStrike(), (x = AddStrike(), x.Setsomething(...)+DeleteStrike())*,
// Use for running: GotoStrike(0), StepStrike()*

#endif // __REPLAY_H__