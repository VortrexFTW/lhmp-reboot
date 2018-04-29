// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CHooks.h"
#include "CCore.h"
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <tchar.h>
#include <psapi.h>
#include <detours.h>

extern CCore* g_CCore;

DWORD returnBack = 0x00601269;
DWORD returnSkip = 0x0060104A;
DWORD returnbad = 0x00601262;

bool ignore = true;

__declspec(naked) void SpawnManager()
{
	_asm
	{
		cmp ebx, 1
			jnz good
			MOV EDX, DWORD PTR SS : [ESP + 0x214]; 0060125B
			jmp DWORD PTR SS : [returnbad]
		good :
			 pushad
	}
	if (g_CCore->GetGame()->GameSpawnManager() == 0)
	{
		_asm
		{
			popad
				jmp DWORD PTR SS : [returnBack]
		}
	}
	else

	{
		_asm
		{
			popad
				jmp DWORD PTR SS : [returnSkip]
		}
	}

}

__declspec(naked) void MainThreadTick()
{
	if (g_CCore != NULL)
		g_CCore->GetGame()->Tick();

	_asm mov eax, dword ptr ds : [0x6F9528];
	_asm ret;
}
__declspec(naked) void SetIsLoaded(){
	*(byte*)(0x101C16AD) = 1;		// game will run after alt tab
	g_CCore->SetLoaded(true);
	
	if (g_CCore != NULL)
	{
		g_CCore->m_bIsGameLoaded = true;
	}
	g_CCore->GetCrashHandler()->Prepare();
	//g_CCore->GetKeyboard()->ApplyHook();
	
	_asm ret;
}
__declspec(naked) void Freeride_ChangeCarSpawn(){
	_asm {
		mov DWORD PTR DS : [eax], 0x0			//posX
			mov DWORD PTR DS : [eax + 0x4], 0x0		//posY
			mov DWORD PTR DS : [eax + 0x8], 0x0		//posZ

			ret
	}
}
__declspec(naked) void Hook_respawn()
{
	_asm pushad;
	g_CCore->GetGame()->PreRespawn();
	_asm {
		popad
			mov eax, 0x4FD
			ret
	}
}
_declspec(naked) void Respawn()
{
	_asm pushad
	if (g_CCore->IsRunning())
		g_CCore->GetGame()->Respawn();
	_asm
	{
		popad
			PUSH 0x005F97F2
			ret
	}
}
_declspec(naked) void AfterRespawn()
{
	_asm pushad
	g_CCore->GetGame()->AfterRespawn();

	_asm
	{
		popad
			PUSH 0x64D7A0			// GameAfterLoad or what
			push 0x00558D79
			ret
	}
}

_declspec(naked) void Hook_ThrowAwayWeapon()
{
	_asm pushad
	g_CCore->GetGame()->ThrowAwayWeapon();
	_asm
	{
		popad
			MOV WORD PTR DS : [ESI], 0;  odhodi zbran // departures collected
			push 0x0055B636
			ret
	}
}
void TakeWeapon(DWORD wepId, DWORD wepLoaded, DWORD wepHidden)
{
	char buff[255];
	sprintf(buff, "TakeWep: %i %i %i", wepId, wepLoaded, wepHidden);
	g_CCore->GetLog()->AddLog(buff);
	g_CCore->GetGameSync()->Engine_onPlayerTakeWeapon(wepId, wepLoaded, wepHidden);
}
_declspec(naked) void Hook_TakeWeaponFirst()
{
	_asm
	{
		pushad
		mov EAX, DWORD PTR DS : [EBP+0x8]
		push EAX
		mov EAX, DWORD PTR DS : [EBP+0x4]
		push EAX
		mov EAX, DWORD PTR DS : [EBP]
		push EAX
		mov EAX, TakeWeapon
		call EAX
		add ESP, 0xC
		popad
		MOV DWORD PTR DS : [ESI], EAX
		MOV ECX, DWORD PTR SS : [EBP + 4]
		push 0x0055B03A
		ret
	}
}
_declspec(naked) void Hook_TakeWeaponSecond()
{
	_asm
	{
		pushad
		mov EAX, DWORD PTR DS : [EBP + 0x8]
		push EAX
		mov EAX, DWORD PTR DS : [EBP + 0x4]
		push EAX
		mov EAX, DWORD PTR DS : [EBP]
		push EAX
		mov EAX, TakeWeapon
		call EAX
		add ESP, 0xC
		popad
		MOV DWORD PTR DS : [ESI], EAX;  zoberie zbran
		MOV ECX, DWORD PTR SS : [EBP + 4]
		push 0x0055AFEC
		ret
	}
}
_declspec(naked) void Hook_TakeWeaponThird()
{
	_asm
	{
		pushad
			mov EAX, DWORD PTR DS : [EBP + 0x8]
			push EAX
			mov EAX, DWORD PTR DS : [EBP + 0x4]
			push EAX
			mov EAX, DWORD PTR DS : [EBP]
			push EAX
			mov EAX, TakeWeapon
			call EAX
			add ESP, 0xC
		popad
			MOV DWORD PTR DS : [ESI], EDX
			MOV EAX, DWORD PTR SS : [EBP + 4]
			push 0x0055AEB1
			ret
	}
}
void ChangeWeapon(DWORD testPed,DWORD wepId)
{
	if ((g_CCore->GetLocalPlayer()->GetBase() + 0x4A0) == testPed)
	{
		char buff[255];
		sprintf(buff, "SwitchWep: %i", wepId);
		g_CCore->GetLog()->AddLog(buff);
		g_CCore->GetGameSync()->Engine_onChangeWeapon(wepId);
	}
}
_declspec(naked) void Hook_ChangeWep()
{
	_asm {
		MOV DWORD PTR DS : [ESI], ECX
			MOV EDX, DWORD PTR DS : [EAX + 4]
			pushad
			mov EAX, DWORD PTR DS : [ESI] // id
			PUSH EAX
			mov EAX, ESI // ped
			PUSH EAX
			mov EAX, ChangeWeapon
			call EAX
			add ESP, 0x8
		popad
			push 0x00559E4F
			ret
	}
}
_declspec(naked) void Hook_ChangeWep2()
{
	_asm {
		MOV DWORD PTR DS : [ESI], EDX
			MOV EDX, DWORD PTR DS : [ECX + 4]
			pushad
			mov EAX, DWORD PTR DS : [ESI] // id
			PUSH EAX
			mov EAX, ESI // ped
			PUSH EAX
			mov EAX, ChangeWeapon
			call EAX
			add ESP, 0x8
			popad
			push 0x00559D33
			ret
	}
}
_declspec(naked) void Hook_ChangeWep3()
{
	_asm {
		mov dword ptr ds : [esi], ebp
			MOV EBP, DWORD PTR DS : [EBX + 4]
			pushad
			mov EAX, DWORD PTR DS : [ESI] // id
			PUSH EAX
			mov EAX, ESI // ped
			PUSH EAX
			mov EAX, ChangeWeapon
			call EAX
			add ESP, 0x8
			popad
			push 0x00559E9D
			ret
	}
}

_declspec(naked) void Hook_ChangeWep4()
{
	_asm {
		mov dword ptr ds : [esi], edi
			pushad
			mov EAX, DWORD PTR DS : [ESI] // id
			PUSH EAX
			mov EAX, ESI // ped
			PUSH EAX
			mov EAX, ChangeWeapon
			call EAX
			add ESP, 0x8
			popad

			MOV EDX, EBP
			POP EDI
			MOV EAX, DWORD PTR DS : [EDX]
			MOV DWORD PTR DS : [ESI], EAX
			MOV ECX, DWORD PTR DS : [EDX + 4]

			push 0x00559ED7
			ret
	}
}

_declspec(naked) void Hook_HideWeapon()
{
	_asm {
			pushad
	}
	g_CCore->GetGame()->HideWeapon();
	_asm
	{
		popad
		MOV WORD PTR DS : [EBX], 0
		push 0x0055A534
		ret
	}
}

