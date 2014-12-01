//------------------------------------------------------------------------------

/*
	SA-MP Vending Machine include

	Description:
		This include provide code to create server-side vending machines in SA-MP.

	License:
		The MIT License (MIT)
		Copyright (c) 2014 Larceny
		Permission is hereby granted, free of charge, to any person obtaining a copy
		of this software and associated documentation files (the "Software"), to deal
		in the Software without restriction, including without limitation the rights
		to use, copy, modiY, merge, publish, distribute, sublicense, and/or sell
		copies of the Software, and to permit persons to whom the Software is
		furnished to do so, subject to the following conditions:
		The above copyright notice and this permission notice shall be included in all
		copies or substantial portions of the Software.
		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
		SOFTWARE.

	Author:
		Larceny

	Contributors:
		Y_Less - ALS Hooking method

	Version:
		1.0
*/

//------------------------------------------------------------------------------

#if defined _vendingm_included
	#endinput
#endif
#define _vendingm_included

//------------------------------------------------------------------------------

#if !defined MAX_MACHINES
	#define MAX_MACHINES 			32
#endif

#define MACHINE_SNACK 				956
#define MACHINE_SPRUNK 				955
#define MACHINE_SODA				1302

#define SODA_RADIUS					1.2
#define SPRUNK_RADIUS				1.05
#define SNACK_RADIUS				1.05

#define INVALID_MACHINE_ID			-1

//------------------------------------------------------------------------------

/*Natives
native CreateMachine(objectid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz);
native OnPlayerUseMachine(playerid, machineid);
native GetMachineRot(machineid, &Float:rx, &Float:ry, &Float:rz);
native SetMachineRot(machineid, Float:rx, Float:ry, Float:rz);
native GetMachinePos(machineid, &Float:x, &Float:y, &Float:z);
native SetMachinePos(machineid, Float:x, Float:y, Float:z);
native DestroyMachine(machineid);
native GetMachineType(machineid);
native IsValidMachine(machineid);*/

//------------------------------------------------------------------------------

enum E_VENDING_DATA
{
	Float:E_VENDING_X,
	Float:E_VENDING_Y,
	Float:E_VENDING_Z,
	Float:E_VENDING_RX,
	Float:E_VENDING_RY,
	Float:E_VENDING_RZ,
	Float:E_VENDING_RADIUS,
	E_VENDING_TYPE,
	E_VENDING_ID
}
static g_eVendingData[MAX_MACHINES][E_VENDING_DATA];

//------------------------------------------------------------------------------

forward OnPlayerUseVendingMachine(playerid, machineid);
forward OnPlayerDrinkSprunk(playerid);
forward ven_SetPlayerAnimation(playerid);

//------------------------------------------------------------------------------

stock CreateMachine(objectid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
	new machineid = GetFreeMachineID();

	if(machineid == INVALID_MACHINE_ID)
	{
		print("ERROR: Limit of vending machines exceeded! Increase the limit or reduce the created machines.");
		return 0;
	}

	switch(objectid)
	{
		case MACHINE_SPRUNK:
			g_eVendingData[machineid][E_VENDING_RADIUS] = SPRUNK_RADIUS;
		case MACHINE_SNACK:
			g_eVendingData[machineid][E_VENDING_RADIUS] = SNACK_RADIUS;
		case MACHINE_SODA:
			g_eVendingData[machineid][E_VENDING_RADIUS] = SODA_RADIUS;
		default:
		{
			printf("ERROR: Invalid vending object id! (Used ID: %i - Valid IDs: 955, 956 or 1302)", objectid);
			return 0;
		}
	}

	g_eVendingData[machineid][E_VENDING_ID]	= CreateObject(objectid, x, y, z, rx, ry, rz, 300.0);

	g_eVendingData[machineid][E_VENDING_X]	= x;
	g_eVendingData[machineid][E_VENDING_Y]	= y;
	g_eVendingData[machineid][E_VENDING_Z]	= z;

	g_eVendingData[machineid][E_VENDING_RX]	= rx;
	g_eVendingData[machineid][E_VENDING_RY]	= ry;
	g_eVendingData[machineid][E_VENDING_RZ]	= rz;

	g_eVendingData[machineid][E_VENDING_TYPE] = objectid;
	return machineid;
}

//------------------------------------------------------------------------------

stock DestroyMachine(machineid)
{
	if(!IsValidMachine(machineid))
		return 0;

    DestroyObject(g_eVendingData[machineid][E_VENDING_ID]);

    g_eVendingData[machineid][E_VENDING_ID] = INVALID_MACHINE_ID;

	g_eVendingData[machineid][E_VENDING_X] = 0.0;
	g_eVendingData[machineid][E_VENDING_Y] = 0.0;
	g_eVendingData[machineid][E_VENDING_Z] = 0.0;

	g_eVendingData[machineid][E_VENDING_RX] = 0.0;
	g_eVendingData[machineid][E_VENDING_RY] = 0.0;
	g_eVendingData[machineid][E_VENDING_RZ] = 0.0;

	g_eVendingData[machineid][E_VENDING_TYPE] = 0;
    return 1;
}

//------------------------------------------------------------------------------

