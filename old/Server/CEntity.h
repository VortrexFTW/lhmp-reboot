// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef ENTITY_H
#define ENTITY_H

#include "../shared/structures.h"

class CEntity
{
protected:
	bool			Active;

	Vector3D		Position;
	Vector3D		Rotation;
	float			Health;
	byte			Status;
	int				SkinID;
	RakNet::TimeMS	SyncTime;

	// bools
	bool			Spawned;
	RakNet::TimeMS	Timestamp;
	bool			ShouldUpdate;
public:
	CEntity();
	~CEntity();

	void SetPosition(Vector3D);
	virtual void SetRotation(Vector3D);
	void SetHealth(float);
	void SetStatus(byte);
	void SetSkin(int);
	void SetIsSpawned(bool);
	void SetIsActive(bool);

	Vector3D		GetPosition();
	virtual  Vector3D		GetRotation();
	//float	GetRotationSecond();
	float			GetHealth();
	byte			GetStatus();
	int				GetSkin();
	bool			IsSpawned();
	bool			IsDucking();
	bool			IsActive();

	void			SetTimeStamp(RakNet::TimeMS);
	RakNet::TimeMS	GetTimeStamp();

	bool			GetShouldUpdate();
	void			SetShouldUpdate(bool);
};
#endif