void OnShoot(DWORD testPed, float x, float y, float z)
{
	if ((g_CCore->GetLocalPlayer()->GetBase()) == testPed)
	{
		char buff[255];
		sprintf(buff, "On Shot: %f %f %f", x, y, z);
		g_CCore->GetLog()->AddLog(buff);

		Vector3D position = Vector3D(x, y, z);
		PED* ped = (PED*)g_CCore->GetLocalPlayer()->GetBase();
		g_CCore->GetGameSync()->Engine_onShoot(position, ped->inventary.slot[0].weaponType);
	}
}

_declspec(naked) void Hook_OnShoot()
{
	_asm {
		MOV EAX, 0x004A4B70
			CALL EAX; Game.004A4B70;  samotny vystrel //he shot himself
			cmp edx, 0x1
			jz goodCode
			cmp eax, 0x133
			jz end						// if EAX was 133, then shoot wasn't correct (and PED is in car)
			cmp edx, 0x007F007F
			jz end						// if EDX was 007F007F, then shoot wasn't correct (and PED is onfoot)
			cmp eax, 0x2
			jz goodCode						// successful shot (on foot)
			cmp eax, 0x3				// successful on foot
			jz goodCode
			cmp ecx, 0xA				// successful on foot
			jz goodCode
			cmp eax, 0x0
			jnz end						// successful shot (in car)
		goodCode :
		pushad
		mov EBP, DWORD PTR DS : [ESP + 0x34]
		MOV EAX, DWORD PTR DS : [EBP + 0x8]
		PUSH EAX;
		MOV EAX, DWORD PTR DS : [EBP + 0x4]
		PUSH EAX;
		MOV EAX, DWORD PTR DS : [EBP]
		PUSH EAX;
		PUSH ESI	// testPed
		mov EAX, OnShoot
		call EAX
		add ESP, 0x10
		popad
		end :
		push 0x004A3BC4
			ret
	}
}
void	OnPlayerHit(DWORD attacker)
{
	unsigned int ID = g_CCore->GetPedPool()->GetPedIdByBase(attacker);
	g_CCore->GetGameSync()->Engine_onPlayerGetHit(g_CCore->GetPedPool()->Return(ID));
}

_declspec(naked) void Hook_PreventHit()
{
	// ECX = attacker
	// ESI = victim

	if (g_CCore->GetGame()->ShouldKill)
	{
		_asm
		{
			fstp dword ptr[esi + 0x644]
				pushad
				push ECX
				call OnPlayerHit
				add ESP, 0x4
				popad
				ret
		}
	}
	else
	{
		_asm
		{
			cmp byte ptr[esi + 0x10], 0x1B
				jz ped
				fstp dword ptr[esi + 0x644]
				pushad
				push ECX
				call OnPlayerHit
				add ESP, 0x4
				popad
			ped :
			ret
		}
	}
}

void OnDeath(DWORD killerBase,unsigned char hitbox)
{
	int playerID = 0;
	playerID = g_CCore->GetPedPool()->GetPedIdByBase(killerBase);
	if (playerID != -1)
	{
		char buff[255];
		sprintf(buff, "Killed by 0x%i [Part %d]", playerID, hitbox);
		g_CCore->GetLog()->AddLog(buff);
		g_CCore->GetGameSync()->Engine_onPlayerDie(g_CCore->GetPedPool()->Return(playerID), hitbox);
	}
}

// death reason ESP+0x60
_declspec(naked) void Hook_OnDeath()
{
	_asm
	{
		MOV DWORD PTR DS : [ESI + 0x644], EDI
			cmp BYTE PTR SS : [ESI + 0x10], 0x2
			jnz ped
			cmp EBP, 0x0		// if EBP[killerBase] is 0 = suicide
			jz suicide
			// move DEATH part into EAX
		mov EAX, DWORD PTR DS : [ESP + 0x64]
		pushad
		PUSH EAX; // part - hitbox
		PUSH EBP; // killer - PED
		mov EAX, OnDeath
		call EAX
		add ESP, 0x8
		popad
			jmp ped
		suicide :
		pushad
	}
	g_CCore->GetGame()->OnSuicide();
	_asm
	{
		popad
		ped :
		ret
	}
}
_declspec(naked) void Hook_OnDeath2()
{
	_asm
	{
		MOV DWORD PTR DS : [EDI + 0x644], EDI
			cmp BYTE PTR SS : [EDI + 0x10], 0x2
			jnz ped
		pushad
	}
	//g_CCore->GetGame()->OnSuicide();
	g_CCore->GetLocalPlayer()->OnDeath(-1, DEATH_EXPLODEINCAR, 0);
	_asm
	{
		popad
		ped:
		push 0x004AAB7B
		ret
	}
}

// in car by damage
_declspec(naked) void Hook_OnDeath3()
{
	_asm
	{
		MOV DWORD PTR DS : [esi + 0x644], 0x0
		//MOV DWORD PTR DS : [EDI + 0x644], EDI
			cmp BYTE PTR SS : [ESI + 0x10], 0x2
			jnz ped
			pushad
	}
	g_CCore->GetGame()->OnSuicide();
	_asm
	{
		popad
		ped :
		push 0x004987C2
			ret
	}
}

// player sink
_declspec(naked) void Hook_OnDeath4()
{
	_asm
	{
		MOV DWORD PTR DS : [esi + 0x644], 0x0
			cmp BYTE PTR SS : [ESI + 0x10], 0x2
			jnz ped
			pushad
	}
	g_CCore->GetLocalPlayer()->OnDeath(-1, DEATH_DROWN, 0);
	_asm
	{
		popad
		ped :
		push 0x0049BFF4
		ret
	}
}
void PlayerEnteredVehicle(DWORD vehicle, DWORD seatID)
{
	char buff[255];
	int vehID = g_CCore->GetVehiclePool()->GetVehicleIdByBase(vehicle);
	CVehicle* veh = g_CCore->GetVehiclePool()->Return(vehID);
	if (veh)
	{
		veh->PlayerEnter(g_CCore->GetLocalPlayer()->GetOurID(), seatID);
		g_CCore->GetLocalPlayer()->SetIsOnFoot(0);
		sprintf(buff, "[PEV] vehicle %x, seat id: %d", vehicle, seatID);
		g_CCore->GetLog()->AddLog(buff);

		g_CCore->GetGameSync()->Engine_onPlayerEnterVehicle(veh, seatID);
	}
	else {
		g_CCore->GetGame()->ConsoleAddText("You are entering an unsynced vehicle !", 0xFFFFFFFF);
	}
}
_declspec(naked) void Hook_OnPlayerEnteredVehicle()
{
	_asm
	{
		PUSH 0; / Arg4 = 00000000
			PUSH EAX; | Arg3
			PUSH 1; | Arg2 = 00000001
			PUSH ESI; | Arg1
			MOV ECX, EBP; |
			MOV EAX, 0x004A26D0
			CALL EAX; Game.004A26D0; \Game.004A26D0
			cmp EAX, 0x4
			jge PEVped
			cmp BYTE PTR SS : [EBP + 0x10], 0x2
			jnz PEVped
			pushad
		PUSH EAX
		PUSH ESI
		mov EAX, PlayerEnteredVehicle
		call EAX
		add ESP, 0x8
		popad
		PEVped :
		push 0x004CC8B2
			ret
	}
}

_declspec(naked) void Hook_OnPlayerEnteredVehicle2()
{
	_asm
	{
		MOV EBP, ECX
			PUSH 1
			PUSH EAX
			PUSH 1
			PUSH ESI
			MOV EAX, 0x004A26D0
			CALL EAX; game.004A26D0
			cmp BYTE PTR SS : [EBP + 0x10], 0x2
			jnz PEVped
			mov EAX, 0x0
			pushad
			PUSH EAX
			PUSH ESI
			mov EAX, PlayerEnteredVehicle
			call EAX
			add ESP, 0x8
			popad
		PEVped :
		push 0x004CC8B2
			ret
	}
}

