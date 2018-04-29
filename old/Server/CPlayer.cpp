// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CCore.h"

#include "CPlayer.h"
#include "../shared/tools.h"
#include "../shared/CBitArray.h"

extern CCore *g_CCore;

CPlayer::CPlayer()
{
	this->SetCurrentCar(NO_CAR);
	this->Active  = 1;
	this->SetNickname("Player");
	this->SetNicknameColour(0xFFFFFFFF);
	this->Health	= 200.0f;
	this->Spawned = 1;

	this->Weapon = 0;
	for(int i = 0; i < 8;i++)
		this->Weapons[i].wepID = 0;
	this->Money = 0;
	this->FieldOfView = 0;
	this->Locked = false;
}

CPlayer::~CPlayer()
{
}


void CPlayer::SetFloatRotation(float rot)
{
	this->FloatRotation = rot;
}
float CPlayer::GetFloatRotation()
{
	return this->FloatRotation;
}

char* CPlayer::GetNickname()
{
	return this->nickname;
}

int CPlayer::GetCurrentWeapon()
{
	return this->Weapon;
}
SWeapon* CPlayer::GetWeapon(int index)
{
	return &Weapons[index];
}

bool CPlayer::IsAim()
{
	return this->Aiming;
}
bool CPlayer::IsDucking()
{
	return this->Ducking;
}

bool CPlayer::IsActive()
{
	return this->Active;
}

bool	 CPlayer::IsCarAnim()
{
	return this->AimingInVehicle;
}

void CPlayer::SetMoney(int money)
{
	this->Money = money;
}
void CPlayer::SetIsAim(bool aim)
{
	this->Aiming = aim;
}

void CPlayer::SetIsDucking(bool is)
{
	this->Ducking = is;
}
void CPlayer::SetIsActive(bool is)
{
	this->Active = is;
}

void CPlayer::SetNickname(char nick[])
{
	sprintf(this->nickname,"%s",nick);
}
void CPlayer::SetNickname(const char* nick)
{
	sprintf(this->nickname,"%s",nick);
}

void CPlayer::SetCurrentWeapon(int ID)
{
	this->Weapon = ID;
}

void CPlayer::AddWeapon(int ID, int Loaded,int Hidden)
{
	if(this->Weapon == 0)
		Weapon = ID;
	// If we already have that weapon && it must be a shot gun
	if (Tools::isShootingArm(ID))
	{
		for (int i = 0; i < 8; i++)
		{
			if (Weapons[i].wepID == ID)
			{
				Weapons[i].wepHidden += Hidden;
				return;
			}
		}
	}
	// else find first empty slot
	for(int i = 0; i < 8;i++)
	{
		if(Weapons[i].wepID == NULL)
		{
			Weapons[i].wepID			= ID;
			Weapons[i].wepLoaded		= Loaded;
			Weapons[i].wepHidden		= Hidden;
			break;
		}
	}
}

void CPlayer::DropWeapon(int ID)
{
	if(Weapon == ID)
		Weapon = 0;
	for(int i = 0; i < 8;i++)
	{
		if(Weapons[i].wepID == ID)
		{
			Weapons[i].wepID			= 0;
			Weapons[i].wepLoaded		= 0;
			Weapons[i].wepHidden		= 0;
			break;
		}
	}
}

void CPlayer::SwitchWeapon(int ID)
{
	Weapon = ID;
}

void CPlayer::OnShoot()
{
	for(int i = 0; i < 8;i++)
	{
		if(Weapons[i].wepID == Weapon)
		{
			if(Weapons[i].wepLoaded == 0)
			{
				// TODO: reload podla poctu nabojov v zasobniku
				Weapons[i].wepLoaded = Weapons[i].wepHidden;
				Weapons[i].wepHidden = 0;
			} else
			{
				Weapons[i].wepLoaded--;
			}
			break;
		}
	}
}

void CPlayer::ResetWeapons()
{
	for(int i = 0; i < 8;i++)
	{
		this->Weapons[i].wepID = 0;
		this->Weapons[i].wepLoaded = 0;
		this->Weapons[i].wepHidden = 0;
	}
	this->Weapon = 0;
}

void CPlayer::SetCarAim(float f)
{
	this->VehicleAim = f;
}

void CPlayer::SetFieldOfView(float r)
{
	this->FieldOfView = r;
}


void CPlayer::SetIsCarAnim(bool b)
{
	this->AimingInVehicle = b;
}

float CPlayer::GetFieldOfView()
{
	return this->FieldOfView;
}

float CPlayer::GetCarAim()
{
	return this->VehicleAim;
}

int CPlayer::GetMoney()
{
	return this->Money;
}

void CPlayer::OnThrowGrenade()
{
	for (int i = 0; i < 8; i++)
	{
		if (Weapons[i].wepID == Weapon)
		{
			Weapons[i].wepID = 0;
			Weapons[i].wepLoaded = 0;
			Weapons[i].wepHidden = 0;
			break;
		}
	}
	Weapon = 0;
}


bool	CPlayer::IsLocked()
{
	return this->Locked;
}
void	CPlayer::SetLocked(bool status)
{
	this->Locked = status;
}

void	CPlayer::SetCurrentCar(int status)
{
	this->VehicleID = status;
}
int		CPlayer::GetCurrentCar()
{
	return this->VehicleID;
}

void	CPlayer::SetNicknameColour(unsigned int color)
{
	this->NicknameColour = color;
}
unsigned int CPlayer::GetNicknameColour()
{
	return this->NicknameColour;
}

//------------ Network


