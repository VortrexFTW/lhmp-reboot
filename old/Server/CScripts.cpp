// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CCore.h"
#include "CScripts.h"

extern CCore* g_CCore;

CScripts::CScripts()
{
	// reset all script slots
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		m_pScripts[i] = NULL;
	}
}

CScripts::~CScripts()
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i] != NULL) delete m_pScripts[i];
		m_pScripts[i] = NULL;
	}
}

bool CScripts::LoadScript(const char * szScriptName)
{
	// make sure a script with the same name isn't already loaded
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (!m_pScripts[i]) continue;
		if (!strcmp(m_pScripts[i]->GetScriptName(), szScriptName)) {
			// a script with the same name already exists
			return false;
		}
	}

	// find a free script slot
	int iSlot = -1;

	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (!m_pScripts[i]) {
			// found a free slot
			iSlot = i;
			break;
		}
	}

	if (iSlot == -1) {
		// no free script slot found
		return false;
	}

	if (m_pScripts[iSlot] == NULL)
	{
		m_pScripts[iSlot] = new CScript(szScriptName);
	}
	// script loaded successfully
	return true;
}

bool CScripts::UnloadScript(const char * szScriptName)
{
	// find the script slot
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (!strcmp(m_pScripts[i]->GetScriptName(), szScriptName)) {
			// found the script slot, unload the script

			delete m_pScripts[i];

			m_pScripts[i] = NULL;
			// script unloaded successfully
			return true;
		}
	}

	// script not found
	return false;
}

void CScripts::UnloadAll()
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i] != NULL)
		{
			delete m_pScripts[i];

			m_pScripts[i] = NULL;
		}
	}
}

void CScripts::Call(const char * szFunc, int iArgCount, SQObject * pArguments)
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, szFunc, -1);

			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				if (pArguments != NULL)
				{
					for (int j = 0; j < iArgCount; ++j)
						sq_pushobject(pVM, pArguments[j]);
					//pVM->Push(pArguments[j]);
				}
				sq_call(pVM, iArgCount + 1, true, true);

			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onServerInit()
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onServerInit", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Call the function
				sq_call(pVM, 1, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onServerExit()
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onServerExit", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Call the function
				sq_call(pVM, 1, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

bool CScripts::onPlayerText(int playerId, const char *text)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerText", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 3, true, true)))
				{
					SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
	return ret;
}
void CScripts::onPlayerCommand(int playerId, const char *text,const char* params)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerCommand", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				sq_pushstring(pVM, params, -2);

				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 4, false, true)))
				{
					/*SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;*/
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerConnect(int playerID)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerConnect", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerID);

			/*	// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				sq_pushstring(pVM, params, -2);
				*/
				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 2, false, true)))
				{
					/*SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;*/
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}
void CScripts::onServerTick(int ticks)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onServerTick", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, ticks);

				/*	// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				sq_pushstring(pVM, params, -2);
				*/
				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 2, false, true)))
				{
					/*SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;*/
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}
void CScripts::onServerTickSecond(int ticks)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onServerTickSecond", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, ticks);

				/*	// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				sq_pushstring(pVM, params, -2);
				*/
				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 2, false, true)))
				{
					/*SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;*/
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}
void CScripts::onPlayerDisconnect(int playerID)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerDisconnect", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerID);
				/*
				// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				sq_pushstring(pVM, params, -2);*/

				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 2, false, true)))
				{
					/*SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;*/
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}
void CScripts::onPlayerEnterVehicle(int playerID, int vehID, int seatID)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerEnterVehicle", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerID);
				sq_pushinteger(pVM, vehID);
				sq_pushinteger(pVM, seatID);

				/*// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				sq_pushstring(pVM, params, -2);*/

				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 4, false, true)))
				{
					/*SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;*/
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}
void CScripts::onPlayerExitVehicle(int playerID, int vehID)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerExitVehicle", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerID);
				sq_pushinteger(pVM, vehID);

				/*// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				sq_pushstring(pVM, params, -2);*/

				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 3, false, true)))
				{
					/*SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;*/
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerExitVehicleFinish(int playerID)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerExitVehicleFinish", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerID);

				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 3, false, true)))
				{
					/*SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;*/
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerSpawn(int playerID)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerSpawn", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerID);

				/*// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				sq_pushstring(pVM, params, -2);*/

				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 2, false, true)))
				{
					/*SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;*/
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerIsKilled(int playerID, int killerID, int reason, int hitbox)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerIsKilled", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerID);

				sq_pushinteger(pVM, killerID);

				sq_pushinteger(pVM, reason);

				sq_pushinteger(pVM, hitbox);

				/*// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				sq_pushstring(pVM, params, -2);*/

				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 5, false, true)))
				{
					/*SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;*/
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerKeyPressed(int ID,int key)
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerKeyPressed", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack

				sq_pushinteger(pVM, ID);
				sq_pushinteger(pVM, key);

				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 3, false, true)))
				{
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onVehicleExploded(int ID)
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onVehicleExploded", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack

				sq_pushinteger(pVM, ID);

				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 2, false, true)))
				{
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

/*
void CScripts::onDoorSetState(int ID, int state, bool facing, char name)
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerKeyPressed", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack

				sq_pushinteger(pVM, ID);
				sq_pushinteger(pVM, state);
				sq_pushbool(pVM, facing);
				sq_pushstring(pVM, (char)name, sizeof(name));

				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 3, false, true)))
				{
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}
*/

void CScripts::onPlayerShoot(int ID, int weaponID)
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerShoot", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack

				sq_pushinteger(pVM, ID);
				sq_pushinteger(pVM, weaponID);

				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 3, false, true)))
				{
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

/*
void CScripts::onVehicleShot(int vehID, int playerID, byte damage, int wepID)
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onVehicleShot", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack

				sq_pushinteger(pVM, vehID);
				sq_pushinteger(pVM, playerID);
				sq_pushinteger(pVM, wepID);
				sq_pushinteger(pVM, (int)damage);

				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 5, false, true)))
				{
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}
*/

void CScripts::onPlayerHit(int ID,int attackerID)
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerHit", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack

				sq_pushinteger(pVM, ID);
				sq_pushinteger(pVM, attackerID);

				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 3, false, true)))
				{
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPickupTaken(int pickupID, int playerID)
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPickupTaken", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack

				sq_pushinteger(pVM, pickupID);
				sq_pushinteger(pVM, playerID);

				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 3, false, true)))
				{
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}


