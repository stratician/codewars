
#include "../CodeSupp/build.h"
#include "../CodeSupp/stdafx.h"
#include "../CodeSupp/GameAppView.h"	
#include "../Codedx9/font.h"


#include "Achievements.h"
#include <iostream>


// Achiemvenet Object

IDirect3DTexture9 *achievementsBGTex;
IDirect3DTexture9 *achievementsProgressTex;
LPD3DXSPRITE achievementsBGSpr;
Font *g_pFontA;
Font *g_pFontB;



LRESULT CALLBACK CAchievements::WndProcBase(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#ifndef MAIN_SERVER

	switch (msg)
	{


	case WM_CLOSE:
		DestroyWindow(hwnd);
		if (g_gameAppView->achievements)
		{
			g_gameAppView->achievements->OnClose();
			delete(g_gameAppView->achievements);
			g_gameAppView->achievements = NULL;
		}
		break;


	case WM_LBUTTONDOWN:
		if (g_gameAppView->achievements)
		{
			g_gameAppView->achievements->MouseDown(true);
			SetCapture(hwnd);
		}
		break;

	case WM_LBUTTONUP:
		if (g_gameAppView->achievements)
		{
			g_gameAppView->achievements->MouseDown(false);
			ReleaseCapture();
		}
		
		break;

	case WM_MOUSEMOVE:
		if (g_gameAppView->achievements)
		{
			g_gameAppView->achievements->MouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}

		break;

	case WM_DESTROY:
		// PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);

	}

#endif

	return 0;

}


void CAObject::RenderStars()
{
	// Render 1, 2 or 3 stars based on percentage complete
	
	int StarsNum = 0;

	if (percentComplete > 1)
	{
		StarsNum = percentComplete / 33;
	}

	D3DXVECTOR2 starPos[3];
	starPos[0].x = pos[0] + 11;
	starPos[0].y = pos[1] + 9;

	starPos[1].x = pos[0] + 42;
	starPos[1].y = pos[1] - 5;

	starPos[2].x = pos[0] + 75;
	starPos[2].y = pos[1] + 9;

	RECT *srcRect = NULL;

	for (int i = 0;i < StarsNum;i++)
	{
		// g_pFontA->Draw(achievementsBGSpr, "{", starPos[i].x, starPos[i].y, 0.0f, 1.0f, 0.5f, 0.5f); // ASCII { - Star Off
		g_pFontB->Draw(achievementsBGSpr, "}", starPos[i].x, starPos[i].y, 0.0f, 1.0f, 1.0f, 1.0f); // ASCII } - Star On
	}


	// Also Render achievement idx
	char buffer[256];
	sprintf(buffer, "#%d", achievementIdx+1);
	D3DXCOLOR color;
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;
	g_pFontA->Draw(achievementsBGSpr, buffer, pos[0] + 10, pos[1] + 3, 0.0f, 1.0f, 0.4f, 0.4f, 0, color);


}


void CAObject::Process()
{
	// Progress Bar Processing. 
	// Make the progress bar increase to the target percent
	if (percentComplete < targetPercent)
	{
		int delta = sqrt((targetPercent - percentComplete) / 2);
		if (delta < 1) delta = 1;

		percentComplete += delta;
	}
}


void CAObject::SetPercent(int percent)
{
	if (percent < 0) percent = 0;
	else if (percent > 100) percent = 100;
	else;

	targetPercent = percent;

}

void CAObject::RenderProgressBar()
{
	// Draw Progress Bar BG (Grey Rectangle)



	D3DXCOLOR color;
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;



	HRESULT hr;



	D3DXMATRIX matWorld;
	D3DXMATRIX matScale;
	D3DXMATRIX matRotX;
	D3DXMATRIX matRotY;
	D3DXMATRIX matRotZ;
	D3DXMATRIX matTrans;

	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matWorld);




	// Calculate Scale matrix
	D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 1.0f);

	// Calculate rotation matrix
	D3DXMatrixRotationX(&matRotX, 0.0f);
	D3DXMatrixRotationY(&matRotY, 0.0f);
	D3DXMatrixRotationZ(&matRotZ, 0.0f);

	// Calculate a translation matrix
	D3DXMatrixTranslation(&matTrans, pos[0] + 370.0f, pos[1] + 10.0f, pos[2]);
	matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;
	achievementsBGSpr->SetTransform(&matWorld);

	// get width of our ProgressPercent png
	D3DSURFACE_DESC desc;
	achievementsProgressTex->GetLevelDesc(0, &desc);

	RECT srcRect;
	srcRect.left = 0;
	srcRect.top = 0;

	float percent = percentComplete;
	if (percent < 100) percent *= 0.95f; // We scale this down a bit, so when we're at a high percent like 96, it doesn't look full

	srcRect.right = desc.Width * (float)percent / 100.0f;
	srcRect.bottom = desc.Height;

	achievementsBGSpr->Draw(achievementsProgressTex, &srcRect, NULL, NULL, color);

	// Render the percent complete (if not 100%)
	if (percentComplete < 100)
	{
		
		char buffer[256];
		sprintf(buffer, "%d%%", percentComplete);
		g_pFontB->Draw(achievementsBGSpr, buffer, pos.x + 420, pos.y + 25, 0.0f, 1.0f, 0.30f, 0.30f, 0, color);
	}

}