void PlayerExitVehicle(DWORD vehicle)
{
	char buff[255];

	int vehID = g_CCore->GetVehiclePool()->GetVehicleIdByBase(vehicle);
	CVehicle* veh = g_CCore->GetVehiclePool()->Return(vehID);
	if (veh)
	{
		veh->PlayerExit(g_CCore->GetLocalPlayer()->GetOurID());
		g_CCore->GetLocalPlayer()->SetIsOnFoot(1);
		sprintf(buff, "[PEV] Exit vehicle %x", vehicle);
		g_CCore->GetLog()->AddLog(buff);
		g_CCore->GetGameSync()->Engine_onPlayerExitVehicle(veh);
	}
}

_declspec(naked) void Hook_OnPlayerExitVehicle()
{
	_asm
	{
			PUSH ECX	

			PUSH EBX
			PUSH EBX
			PUSH 2
			PUSH ECX
			MOV ECX, EDI
			MOV EAX, 0x004A26D0
			CALL EAX; game.004A26D0
			POP ECX
			cmp BYTE PTR SS : [EDI + 0x10], 0x2
			jnz PEVped
			pushad
			PUSH ECX		// vehicle
			MOV EAX, PlayerExitVehicle
			call EAX
			add ESP, 0x4
		popad
		PEVped :
		push 0x004CCA6D
			ret
	}
}
_declspec(naked) void Hook_OnPlayerExitVehicleFinish()
{
	_asm
	{
		pushad
			cmp DWORD PTR DS : [ESI + 0x10], 0x2	// isLocalPlayer
			jnz somethingElse
	}
	g_CCore->GetGame()->PlayerExitVehicleFinish();
	_asm
	{
	somethingElse:
		popad
			CMP ECX, EBX
			MOV DWORD PTR DS : [ESI + 0x98], EBX
			mov DWORD PTR DS : [ESP], 0x00491EEB // we need to return a byte fahrer
			ret
	}
}
_declspec(naked) void Hook_VehicleDisableMove()
{
	_asm{
		pushad
			cmp BYTE PTR SS : [ESI - 0x60], 0x4		// isCar
			jnz nonCar
			//its car
			sub ESI, 0x70	// now ESI = vehBase
			MOV EAX, DWORD PTR DS : [0x6F9464]
			test eax, eax
			jz nonCar		// if PED doesnt exist
			MOV EAX, DWORD PTR SS : [EAX + 0xE4]
			MOV EAX, DWORD PTR SS : [EAX + 0x98]
			cmp EAX, ESI
			jz nonCar
			popad
			jmp exitVDM
		nonCar :
		popad
			PUSH ECX
			PUSH EDX
			PUSH EAX
			MOV ECX, ESI
			MOV EAX, 0x0052E6D0
			CALL EAX
		exitVDM :
		mov DWORD PTR SS : [ESP], 0x0052D311
			retn



	}
}
void OnCarJack(int carBase, int seatId)
{
	int carId = -1;
	carId = g_CCore->GetVehiclePool()->GetVehicleIdByBase(carBase);
	char buff[255];
	sprintf(buff, "Car jack ID: %i Seat: %i", carId, seatId);
	g_CCore->GetLog()->AddLog(buff);

	// TODO: move following sequence into GameSync class
	CVehicle* veh = g_CCore->GetVehiclePool()->Return(carId);
	if (veh != NULL)
	{
		if (veh->GetSeat(seatId) != -1)
		{
			CPed* ped = g_CCore->GetPedPool()->Return(veh->GetSeat(seatId));
			ped->InCar = -1;
			veh->PlayerExit(veh->GetSeat(seatId));
			ped->SetIsOnFoot(true);
		}
		g_CCore->GetGameSync()->Engine_onPlayerCarjack(veh, seatId);
	}
	
}

_declspec(naked) void Hook_OnCarJack()
{
	_asm
	{
		PUSH EDI; seat
			PUSH ECX; carBase
			MOV EBX, ECX
			MOV ECX, EBP; actorBase(the jacker - ten kto ho ide vyhodit)
			MOV EAX, 0x004A82E0
			CALL EAX; game.004A82E0
			cmp byte ptr ds : [EBP + 0x10], 0x2
			jnz endCarJack
			// our local player case:
			pushad
			push EDI
			PUSH EBX
			mov EAX, OnCarJack
			call EAX
			add ESP, 0x8
		popad
			// it's jump back
		endCarJack :
		push 0x004CC8B2
			ret
	}
}

_declspec(naked) void Hook_PreventCarandPedsSpawn()
{
	_asm
	{
		//cmp EAX, 0xC			// 0x15 - fake cars ?
		//	jz preventSpawn
		cmp EAX, 0x15			// 0x15 - dog
			jz preventSpawn
		cmp EAX, 0x12			// 0x12 - fake PEDs
		jz preventSpawn
		//test
		cmp EAX, 0x4			// 0x4 - car
			jz preventSpawn
			cmp EAX, 0x19		// 0x19 - pumper
			jz preventSpawn
			cmp EAX, 0x1B		// 0x1B	- PED
			jz preventSpawn
			jmp spawnIt
			// we need to destroy frame of non-spawned obj
		preventSpawn :
		pushad
			mov eax, edi
			push eax
			mov ecx, [eax]
			call dword ptr ds : [ecx]
			popad
			// tell engine not to spawn object
			push 0x0060409A
			ret
			// spawn object using normal way
		spawnIt :
		PUSH EAX
			MOV ECX, EBP
			MOV EAX, 0x005FED70
			CALL EAX; Game.005FED70
			push 0x0060404E
			ret
	}
}
_declspec(naked) void Hook_PEDSpawnCorrectPos()
{
	_asm
	{
		cmp BYTE PTR DS : [ESI + 0x10], 0x1B		// isPed
			jnz nonPED
			//MOV ECX, DWORD PTR DS : [EDX + 4]
			mov ECX, 0xc4f83eef
			MOV DWORD PTR DS : [EAX], ECX
			mov ECX, 0x40a12f8c
			MOV DWORD PTR DS : [EAX + 4], ECX
			//MOV EDX, DWORD PTR DS : [EDX + 8]
			MOV EDX, 0x40891d93
			MOV DWORD PTR DS : [EAX + 8], EDX
			jmp eitherEnd
		nonPED :
		MOV ECX, DWORD PTR DS : [EDX + 4]
			MOV DWORD PTR DS : [EAX + 4], ECX
			MOV EDX, DWORD PTR DS : [EDX + 8]
			MOV DWORD PTR DS : [EAX + 8], EDX
		eitherEnd :
		push 0x0044BB72
			ret
	}
}

_declspec(naked) void Hook_CollisionDamage()
{
	_asm
	{
		pushad
		mov eax, DWORD PTR DS : [0x006F9464]
		mov eax, DWORD PTR DS : [eax + 0xE4]

		mov eax, DWORD PTR DS : [eax + 0x98]
		mov edx, ecx
		sub edx, 0x70
		cmp edx, eax
		jne localIsNotInCar
		fst dword ptr[ecx + 0x234]
	}
	g_CCore->GetGame()->OnCollision();
	_asm {
		localIsNotInCar:
		popad
		push 0x00522FBA
		ret
	}
}
_declspec(naked) void Hook_CarShot()
{
	_asm
	{
		pushad
			mov eax, DWORD PTR DS : [0x006F9464]
			mov eax, DWORD PTR DS : [eax + 0xE4]
			cmp edx, ebp
			jne localIsNotInCar
			mov	[ebp + 0x2094], ecx
	}
	g_CCore->GetGame()->OnCarShot();
	_asm {
	localIsNotInCar:
		popad
			push 0x0046ADA6
			ret
	}
}

void OnThrowGranade(float x, float y, float z)
{
	char buff[250];
	sprintf(buff, "Throw: %f %f %f", x, y, z);
	g_CCore->GetLog()->AddLog(buff);

	Vector3D pos = Vector3D(x,y,z);
	g_CCore->GetGameSync()->Engine_onPlayerThrowGranade(pos);
	

}
_declspec(naked) void Hook_ThrowGranade()
{
	_asm
	{
		PUSH EBX
		PUSH 0x1
		PUSH EBX
		MOV ECX, ESI
		MOV DWORD PTR DS : [ESI + 0x74], 0x0A3
		MOV EAX, 0x004942C0
		CALL EAX; Game.004942C0
		pushad
		mov EAX, DWORD PTR DS : [ESI+0x10]
		CMP EAX, 0x2
		JNZ noLocalPlayer
		mov EAX, DWORD PTR DS : [ESI+0x208]
		PUSH EAX
		mov EAX, DWORD PTR DS : [ESI + 0x204]
		PUSH EAX
		mov EAX, DWORD PTR DS : [ESI + 0x200]
		PUSH EAX
		call OnThrowGranade
		add ESP, 0xC
		noLocalPlayer :
		popad
		PUSH 0x004A3FB8
		ret
	}
}

