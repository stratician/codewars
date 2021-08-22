	#pragma once


	// #include "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\d3dx9.h"
	#include <string>

#define MAX_CHALKS		11


extern char *chalk_names[];
extern char *chalk_names_id[];
extern int chalk_prices[];
extern int chalk_levels[];
extern DWORD chalk_col[];

class CChalkChanger
{

public: 
	
	
	bool bLButton;
	int chalk_idx;
	
	char chalk_type;
	int chalk_width;
	int chalk_height;
	IDirect3DTexture9 **chalks_tex;
	IDirect3DTexture9 *chalks_glow_tex;
	
	bool bMouseOver_chalks_tab;
	bool bMouseOver_uparrow;
	bool bMouseOver_downarrow;
	bool bMouseOver_select;
	bool bMouseOver_buy;


	IDirect3DTexture9 *chalks_tab_tex;
	IDirect3DTexture9 *chalks_tab_mo_tex;
	IDirect3DTexture9 *chalks_tab_off_tex;
	IDirect3DTexture9 *chalks_list_tex;
	IDirect3DTexture9 *chalk_void_tex;
	IDirect3DTexture9 *chalk_select_tex;
	int chalks_tab_x;
	int chalks_tab_y;
	int chalks_tab_width;
	int chalks_tab_height;




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

	int chalk_index;
	int chalk_index_selected;
	int chalk_select;
	int chalk_select_target;

	float chalk_posyorig;
	float chalk_posx;
	float chalk_posy;
	float chalk_alpha;

	float chalk_select_y;
	float chalk_select_vy;

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
	bool bLevelReq;
	bool bInActive;
	bool bShowLevelReq;

	bool bMouseCursorHand;
	float chalk_new_y;
	float chalk_new_vy;
	float ticker;

	char info_tf[256];

	DWORD col;
	
	float sel_x;
	float sel_y;
	float sel_vx;
	float sel_vy;

	CWnd *m_pParentWnd;
	

	IDirect3DTexture9 *bgTexture;


public:
	CChalkChanger(int level);
	~CChalkChanger(void);

	//void PassParent(CWnd *pParent);

	void Init(CWnd *pParent);
	// void UpdateLevel(void);
	void Render(void);
	void Process(void);
	
	void MouseDetections(void);
	bool MouseOver(int mx, int my);

	bool ChalkTabDetections(int mx, int my);
	void UpArrowDetections(int mx, int my);
	void DownArrowDetections(int mx, int my);
	void SelectDetections(int mx, int my);
	void BuyDetections(int mx, int my);

	void SelectChalk(int idx);

	void OnSelectClick(void);
	void OnBuyClick(void);
	void OnUpClick(void);
	void OnDownClick(void);
	void SetChalk(int oldIdx, int newIdx);
	bool PurchasedChalk(int chalkIdx);

	void InitChalk(int index);

	void RenderLevelAccess(void);
	bool HasAccessTab(void);
	bool HasAccessChalk(int idx);

	void RenderChalkList(int x, int y);

};