void CAObject::Render()
{
	// We will render to a temp surface, then b


	static float timer = 0.0f;
	timer += 0.1f;

	/*
	D3DXVECTOR3 pos;
	pos.x = 150 + sinf(timer) * 100;
	pos.y = 150 + cosf(timer) * 20;
	pos.z = 0;
	*/

	

	D3DXCOLOR color;
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;

	if (this->GetStars() == 3)
	{
		color.r = 1.0f;
		color.g = 1.0f;
		color.b = 0.85f;
		color.a = 1.0f;
	}


	
	HRESULT hr;


	
	D3DXMATRIX matWorld;
	D3DXMATRIX matScale;
	D3DXMATRIX matRotX;
	D3DXMATRIX matRotY;
	D3DXMATRIX matRotZ;
	D3DXMATRIX matTrans;

	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matWorld);




	// Calculate Scale matrix
	D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 1.0f);

	// Calculate rotation matrix
	D3DXMatrixRotationX(&matRotX, 0.0f);
	D3DXMatrixRotationY(&matRotY, 0.0f);
	D3DXMatrixRotationZ(&matRotZ, 0.0f);

	// Calculate a translation matrix
	D3DXMatrixTranslation(&matTrans, pos[0], pos[1], pos[2]);
	matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;
	achievementsBGSpr->SetTransform(&matWorld);
	
	achievementsBGSpr->Draw(achievementsBGTex, NULL, NULL, NULL, color);
	// pos.y += 10;
	// achievementsBGSpr->Draw(achievementsBGTex, NULL, NULL, &pos, color);
	

	
	
	char buffer[256];
	sprintf(buffer, name);
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;
	g_pFontA->Draw(achievementsBGSpr, buffer, pos.x + 120, pos.y + 5, 0.0f, 1.0f, 0.5f, 0.5f, 0, color);
	// g_pFontA->Render(buffer, pos.x + 10, pos.y + 40, 0.0f, 1.0f, 1.0f, 1.0f);
	
	sprintf(buffer, desc);
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;
	g_pFontB->Draw(achievementsBGSpr, buffer, pos.x + 120, pos.y + 30, 0.0f, 1.0f, 0.4f, 0.4f, 0, color);


	// If we have completed achievement (i.e. 100%) then display text, "Completed!"
	if (this->GetStars() == 3)
	{
		sprintf(buffer, desc);
		color.r = 0.5f;
		color.g = 1.0f;
		color.b = 0.5f;
		color.a = 1.0f;
		g_pFontA->Draw(achievementsBGSpr, "Completed!", pos.x + 390, pos.y + 36, -0.15f, 1.0f, 0.38f, 0.38f, 0, color);
	}

	RenderStars();

	RenderProgressBar();
}



void CAchievements::OnClose()
{
	// Close Window
	int p;
	p = 0;

	CWindowBase::OnClose();

	
}

CAchievements::CAchievements(int maxAchievements)
{


	m_maxAchievements = maxAchievements;


}




CAchievements::~CAchievements()
{
	d3d::Delete<Font*>(g_pFontB);
	d3d::Delete<Font*>(g_pFontA);


	
	achievementsProgressTex->Release();
	achievementsBGTex->Release();
	achievementsBGSpr->Release();

	::CWindowBase::Destroy();
	
 	
}


int CAchievements::CreateAchievement(char *title, char *desc, int level)
{
	int objIdx = -1;
	

	for (int i = 0;i < m_maxAchievements;i++)
	if(m_obj[i] == NULL)
	{
		m_obj[i] = new CAObject(title, desc, i, level);

		// Debug.. set random 
		m_obj[i]->SetPercent(rand() % 100);
		objIdx = i;
		break;
	}

	return objIdx;
}


bool bRegisteredClass = false;