HMODULE WINAPI NEWLoadLibrary(LPCTSTR lpFileName)
{
	SetLastError(ERROR_ACCESS_DENIED);
	return NULL;
}

void OnEngineLoad()
{
	PBYTE loadlib = NULL;
	_asm {
		mov eax, LoadLibrary
			mov loadlib, eax
	}
}

_declspec(naked) void Hook_EngineLoad()
{
	_asm
	{
		pushad
			call OnEngineLoad
		popad
		MOV ECX, 0x0065138C;  ASCII "logo1.avi"
		RET
	}
}

void OnDoorStateChange(DWORD doorActor, int state)
{
	bool isLocked = *(bool*)(doorActor + 0x125);
	// if doors are not locked (and thus we can open/close them correctly)
	if (isLocked == false)
	{
		OBJECT* doors = (OBJECT*)(doorActor);
		char* actorName = g_CCore->GetGame()->GetFrameName(doors->frame);
		bool facing = *(bool*)(doorActor + 0x11E);

		char buff[255];
		sprintf(buff, "[Hook] DoorChange: %s %d %d", actorName, state, facing);
		g_CCore->GetLog()->AddLog(buff);
		g_CCore->GetLog()->AddLog(actorName);
		g_CCore->GetGameSync()->Engine_onDoorStateChange(actorName, state, facing);
	}
}

__declspec(naked) void Hook_OnDoorStateChange()
{
	_asm
	{
			PUSH EDX // state
			PUSH ECX // doorActor

			PUSH 0
			PUSH 0
			PUSH 0
			PUSH ECX
			MOV ECX, EBP
			MOV EAX, 0x004A26D0
			CALL EAX
			POP ECX
			POP EDX
			// start custom code
			pushad
			PUSH EDX
			PUSH ECX
			MOV EAX, OnDoorStateChange
			CALL EAX
			ADD ESP, 0x8
			popad
			// end custom code
			mov eax, 0x004CC8B2
			jmp eax
	}
}
char endofmissionScript[] = "dim_act 1\r\n"
							"dim_flt 1\r\n"
							"findactor 0, \"Tommy\"\r\n"
							"label 101\r\n"
							"human_getproperty 0, 0, Energy\r\n"
							"if flt[0] = 0, -1, 101\r\n"
							"endofmission 0, 00980004\r\n"
							"end\r\n"; 

char omg[] = "end\r\n";
char* OnScriptLoad(char* input)
{
	// now instead of returning just change input address
	if (strstr(input, "endofmission") != NULL || strstr(input, "player_lockcontrols") != NULL)
	{
		int len = strlen(endofmissionScript);
		char* script = new char[len + 1];
		strcpy(script, endofmissionScript);
		input = script;
		return script;
	}
	else if (strstr(input, "change_mission") != NULL)
	{
		int len = strlen(omg);
		char* script = new char[len + 1];
		strcpy(script, omg);
		input = script;
		return script;
	}

	g_CCore->GetLog()->AddLog("Loading script ORIGINAL !");
	return input;
}

// this is called afte OnScriptLoad
void OnScriptDeload(char* script)
{
	if (strcmp(script, "NoAnimPreload") != NULL)
	{
		_asm {
			//mov EDI, script
			PUSH EDI
			MOV EAX, 0x006243AC
			CALL EAX; 006243AC
			ADD ESP, 4
		}
	}
}

__declspec(naked) void Hook_OnScriptLoad()
{
	_asm {
		sub ESP, 0x4
		mov DWORD PTR DS : [ESP],EDI // pointer to script
		MOV EAX, ESP
		pushad
		push EAX	// push script pointer
		//push EDI
		call OnScriptLoad
		add ESP, 0x4
		popad
		// now ESP stores our param
		pop EDI			// pop EDI works as mov EDI, [ESP]

		PUSH EAX; / Arg4
		PUSH ECX; | Arg3
		PUSH EBX; | Arg2
		PUSH EDI; | Arg1 = 11C2AD50 script ktory sa bude prekladat
		MOV ECX, ESI;
		//pop edi
		PUSH 0x005AF869
		retn
	}
}

void HookLoadGameScript(DWORD a, DWORD script, DWORD b, DWORD c, DWORD d)
{
	char* newscript = OnScriptLoad((char*) script);
	g_CCore->GetLog()->AddLog("Loading script...");
	//g_CCore->GetLog()->AddLog(newscript);



	// loads script
	_asm {
		mov EAX, d
			PUSH EAX
			mov EAX, c
			PUSH EAX
			mov EAX, b
			PUSH EAX
			mov EAX, newscript
			PUSH EAX
			MOV ESI, a
			MOV ECX, ESI
			MOV EAX, 0x005BA6A0
			CALL EAX; Game.005BA6A0; \Game.005BA6A0
	}

	if ((char*) script == newscript)
	{
		// delete it if it's genuie
		_asm {
			PUSH EDI
				MOV EAX, 0x006243AC
				CALL EAX; Game.006243AC;  free script memory
				ADD ESP, 4
		}
	}
}

__declspec(naked) void Hook_OnScriptLoadFinal()
{
	_asm {
		pushad
		PUSH EAX; / Arg4
		PUSH ECX; | Arg3
		PUSH EBX; | Arg2
		PUSH EDI; | Arg1
		push ESI; |
		call HookLoadGameScript
		add ESP, 0x14
		popad

		
		PUSH 0x005AF877
		retn
	}
}
void OnChangemapRespawn()
{
	// finish connecting, otherwise tell classify it as respawn
	if (g_CCore->GetNetwork()->IsConnected() == false)
	{
		g_CCore->GetGame()->SetTrafficVisible(false);
		//g_CCore->GetGame()->UpdateControls();		// --- NO LONGER NEEDED
		g_CCore->GetGame()->PoliceManager();
		g_CCore->GetGame()->DisableBridges();
		g_CCore->m_bIsRespawning = false;

		g_CCore->GetNetwork()->OnConnectionIsAboutFinish();
	}
	else {
		g_CCore->GetGame()->AfterRespawn();
	}
}

// Hook_Respawn01 is called after map change (only when map is changed during playing another one)
_declspec(naked) void Hook_Respawn01()
{
	_asm {
		MOV DWORD PTR DS : [EAX + 0xE4], ESI
		cmp ESI, 0x0
		jz end
		pushad
		call OnChangemapRespawn
		popad
		end:
		push 0x005F93CA
		retn
	}

}

_declspec(naked) void Hook_Respawn02()
{
	_asm {
		MOV DWORD PTR DS : [ESI + 0xE4], EDX
		cmp EDX, 0x0
			jz end
			pushad
			call OnChangemapRespawn
			popad
		end :
		push 0x005DD1B7
		retn
	}

}

_declspec(naked) void Hook_CarMolotovDamage()
{
	_asm
	{
		pushad
			mov eax, DWORD PTR DS : [0x006F9464]
			mov eax, DWORD PTR DS : [eax + 0xE4]

			mov eax, DWORD PTR DS : [eax + 0x98]
			//mov edx, ecx
			//sub edx, 0x70
			cmp ebp, eax
			jne localIsNotInCar
			fst dword ptr[ebp + 0x2A4]
	}
	//g_CCore->GetGame()->OnCollision();
	_asm {
	localIsNotInCar:
		popad
			push 0x0046B7ED
			ret
	}
}

