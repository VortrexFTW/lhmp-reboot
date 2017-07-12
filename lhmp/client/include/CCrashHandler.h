// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef __CCRASH_HANDLER
#define __CCRASH_HANDLER

#include "../../RakNet/RakPeerInterface.h"
#include "../../RakNet/MessageIdentifiers.h"
#include "../../RakNet/BitStream.h"
#include "../../RakNet/RakNetTypes.h"
#include "../../RakNet/RakSleep.h"
#include "../../RakNet/TCPInterface.h"
#include <mutex>
using namespace RakNet;
class CCrashHandler
{
public:
	void	Prepare();

	void	SaveDumpOnDisk(char*);
	void	SendReport(char*);
	std::mutex	mtx;
private:
};
#endif