int CAchievements::Create(HINSTANCE hInstance, HWND parentWnd, char *title, int x, int y, int w, int h, Player plr)
{

	Mmain_log("CAchievements Create [1.0]");

	CWindowBase::Create(hInstance, parentWnd, title, x, y, w, h);

	// Create a windows dialog (basic)

	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;

	/*
#define ACHIEVEMENT_WINS							0
#define ACHIEVEMENT_HIGHBREAK_SEMI					1
#define ACHIEVEMENT_CENTURIES_SEMI					2
#define ACHIEVEMENT_147_SEMI						3
#define ACHIEVEMENT_HIGHBREAK_PRO					4
#define ACHIEVEMENT_CENTURIES_PRO					5
#define ACHIEVEMENT_147_PRO							6
#define ACHIEVEMENT_RATING							7
#define ACHIEVEMENT_TOURNAMENTWINS					8
#define ACHIEVEMENT_HIGHSTREAK						9
*/
	


	MSG msg;
	WNDCLASSEX wc;

	//Step 1: Registering the Window Class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProcBase;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "BaseWindow";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


	if (!bRegisteredClass)
	{
		if (!RegisterClassEx(&wc))
		{
			MessageBox(NULL, "Window Registration Failed!", "Error!",
				MB_ICONEXCLAMATION | MB_OK);
			return 0;
		}
	}
	Mmain_log("CAchievements Create [1.1]");

	bRegisteredClass = true;

	hWnd = CreateWindowEx(NULL, "BaseWindow", TEXT(title), WS_OVERLAPPEDWINDOW&~(WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX), x, y, w, h, parentWnd, NULL, hInstance, NULL);
	Mmain_log("CAchievements Create [1.1.1]");


	ShowWindow(hWnd, SW_SHOW);
	// UpdateWindow(hWnd);


	InitD3D();

	
	bool result;
	// Create Font
	g_pFontA = new Font(m_pDevice);
	g_pFontB = new Font(m_pDevice);
	result = g_pFontA->Load(1024, 150, 5, "Graphics/HUD/font5.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ,.<>/\\?;:1234567890!@#$%^&*()-_=+[]abcdefghijklmnopqrstuvwxyz{}");
	result = g_pFontB->Load(1024, 150, 12, "Graphics/HUD/font6.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ,.<>/\\?;:1234567890!@#$%^&*()-_=+[]abcdefghijklmnopqrstuvwxyz{}");
	

	
	D3DXCreateTextureFromFile(m_pDevice, "Graphics/images/achievementbg.png", &achievementsBGTex);
	D3DXCreateTextureFromFile(m_pDevice, "Graphics/images/achievementProgress.png", &achievementsProgressTex);
	
	HRESULT hr;
	//hr = D3DXCreateTextureFromFileEx(m_pDevice, "Graphics/images/achievementbg.png", 512, 64, 0, D3DPOOL_DEFAULT, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &achievementsBGTex);
	D3DXCreateSprite(m_pDevice, &achievementsBGSpr);

	D3DSURFACE_DESC achievementObjectDesc;
	Mmain_log("CAchievements Create [1.2]");
	achievementsBGTex->GetLevelDesc(0, &achievementObjectDesc);
	int s = achievementObjectDesc.Height;

	m_obj = new CAObject*[m_maxAchievements];

	for (int i = 0;i < m_maxAchievements;i++)
	{
		m_obj[i] = NULL;
	}

	
	CreateAchievements(plr);
	UpdateAchievements(title, plr);

	// Create our vertical Scroll bar texture background
	vscrollMaxHeight = maxAchievementsOnPage * (achievementObjectDesc.Height + acheivementObjectGap) - 20;
	vscrollHeight = vscrollMaxHeight;
	vscrollPos = 0;

	D3DXCreateTexture(m_pDevice, 20, vscrollMaxHeight, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &vScrollBarTex);
	D3DLOCKED_RECT lockedRect;
	
	Mmain_log("CAchievements Create [1.3]");
	vScrollBarTex->LockRect(0, &lockedRect, NULL, D3DLOCK_NOSYSLOCK);

	DWORD *pPixel32;

	pPixel32 = (DWORD*)lockedRect.pBits;

	// Clear the texture to red
	for (int y = 0;y < vscrollMaxHeight;y++)
	{
		for (int x = 0;x < vscrollWidth;x++)
		{
			pPixel32[x + y*(lockedRect.Pitch / 4)] = 0xFFFFFFFF;
		}
	}

	vScrollBarTex->UnlockRect(0);
	

	
	// Create our achievement clipping texture (same color as background)
	D3DXCreateTexture(m_pDevice, achievementObjectDesc.Width, achievementObjectDesc.Height, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &achievementClipTex);
	

	achievementClipTex->LockRect(0, &lockedRect, NULL, D3DLOCK_NOSYSLOCK);

	pPixel32 = (DWORD*)lockedRect.pBits;

	// Clear the texture to red
	for (int y = 0;y < achievementObjectDesc.Height;y++)
	{
		for (int x = 0;x < achievementObjectDesc.Width;x++)
		{
			pPixel32[x + y*(lockedRect.Pitch / 4)] = backgroundCol;
		}
	}

	achievementClipTex->UnlockRect(0);
	Mmain_log("CAchievements Create [1.4]");

	// Create our achievement footer background texture (darker color than background)
	D3DXCreateTexture(m_pDevice, this->width, achievementObjectDesc.Height, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &achievementFooterTex);


	achievementFooterTex->LockRect(0, &lockedRect, NULL, D3DLOCK_NOSYSLOCK);

	pPixel32 = (DWORD*)lockedRect.pBits;

	// Clear the texture to red
	for (int y = 0;y < achievementObjectDesc.Height;y++)
	{
		for (int x = 0;x < this->width;x++)
		{
			pPixel32[x + y*(lockedRect.Pitch / 4)] = 0xAA999999;//backgroundCol;
		}
	}

	achievementFooterTex->UnlockRect(0);
	Mmain_log("CAchievements Create [1.5]");


	bInitialised = true;
}

// Count the achievements, and return the number of thme
int CAchievements::GetTotalAchievementsActive()
{
	int total = 0;

	for (int i = 0;i < m_maxAchievements;i++)
	{
		if (m_obj[i]) total++;
	}
	
	return total;
}