_declspec(naked) void Hook_CarMolotovDamagePart2()
{
	_asm
	{
		pushad
			mov eax, DWORD PTR DS : [0x006F9464]
			mov eax, DWORD PTR DS : [eax + 0xE4]

			mov eax, DWORD PTR DS : [eax + 0x98]
			//mov edx, ecx
			//sub edx, 0x70
			cmp ebp, eax
			jne localIsNotInCar
			mov[ebp + 0x000002A4], 0
	}
	//g_CCore->GetGame()->OnCollision();
	_asm {
	localIsNotInCar:
		popad
			push 0x0046B804
			ret
	}
}

_declspec(naked) void Hook_CarGranadeDamage()
{
	_asm
	{
		pushad
			mov eax, DWORD PTR DS : [0x006F9464]
			mov eax, DWORD PTR DS : [eax + 0xE4]

			mov eax, DWORD PTR DS : [eax + 0x98]
			//mov edx, ecx
			//sub edx, 0x70
			cmp ebp, eax
			jne localIsNotInCar
			fst dword ptr[ebp + 0x2A4]
	}
	//g_CCore->GetGame()->OnCollision();
	_asm {
	localIsNotInCar:
		popad
			push 0x0046B2B9
			ret
	}
}

_declspec(naked) void Hook_CarGranadeDamagePart2()
{
	_asm
	{
		pushad
			mov eax, DWORD PTR DS : [0x006F9464]
			mov eax, DWORD PTR DS : [eax + 0xE4]

			mov eax, DWORD PTR DS : [eax + 0x98]
			//mov edx, ecx
			//sub edx, 0x70
			cmp ebp, eax
			jne localIsNotInCar
			mov[ebp + 0x000002A4], 0
	}
	//g_CCore->GetGame()->OnCollision();
	_asm {
	localIsNotInCar:
		popad
			push 0x0046B2D0
			ret
	}
}

void EngineOnLoadProgress(float progress)
{
	g_CCore->GetGame()->LoadingStatus = progress;
}
_declspec(naked) void Hook_EngineOnLoadProgress()
{
	_asm
	{
		MOV EDI, DWORD PTR SS : [ESP + 0x10]
		MOV DWORD PTR DS : [EDX + 0x2C], EDI
		pushad
		push EDI
		call EngineOnLoadProgress
		add ESP, 0x4
		popad
		push 0x0056F20E
		ret
	}
}

void	ExplodeTransformCar(DWORD oldbase,DWORD newbase)
{
	char buff[100];
	sprintf(buff, "ExplodeCar: %p %p", oldbase, newbase);
	g_CCore->GetLog()->AddLog(buff);

	int ID = g_CCore->GetVehiclePool()->GetVehicleIdByBase(oldbase);
	if (ID != -1)
	{
		CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
		if (veh != NULL)
		{
			veh->SetExplodedCar(newbase);
		}
	}

}
_declspec(naked) void Hook_ExplodeTransformCar()
{
	_asm
	{
		PUSH 0x21F4
		MOV EAX, 0x00624934
		CALL EAX; Game.00624934;  creates new frame / object ? ? ? ? EDI = old car
		ADD ESP, 0x4
		pushad
		PUSH EAX
		PUSH EDI
		CALL ExplodeTransformCar
		ADD ESP, 0x8
		popad
		PUSH 0x00468E0E
		RETN
	}
}

void	OnExplodeCar(DWORD carBase)
{
	int ID = g_CCore->GetVehiclePool()->GetVehicleIdByBase(carBase);
	if (ID != -1)
	{
		CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
		if (veh != NULL)
		{
				char buff[100];
				sprintf(buff, "CarExplodedEvent: %p", carBase);
				g_CCore->GetLog()->AddLog(buff);

				// TODO: add here some documentation - WTF is this engine function good for ?
				// does it blow up a car ? 
				_asm {
					PUSH 0
					MOV ECX, carBase
					MOV EAX, 0x00468BC0
					CALL EAX; 0x00468BC0
				}
				g_CCore->GetGameSync()->Engine_onExplodeCar(veh);
		}
	}
}

_declspec(naked) void Hook_ExplodeCar()
{
	_asm
	{
		// call func
		PUSH ECX
		CALL OnExplodeCar
		ADD ESP, 0x4
		// swap last 2 stack values
		MOV EAX, DWORD PTR DS : [ESP]
		MOV DWORD PTR DS : [ESP + 0x4], EAX
		ADD ESP, 0x4
			RETN
	}
}

// TODO: NOT USED Hook_DoorUnlock
_declspec(naked) void Hook_DoorUnlock()
{
	_asm
	{

		cmp ESI, 0x0
		JE skipThis
		cmp byte ptr ds:[ESI+0x10], 0x6	// if object is door
		JNE skipThis
		MOV BYTE PTR DS : [ESI +0x125],0x0	// then set lock byte to 0
		skipThis:
		LEA ECX, DWORD PTR SS : [ESP + 0x14]
		MOV EAX, 0x005A61B0;
		CALL EAX; game.005A61B0
		MOV EAX, 0x0060409A
		PUSH EAX
		RETN
	}
}

void NewMessage(MSG message)
{
	_asm pushad;
	char buff[250];
	sprintf(buff,"ID: %u", message.message);
	if (message.message > 0)
	{
		g_CCore->GetChat()->AddMessage(buff);
	}
	_asm popad;
}
_declspec(naked) void Hook_PeekMessage()
{
	_asm
	{
		PUSH 1
		PUSH EDI
		PUSH EDI
		PUSH EDI
		LEA EAX, DWORD PTR SS : [EBP - 0x1C]
		PUSH EAX
		CALL DWORD PTR DS : [0x71CAF190]; USER32.PeekMessageW
		TEST EAX, EAX;
		JZ end
		LEA EAX, DWORD PTR SS : [ESP + 0xC]
		push EAX
		call NewMessage
		add ESP, 0x4
		mov EAX, 0x1
		end :
		//return back
		PUSH 0x71C8C399
		retn;

	}


}
void	OnPhysicsTick(int deltatime)
{
	for (int i = 0; i < 100; i++)
	{
		CVehicle* veh = g_CCore->GetVehiclePool()->Return(i);
		if (veh)
		{
			if (veh->GetEntity())
			{
				VEHICLE* entity = (VEHICLE*)veh->GetEntity();
				if (veh->GetSeat(0) != g_CCore->GetLocalPlayer()->GetOurID())
				{
					entity->position = veh->GetPosition();
					entity->rotation = veh->GetRotation();
					entity->speed = veh->GetSpeed();
				}
				_asm{
					mov ECX, entity
						push deltatime
						mov EAX, 0x0044BEA0
						call EAX
				}
			}
		}
	}
}

_declspec(naked) void Hook_GamePhysics()
{
	_asm
	{
		XOR ESI, ESI
			start:
			MOV ECX, DWORD PTR SS : [EBP + 0xF0]
			TEST ECX, ECX
			JE SHORT end
			MOV EAX, DWORD PTR SS : [EBP + 0xF4]
			SUB EAX, ECX
			SAR EAX, 2
			CMP ESI, EAX
			JNB SHORT end
			MOV ECX, DWORD PTR DS : [ECX + ESI * 0x4]
			cmp DWORD PTR DS : [ECX + 0x10],0x4
			JE next	// jmp if object is a car
			PUSH EBX
			MOV EAX, 0x0044BEA0
			CALL EAX
			next:
			INC ESI
			JMP SHORT start
			end:
			push EBX
			call OnPhysicsTick
			add ESP, 0x4

			PUSH 0x005E1050
			retn;
	}


}

void gameTick()
{
	if (g_CCore->m_bIsRespawning == false)
	{
		g_CCore->GetLocalPlayer()->Pulse();
	}
	//g_CCore->GetGame()->UpdateCars();
}

_declspec (naked) void Hook_gamePhysicsUpdate()
{
	_asm 
	{	pushad
		call gameTick
		popad
		MOV EAX, DWORD PTR SS : [EBP + 0x114]
		PUSH 0x005E1056
		retn
		}


}

bool IsCarAbandoned(DWORD car)
{
	int ID = g_CCore->GetVehiclePool()->GetVehicleIdByBase(car);
	if (ID != -1)
	{
		CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
		if (veh)
		{
			if (veh->GetSeat(0) != -1)
				return false;
		}
	}
	return true;
}

