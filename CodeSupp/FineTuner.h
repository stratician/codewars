#pragma once


// #include <d3dx9.h>
#include <string>

class CFineTuner
{
public:
	CFineTuner(int level);
	~CFineTuner(void);


	IDirect3DTexture9 *fineTuner_bg_tex;
	IDirect3DTexture9 *fineTuner_slider_tex;
	IDirect3DTexture9 *fineTuner_tab;

	float posx_orig;
	float posy_orig;

	float posx;
	float posvx;
	float posy;

	bool bMouseOver;
	int HideDelay;


	int tuner_x;
	int tuner_y;
	int tuner_width;
	int tuner_height;

	int tunerSlider_x;
	int tunerSlider_y;
	int tunerSliderBoundXmin;
	int tunerSliderBoundXmax;
	int tunerSliderBoundYmin;
	int tunerSliderBoundYmax;

	bool tunerSliderMouseDown;
	M3d_vtx m_sPointOrig;

	int access_level;
	bool bLevelReq;
	bool bShowLevelReq;

	char info_tf[256];

	float alpha;
	
	M3d_vtx m_tableMousePosOrig;

	CWnd *m_pParentWnd;
	
	void Init(CWnd *pParent, int x, int y);
	// void UpdateLevel(void);
	void Render(void);
	void Process(void);
	
	void MouseDetections(void);
	bool MouseOver(int mx, int my);
	void OnClick(void);
	
	void RenderLevelAccess(void);
	bool HasAccess(void);
	void Reset(bool bLineSet);
};