// Count the achievements, and return the number of thme
int CAchievements::GetTotalAchievementsCompletedActive()
{
	int total = 0;

	for (int i = 0;i < m_maxAchievements;i++)
	{
		if (m_obj[i])
		{
			if (m_obj[i]->GetStars() == 3) total++;
		}
	}

	return total;
}





// Count the achievements (for all achievement levels), and return the number of thme
int CAchievements::GetTotalAchievements()
{
	int total = 0;

	for (int i = 0;i < m_maxAchievements;i++)
	{
		if (m_obj[i]) total += (m_obj[i]->level+1);
	}

	return total;
}


// Count the achievements star for active levels.. plus completed achievements (3 stars per completed achievemeent)
int CAchievements::GetTotalAchievementsCompleted()
{
	int total = 0;

	for (int i = 0;i < m_maxAchievements;i++)
	{
		
		if (m_obj[i])
		{
			// get achive achievement stars
			if (m_obj[i]->GetStars() == 3) total++;

			// Now include the completed achievement stars (3 for each)
			total += m_obj[i]->level;  // i.e. if we are on level 2, then it means we have 2 previous levels completed


		}

		
	}

	return total;
}

void CAchievements::ProcessVScrollBar()
{
	// get number of achievements
	int total = GetTotalAchievementsActive();

	// If we have less than or equal to 'maxAchievementsOnPage', then show a full scroll bar, that we cannot move (alpha it)
	if (total <= maxAchievementsOnPage)
	{
		vscrollHeight = vscrollMaxHeight;
	}
	else
	{
		// Reduce vscroll Height inversely proportional to number of achivement objects
		vscrollHeight = vscrollMaxHeight  * ((float)maxAchievementsOnPage / (float)total);
		vscrollHeight -= 14;
	}

	
	// If our Mouse is Down, check if we are moving, if so, then move scrollbar position 
	if (bMouseDown)
	{
		int dy = iMouseY - iMouseOldY;

		vscrollPos = vscrollOldPos - dy;
		
		
		// Boundaries
		if (vscrollPos < 0)
		{
			vscrollPos = 0;
			vscrollOldPos = vscrollPos;
			iMouseOldY = iMouseY;
		}

		if ( (vscrollPos + vscrollHeight) > vscrollMaxHeight)
		{
			vscrollPos = vscrollMaxHeight - vscrollHeight;
			vscrollOldPos = vscrollPos;
			iMouseOldY = iMouseY;
		}
	}

}

void CAchievements::RenderVScrollBar()
{
	
	// Draw Progress Bar BG (Grey Rectangle)



	D3DXCOLOR color;
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;



	HRESULT hr;



	D3DXMATRIX matWorld;
	D3DXMATRIX matScale;
	D3DXMATRIX matRotX;
	D3DXMATRIX matRotY;
	D3DXMATRIX matRotZ;
	D3DXMATRIX matTrans;

	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matWorld);




	// Calculate Scale matrix
	D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 1.0f);

	// Calculate rotation matrix
	D3DXMatrixRotationX(&matRotX, 0.0f);
	D3DXMatrixRotationY(&matRotY, 0.0f);
	D3DXMatrixRotationZ(&matRotZ, 0.0f);

	// Calculate a translation matrix
	D3DXMatrixTranslation(&matTrans, 512, 0, 0);
	matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;
	achievementsBGSpr->SetTransform(&matWorld);

	// get width of our ProgressPercent png
	D3DSURFACE_DESC desc;
	vScrollBarTex->GetLevelDesc(0, &desc);


	// Draw the background first (light grey
	RECT srcRect;
	srcRect.left = 0;
	srcRect.top = 0;
	srcRect.right = desc.Width;
	srcRect.bottom = desc.Height;

	color.r = 0.7;
	color.g = 0.7;
	color.b = 0.7;
	color.a = 1.0;
	achievementsBGSpr->Draw(vScrollBarTex, &srcRect, NULL, NULL, color);

	// Render Scroll bar.

	// Calculate a translation matrix
	D3DXMatrixTranslation(&matTrans, 513, vscrollPos, 0);
	matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;
	achievementsBGSpr->SetTransform(&matWorld);

	

	srcRect.left = 1;
	srcRect.top =  0;
	srcRect.right = desc.Width-2;
	srcRect.bottom = vscrollHeight;

	color.r = 0.0f;
	color.g = 0.55f;
	color.b = 1.0f;
	color.a = 1.0;
	achievementsBGSpr->Draw(vScrollBarTex, &srcRect, NULL, NULL, color);


}

