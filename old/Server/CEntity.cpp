// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CEntity.h"

CEntity::CEntity()
{
	this->Active		= 1;
	this->Health		= 200.0f;
	this->Spawned		= 1;
	this->ShouldUpdate	= true;
	this->Timestamp		= NULL;
	this->SkinID		= 0;
	this->Status		= 0;

}

CEntity::~CEntity()
{
}

float CEntity::GetHealth()
{
	return this->Health;
}

Vector3D CEntity::GetPosition()
{
	return this->Position;
}

Vector3D CEntity::GetRotation()
{
	return this->Rotation;
}
/*float CEntity::GetRotationSecond()
{
	return this->degree_second;
}*/

int CEntity::GetSkin()
{
	return this->SkinID;
}
byte CEntity::GetStatus()
{
	return this->Status;
}

bool CEntity::IsSpawned()
{
	return this->Spawned;
}
bool CEntity::IsActive()
{
	return this->Active;
}

void CEntity::SetHealth(float Health)
{
	this->Health = Health;
}

void CEntity::SetIsActive(bool IsActive)
{
	this->Active = IsActive;
}

void CEntity::SetIsSpawned(bool IsSpawned)
{
	this->Spawned = IsSpawned;
}
void CEntity::SetPosition(Vector3D Position)
{
	this->Position = Position;
}

void CEntity::SetRotation(Vector3D rot)
{
	/*this->degree = r1;
	this->degree_second = r2;*/
	this->Rotation = rot;
}

void CEntity::SetSkin(int ID)
{
	this->SkinID = ID;
}

void CEntity::SetStatus(byte state)
{
	this->Status = state;
}

void CEntity::SetTimeStamp(RakNet::TimeMS ts)
{
	this->Timestamp = ts;
	this->ShouldUpdate = true;
}
RakNet::TimeMS CEntity::GetTimeStamp()
{
	return this->Timestamp;
}

bool CEntity::GetShouldUpdate()
{
	return this->ShouldUpdate;
}


void CEntity::SetShouldUpdate(bool b)
{
	this->ShouldUpdate = b;
}