stock IsValidMachine(machineid)
	return !(g_eVendingData[machineid][E_VENDING_X] == 0.0 && g_eVendingData[machineid][E_VENDING_Y] == 0.0);

//------------------------------------------------------------------------------

stock GetMachineType(machineid)
	return g_eVendingData[machineid][E_VENDING_TYPE];

//------------------------------------------------------------------------------

stock GetFreeMachineID()
{
	for(new i; i < MAX_MACHINES; i++)
		if(g_eVendingData[i][E_VENDING_X] == 0.0 && g_eVendingData[i][E_VENDING_Y] == 0.0)
			return i;

	return INVALID_MACHINE_ID;
}

//------------------------------------------------------------------------------

stock SetMachinePos(machineid, Float:x, Float:y, Float:z)
{
	if(!IsValidMachine(machineid))
		return 0;

	g_eVendingData[machineid][E_VENDING_X] = x;
	g_eVendingData[machineid][E_VENDING_Y] = y;
	g_eVendingData[machineid][E_VENDING_Z] = z;
	SetObjectPos(g_eVendingData[machineid][E_VENDING_ID], x, y, z);
	return 1;
}

//------------------------------------------------------------------------------

stock SetMachineRot(machineid, Float:rx, Float:ry, Float:rz)
{
	if(!IsValidMachine(machineid))
		return 0;

	g_eVendingData[machineid][E_VENDING_RX] = rx;
	g_eVendingData[machineid][E_VENDING_RY] = ry;
	g_eVendingData[machineid][E_VENDING_RZ] = rz;
	SetObjectRot(g_eVendingData[machineid][E_VENDING_ID], rx, ry, rz);
	return 1;
}

//------------------------------------------------------------------------------

stock GetMachinePos(machineid, &Float:x, &Float:y, &Float:z)
{
	if(!IsValidMachine(machineid))
		return 0;

	x = g_eVendingData[machineid][E_VENDING_X];
	y = g_eVendingData[machineid][E_VENDING_Y];
	z = g_eVendingData[machineid][E_VENDING_Z];
	return 1;
}

//------------------------------------------------------------------------------

stock GetMachineRot(machineid, &Float:rx, &Float:ry, &Float:rz)
{
	if(!IsValidMachine(machineid))
		return 0;

	rx = g_eVendingData[machineid][E_VENDING_RX];
	ry = g_eVendingData[machineid][E_VENDING_RY];
	rz = g_eVendingData[machineid][E_VENDING_RZ];
	return 1;
}

//------------------------------------------------------------------------------

stock GetMachineObjectID(machineid)
	return g_eVendingData[machineid][E_VENDING_ID];

//------------------------------------------------------------------------------

GetXYInFrontOfVending(machineid, &Float:x, &Float:y, Float:distance)
{
	new Float:a, Float:z;
	GetMachineRot(machineid, x, y, a);
	GetMachinePos(machineid, x, y, z);

	a += 180.0;
	if(a > 359.0) a -= 359.0;

	x += (distance * floatsin(-a, degrees));
	y += (distance * floatcos(-a, degrees));
}

//------------------------------------------------------------------------------

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(newkeys & KEY_SECONDARY_ATTACK && GetPlayerAnimationIndex(playerid) != 1660)
	{
		for(new i; i < MAX_MACHINES; i++)
		{
			if(!IsValidMachine(i))
				continue;

			new Float:x, Float:y;
			GetXYInFrontOfVending(i, x, y, 0.5);
			if(!IsPlayerInRangeOfPoint(playerid, g_eVendingData[i][E_VENDING_RADIUS], x, y, g_eVendingData[i][E_VENDING_Z]))
				continue;

			SetPlayerFacingAngle(playerid, g_eVendingData[i][E_VENDING_RZ]);
			ApplyAnimation(playerid, "VENDING", "VEND_USE", 10.0, 0, 0, 0, 0, 0, 1);
			if(g_eVendingData[i][E_VENDING_TYPE] == MACHINE_SNACK) PlayerPlaySound(playerid, 42601, 0.0, 0.0, 0.0);
			else PlayerPlaySound(playerid, 42600, 0.0, 0.0, 0.0);
			OnPlayerUseVendingMachine(playerid, i);
			SetTimerEx("ven_SetPlayerAnimation", 2500, false, "i", playerid);
		}
	}
	
	if((newkeys & KEY_FIRE) && (GetPVarInt(playerid, "ven_lastDrink") < GetTickCount()) && (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_DRINK_SPRUNK))
	{
		OnPlayerDrinkSprunk(playerid);
		SetPVarInt(playerid, "ven_lastDrink", GetTickCount() + 2350);
	}
	#if defined inc_Ven_OnPlayerKeyStateChange
		return inc_Ven_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#else
		return 0;
	#endif
}
#if defined _ALS_OnPlayerKeyStateChange
	#undef OnPlayerKeyStateChange
#else
	#define _ALS_OnPlayerKeyStateChange
#endif

#define OnPlayerKeyStateChange inc_Ven_OnPlayerKeyStateChange
#if defined inc_Ven_OnPlayerKeyStateChange
	forward inc_Ven_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

//------------------------------------------------------------------------------

public ven_SetPlayerAnimation(playerid)
{
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);
	return 1;
}

//------------------------------------------------------------------------------
