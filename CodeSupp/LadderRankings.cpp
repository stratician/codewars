
#include "../CodeSupp/build.h"
#include "../CodeSupp/stdafx.h"
#include "../CodeSupp/GameAppView.h"	

#include "LadderRankings.h"
#include <iostream>





LRESULT CALLBACK CLadderRankings::WndProcBaseLadder(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#ifndef MAIN_SERVER

	switch (msg)
	{


	case WM_CLOSE:
		DestroyWindow(hwnd);
		if (g_gameAppView->ladderRankings)
		{
			g_gameAppView->ladderRankings->OnClose();
			delete(g_gameAppView->ladderRankings);
			g_gameAppView->ladderRankings = NULL;
		}
		break;


	case WM_LBUTTONDOWN:
		if (g_gameAppView->ladderRankings)
		{
			g_gameAppView->ladderRankings->MouseDown(true);
			SetCapture(hwnd);
		}
		break;

	case WM_LBUTTONUP:
		if (g_gameAppView->ladderRankings)
		{
			g_gameAppView->ladderRankings->MouseDown(false);
			ReleaseCapture();
		}
		
		break;

	case WM_MOUSEMOVE:
		if (g_gameAppView->ladderRankings)
		{
			g_gameAppView->ladderRankings->MouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
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

CLadderRankings::CLadderRankings()
{
	// Constructor


	m_pParent = NULL;

	backgroundCol = D3DCOLOR_COLORVALUE(0.3f, 0.3f, 0.3f, 1.0f);
}


CLadderRankings::~CLadderRankings()
{
	d3d::Delete<Font*>(m_pFontB);
	d3d::Delete<Font*>(m_pFontA);


	
	rankingsBGTex->Release();
	rankingsBGSpr->Release();

	::CWindowBase::Destroy();
	
 	
}


void CLadderRankings::PassParent(CWnd *pParent)
{
	m_pParent = pParent;
}


void CLadderRankings::OnClose()
{
	// Close Window
	CWindowBase::OnClose();
}

bool bRegisteredClassLadder = false;

int CLadderRankings::Create(HINSTANCE hInstance, HWND parentWnd, char *title, int x, int y, int w, int h, std::array<ladderObject, 20> ladderRanks)
{

	Mmain_log("CAchievements Create [1.0]");

	CWindowBase::Create(hInstance, parentWnd, title, x, y, w, h);

	// Create a windows dialog (basic)

	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;

	MSG msg;
	WNDCLASSEX wc;

	//Step 1: Registering the Window Class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProcBaseLadder;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "BaseWindowLadder";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


	if (!bRegisteredClassLadder)
	{
		if (!RegisterClassEx(&wc))
		{
			MessageBox(NULL, "Window Registration Failed!", "Error!",
				MB_ICONEXCLAMATION | MB_OK);
			return 0;
		}
	}
	Mmain_log("CAchievements Create [1.1]");

	bRegisteredClassLadder = true;

	hWnd = CreateWindowEx(NULL, "BaseWindowLadder", TEXT(title), WS_OVERLAPPEDWINDOW&~(WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX), x, y, w, h, parentWnd, NULL, hInstance, NULL);
	Mmain_log("CAchievements Create [1.1.1]");


	ShowWindow(hWnd, SW_SHOW);
	// UpdateWindow(hWnd);


	InitD3D();

	
	bool result;
	// Create Font
	m_pFontA = new Font(m_pDevice);
	m_pFontB = new Font(m_pDevice);
	result = m_pFontA->Load(1024, 150, 5, "Graphics/HUD/font5.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ,.<>/\\?;:1234567890!@#$%^&*()-_=+[]abcdefghijklmnopqrstuvwxyz{}");
	result = m_pFontB->Load(1024, 150, 12, "Graphics/HUD/font7.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ,.<>/\\?;:1234567890!@#$%^&*()-_=+[]abcdefghijklmnopqrstuvwxyz{}");
	
	HRESULT hr;

	
	hr = D3DXCreateTextureFromFile(m_pDevice, "Graphics/images/ladderrankingsbg.png", &rankingsBGTex);
		
	
	//hr = D3DXCreateTextureFromFileEx(m_pDevice, "Graphics/images/achievementbg.png", 512, 64, 0, D3DPOOL_DEFAULT, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &achievementsBGTex);
	D3DXCreateSprite(m_pDevice, &rankingsBGSpr);


	UpdateLadderRankings(title, ladderRanks);


	bInitialised = true;
}


void CLadderRankings::UpdateLadderRankings(char *title, std::array<ladderObject, 20> ladderRanks)
{
	
	// Update Window Title
	SetWindowText(hWnd, TEXT(title));



	// Create the rankings information

	// copy over our ladder ranks data
	for (int i = 0; i < ladderRanks.size(); i++)
	{
		this->ladderRanks[i] = ladderRanks[i];
	}
	

}


int ladderPrizesVal[10] = {500, 400, 350, 3000, 250, 200, 150, 100, 50, 50};
char *ladderPrizes[10] = { "$5", "$4", "$3.50", "$3", "$2.50", "$2", "$1.50", "$1", "$0.50", "$0.50" };



void CLadderRankings::RenderLadderRank(char *name, int ladderPoints, int ladderRank, int x, int y)
{



	D3DXCOLOR color;
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;


	// If this is ourself, then make the background a tint of green

	HRESULT hr;

	// Check if name is us..
	if (g_gameAppView->m_playerSockIdx != -1)
	{
		if (g_gameAppView->m_playerIdx >= 0)
		{
			int pIdx = g_gameAppView->m_playerIdx;
			if (strcmpi(m_player[pIdx].name, name) == 0)
			{
				color.r = 0.4f;
				color.g = 1.0f;
				color.b = 0.4f;
				color.a = 1.0f;
			}
		}
	}




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
	D3DXMatrixTranslation(&matTrans, 0, y, 0);
	matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;
	rankingsBGSpr->SetTransform(&matWorld);

	// color.a = 0.2f;
	rankingsBGSpr->Draw(rankingsBGTex, NULL, NULL, NULL, color);
	// pos.y += 10;
	// achievementsBGSpr->Draw(achievementsBGTex, NULL, NULL, &pos, color);


	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;


	char buffer[128];


	sprintf(buffer, "%d", ladderRank);
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;
	m_pFontA->Draw(rankingsBGSpr, buffer, x, y + 5, 0.0f, 1.0f, 0.45f, 0.45f, 0, color);


	sprintf(buffer, name);
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;
	m_pFontA->Draw(rankingsBGSpr, buffer, x + 60, y + 5, 0.0f, 1.0f, 0.45f, 0.45f, 0, color);



	sprintf(buffer, "%d", ladderPoints);
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;
	m_pFontA->Draw(rankingsBGSpr, buffer, x + 300, y + 5, 0.0f, 1.0f, 0.45f, 0.45f, 0, color);

	// Prizes are only for iSnooker at momment
	if (g_build.gameType == GAMETYPE_SNOOKER)
	{
		if (ladderRank <= 10)
		{
			sprintf(buffer, "%s", ladderPrizes[ladderRank - 1]);
			color.r = 1.0f;
			color.g = 0.7f;
			color.b = 0.4f;
			color.a = 1.0f;
			m_pFontA->Draw(rankingsBGSpr, buffer, x + 380, y + 5, 0.0f, 1.0f, 0.40f, 0.40f, 0, color);

		}
	}
	

	
}

char *timestr;

void CLadderRankings::RenderLadderRankings()
{
	
	// Display the Top 20 Rankings for now




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
	D3DXMatrixTranslation(&matTrans, 100, 20, 0);
	matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;
	rankingsBGSpr->SetTransform(&matWorld);

	


	char buffer[256];
	sprintf(buffer, "Ladder Rankings - Top 20");
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;
	m_pFontA->Draw(rankingsBGSpr, buffer, this->width / 2 - 20, 10, 0.0f, 1.0f, 0.6f, 0.6f, MGFX_FNTDRAW_CENTER, color);

	// Render Time info

	// Get (Local) time
	
	
	CTime m_time = g_gameAppView->GetServerTime();

	
	CString m_stringTimeStamp = m_time.Format("%A, %B %d - %H:%M:%S");



	timestr = m_stringTimeStamp.GetBuffer(0);
	// sprintf(buffer, "Ladder Rankings - Top 20");
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;
	m_pFontB->Draw(rankingsBGSpr, timestr, this->width / 2 - 20, 36, 0.0f, 1.0f, 0.4f, 0.4f, MGFX_FNTDRAW_CENTER, color);

	
	m_pFontB->Draw(rankingsBGSpr, "(Next season, Sun midnight GMT)", this->width / 2 - 20, 55, 0.0f, 1.0f, 0.4f, 0.4f, MGFX_FNTDRAW_CENTER, color);

	
	

	// Show the top 10
	for (int i = 0; i < ladderRanks.size(); i++)
	{
		RenderLadderRank(ladderRanks[i].name, ladderRanks[i].ladderPoints, ladderRanks[i].ladderRank, 65, 85 + i * 26);
	}


	if (g_gameAppView->m_playerSockIdx != -1)
	{
		if (g_gameAppView->m_playerIdx >= 0)
		{
			int pIdx = g_gameAppView->m_playerIdx;

			int xoffset = 120;
			int yoffset = 620;
			// Show your Ranking information
			sprintf(buffer, "My Ladder Stats");
			color.r = 1.0f;
			color.g = 1.0f;
			color.b = 1.0f;
			color.a = 1.0f;
			m_pFontB->Draw(rankingsBGSpr, buffer, xoffset, yoffset, 0.0f, 1.0f, 0.6f, 0.6f, 0, color);


			sprintf(buffer, "Name: %s", m_player[pIdx].name);
			// sprintf(buffer, "Name: %s", "Stratician");
			color.r = 1.0f;
			color.g = 1.0f;
			color.b = 1.0f;
			color.a = 1.0f;
			m_pFontB->Draw(rankingsBGSpr, buffer, xoffset, yoffset + 30, 0.0f, 1.0f, 0.4f, 0.4f, 0, color);

			sprintf(buffer, "Ladder Rank: %d", m_player[pIdx].ladderRank); // rank starts from 0
			// sprintf(buffer, "Ladder Rank: %d", 44);
			color.r = 1.0f;
			color.g = 1.0f;
			color.b = 1.0f;
			color.a = 1.0f;
			m_pFontB->Draw(rankingsBGSpr, buffer, xoffset, yoffset + 50, 0.0f, 1.0f, 0.4f, 0.4f, 0, color);

			sprintf(buffer, "Ladder Points: %d", m_player[pIdx].ladderPoints);
			// sprintf(buffer, "Ladder Points: %d", 65431);
			color.r = 1.0f;
			color.g = 1.0f;
			color.b = 1.0f;
			color.a = 1.0f;
			m_pFontB->Draw(rankingsBGSpr, buffer, xoffset, yoffset + 70, 0.0f, 1.0f, 0.4f, 0.4f, 0, color);

			sprintf(buffer, "Highest Semi Pro Break: %d", m_player[pIdx].ladderHighBreak&0x0000ffff);
			// sprintf(buffer, "Highest Semi Pro Break: %d", 125);
			color.r = 1.0f;
			color.g = 1.0f;
			color.b = 1.0f;
			color.a = 1.0f;
			m_pFontB->Draw(rankingsBGSpr, buffer, xoffset, yoffset + 90, 0.0f, 1.0f, 0.4f, 0.4f, 0, color);

			sprintf(buffer, "Highest Pro Break: %d", (m_player[pIdx].ladderHighBreak & 0xffff0000)>>16);
			// sprintf(buffer, "Highest Pro Break: %d", 69);
			color.r = 1.0f;
			color.g = 1.0f;
			color.b = 1.0f;
			color.a = 1.0f;
			m_pFontB->Draw(rankingsBGSpr, buffer, xoffset, yoffset + 110, 0.0f, 1.0f, 0.4f, 0.4f, 0, color);


		}
	}
}




void CLadderRankings::Render2D()
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

	
	
	rankingsBGSpr->Begin(D3DXSPRITE_ALPHABLEND);
	
	
	RenderLadderRankings();
	

	rankingsBGSpr->End();

	m_pDevice->EndScene();


	hr = m_pDevice->Present(NULL, NULL, hWnd, NULL);
	
	
#endif

	// Base Class Rendering (for Background)
	CWindowBase::Render2D();

}




void CLadderRankings::SaveResults()
{
	// Only save if <laddersave></laddersave> is set
	if (!g_laddersave) return;

	FILE *fptr;

	fptr = fopen("ladderrankings.txt", "a");



	CTime m_time = g_gameAppView->GetServerTime();
	CString m_stringTimeStamp = m_time.Format("%B %d - %H:%M:%S");

	timestr = m_stringTimeStamp.GetBuffer(0);
	
	fprintf(fptr, "%s\n\n", timestr);

	// Get Ladder Rankings
	std::array<ladderObject, 20> data = GetLadderRankings();

	// Show the top 10
	for (int i = 0; i < data.size(); i++)
	{
		int rank = data[i].ladderRank;
		if(i <= 9) fprintf(fptr, "%s - pts:%d, rank:%d, prize:%s\n", data[i].name, data[i].ladderPoints, rank, ladderPrizes[rank - 1]);
			else fprintf(fptr, "%s - pts:%d, rank:%d\n", data[i].name, data[i].ladderPoints, rank);

	}

	fprintf(fptr, "-----------------------------------\n\n\n");

	fclose(fptr);



}


/* Update our player stats, for the Ladder Cash prizes that we receive */

void CLadderRankings::UpdatePlayerStats()
{
	for (int i = 0; i < ladderRanks.size(); i++)
	{
		// Check if player is online, if so, Update their Cash stat on game. and in database
		int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->FindPlayerIdx(ladderRanks[i].name);
		int realCash = 0;
		int rank = ladderRanks[i].ladderRank;


		if (plrIdx != -1)
		{
			realCash = m_player[plrIdx].realcash;
			realCash += ladderPrizesVal[rank - 1];
			// Player is online, so update their Cash Stat
			
			m_player[plrIdx].realcash = realCash;

			// Notify players of new virtual cash
			((CGameAppView *)m_pParent)->SendPlayerChangeData(plrIdx, PLAYERCHANGE_REALCASH);
		}

		// We must update the DataBase always, for a change in the realCash
		UpdateCash(ladderRanks[i].name, m_player[plrIdx].cash);

		
		
	}

}