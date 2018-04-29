// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef __CORE_H
#define __CORE_H

#include <stdio.h>
#include "CGameSync.h"
#include "CNetwork.h"
#include "CLocalPlayer.h"

#include <d3d8.h>				
#include "CChat.h"
#include "CGame.h"
#include "CGraphics.h"
#include "CEngineStack.h"
#include "CPedPool.h"
#include "CVehiclePool.h"
#include "CLog.h"
#include "CCrashHandler.h"
#include "CKeyboard.h"
#include "CIngameMenu.h"
#include "CColoredText.h"
#include "CPickupPool.h"
#include "CFileTransfer.h"
#include "CFileSystem.h"
#include "CNametags.h"
#include "CSquirrel.h"
#include "CSQImages.h"
#include "CSQFonts.h"
#include <time.h>

#include "../shared/tools.h"


class CCore
{
private:
	bool					Running;
	bool					GameLoaded;

	CFileTransfer			m_cFileTransfer;
	CFileSystem				m_cFileSystem;

	CNetworkManager			m_cNetwork;
	CGameSync				m_cGameSync;
	CLocalPlayer			m_cLocalPlayer;
	CChat					m_cChat;
	CGame					m_cGame;
	CGraphics				m_cGraphics;
	CEngineStack			m_cEngineStack;
	CPedPool				m_cPedPool;
	CVehiclePool			m_cVehPool;
	CLog					m_cLog;
	CCrashHandler			m_crashhandler;
	CKeyboard				m_cKeyboard;
	CIngameMenu				m_cIngameMenu;
	CPickupPool				m_cPickuppool;
	CNametags				m_cNametags;
	CSquirrel				m_cSquirrel;
	CSQImages				m_cSquirrelImages;
	CSQFonts				m_cSquirrelFonts;



public:
	CCore();
	~CCore();
	void					Run();
	void					Start();
	void					Pulse();
	void					PulseAfterSec();
	bool					IsRunning();
	void					SetRunning(bool);
	bool					IsLoaded();
	void					SetLoaded(bool);
	void					OnLostConnection();
	CChat*					GetChat();
	CNetworkManager*		GetNetwork();
	CGameSync*				GetGameSync();
	CLocalPlayer*			GetLocalPlayer();
	CGame*					GetGame();
	CGraphics*				GetGraphics();
	//CPlayerSync*			GetPlayerSync();
	CEngineStack*			GetEngineStack();
	CPedPool*				GetPedPool();
	CVehiclePool*			GetVehiclePool();
	CLog*					GetLog();
	CCrashHandler*			GetCrashHandler();
	CKeyboard*				GetKeyboard();
	CIngameMenu*			GetIngameMenu();
	CPickupPool*			GetPickupPool();
	CFileTransfer*			GetFileTransfer();
	CFileSystem*			GetFileSystem();
	CNametags*				GetNametags();
	CSquirrel*				GetSquirrel();
	CSQImages*				GetSquirrelImages();
	CSQFonts*				GetSquirrelFonts();

	bool					m_bIsGameLoaded;
	bool					m_bIsRespawning;         

	bool					TestStop;

	void					ShutdownClient();

};



#endif