void CAchievements::RenderAchievements()
{
	

	HRESULT hr;




	// Render Achievement Objects
	
	// This variable keeps track of our total achievement object height. If this exceeds vscrollMaxHeight, then stop rendering objects
	
	// must have at least one achievement to do this code
	if (m_obj[0] == NULL) return;

	int total = GetTotalAchievementsActive();
	
	int objectHeight = m_obj[0]->GetHeight() + acheivementObjectGap;
	float offsetY;
	
	if (vscrollHeight == vscrollMaxHeight) offsetY = 0;
	else
	{
		offsetY = (float)vscrollPos / (float)vscrollMaxHeight;
		offsetY *= ((float)total * (float)objectHeight);
	}
	
	int ypos;

	for (int i = 0;i<m_maxAchievements;i++)
	{
		if (m_obj[i])
		{
			// Bounding Clipping. If object is outside of our Viewing region, then don't bother drawing it
			// Top Clipping is -objectHeight
			// Bottom Clipping is vscrollMaxHeight

			ypos = i*objectHeight - offsetY;
			if (ypos > -objectHeight && ypos < vscrollMaxHeight)
			{
				m_obj[i]->SetPosition(0, i*objectHeight - offsetY);
				m_obj[i]->Render();
			}
		}
	}
	




	// Draw rectangle at bottom, to give the effect of clipping the last object
	D3DXCOLOR color;
	color.r = 1.0;
	color.g = 1.0;
	color.b = 1.0;
	color.a = 1.0;



	D3DXMATRIX matWorld;
	D3DXMATRIX matScale;
	D3DXMATRIX matRotX;
	D3DXMATRIX matRotY;
	D3DXMATRIX matRotZ;
	D3DXMATRIX matTrans;

	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matWorld);



	// Calculate Scale matrix
	D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 1.0f);

	// Calculate rotation matrix
	D3DXMatrixRotationX(&matRotX, 0.0f);
	D3DXMatrixRotationY(&matRotY, 0.0f);
	D3DXMatrixRotationZ(&matRotZ, 0.0f);

	// Calculate a translation matrix
	D3DXMatrixTranslation(&matTrans, 0, vscrollMaxHeight + 1, 0);
	matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;
	achievementsBGSpr->SetTransform(&matWorld);

	// get width of our ProgressPercent png
	D3DSURFACE_DESC desc;
	achievementClipTex->GetLevelDesc(0, &desc);


	// Draw the clipping area
	RECT srcRect;
	srcRect.left = 0;
	srcRect.top = 0;
	srcRect.right = desc.Width;
	srcRect.bottom = desc.Height;

	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;
	achievementsBGSpr->Draw(achievementClipTex, &srcRect, NULL, NULL, color);


	// Draw the footer

	// Calculate a translation matrix
	D3DXMatrixTranslation(&matTrans, 0, vscrollMaxHeight + 3, 0);
	matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;
	achievementsBGSpr->SetTransform(&matWorld);

	achievementFooterTex->GetLevelDesc(0, &desc);
	srcRect.left = 0;
	srcRect.top = 0;
	srcRect.right = desc.Width;
	srcRect.bottom = desc.Height;

	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;
	achievementsBGSpr->Draw(achievementFooterTex, &srcRect, NULL, NULL, color);


	

}




void CAchievements::RenderStatsSummary()
{
	// Total Achievments
	// Also Render achievement idx
	
	D3DXCOLOR color;

	char buffer[256];
	sprintf(buffer, "Completed");
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;
	g_pFontB->Draw(achievementsBGSpr, buffer, this->width / 2, 580, 0.0f, 1.0f, 0.4f, 0.4f, MGFX_FNTDRAW_CENTER, color);


	//int totalCompleted = GetTotalAchievementsCompleted();
	//int totalAchievements = GetTotalAchievements();
	
	
	// sprintf(buffer, "%d%d", 55, 66);
	//sprintf(buffer, "55\/66");
	
	sprintf(buffer, "%d\/%d", GetTotalAchievementsCompleted(), GetTotalAchievements());

	
	
	//Mmain_log("***BUG***");
	// Mmain_log(buffer);
	//Mmain_log("*********");
	
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;
	g_pFontA->Draw(achievementsBGSpr, buffer, this->width / 2, 600, 0.0f, 1.0f, 0.6f, 0.6f, MGFX_FNTDRAW_CENTER, color);
	

	

}

void CAchievements::Process()
{
	// Achievement Process
	for (int i = 0;i<m_maxAchievements;i++)
	{
		if (m_obj[i])
		{
			m_obj[i]->Process();
		}
	}

	ProcessVScrollBar();
}


void CAchievements::Render2D()
{

#ifndef MAIN_SERVER


	if (!bInitialised) return;

	// Check that our Window is visible before bothering with rendering
	if (!hWnd) return;
	else
	{
		if (::IsWindowVisible(hWnd) == false) return;
	}

	if (!m_pDevice) return;
	//
	// Set the projection matrix.
	//



	m_pDevice->SetRenderState(D3DRS_CLIPPING, false);


	HRESULT hr;



	// Set Draw Target
	// hr = m_pDevice->SetRenderTarget(0, pSurface);
	

	// Now we can clear just view-port's portion of the buffer to red...
	hr = m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET,
		backgroundCol, 1.0f, 0L);



	

	m_pDevice->BeginScene();

	
	
	achievementsBGSpr->Begin(D3DXSPRITE_ALPHABLEND);
	
	
	RenderAchievements();
	RenderVScrollBar();
	
	RenderStatsSummary();


	achievementsBGSpr->End();

	m_pDevice->EndScene();


	
	CRect srect;
	CRect drect;
	::GetWindowRect(m_hWnd, srect);
	/*
	srect.left = 0;
	srect.top = 0;
	srect.right = mainViewPort.Width;
	srect.bottom = mainViewPort.Height;
	*/

	drect.left = 0;
	drect.top = 0;
	drect.right = 200;
	drect.bottom = 200;

	hr = m_pDevice->Present(NULL, NULL, hWnd, NULL);
	
	// hr = m_pSwapChain->Present(NULL, NULL, hWnd, NULL, NULL);
	
