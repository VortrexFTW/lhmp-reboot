// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef __LOCALPLAYER_H
#define __LOCALPLAYER_H

#include "../shared/structures.h"

class CLocalPlayer
{
private:
	bool			bIsOnFoot;
	int				ourID;
	char			nickname[80];
	int				ping;
public:
	CLocalPlayer();
	~CLocalPlayer();
	void			Init();
	int				GetPing();
	
	Vector3D		GetLocalRot();
	int				GetOurID();
	Vector3D		GetLocalPos();
	char*			GetNickname();
	void			SetNickname(char*);
	bool			IsOnFoot();
	void			SetIsOnFoot(bool);
	void			Pulse();
	void			SetLocalPos(Vector3D);
	void			SetLocalRot(Vector3D);
	void			SetID(int);
	void			SetPedStatus(int);
	void			SetPing(int);
	void			SetMoney(int);
	int				GetMoney();
	void			EnableMoney(int);
	int				ourSkin;
	int				ourAnim;
	DWORD			GetBase();
	DWORD			GetEntity();
	int				IDinCar;
	void			SetHealth(float);
	float			carAim;

	bool			HasWeapon(int);
	void			SetWeapon(int, int, int);

	byte			GetStatus();
	bool			GetIsAiming();
	bool			GetIsDucking();
	bool			GetIsCarAnim();
	float			GetHealth();
	float			GetCarAim();

	void			SetSkin(int);
	// Server
	void			ServerUpdateWeapon();

	// Callback

	void			OnDeath(int killer, int reason, int part);

};
#endif