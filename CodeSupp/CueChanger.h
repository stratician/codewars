	#pragma once


	// #include "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\d3dx9.h"
	#include <string>

#define MAX_CUES		8


extern char *cue_names[];
extern char *cue_names_id[];
extern int cue_prices[];




class CCueChanger
{

public: 
	
	

	int cue_idx;
	
	char cue_type;
	int cue_width;
	int cue_height;
	IDirect3DTexture9 **cues_tex;
	IDirect3DTexture9 *cue_glow_tex;
	
	bool bMouseOver_cues_tab;
	bool bMouseOver_uparrow;
	bool bMouseOver_downarrow;
	bool bMouseOver_select;
	bool bMouseOver_buy;


	IDirect3DTexture9 *cues_tab_tex;
	IDirect3DTexture9 *cues_tab_mo_tex;
	IDirect3DTexture9 *cues_tab_off_tex;
	int cues_tab_x;
	int cues_tab_y;
	int cues_tab_width;
	int cues_tab_height;




	IDirect3DTexture9 **upArrow_tex;
	IDirect3DTexture9 **downArrow_tex;
	

	float upArrow_posx;
	float upArrow_posy;

	float downArrow_posx;
	float downArrow_posy;
	
	IDirect3DTexture9 **buy_tex;
	IDirect3DTexture9 **select_tex;

	int select_posx;
	int select_posy;
	int select_width;
	int select_height;

	int buy_posx;
	int buy_posy;
	int buy_width;
	int buy_height;

	int cue_index;
	int cue_index_selected;
	int cue_select;
	int cue_select_target;

	float cue_posyorig;
	float cue_posx;
	float cue_posy;
	float cue_alpha;

	float cue_select_y;
	float cue_select_vy;

	bool bShow;

	float pos_xorig;
	float pos_yorig;

	float pos_x;
	float pos_y;
	float pos_vx;
	float pos_vy;

	float width;
	float height;

	float pos_indent;
	
	int access_level;
	//int levelReq:CLevelRequired;
	bool bLevelReq;
	bool bInActive;
	bool bShowLevelReq;

	bool bMouseCursorHand;
	float cue_new_y;
	float cue_new_vy;
	float ticker;

	char info_tf[256];
	

	CWnd *m_pParentWnd;
	

	IDirect3DTexture9 *bgTexture;


public:
	CCueChanger(int level);
	~CCueChanger(void);

	//void PassParent(CWnd *pParent);

	void Init(CWnd *pParent);
	// void UpdateLevel(void);
	void Render(void);
	void Process(void);
	
	void MouseDetections(void);
	bool MouseOver(int mx, int my);

	bool CueTabDetections(int mx, int my);
	void UpArrowDetections(int mx, int my);
	void DownArrowDetections(int mx, int my);
	void SelectDetections(int mx, int my);
	void BuyDetections(int mx, int my);

	void SelectCue(int idx);

	void OnSelectClick(void);
	void OnBuyClick(void);
	void OnUpClick(void);
	void OnDownClick(void);
	void SetCue(int oldIdx, int newIdx);
	bool PurchasedCue(int cueIdx);

	void InitCue(int index);

	void RenderLevelAccess(void);
	bool HasAccess(void);

};

