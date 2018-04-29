// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef __ENTITY_H
#define	__ENTITY_H
#include "CInterpolation.h"
#include "../shared/structures.h"
class CEntity
{
protected:
	DWORD		EntityBase;
	bool		isActive;
	Vector3D	playerPos;
	Vector3D	rotation;
	float		fHealth;
	byte		state;
	byte		isSpawned;
	int			skinId;

	// interpolation
	RakNet::TimeMS	timestamp;
	bool			shouldUpdate;
	//bool			bIsTempInterpolationRunning;
	//bool			bIsInterpolationRunning;
	//RakNet::TimeMS	dwMsTempRun;

	CInterpolation	interpolation;



public:
	CEntity();
	DWORD		GetEntity();
	void		SetEntity(DWORD);

	void		SetPosition(Vector3D vect);
	void		SetName(char[]);
	void		SetHealth(float);
	virtual void		SetRotation(Vector3D);
	virtual Vector3D	GetRotation();
	void		SetState(byte);
	void		SetSkinId(int);
	void		SetActive(bool);

	bool		IsActive();
	char*		GetName();
	float		GetHealth();
	byte		GetState();
	virtual	Vector3D	GetPosition();

	int			GetSkin();

	void			SetTimeStamp(RakNet::TimeMS);
	RakNet::TimeMS	GetTimeStamp();

	bool			ShouldUpdate();
	void			SetShouldUpdate(bool);

	virtual		void SetUpInterpolation() = 0;
	virtual		void Interpolate() = 0;

	CInterpolation* GetInterpolation();

};
#endif