void	CPlayer::OnFootUpdate(SYNC::ON_FOOT_SYNC syncData, RakNet::TimeMS time)
{
	this->SetPosition(syncData.position);
	float rotation = syncData.rotation * 360.0f / MAX_USHORT;

	this->SetFloatRotation(rotation);
	this->SetHealth((float)syncData.health);
	this->SetStatus(syncData.animStatus);

	CBitArray states(syncData.states);
	this->SetIsDucking(states.GetBit(ONFOOT_ISDUCKING));
	this->SetIsCarAnim(states.GetBit(ONFOOT_ISCARANIM));
	this->SetIsAim(states.GetBit(ONFOOT_ISAIMING));

	this->SetTimeStamp(time);
	g_CCore->GetPickupPool()->TestPlayer(g_CCore->GetPlayerPool()->GetID(this));
}


void	CPlayer::OnVehicleUpdate(SYNC::IN_CAR syncData)
{
	this->SetHealth(syncData.health);
	this->SetIsAim(syncData.isAiming);
	this->SetCarAim(syncData.aim);
	g_CCore->GetPickupPool()->TestPlayer(g_CCore->GetPlayerPool()->GetID(this));
}

void	CPlayer::OnChangeSkin(int skin)
{
	if (skin > 303)
		return;
	this->SetSkin(skin);

	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_CHANGESKIN);
	bsOut.Write(g_CCore->GetPlayerPool()->GetID(this));
	bsOut.Write(skin);
	g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, MEDIUM_PRIORITY, RELIABLE, LHMP_NETCHAN_PLAYERPROP, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void	CPlayer::OnRespawn()
{
	int ID = g_CCore->GetPlayerPool()->GetID(this);
	this->ResetWeapons();
	if (this->GetCurrentCar() != NO_CAR)
	{
		g_CCore->GetVehiclePool()->Return(this->GetCurrentCar())->PlayerExit(ID);
	}

	g_CCore->GetNetworkManager()->SendHimDoors(ID);
	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_RESPAWN);
	bsOut.Write(ID);
	g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE, LHMP_NETCHAN_STATECHANGE, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), true);
	g_CCore->GetScripts()->onPlayerSpawn(ID);
}


void	CPlayer::OnAddWeapon(int wepID, int ammoA, int ammoB, bool shouldSendHim)
{
	int ID = g_CCore->GetPlayerPool()->GetID(this);
	this->AddWeapon(wepID,ammoA,ammoB);

	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_ADDWEAPON);
	bsOut.Write(ID);
	bsOut.Write(wepID);
	bsOut.Write(ammoA);
	bsOut.Write(ammoB);
	if (shouldSendHim)
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, MEDIUM_PRIORITY, RELIABLE, LHMP_NETCHAN_STATECHANGE, UNASSIGNED_SYSTEM_ADDRESS,true);	// send this message everybody including this player - when using Squirrel
	else 
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, MEDIUM_PRIORITY, RELIABLE, LHMP_NETCHAN_STATECHANGE, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), true);	// else, when it arrives from player
}

void	CPlayer::OnDropWeapon(int wepID, bool shouldSendHim)
{
	int ID = g_CCore->GetPlayerPool()->GetID(this);
	
	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_DELETEWEAPON);
	bsOut.Write(ID);
	bsOut.Write(wepID);
	if (shouldSendHim)
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE, LHMP_NETCHAN_STATECHANGE, UNASSIGNED_SYSTEM_ADDRESS, true);	// send this message everybody including this player - when using Squirrel
	else
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE, LHMP_NETCHAN_STATECHANGE, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), true);	// else, when it arrives from player

	this->DropWeapon(wepID);
}

void	CPlayer::OnSwitchWeapon(int wepID, bool shouldSendHim)
{
	int ID = g_CCore->GetPlayerPool()->GetID(this);
	this->SwitchWeapon(wepID);

	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_SWITCHWEAPON);
	bsOut.Write(ID);
	bsOut.Write(wepID);
	if (shouldSendHim)
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, MEDIUM_PRIORITY, RELIABLE, LHMP_NETCHAN_STATECHANGE, UNASSIGNED_SYSTEM_ADDRESS, true);	// send this message everybody including this player - when using Squirrel
	else
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, MEDIUM_PRIORITY, RELIABLE, LHMP_NETCHAN_STATECHANGE, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), true);	// else, when it arrives from player
}

void	CPlayer::OnPlayerShoot(Vector3D position, bool shouldSendHim)
{
	int ID = g_CCore->GetPlayerPool()->GetID(this);
	this->OnShoot();
	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_SHOOT);
	bsOut.Write(ID);
	bsOut.Write(position);
	bsOut.Write(this->GetCurrentWeapon());
	if (shouldSendHim)
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE, LHMP_NETCHAN_SHOOT, UNASSIGNED_SYSTEM_ADDRESS, true);	// send this message everybody including this player - when using Squirrel
	else
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE, LHMP_NETCHAN_SHOOT, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), true);	// else, when it arrives from player
		g_CCore->GetScripts()->onPlayerShoot(ID, this->GetCurrentWeapon());
}

void	CPlayer::OnPlayerThrowGrenade(Vector3D position, bool shouldSendHim)
{
	int ID = g_CCore->GetPlayerPool()->GetID(this);


	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_THROWGRANADE);
	bsOut.Write(ID);
	bsOut.Write(position);
	if (shouldSendHim)
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE, LHMP_NETCHAN_SHOOT, UNASSIGNED_SYSTEM_ADDRESS, true);	// send this message everybody including this player - when using Squirrel
	else
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE, LHMP_NETCHAN_SHOOT, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), true);	// else, when it arrives from player
		//g_CCore->GetScripts()->onPlayerThrowGranade(ID, this->GetCurrentWeapon());
	
	this->OnThrowGrenade();
	
}


void	CPlayer::ChangeNickColour(unsigned int color)
{
	this->SetNicknameColour(color);
}