#endif


	// m_pDevice->SetRenderState(D3DRS_CLIPPING, false);

	// Base Class Rendering (for Background)
	CWindowBase::Render2D();

}


void CAchievements::SetAchievementPercent(int achievementIdx, int percent)
{
	
	if(m_obj[achievementIdx])
	{
		m_obj[achievementIdx]->SetPercent(percent);
	}

}



void CAchievements::RemoveAllAchievements()
{
	for (int i = 0;i < m_maxAchievements;i++)
	{
		m_obj[i] = NULL;
	}
	
}


#define ACHIEVEMENT_HIGHSTREAK						0
#define ACHIEVEMENT_WINS							1
#define ACHIEVEMENT_HIGHBREAK_SEMI					2
#define ACHIEVEMENT_CENTURIES_SEMI					3
#define ACHIEVEMENT_147_SEMI						4
#define ACHIEVEMENT_HIGHBREAK_PRO					5
#define ACHIEVEMENT_CENTURIES_PRO					6
#define ACHIEVEMENT_147_PRO							7
#define ACHIEVEMENT_RATING							8
#define ACHIEVEMENT_TOURNAMENTWINS					9




void CAchievements::CreateAchievements(Player plr)
{

	// Make sure achievements are all removed
	RemoveAllAchievements();


	// High Streak
	if (plr.highstreak < 3) CreateAchievement("Skilled Streak", "Win 3 consecutive games", 0); // HIGHSTREAK
	else if (plr.highstreak < 5) CreateAchievement("Master Streak", "Win 5 consecutive games", 1); // HIGHSTREAK
	else if (plr.highstreak < 10) CreateAchievement("Elite Streak", "Win 10 consecutive games", 2); // HIGHSTREAK
	else CreateAchievement("Eternal Streak", "Win 20 consecutive games", 3); // HIGHSTREAK


	if (plr.wins < 1) CreateAchievement("First Win", "Win your first game", 0); // ACHIEVEMENT_WINS
	else if (plr.wins < 10) CreateAchievement("Decade Wins", "Win 10 Games", 1); // ACHIEVEMENT_WINS
	else if (plr.wins < 100) CreateAchievement("Century Wins", "Win 100 Games", 2); // ACHIEVEMENT_WINS
	else if (plr.wins < 1000) CreateAchievement("Millennium Wins", "Win 1000 Games", 3); // ACHIEVEMENT_WINS
	else if (plr.wins < 10000) CreateAchievement("Eternal Wins", "Win 10,000 Games", 4); // ACHIEVEMENT_WINS
	else CreateAchievement("Epic Wins", "Win 50,000+ Games", 5); // ACHIEVEMENT_WINS



	if (plr.HighBreak < 30) CreateAchievement("Rookie High Break SP", "Semi Pro Break of 30 or more", 0); // ACHIEVEMENT_HIGHBREAK_SEMI
	else if (plr.HighBreak < 50) CreateAchievement("Skilled High Break SP", "Semi Pro Break of 50 or more", 1); // ACHIEVEMENT_HIGHBREAK_SEMI
	else if (plr.HighBreak < 100) CreateAchievement("Master High Break SP", "Semi Pro Break of 100 or more", 2); // ACHIEVEMENT_HIGHBREAK_SEMI
	else CreateAchievement("Elite High Break SP", "Semi Pro Break of 140 or more", 3);	// ACHIEVEMENT_HIGHBREAK_SEMI


	if (plr._100s < 1) CreateAchievement("Century Rookie SP", "Make a Century Break", 0); // ACHIEVEMENT_CENTURIES_SEMI
	else if (plr._100s < 10) CreateAchievement("Century Wizard SP", "Make 10 Century Breaks", 1); // ACHIEVEMENT_CENTURIES_SEMI
	else if (plr._100s < 50) CreateAchievement("Century King SP", "Make 50 Century Breaks", 2); // ACHIEVEMENT_CENTURIES_SEMI
	else if (plr._100s < 250) CreateAchievement("Century Champion SP", "Make 250 Century Breaks", 3); // ACHIEVEMENT_CENTURIES_SEMI
	else if (plr._100s < 1000) CreateAchievement("Century Master SP", "Make 1,000 Century Breaks", 4); // ACHIEVEMENT_CENTURIES_SEMI
	else CreateAchievement("Epic Centuries SP", "Make 20,000 Century Breaks", 5); // ACHIEVEMENT_CENTURIES_SEMI


	CreateAchievement("Maxi Expert", "Make a 147 on Semi Pro", 0);	// ACHIEVEMENT_147_SEMI








	if (plr.pHighBreak < 30) CreateAchievement("Rookie High Break Pro", "Pro Break of 30 or more", 0); // ACHIEVEMENT_HIGHBREAK_PRO
	else if (plr.pHighBreak < 50) CreateAchievement("Skilled High Break Pro", "Pro Break of 50 or more", 1); // ACHIEVEMENT_HIGHBREAK_PRO
	else if (plr.pHighBreak < 100) CreateAchievement("Master High Break Pro", "Pro Break of 100 or more", 2); // ACHIEVEMENT_HIGHBREAK_PRO
	else CreateAchievement("Elite High Break Pro", "Pro Break of 140 or more", 3); // ACHIEVEMENT_HIGHBREAK_PRO


	if (plr._p100s < 1) CreateAchievement("Century Rookie Pro", "Make a Pro Century Break", 0); // ACHIEVEMENT_CENTURIES_SEMI
	else if (plr._p100s < 10) CreateAchievement("Century Wizard Pro", "Make 10 Pro Century Breaks", 1); // ACHIEVEMENT_CENTURIES_SEMI
	else if (plr._p100s < 50) CreateAchievement("Century King Pro", "Make 50 Pro Century Breaks", 2); // ACHIEVEMENT_CENTURIES_SEMI
	else if (plr._p100s < 250) CreateAchievement("Century Champion Pro", "Make 250 Pro Century Breaks", 3); // ACHIEVEMENT_CENTURIES_SEMI
	else if (plr._p100s < 1000) CreateAchievement("Century Master Pro", "Make 1,000 Pro Century Breaks", 4); // ACHIEVEMENT_CENTURIES_SEMI
	else CreateAchievement("Epic Centuries Pro", "Make 20,000 Pro Century Breaks", 5); // ACHIEVEMENT_CENTURIES_SEMI


	CreateAchievement("Maxi Expert", "Make a 147 on Pro", 0); // ACHIEVEMENT_147_PRO



	// Rating
	if (plr.rating < 1500) CreateAchievement("Rookie", "Reach a rating of 1500", 0); // ACHIEVEMENT_RATING
	else if (plr.rating < 1800) CreateAchievement("Skilled", "Reach a rating of 1800", 1); // ACHIEVEMENT_WINS
	else if (plr.rating < 2100) CreateAchievement("Master", "Reach a rating of 2100", 2); // ACHIEVEMENT_WINS
	else CreateAchievement("Elite", "Reach a rating of 2400", 3); // ACHIEVEMENT_WINS



	// Tournament Wins
	if (plr.winner < 1) CreateAchievement("Tourney Victor", "Win a daily Tournament", 0); // TOURNAMENT_WINS
	else if (plr.winner < 5) CreateAchievement("Tourney Wizard", "Win 5 daily Tournaments", 1); // TOURNAMENT_WINS
	else if (plr.winner < 20) CreateAchievement("Tourney Champion", "Win 20 daily Tournaments", 2); // ACHIEVEMENT_WINS
	else if (plr.winner < 100) CreateAchievement("Epic Master", "Win 100 daily Tournaments", 3); // ACHIEVEMENT_WINS
	else CreateAchievement("Tourney King", "Win over 1,000 daily Tournaments", 4); // ACHIEVEMENT_WINS



	



}