_declspec (naked) void Hook_PreventCarMoveWhenAbandoned()
{
	_asm {
		pushad
			sub ESI, 0x70
			push ESI
			call IsCarAbandoned
			add ESP, 0x4
			cmp EAX, 1
			popad
			JE end

			// entire move
			PUSH ECX
			PUSH EDX
			PUSH EAX
			MOV ECX, ESI
			MOV EAX, 0x0052E6D0
			CALL EAX; 0x0052E6D0

	end:
		PUSH 0x0052D311
		ret
	}
}

//-------------------------------- Hit call back -----------------------------
bool OnHitCallback(DWORD victim,DWORD reason, DWORD unk1, DWORD unk2, DWORD unk3, float damage, DWORD attacker,  DWORD playerPart,DWORD unk4)
{
	PED* ped = (PED*)victim;

	// prevent hit if it's enemy
	if (ped->object.objectType == OBJECT_ENEMY && g_CCore->GetGame()->ShouldKill == false)
		return true;
	
	//g_CCore->GetChat()->AddMessage("CHooks::OnHitCall");
	
	// Check whether player is already dead
	bool isAlreadyDead = (ped->object.isActive == 0);
	DWORD result;
	// call original function
	_asm {
		MOV EAX, unk4
		PUSH EAX
		MOV EAX, playerPart
		PUSH EAX
		MOV EAX, attacker
		PUSH EAX
		MOV EAX, damage
		PUSH EAX
		MOV EAX, unk3
		PUSH EAX
		MOV EAX, unk2
		PUSH EAX
		MOV EAX, unk1
		PUSH EAX
		MOV EAX, reason
		PUSH EAX
		MOV ECX, victim
		MOV EAX, 0x00496710
		CALL EAX; Game.00496710; \Game.00496710
		MOV result, EAX
	}
	// if player is already dead, we won't sync this event
	if (!isAlreadyDead)
	{
		// if victim is local player
		if (ped->object.objectType == OBJECT_LOCALPLAYER)
		{
			// is dead
			if (ped->object.isActive == 0)
			{
				// if attacker == NULL -> suicide
				if (attacker != NULL)
				{
					PED* anotherPed = (PED*)attacker;
					// if attacker is another player
					if (anotherPed->object.objectType == OBJECT_ENEMY)
					{
						// killed by player
						int ID = g_CCore->GetPedPool()->GetPedIdByBase(attacker);
						g_CCore->GetLocalPlayer()->OnDeath(ID, reason, playerPart);
						return (result == 1);
					}
					// if it's vehicle, then let's try to get its driver
					else if (anotherPed->object.objectType == OBJECT_VEHICLE)
					{
						int ID = g_CCore->GetVehiclePool()->GetVehicleIdByBase(attacker);
						if (ID != -1)
						{
							CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
							if (veh)
							{
								g_CCore->GetLocalPlayer()->OnDeath(veh->GetSeat(0), reason, playerPart);
								return (result == 1);
							}
						}
					}

				}
				// otherwise it's suicide
				g_CCore->GetLocalPlayer()->OnDeath(-1, reason, playerPart);
			}
		}
	}
	return (result == 1);
}


//0041223B    CALL Game.00496710; \Game.00496710
_declspec (naked) void Hook_OnPlayerHit01()
{
	_asm {
		PUSH EAX; / Arg8
		MOV EAX, DWORD PTR SS : [ESP + 24]; |
		PUSH ECX; | part(head = 6)
		MOV ECX, DWORD PTR SS : [ESP + 24]; |
		PUSH EDI; | Arg6 owner(could be car too, wtf ? )
		PUSH EDX; | Arg5 range ? (50.0 in this case)
		MOV EDX, DWORD PTR SS : [ESP + 28]; |
		PUSH EAX; | Arg4 again some rot ?
		MOV EAX, DWORD PTR SS : [ESP + 28]; |
		PUSH ECX; | Arg3 position vector
		PUSH EDX; | Arg2 rotation
		PUSH EAX; | Arg1
		PUSH ESI; |
		MOV EAX, OnHitCallback
		CALL EAX
		ADD ESP, 0x24

		// return
		PUSH 0x00412240
		ret
	}
}

//004CBC5A    CALL Game.00496710; \Game.00496710
_declspec (naked) void Hook_OnPlayerHit02()
{
	_asm {
		PUSH EAX; / Arg8
		MOV EAX, DWORD PTR SS : [ESP + 0x1C]; |
		PUSH ECX; | Arg7
		MOV ECX, DWORD PTR SS : [ESP + 0x1C]; |
		PUSH EDX; | Arg6
		MOV EDX, DWORD PTR SS : [ESP + 0x1C]; |
		PUSH EAX; | Arg5
		MOV EAX, DWORD PTR SS : [ESP + 0x1C]; | Arg4 = position
		PUSH ECX; | Arg4
		MOV ECX, DWORD PTR SS : [ESP + 0x1C]; | Arg3 is again a position vector
		PUSH EDX; | Arg3
		PUSH EAX; | Arg2
		PUSH ECX; | Arg1
		PUSH ESI; | Arg2 = rotation vector
		MOV EAX, OnHitCallback
		CALL EAX
		ADD ESP, 0x24
			// return
		PUSH 0x004CBC5F
		ret
	}
}

// This hook fixes remote player drive-by problem
// (weapon's rotation was desynced)

_declspec (naked) void Hook_OnChangePlayerCarRotation()
{
	// PatchBytes(0x0049BB23, drivebyshootfix);
	_asm {
		// is local
		CMP DWORD PTR DS : [ESI + 0x10], 0x02
		JNE end
		FSTP DWORD PTR DS : [ESI + 0x5F4]
	end:
	   // jmp back to code
	   PUSH 0x0049BB29
	   RETN
	}
}

void RestInPeace()
{
	// Send info to server
	g_CCore->GetGameSync()->Engine_onMapRespawn();
}

_declspec (naked) void Hook_RestInPeace()
{
	_asm {
		cmp DWORD PTR DS : [ECX+0x10],0x2
		JNE end
		pushad
			push ECX
			call RestInPeace
			add ESP, 0x4
		popad
		end:
		push 0x004ABE40
		retn
	
	}
}

/* TODO
- radar rendering is disabled -> all radar hooks are obsolete
*/

int ChangeRadarRendering()
{
	float radarwidth = *(float*)0x0063D29C;
	float posun = 1920.0f - (radarwidth + 320.0f);
	float restx = (g_CCore->GetGraphics()->GetResolution().x / 1920.0f) * posun;

	__asm mov eax, restx
}

__declspec(naked) void Hook_ChangeRadarRendering()
{
	__asm
	{
		call ChangeRadarRendering
			PUSH EAX
			FLD DWORD PTR DS : [ESP]
			POP EAX

			MOV EAX, 0x0054FDC2
			JMP EAX
	}
}

void OnPlayerVehicleEngineStateChange(DWORD vehicle, BYTE state)
{
	if (vehicle != NULL)
	{
		int vehID = g_CCore->GetVehiclePool()->GetVehicleIdByBase(vehicle);
		int ourID = g_CCore->GetLocalPlayer()->GetOurID();

		char buff[500];
		sprintf(buff, "[Hook] OnEngineStateC %d %d 0x%p", vehID, ourID,vehicle);
		g_CCore->GetLog()->AddLog(buff);

		// if that vehicle is valid LHMP vehicle (that means synced)
		if (vehID != -1)
		{
			CVehicle* veh = g_CCore->GetVehiclePool()->Return(vehID);
			if (veh == NULL)
			{
				g_CCore->GetLog()->AddLog("VEHICLE NOT FOUND !");
				return;
			}
			int playerid = veh->GetSeat(0);

			//sprintf(buff, "[Hook] OnEngineStateC %d 0x%p", playerid, veh->GetEntity());
			//sprintf(buff, "[Hook] OnEngineStateC %d | %d %d %d %d", playerid, veh->GetPlayerSeat(0), veh->GetPlayerSeat(1), veh->GetPlayerSeat(2), veh->GetPlayerSeat(3));
			g_CCore->GetLog()->AddLog(buff);

			// if localPlayer == driver of that vehicle
			if (playerid == g_CCore->GetLocalPlayer()->GetOurID())
			{
				//Send data 
				g_CCore->GetGameSync()->Engine_onVehicleEngineStateChange(veh,state);
			}
		}

	}
}

