// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CCore.h"
#include "CIngameMenu.h"



extern CCore *g_CCore;

CIngameMenu::CIngameMenu()
{
	itemSelect = 0;
	this->m_bIsActive = false;
	sprintf(newsBuff, "");
}

void Request(char* data)
{
	sprintf(g_CCore->GetIngameMenu()->newsBuff, data);
}

void CIngameMenu::Init()
{
	g_CCore->GetNetwork()->httpRequest(NULL, "http://vortrex.info/lhmp/news.php", Request);

	D3DXCreateFont(g_CCore->GetGraphics()->GetDevice(), CreateFont(80, 0, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 0, TEXT("arialbold")), &ahojmoj);
	D3DXCreateFont(g_CCore->GetGraphics()->GetDevice(), CreateFont(40, 0, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 0, TEXT("Arial")), &menuitem);
	D3DXCreateFont(g_CCore->GetGraphics()->GetDevice(), CreateFont(25, 0, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 0, TEXT("Arial")), &clock);
}
bool CIngameMenu::isActive()
{
	return this->m_bIsActive;
}

void CIngameMenu::setActive(bool b)
{
	this->m_bIsActive = b;
}
void CIngameMenu::Tick()
{
	if (this->isActive())
	{
		this->Render();
	}
}

void CIngameMenu::Render()
{
	Vector2D screen = g_CCore->GetGraphics()->GetResolution();
	g_CCore->GetGraphics()->FillARGB(0, 0, screen.x, screen.y, D3DCOLOR_ARGB(200, 0, 0, 0));
	g_CCore->GetGraphics()->DrawTextA("LH", 50, screen.y - 260, 0xffffffff, false, ahojmoj);
	g_CCore->GetGraphics()->DrawTextA("MP", 140, screen.y - 260, 0xffff0000, false, ahojmoj);


	g_CCore->GetGraphics()->DrawTextA("Version 1.0", 250, screen.y - 212, 0xffffffff, false);

	if (this->itemSelect == 0)
	{
		g_CCore->GetGraphics()->DrawTextA("Back to game (ESC)", 50, screen.y - 160, 0xffcccccc, false, menuitem);
		g_CCore->GetGraphics()->FillARGB(50, screen.y - 120, 315, 2, D3DCOLOR_XRGB(200, 200, 200));
	}
	else
		g_CCore->GetGraphics()->DrawTextA("Back to game (ESC)", 50, screen.y - 160, 0xffffffff, false, menuitem);
	if (this->itemSelect == 1)
	{
		g_CCore->GetGraphics()->DrawTextA("Exit", 50, screen.y - 120, 0xffcccccc, false, menuitem);
		g_CCore->GetGraphics()->FillARGB(50, screen.y - 80, 60, 2, D3DCOLOR_XRGB(200, 200, 200));
	}
	else
		g_CCore->GetGraphics()->DrawTextA("Exit", 50, screen.y - 120, 0xffffffff, false, menuitem);


	// right corner
	time_t rawtime;
	struct tm * timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	char buff[200];
	char sec[20];
	sprintf(sec, "%s'", this->ZeroFormat(timeinfo->tm_sec, buff));

	g_CCore->GetGraphics()->DrawTextA(this->ZeroFormat(timeinfo->tm_hour, buff), screen.x - 150, 20, 0xffffffff, false, menuitem);
	g_CCore->GetGraphics()->DrawTextA(":", screen.x - 110, 20, 0xffff0000, false, menuitem);
	g_CCore->GetGraphics()->DrawTextA(this->ZeroFormat(timeinfo->tm_min, buff), screen.x - 100, 20, 0xffffffff, false, menuitem);
	g_CCore->GetGraphics()->DrawTextA(sec, screen.x - 60, 32, 0xffffffff, false, clock);

	char* pch, *start = newsBuff;
	int count = 0;
	while (1 == 1)
	{
		pch = strchr(start, '\n');
		if (pch != NULL)
		{
			memcpy(buff, start, pch - start);
			buff[pch - start] = 0x0;
			if (count % 2 == 0)
				g_CCore->GetGraphics()->DrawTextA(buff, 50, 50 + (count * 20), 0xffffffff, false);
			else
				g_CCore->GetGraphics()->DrawTextA(buff, 50, 50 + (count * 20), 0xffff0000, false);
			start = ++pch;
			count++;
		}
		else
		{
			break;
		}
	}

}


void	CIngameMenu::OnArrowUp()
{
	itemSelect = !itemSelect;
}
void	CIngameMenu::OnArrowDown()
{
	itemSelect = !itemSelect;
}

void	CIngameMenu::OnPressEnter()
{
	if (this->isActive())
	{
		if (itemSelect == 0)
			this->setActive(false);
		else
		{
			g_CCore->ShutdownClient();
		}
	}
}

void	CIngameMenu::OnLostDevice()
{
	if (ahojmoj)
		ahojmoj->OnLostDevice();
	if (menuitem)
		menuitem->OnLostDevice();
	if (clock)
		clock->OnLostDevice();
}
void	CIngameMenu::OnResetDevice()
{
	if (ahojmoj)
		ahojmoj->OnResetDevice();
	if (menuitem)
		menuitem->OnResetDevice();
	if (clock)
		clock->OnResetDevice();
}


char*	CIngameMenu::ZeroFormat(int input, char* out)
{
	if (input > 9)
		sprintf(out, "%d", input);
	else
		sprintf(out, "0%d", input);
	return out;
}