void CScripts::callServerFunc(int playerID, BitStream* bsIn)
{
	char script_name[500];
	char func_name[500];

	bsIn->Read(script_name);
	bsIn->Read(func_name);

	g_CCore->GetLog()->AddNormalLog("callServerFunc[%d] [%s][%s]", playerID, script_name, func_name);

	CScript* pointer = NULL;
	for (int i = 0; i < 10; i++)
	{
		if (this->m_pScripts[i] != NULL)
		{
			g_CCore->GetLog()->AddNormalLog("Comparing: %s %s", script_name, this->m_pScripts[i]->GetScriptName());
			if (strcmp(this->m_pScripts[i]->GetScriptName(),script_name) == 0)
			{
				pointer = this->m_pScripts[i];
				break;
			}
		}
	}

	

	if (pointer)
	{
		// now get type of param and serialize it
		SQInteger iPar;
		SQFloat fPar;
		char parString[500];
		SQBool	bPar;

		SQObjectType type;
		bsIn->Read(type);

		int len;

		int iTop = sq_gettop(pointer->GetVM());
		sq_pushroottable(pointer->GetVM());

		// Push the function name onto the stack
		sq_pushstring(pointer->GetVM(), func_name, -1);
		// Get the closure for the function
		if (SQ_SUCCEEDED(sq_get(pointer->GetVM(), -2))) {
			// Push the root table onto the stack
			sq_pushroottable(pointer->GetVM());
			sq_pushinteger(pointer->GetVM(), playerID);
			switch (type)
			{
			case OT_INTEGER:
				bsIn->Read(iPar);
				sq_pushinteger(pointer->GetVM(), iPar);
				break;
			case OT_FLOAT:
				bsIn->Read(fPar);
				sq_pushfloat(pointer->GetVM(), fPar);
				break;
			case OT_STRING:
				bsIn->Read(parString);
				len = strlen(parString);
				sq_pushstring(pointer->GetVM(), parString, len);
				break;
			case OT_BOOL:
				bsIn->Read(bPar);
				sq_pushbool(pointer->GetVM(), bPar);
				break;
			case OT_NULL:
				// if null, no extra data are sent
				sq_pushnull(pointer->GetVM());
			default:
			case -1:
				// in case bad type occurs, do nothing
				break;
			}

			g_CCore->GetLog()->AddNormalLog("Calling func");
			sq_call(pointer->GetVM(), 3, true, true);
		}
		sq_settop(pointer->GetVM(), iTop);
	}
	else {
		g_CCore->GetLog()->AddNormalLog("NO SCRIPT FOUND");
	}
}

bool CScripts::callFunc(SQVM *vm)
{
	const SQChar* script_name, *func_name;

	sq_getstring(vm, -3, &script_name);
	sq_getstring(vm, -2, &func_name);

	CScript* pointer = NULL;
	for (int i = 0; i < 10; i++)
	{
		if (this->m_pScripts[i] != NULL)
		{
			g_CCore->GetLog()->AddNormalLog("Comparing: %s %s", script_name, this->m_pScripts[i]->GetScriptName());
			if (strcmp(this->m_pScripts[i]->GetScriptName(), script_name) == 0)
			{
				pointer = this->m_pScripts[i];
				break;
			}
		}
	}

	if (pointer != NULL)
	{
		// save parameter from calling script
		HSQOBJECT	objParam;
		
		sq_getstackobj(vm, -1, &objParam);

		int iTop = sq_gettop(pointer->GetVM());
		sq_pushroottable(pointer->GetVM());

		// Push the function name onto the stack
		sq_pushstring(pointer->GetVM(), func_name, -1);
		// Get the closure for the function
		if (SQ_SUCCEEDED(sq_get(pointer->GetVM(), -2))) {
			// Push the root table onto the stack
			sq_pushroottable(pointer->GetVM());
			sq_pushobject(pointer->GetVM(), objParam);
			sq_call(pointer->GetVM(), 2, true, true);
		}
		sq_settop(pointer->GetVM(), iTop);

		return true;
	}
	else {
		return false;
	}

}