__declspec(naked) void Hook_OnPlayerStartedVehicleEngine()
{
	__asm
	{
		pushad
			SUB ESI, 0x70
			PUSH 1
			PUSH ESI
			CALL OnPlayerVehicleEngineStateChange
			ADD ESP, 0x8
			popad

			MOV AL, BYTE PTR DS : [ESI + 0x50C]
			MOV BYTE PTR DS : [ESI + 0x50D], AL
			POP EDI;  kernel32.7631338A
			POP ESI;  kernel32.7631338A
			POP EBX;  kernel32.7631338A
			RETN 4
	}
}

__declspec(naked) void Hook_OnPlayerStoppedVehicleEngine()
{
	__asm
	{
		pushad
			SUB ESI, 0x70
			PUSH 0
			PUSH ESI
			CALL OnPlayerVehicleEngineStateChange
			ADD ESP, 0x8
			popad

			POP EDI;  ntdll.77C9FD91
			POP ESI;  ntdll.77C9FD91
			POP EBX;  ntdll.77C9FD91
			RETN 4
	}
}

void ValidateCarDeleting(DWORD entity)
{
	DWORD car = *(DWORD*)(entity + 0xC);
	if (g_CCore->GetVehiclePool()->GetVehicleIdByBase(car) == -1)
	{
		// if it returns -1, that means the vehicle is either not synced or doesn't exists
		// let's presume the second case
		// so just delete it from entity
		*(DWORD*)(entity + 0xC) = 0x0;
	}
}

__declspec(naked) void Fix_CarDeleting()
{
	__asm
	{
		MOV ESI, ECX
		PUSHAD
		PUSH ESI
		CALL ValidateCarDeleting
		ADD ESP, 0x4
		POPAD
		MOV EAX, 0x005DAA60
		CALL EAX

		// return back to code
		PUSH 0x005D4AA8
		RETN
	}
}

bool ValidateCarDeletingTwo(DWORD entity)
{
	DWORD car = *(DWORD*)(entity + 0x9C);
	if (g_CCore->GetVehiclePool()->GetVehicleIdByBase(car) == -1)
	{
		// if it returns -1, that means the vehicle is either not synced or doesn't exists
		// let's presume the second case
		// so just delete it from entity
		*(DWORD*)(entity + 0x9C) = 0x0;
		return true;
	}
	return false;
}


__declspec(naked) void Fix_CarDeletingSecond()
{
	__asm
	{
		MOV EAX, DWORD PTR DS : [ESI + 0x9C];  EAX = car
		PUSHAD
		PUSH ESI
		CALL ValidateCarDeletingTwo
		ADD ESP, 0x4
		CMP AL,0x1
		JE badcode
		POPAD
		// return back to code - OK
		PUSH 0x0048DC84
		RETN
	badcode:
			POPAD
			PUSH 0x0048E6FB
			RETN
	
	}
}

void Hook_Car_ProcessEngineStateChange(DWORD car, DWORD shouldStartOrSwitchOff)
{
	/*TODO - remove debug text
	g_CCore->GetChat()->AddMessage("Hook_Car_ProcessEngineStateChange tick");*/
	int ID = g_CCore->GetVehiclePool()->GetVehicleIdByBase(car);
	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh)
	{
		if (veh->GetSeat(0) == g_CCore->GetLocalPlayer()->GetOurID())
		{
			// stream it
			/*TODO - remove debug strings
			
			char buff[500];
			sprintf(buff, "ProcessEngineState %x %u", car, shouldStartOrSwitchOff);
			g_CCore->GetChat()->AddMessage(buff);
			*/
			// if it's streamed, then allow the changes & call engine function
			g_CCore->GetGame()->SetCarEngineState(car, (shouldStartOrSwitchOff == 1));
			// network sync
			g_CCore->GetGameSync()->Engine_onVehicleEngineStateChange(veh, (bool)(shouldStartOrSwitchOff > 0));
		}
		else if (veh->GetEngineState() == shouldStartOrSwitchOff)
		{
			// at least call engine func
			g_CCore->GetGame()->SetCarEngineState(car, (shouldStartOrSwitchOff == 1));
		}
	}
}

_declspec(naked) void Hook_Car_EngineChangeStatus01()
{
	// jmp from 00517991, back to 00517999
	// ECX = 0/1	# new engine status (off/on)
	// ESI = car+0x70
	_asm {
		PUSHAD
		SUB ESI, 0x70
		PUSH ECX
		PUSH ESI
		CALL Hook_Car_ProcessEngineStateChange
		ADD ESP, 0x8
		POPAD
		PUSH 0x00517999
		RET
	}
}

_declspec(naked) void Hook_Car_EngineChangeStatus02()
{
	// jmp from 0052C5B8, back to 0x0052C5C1
	// ON
	// ESI = car+0x70
	_asm {
		PUSHAD
			SUB ESI, 0x70
			PUSH 0x1
			PUSH ESI
			CALL Hook_Car_ProcessEngineStateChange
			ADD ESP, 0x8
			POPAD
			PUSH 0x0052C5C1
			RET
	}
}

_declspec(naked) void Hook_Car_EngineChangeStatus03()
{
	// jmp from 0052D31B, back 0052D324
	// OFF
	// ESI = car+0x70
	_asm {
		PUSHAD
			SUB ESI, 0x70
			PUSH 0
			PUSH ESI
			CALL Hook_Car_ProcessEngineStateChange
			ADD ESP, 0x8
			POPAD
			PUSH 0x0052D324
			RET
	}
}

// During the game loads, the game name(title) is loaded from exe
// This function replaces the loading code with our own title
// address 005FA715
char LHMP_TITLE[] = "Mafia - Lost Heaven Multiplayer";

void CopyTitle(char* dest)
{
	strcpy(dest, LHMP_TITLE);
}
_declspec(naked) void Hook_LoadGameTitle()
{
	_asm {
		LEA EAX, DWORD PTR SS : [ESP + 0x60]
		PUSH EAX
		CALL CopyTitle
		ADD ESP, 0x4
		PUSH 0x005FA728
		RETN
	}
}

// this hook is called when WM_CLOSE is triggered
// -> when user(player) closes the game's window
//1006A1F0
_declspec(naked) void Hook_onWindowClose()
{
	//g_CCore->ShutdownClient();
	g_CCore->GetNetwork()->GetPeer()->Shutdown(100, 0, IMMEDIATE_PRIORITY);
	_asm
	{
		PUSH 0
			MOV EAX, 0x01008A4A0
			CALL EAX; ls3df.1008A4A0
	}
}

