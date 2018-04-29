// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef __PED_H
#define	__PED_H
#include "CEntity.h"
#include "../shared/structures.h"
#include <d3d8.h>
class CPed : public CEntity
{
private:
	bool		isOnFoot;
	char		sName[80];
	byte		isSpawned;
	byte		isDucking;
	byte		isAiming;
	byte		isCarAnim;
	int			animId;

	float		floatRot;

	int			ping;
	unsigned int nickColor;
	// interpolation
	Vector3D			actual, previous;
	// weapons
	int				currentWep;
	SWeapon			weapon[8];

public:
	CPed();
	~CPed();

	LPDIRECT3DTEXTURE8		nametag;		// TODO move to private

	void		SetIsOnFoot(bool);
	bool		IsOnFoot();

	void		UpdateGameObject();
	void		SetName(char[]);
	void		SetDucking(byte);
	void		SetAiming(byte);
	void		SetPing(int);
	void		SetActive(bool);
	void		SetCarAnim(bool);
	bool		IsCarAnim();

	bool		IsActive();
	void		SetSkinId(int id);
	int			GetSkin();
	int			GetPing();
	char*		GetName();
	byte		IsDucking();

	void		SetAnim(int);
	int			GetAnim();
	void		SetUpInterpolation();
	void		Interpolate();

	virtual Vector3D GetPosition();
	bool		HasBase();
	Vector3D	GetPEDFramePosition();

	void		SetNickColor(unsigned int);
	unsigned int GetNickColor();

	// Weapon actions
	SWeapon* GetWeapon(int);
	int	 GetCurrentWeapon();
	void SetCurrentWeapon(int);
	void AddWeapon(int, int, int);
	void DeleteWeapon(int);
	void SwitchWeapon(int);
	void OnShoot();
	void OnThrowGranade();
	void ClearWeapons();

	void CheckWeapons();


	int			InCar;
};
#endif