// Check our Profile and Update our achievements accordingly
void CAchievements::UpdateAchievements(char *title, Player plr)
{
	
	// Update Window Title
	SetWindowText(hWnd, TEXT(title));


	

	
	//Player plr;
	int tempVal;








	// High Streak

	// 3 Wins
	// 5 Wins
	// 10 Wins
	// 20 Wins

	if (plr.highstreak < 3) tempVal = ((float)plr.highstreak / 3) * 100;
	else if (plr.highstreak < 5) tempVal = tempVal = ((float)plr.highstreak / 5) * 100;
	else if (plr.highstreak < 10) tempVal = tempVal = ((float)plr.highstreak / 10) * 100;
	else tempVal = ((float)plr.highstreak / 20) * 100;

	SetAchievementPercent(ACHIEVEMENT_HIGHSTREAK, tempVal);
	

	// CreateAchievement("1st Win", "Win your first game");
	// Check if we have at least a win on our stats. If so, set this achievement to 3 stars. (COMPLETE)

	// 1 win
	// 10+ wins
	// 100+ wins
	// 1000+ wins
	// 10,000+ wins
	// 50,000+ wins

	if (plr.wins <= 1) tempVal = ((float)plr.wins / 1) * 100;
	else if (plr.wins <= 10) tempVal = ((float)plr.wins / 10) * 100;
	else if (plr.wins <= 100) tempVal = ((float)plr.wins / 100) * 100;
	else if (plr.wins <= 1000) tempVal = ((float)plr.wins / 1000) * 100;
	else if (plr.wins <= 10000) tempVal = ((float)plr.wins / 10000) * 100;
	else tempVal = ((float)plr.wins / 50000) * 100;

	if (tempVal > 100) tempVal = 100;
	if (plr.wins > 0) SetAchievementPercent(ACHIEVEMENT_WINS, tempVal);
	
	

	// Centuries Semi Pro

	// 1 
	// 10 
	// 50 
	// 250 
	// 1000 
	// 20000 
	if (plr._100s < 1) tempVal = ((float)plr._100s / 1) * 100;
	else if (plr._100s < 10) tempVal = ((float)plr._100s / 10) * 100;
	else if (plr._100s < 50) tempVal = ((float)plr._100s / 50) * 100;
	else if (plr._100s < 250) tempVal = ((float)plr._100s / 250) * 100;
	else if (plr._100s < 2000) tempVal = ((float)plr._100s / 1000) * 100;
	else if (plr._100s < 20000) tempVal = ((float)plr._100s / 10000) * 100;
	if (tempVal > 100) tempVal = 100;
	SetAchievementPercent(ACHIEVEMENT_CENTURIES_SEMI, tempVal);



	// Centuries ACHIEVEMENT_CENTURIES_PRO

	// 1 
	// 10 
	// 50 
	// 250 
	// 1000 
	// 20000 
	if (plr._p100s < 1) tempVal = ((float)plr._p100s / 1) * 100;
	else if (plr._p100s < 10) tempVal = ((float)plr._p100s / 10) * 100;
	else if (plr._p100s < 50) tempVal = ((float)plr._p100s / 50) * 100;
	else if (plr._p100s < 250) tempVal = ((float)plr._p100s / 250) * 100;
	else if (plr._p100s < 2000) tempVal = ((float)plr._p100s / 1000) * 100;
	else if (plr._p100s < 20000) tempVal = ((float)plr._p100s / 10000) * 100;
	if (tempVal > 100) tempVal = 100;
	SetAchievementPercent(ACHIEVEMENT_CENTURIES_PRO, tempVal);


	


	// Semi Pro High Break (30+, 50+, 100+, 140+)
	// CreateAchievement("HIGH BREAK SEMI", "Semi Pro Break of 30 or more");
	// 10+ break = 1 star (34%)
	// 20+ break = 2 star (67%)
	// 30+ break = 3 star (100%)
	if (plr.HighBreak < 30) tempVal = ((float)plr.HighBreak / 30) * 100;
	else if (plr.HighBreak < 50) tempVal = ((float)plr.HighBreak / 50) * 100;
	else if (plr.HighBreak < 100) tempVal = ((float)plr.HighBreak / 100) * 100;
	else tempVal = ((float)plr.HighBreak / 140) * 100;
	if (tempVal > 100) tempVal = 100;
	SetAchievementPercent(ACHIEVEMENT_HIGHBREAK_SEMI, tempVal);


	// Semi Pro 147
	// CreateAchievement("147 SEMI", "Semi Pro Break of 147");
	tempVal = ((float)plr.HighBreak / 147) * 100;
	if (tempVal > 100) tempVal = 100;
	SetAchievementPercent(ACHIEVEMENT_147_SEMI, tempVal);



	// Pro High Break (30+, 50+, 100+, 140+)
	// CreateAchievement("HIGH BREAK SEMI", "Semi Pro Break of 30 or more");
	// 10+ break = 1 star (34%)
	// 20+ break = 2 star (67%)
	// 30+ break = 3 star (100%)
	if (plr.pHighBreak < 30) tempVal = ((float)plr.pHighBreak / 30) * 100;
	else if (plr.pHighBreak < 50) tempVal = ((float)plr.pHighBreak / 50) * 100;
	else if (plr.pHighBreak < 100) tempVal = ((float)plr.pHighBreak / 100) * 100;
	else tempVal = ((float)plr.pHighBreak / 140) * 100;
	if (tempVal > 100) tempVal = 100;
	SetAchievementPercent(ACHIEVEMENT_HIGHBREAK_PRO, tempVal);


	// Pro 147
	// CreateAchievement("147 SEMI", "Semi Pro Break of 147");
	tempVal = ((float)plr.pHighBreak / 147) * 100;
	if (tempVal > 100) tempVal = 100;
	SetAchievementPercent(ACHIEVEMENT_147_PRO, tempVal);
	


	// Rating 

	// 1500+ 
	// 1800+
	// 2100+
	// 2400+
	
	if (plr.highestrating < 1500) tempVal = ((float)plr.highestrating / 1500) * 100;
	else if (plr.highestrating < 1800) tempVal = ((float)plr.highestrating / 1800) * 100;
	else if (plr.highestrating < 2100) tempVal = ((float)plr.highestrating / 2100) * 100;
	else tempVal = ((float)plr.highestrating / 2400) * 100;
	SetAchievementPercent(ACHIEVEMENT_RATING, tempVal);



	// Tournament Wins

	// 1 Tournament Win
	// 5 Tournament Wins
	// 20 Tournament Wins
	// 100 Tournament Wins
	// 10000 Tournament Wins

	if (plr.winner < 1) tempVal = ((float)plr.winner / 1) * 100;
	else if (plr.winner < 5) tempVal = tempVal = ((float)plr.winner / 5) * 100;
	else if (plr.winner < 20) tempVal = tempVal = ((float)plr.winner / 20) * 100;
	else if (plr.winner < 100) tempVal = tempVal = ((float)plr.winner / 100) * 100;
	else tempVal = ((float)plr.winner / 1000) * 100;
	
	SetAchievementPercent(ACHIEVEMENT_TOURNAMENTWINS, tempVal);

	


}