void SetHooks()
{
	Tools::InstallJmpHook(0x1006A1F0, (DWORD)&Hook_onWindowClose);

	//---------------------- change game's title to LHMP-----------------------//
	Tools::InstallJmpHook(0x005FA715, (DWORD)&Hook_LoadGameTitle);

	//---------------------- Fix deleting car

	Tools::InstallJmpHook(0x005D4AA1, (DWORD)&Fix_CarDeleting);
	Tools::InstallJmpHook(0x0048DC7E, (DWORD)&Fix_CarDeletingSecond);

	/*TODO - delete old vehicle hooks*/
	//---Tools::InstallJmpHook(0x005384BB, (DWORD)&Hook_OnPlayerStartedVehicleEngine);
	//---Tools::InstallJmpHook(0x00538595, (DWORD)&Hook_OnPlayerStoppedVehicleEngine);

	Tools::InstallJmpHook(0x00517991, (DWORD)&Hook_Car_EngineChangeStatus01);
	Tools::InstallJmpHook(0x0052C5B8, (DWORD)&Hook_Car_EngineChangeStatus02);
	Tools::InstallJmpHook(0x0052D31B, (DWORD)&Hook_Car_EngineChangeStatus03);

	// Fixes drive-by rotation
	Tools::InstallJmpHook(0x0049BB23, (DWORD)&Hook_OnChangePlayerCarRotation);

	/*TODO - remove radar rendering hooks*/
	//change radar position on screen
	//Tools::InstallJmpHook(0x0054FDB6, (DWORD)&Hook_ChangeRadarRendering);

	Tools::InstallCallHook(0x004940C0, (DWORD)&Hook_RestInPeace);

	// OnPlayerHit callback - usefull for death detection & hit, shot or explosion splash
	Tools::InstallJmpHook(0x00412221, (DWORD)&Hook_OnPlayerHit01);
	Tools::InstallJmpHook(0x004CBC3C, (DWORD)&Hook_OnPlayerHit02);

	// This should make car static
	Tools::InstallJmpHook(0x0052D307, (DWORD)&Hook_PreventCarMoveWhenAbandoned);


	// 005E1029

	//Tools::InstallJmpHook(0x005E1029, (DWORD)&Hook_GamePhysics);

	Tools::InstallJmpHook(0x005E1050, (DWORD)&Hook_gamePhysicsUpdate);

	//Tools::Nop(0x005DC6DD, 11);
	//Tools::InstallJmpHook(0x005FFF30, (DWORD)&Hook_Loadingmap);


	//Tools::InstallJmpHook(0x00603FF3, (DWORD)&Hook_DoorUnlock);		- WORKING, but some doors are fake, therefore it's disabled

	//Tools::InstallJmpHook(0x71C8C38A, (DWORD)&Hook_PeekMessage);

	//Tools::InstallJmpHook(0x1006DEDB, (DWORD)&Hook_LastASCIIPressedKey);
	Tools::InstallCallHook(0x005FB203, (DWORD)&Hook_EngineLoad);



	//	DWORD lpflOldProtect;
	Tools::InstallCallHook(0x005F9E57, (DWORD)&SetIsLoaded);
	Tools::InstallCallHook(0x005F957E, (DWORD)&MainThreadTick);
	Tools::Nop(0x00560AB7, 14);					// change car spawn
	Tools::InstallCallHook(0x00560AB7, (DWORD)&Freeride_ChangeCarSpawn);
	Tools::InstallJmpHook(0x00601259, (DWORD)&SpawnManager);
	// hook respawn - more times
	Tools::Nop(0x005BF57F, 27);
	Tools::InstallCallHook(0x005BF57F, (DWORD)&Hook_respawn);
	Tools::Nop(0x005E0D4E, 18);
	Tools::InstallCallHook(0x005E0D4E, (DWORD)&Hook_respawn);
	Tools::Nop(0x005E0E49, 17);
	Tools::InstallCallHook(0x005E0E49, (DWORD)&Hook_respawn);
	// Respawn

	// used for Afterrespawn
	Tools::InstallJmpHook(0x00558D74, (DWORD)&AfterRespawn);
	// throw weapon
	Tools::InstallJmpHook(0x0055B631, (DWORD)&Hook_ThrowAwayWeapon);
	// take weapon - more times
	Tools::InstallJmpHook(0x0055B035, (DWORD)&Hook_TakeWeaponFirst);
	Tools::InstallJmpHook(0x0055AFE7, (DWORD)&Hook_TakeWeaponSecond);
	Tools::InstallJmpHook(0x0055AEAC, (DWORD)&Hook_TakeWeaponThird);
	Tools::InstallJmpHook(0x00559E4A, (DWORD)&Hook_ChangeWep);
	Tools::InstallJmpHook(0x00559D2E, (DWORD)&Hook_ChangeWep2);
	Tools::InstallJmpHook(0x00559E98, (DWORD)&Hook_ChangeWep3);
	Tools::InstallJmpHook(0x00559ECD, (DWORD)&Hook_ChangeWep4);
	Tools::InstallJmpHook(0x0055A52F, (DWORD)&Hook_HideWeapon);
	Tools::InstallJmpHook(0x004A3BBF, (DWORD)&Hook_OnShoot);

	/*Tools::Nop(0x00497394, 6);
	Tools::InstallCallHook(0x00497394, (DWORD)&Hook_PreventHit);
	Tools::Nop(0x00496BA9, 6);
	Tools::InstallCallHook(0x00496BA9, (DWORD)&Hook_PreventHit);
	Tools::Nop(0x00497118, 6);
	Tools::InstallCallHook(0x00497118, (DWORD)&Hook_PreventHit);
	Tools::Nop(0x00497024, 6);
	Tools::InstallCallHook(0x00497024, (DWORD)&Hook_PreventHit);
	Tools::Nop(0x0049709D, 6);
	Tools::InstallCallHook(0x0049709D, (DWORD)&Hook_PreventHit);
	Tools::Nop(0x00497483, 6);*/

	// TODO - useless
	// When he is killed by enemy or when falls down or hit by car 
	//--Tools::InstallCallHook(0x00497483, (DWORD)&Hook_OnDeath);


	// Killed by explosion (in car)	- need to be keeped IT ISN'T CALLED BY CALLBACK
	Tools::InstallJmpHook(0x004AAB71, (DWORD)&Hook_OnDeath2);

	// Car sink / car collision / killed in car (by enemy weapon)
	//--Tools::InstallJmpHook(0x004987B8, (DWORD)&Hook_OnDeath3);


	// Player sink
	//0049BFEA | .C786 44060000 >MOV DWORD PTR DS : [ESI + 644], 0 IT ISN'T CALLED BY CALLBACK
	Tools::InstallJmpHook(0x0049BFEA, (DWORD)&Hook_OnDeath4);



	//004987C2
	//Tools::Nop(0x00491C17, 6);
	//Tools::InstallCallHook(0x00491C17, (DWORD)&Hook_OnPlayerEnteredVehicle);

	Tools::InstallJmpHook(0x004CC685, (DWORD)&Hook_OnPlayerEnteredVehicle);
	Tools::InstallJmpHook(0x004CC6EF, (DWORD)&Hook_OnPlayerEnteredVehicle2);
	Tools::InstallJmpHook(0x004CCA61, (DWORD)&Hook_OnPlayerExitVehicle);

	Tools::InstallCallHook(0x00491EE5, (DWORD)&Hook_OnPlayerExitVehicleFinish);		// cause locked doors
	
	Tools::InstallJmpHook(0x004CC822, (DWORD)&Hook_OnCarJack);
	Tools::InstallJmpHook(0x00604046, (DWORD)&Hook_PreventCarandPedsSpawn);

	Tools::InstallJmpHook(0x0044BB66, (DWORD)&Hook_PEDSpawnCorrectPos);


	Tools::InstallJmpHook(0x00522FB4, (DWORD)&Hook_CollisionDamage);			// TODO: NW
	Tools::InstallJmpHook(0x0046ADA0, (DWORD)&Hook_CarShot);					// TODO: NW


	Tools::InstallJmpHook(0x004A3FA6, (DWORD)&Hook_ThrowGranade);
	Tools::InstallJmpHook(0x004CC608, (DWORD)&Hook_OnDoorStateChange);

	Tools::InstallJmpHook(0x005F93C4, (DWORD)&Hook_Respawn01);	

	Tools::InstallJmpHook(0x0046B7E7, (DWORD)&Hook_CarMolotovDamage);
	Tools::InstallJmpHook(0x0046B7FA, (DWORD)&Hook_CarMolotovDamagePart2);
	Tools::InstallJmpHook(0x0046B2B3, (DWORD)&Hook_CarGranadeDamage);
	Tools::InstallJmpHook(0x0046B2C6, (DWORD)&Hook_CarGranadeDamagePart2);


	Tools::InstallJmpHook(0x0056F207, (DWORD)&Hook_EngineOnLoadProgress);

	Tools::InstallJmpHook(0x00468E01, (DWORD)&Hook_ExplodeTransformCar);


	Tools::InstallCallHook(0x00467BB6, (DWORD)&Hook_ExplodeCar);
	Tools::InstallCallHook(0x005A0FEE, (DWORD)&Hook_ExplodeCar);
	Tools::InstallCallHook(0x005C10AC, (DWORD)&Hook_ExplodeCar);
		
}