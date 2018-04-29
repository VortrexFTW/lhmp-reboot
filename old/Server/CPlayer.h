// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef CPLAYER_H
#define CPLAYER_H
#include "../shared/structures.h"
#include "CEntity.h"
class CPlayer: public CEntity
{
public:
	CPlayer();
	~CPlayer();

	void SetFloatRotation(float);
	float GetFloatRotation();

	void	SetNickname(char[]);
	void	SetNickname(const char*);
	char*	GetNickname();
	void	SetIsDucking(bool);
	bool	IsDucking();
	void	SetIsAim(bool);
	bool	IsAim();
	void	SetIsActive(bool);
	bool    IsActive();
	void	SetCurrentWeapon(int);
	int		GetCurrentWeapon();
	void	SetCarAim(float);
	float	GetCarAim();
	void	SetIsCarAnim(bool);
	bool	IsCarAnim();
	void	SetMoney(int);
	int		GetMoney();
	void	SetFieldOfView(float);
	float	GetFieldOfView();

	void	SetCurrentCar(int);
	int		GetCurrentCar();

	void	SetNicknameColour(unsigned int);
	unsigned int GetNicknameColour();

	// Weapon actions
	void AddWeapon(int,int,int);
	void DropWeapon(int);
	void SwitchWeapon(int);
	void OnShoot();
	void ResetWeapons();
	void OnThrowGrenade();

	SWeapon* GetWeapon(int);

	bool	IsLocked();
	void	SetLocked(bool);

	//------ Network functions
	void	OnFootUpdate(SYNC::ON_FOOT_SYNC,RakNet::TimeMS);
	void	OnVehicleUpdate(SYNC::IN_CAR);
	void	OnChangeSkin(int);
	void	OnRespawn();
	void	OnAddWeapon(int,int,int,bool = false);
	void	OnDropWeapon(int, bool = false);
	void	OnSwitchWeapon(int, bool = false);
	void	OnPlayerShoot(Vector3D position, bool = false);
	void	OnPlayerThrowGrenade(Vector3D, bool = false);

	void	ChangeNickColour(unsigned int color);

private:
	char			nickname[255];

	bool			Ducking;
	bool			Aiming;
	bool			AimingInVehicle;

	// ID of car that is used by player
	int				VehicleID;

	// Weapons
	int				Weapon;
	SWeapon			Weapons[8];

	float			VehicleAim;
	float			FieldOfView;
	int				Money;

	bool			Locked;

	float			FloatRotation;

	unsigned int	NicknameColour;
};

#endif