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
		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
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
		Y_Less
		Abagail
		J'Zargo
		Slice

	Version:
		2.1
*/

//------------------------------------------------------------------------------

#if defined _vendingm_included
	#endinput
#endif
#define _vendingm_included

//------------------------------------------------------------------------------

#if !defined MAX_MACHINES
	#define MAX_MACHINES 			128
#endif

#define OBJECT_TYPE_NORMAL			0
#define OBJECT_TYPE_DYNAMIC			1

#define MACHINE_SNACK 				956
#define MACHINE_SNACK2 				1776
#define MACHINE_SPRUNK 				955
#define MACHINE_SPRUNK2				1775
#define MACHINE_SODA				1302
#define MACHINE_SODA2				1209
#define MACHINE_BLACK				1977

#define SODA_RADIUS					1.2
#define SPRUNK_RADIUS				1.05
#define SNACK_RADIUS				1.05

#define INVALID_MACHINE_ID			-1

//------------------------------------------------------------------------------

/*Natives
native CreateVendingMachine(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz);
native GetVendingMachineRot(machineid, &Float:rx, &Float:ry, &Float:rz);
native SetVendingMachineRot(machineid, Float:rx, Float:ry, Float:rz);
native GetVendingMachinePos(machineid, &Float:x, &Float:y, &Float:z);
native SetVendingMachinePos(machineid, Float:x, Float:y, Float:z);
native SetVendingMachineColor(machineid, color);
native OnPlayerUseVendinMachine(playerid, machineid);
native GetVendingMachineColor(machineid);
native DestroyVendingMachine(machineid);
native GetVendingMachineType(machineid);
native IsValidVendingMachine(machineid);*/

//------------------------------------------------------------------------------

// Temporary:

#define CreateMachine 		CreateVendingMachine
#define DestroyMachine 		DestroyVendingMachine

#define GetMachineRot 		GetVendingMachineRot
#define SetMachineRot 		SetVendingMachineRot

#define GetMachinePos 		GetVendingMachinePos
#define SetMachinePos 		SetVendingMachinePos

#define GetMachineType 		GetVendingMachineType
#define IsValidMachine 		IsValidVendingMachine

//------------------------------------------------------------------------------

// Thanks to Slice:

enum E_VENDING_MACHINE
{
	e_Model,
	e_Interior,
	Float:e_PosX,
	Float:e_PosY,
	Float:e_PosZ,
	Float:e_RotX,
	Float:e_RotY,
	Float:e_RotZ,
	Float:e_FrontX,
	Float:e_FrontY
}

stock const Float:sc_VendingMachines[][E_VENDING_MACHINE] =
{
	{955, 0, -862.82, 1536.60, 21.98, 0.00, 0.00, 180.00, -862.84, 1537.60},
	{956, 0, 2271.72, -76.46, 25.96, 0.00, 0.00, 0.00, 2271.72, -77.46},
	{955, 0, 1277.83, 372.51, 18.95, 0.00, 0.00, 64.00, 1278.73, 372.07},
	{956, 0, 662.42, -552.16, 15.71, 0.00, 0.00, 180.00, 662.41, -551.16},
	{955, 0, 201.01, -107.61, 0.89, 0.00, 0.00, 270.00, 200.01, -107.63},
	{955, 0, -253.74, 2597.95, 62.24, 0.00, 0.00, 90.00, -252.74, 2597.95},
	{956, 0, -253.74, 2599.75, 62.24, 0.00, 0.00, 90.00, -252.74, 2599.75},
	{956, 0, -76.03, 1227.99, 19.12, 0.00, 0.00, 90.00, -75.03, 1227.99},
	{955, 0, -14.70, 1175.35, 18.95, 0.00, 0.00, 180.00, -14.72, 1176.35},
	{1977, 7, 316.87, -140.35, 998.58, 0.00, 0.00, 270.00, 315.87, -140.36},
	{1775, 17, 373.82, -178.14, 1000.73, 0.00, 0.00, 0.00, 373.82, -179.14},
	{1776, 17, 379.03, -178.88, 1000.73, 0.00, 0.00, 270.00, 378.03, -178.90},
	{1775, 17, 495.96, -24.32, 1000.73, 0.00, 0.00, 180.00, 495.95, -23.32},
	{1776, 17, 500.56, -1.36, 1000.73, 0.00, 0.00, 0.00, 500.56, -2.36},
	{1775, 17, 501.82, -1.42, 1000.73, 0.00, 0.00, 0.00, 501.82, -2.42},
	{956, 0, -1455.11, 2591.66, 55.23, 0.00, 0.00, 180.00, -1455.13, 2592.66},
	{955, 0, 2352.17, -1357.15, 23.77, 0.00, 0.00, 90.00, 2353.17, -1357.15},
	{955, 0, 2325.97, -1645.13, 14.21, 0.00, 0.00, 0.00, 2325.97, -1646.13},
	{956, 0, 2139.51, -1161.48, 23.35, 0.00, 0.00, 87.00, 2140.51, -1161.53},
	{956, 0, 2153.23, -1016.14, 62.23, 0.00, 0.00, 127.00, 2154.03, -1015.54},
	{955, 0, 1928.73, -1772.44, 12.94, 0.00, 0.00, 90.00, 1929.73, -1772.44},
	{1776, 1, 2222.36, 1602.64, 1000.06, 0.00, 0.00, 90.00, 2223.36, 1602.64},
	{1775, 1, 2222.20, 1606.77, 1000.05, 0.00, 0.00, 90.00, 2223.20, 1606.77},
	{1775, 1, 2155.90, 1606.77, 1000.05, 0.00, 0.00, 90.00, 2156.90, 1606.77},
	{1775, 1, 2209.90, 1607.19, 1000.05, 0.00, 0.00, 270.00, 2208.90, 1607.17},
	{1776, 1, 2155.84, 1607.87, 1000.06, 0.00, 0.00, 90.00, 2156.84, 1607.87},
	{1776, 1, 2202.45, 1617.00, 1000.06, 0.00, 0.00, 180.00, 2202.43, 1618.00},
	{1776, 1, 2209.24, 1621.21, 1000.06, 0.00, 0.00, 0.00, 2209.24, 1620.21},
	{1776, 3, 330.67, 178.50, 1020.07, 0.00, 0.00, 0.00, 330.67, 177.50},
	{1776, 3, 331.92, 178.50, 1020.07, 0.00, 0.00, 0.00, 331.92, 177.50},
	{1776, 3, 350.90, 206.08, 1008.47, 0.00, 0.00, 90.00, 351.90, 206.08},
	{1776, 3, 361.56, 158.61, 1008.47, 0.00, 0.00, 180.00, 361.54, 159.61},
	{1776, 3, 371.59, 178.45, 1020.07, 0.00, 0.00, 0.00, 371.59, 177.45},
	{1776, 3, 374.89, 188.97, 1008.47, 0.00, 0.00, 0.00, 374.89, 187.97},
	{1775, 2, 2576.70, -1284.43, 1061.09, 0.00, 0.00, 270.00, 2575.70, -1284.44},
	{1775, 15, 2225.20, -1153.42, 1025.90, 0.00, 0.00, 270.00, 2224.20, -1153.43},
	{955, 0, 1154.72, -1460.89, 15.15, 0.00, 0.00, 270.00, 1153.72, -1460.90},
	{956, 0, 2480.85, -1959.27, 12.96, 0.00, 0.00, 180.00, 2480.84, -1958.27},
	{955, 0, 2060.11, -1897.64, 12.92, 0.00, 0.00, 0.00, 2060.11, -1898.64},
	{955, 0, 1729.78, -1943.04, 12.94, 0.00, 0.00, 0.00, 1729.78, -1944.04},
	{956, 0, 1634.10, -2237.53, 12.89, 0.00, 0.00, 0.00, 1634.10, -2238.53},
	{955, 0, 1789.21, -1369.26, 15.16, 0.00, 0.00, 270.00, 1788.21, -1369.28},
	{956, 0, -2229.18, 286.41, 34.70, 0.00, 0.00, 180.00, -2229.20, 287.41},
	{955, 256, -1980.78, 142.66, 27.07, 0.00, 0.00, 270.00, -1981.78, 142.64},
	{955, 256, -2118.96, -423.64, 34.72, 0.00, 0.00, 255.00, -2119.93, -423.40},
	{955, 256, -2118.61, -422.41, 34.72, 0.00, 0.00, 255.00, -2119.58, -422.17},
	{955, 256, -2097.27, -398.33, 34.72, 0.00, 0.00, 180.00, -2097.29, -397.33},
	{955, 256, -2092.08, -490.05, 34.72, 0.00, 0.00, 0.00, -2092.08, -491.05},
	{955, 256, -2063.27, -490.05, 34.72, 0.00, 0.00, 0.00, -2063.27, -491.05},
	{955, 256, -2005.64, -490.05, 34.72, 0.00, 0.00, 0.00, -2005.64, -491.05},
	{955, 256, -2034.46, -490.05, 34.72, 0.00, 0.00, 0.00, -2034.46, -491.05},
	{955, 256, -2068.56, -398.33, 34.72, 0.00, 0.00, 180.00, -2068.58, -397.33},
	{955, 256, -2039.85, -398.33, 34.72, 0.00, 0.00, 180.00, -2039.86, -397.33},
	{955, 256, -2011.14, -398.33, 34.72, 0.00, 0.00, 180.00, -2011.15, -397.33},
	{955, 2048, -1350.11, 492.28, 10.58, 0.00, 0.00, 90.00, -1349.11, 492.28},
	{956, 2048, -1350.11, 493.85, 10.58, 0.00, 0.00, 90.00, -1349.11, 493.85},
	{955, 0, 2319.99, 2532.85, 10.21, 0.00, 0.00, 0.00, 2319.99, 2531.85},
	{956, 0, 2845.72, 1295.04, 10.78, 0.00, 0.00, 0.00, 2845.72, 1294.04},
	{955, 0, 2503.14, 1243.69, 10.21, 0.00, 0.00, 180.00, 2503.12, 1244.69},
	{956, 0, 2647.69, 1129.66, 10.21, 0.00, 0.00, 0.00, 2647.69, 1128.66},
	{1209, 0, -2420.21, 984.57, 44.29, 0.00, 0.00, 90.00, -2419.21, 984.57},
	{1302, 0, -2420.17, 985.94, 44.29, 0.00, 0.00, 90.00, -2419.17, 985.94},
	{955, 0, 2085.77, 2071.35, 10.45, 0.00, 0.00, 90.00, 2086.77, 2071.35},
	{956, 0, 1398.84, 2222.60, 10.42, 0.00, 0.00, 180.00, 1398.82, 2223.60},
	{956, 0, 1659.46, 1722.85, 10.21, 0.00, 0.00, 0.00, 1659.46, 1721.85},
	{955, 0, 1520.14, 1055.26, 10.00, 0.00, 0.00, 270.00, 1519.14, 1055.24},
	{1775, 6, -19.03, -57.83, 1003.63, 0.00, 0.00, 180.00, -19.05, -56.83},
	{1775, 18, -16.11, -91.64, 1003.63, 0.00, 0.00, 180.00, -16.13, -90.64},
	{1775, 16, -15.10, -140.22, 1003.63, 0.00, 0.00, 180.00, -15.11, -139.22},
	{1775, 17, -32.44, -186.69, 1003.63, 0.00, 0.00, 180.00, -32.46, -185.69},
	{1775, 16, -35.72, -140.22, 1003.63, 0.00, 0.00, 180.00, -35.74, -139.22},
	{1776, 6, -36.14, -57.87, 1003.63, 0.00, 0.00, 180.00, -36.16, -56.87},
	{1776, 18, -17.54, -91.71, 1003.63, 0.00, 0.00, 180.00, -17.56, -90.71},
	{1776, 16, -16.53, -140.29, 1003.63, 0.00, 0.00, 180.00, -16.54, -139.29},
	{1776, 17, -33.87, -186.76, 1003.63, 0.00, 0.00, 180.00, -33.89, -185.76},
	{1775, 6, -19.03, -57.83, 1003.63, 0.00, 0.00, 180.00, -19.05, -56.83},
	{1776, 6, -36.14, -57.87, 1003.63, 0.00, 0.00, 180.00, -36.16, -56.87},
	{1775, 18, -16.11, -91.64, 1003.63, 0.00, 0.00, 180.00, -16.13, -90.64},
	{1776, 18, -17.54, -91.71, 1003.63, 0.00, 0.00, 180.00, -17.56, -90.71},
	{1776, 16, -16.53, -140.29, 1003.63, 0.00, 0.00, 180.00, -16.54, -139.29},
	{1775, 16, -15.10, -140.22, 1003.63, 0.00, 0.00, 180.00, -15.11, -139.22},
	{1776, 17, -33.87, -186.76, 1003.63, 0.00, 0.00, 180.00, -33.89, -185.76},
	{1775, 17, -32.44, -186.69, 1003.63, 0.00, 0.00, 180.00, -32.46, -185.69},
	{1775, 16, -35.72, -140.22, 1003.63, 0.00, 0.00, 180.00, -35.74, -139.22}
};

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
	E_VENDING_COLOR,
	E_VENDING_TYPE,
	E_VENDING_ID
}
static g_eVendingData[MAX_MACHINES][E_VENDING_DATA];

//------------------------------------------------------------------------------

forward OnPlayerUseVendingMachine(playerid, machineid);
#if !defined NO_SPRUNK_ACTION
forward OnPlayerDrinkSprunk(playerid);
forward ven_SetPlayerAnimation(playerid);
#endif

//------------------------------------------------------------------------------

stock CreateVendingMachine(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
	new machineid = GetFreeMachineID();

	if(machineid == INVALID_MACHINE_ID)
	{
		print("ERROR: Limit of vending machines exceeded! Increase the limit or reduce the created machines.");
		return 0;
	}

	switch(modelid)
	{
		case MACHINE_SPRUNK2:
			g_eVendingData[machineid][E_VENDING_RADIUS] = SPRUNK_RADIUS;
		case MACHINE_SPRUNK:
			g_eVendingData[machineid][E_VENDING_RADIUS] = SPRUNK_RADIUS;
		case MACHINE_SNACK:
			g_eVendingData[machineid][E_VENDING_RADIUS] = SNACK_RADIUS;
		case MACHINE_SNACK2:
			g_eVendingData[machineid][E_VENDING_RADIUS] = SNACK_RADIUS;
		case MACHINE_SODA:
			g_eVendingData[machineid][E_VENDING_RADIUS] = SODA_RADIUS;
		case MACHINE_SODA2:
			g_eVendingData[machineid][E_VENDING_RADIUS] = SODA_RADIUS;
		case MACHINE_BLACK:
			g_eVendingData[machineid][E_VENDING_RADIUS] = SPRUNK_RADIUS;
		default:
		{
			printf("ERROR: Invalid vending object id! (Used ID: %i - Valid IDs: 955, 956 or 1302)", modelid);
			return 0;
		}
	}

	#if defined Streamer_IncludeFileVersion
		g_eVendingData[machineid][E_VENDING_ID]	= CreateDynamicObject(modelid, x, y, z, rx, ry, rz);
	#else
		g_eVendingData[machineid][E_VENDING_ID]	= CreateObject(modelid, x, y, z, rx, ry, rz, 300.0);
	#endif

	g_eVendingData[machineid][E_VENDING_X]	= x;
	g_eVendingData[machineid][E_VENDING_Y]	= y;
	g_eVendingData[machineid][E_VENDING_Z]	= z;

	g_eVendingData[machineid][E_VENDING_RX]	= rx;
	g_eVendingData[machineid][E_VENDING_RY]	= ry;
	g_eVendingData[machineid][E_VENDING_RZ]	= rz;

	g_eVendingData[machineid][E_VENDING_TYPE] = modelid;
	return machineid;
}

//------------------------------------------------------------------------------

stock DestroyVendingMachine(machineid)
{
	if(!IsValidVendingMachine(machineid))
		return 0;

	#if defined Streamer_IncludeFileVersion
	DestroyDynamicObject(g_eVendingData[machineid][E_VENDING_ID]);
	#else
	DestroyObject(g_eVendingData[machineid][E_VENDING_ID]);
	#endif

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

stock SetVendingMachineColor(machineid, color)
{
	if(!IsValidVendingMachine(machineid) || GetVendingMachineType(machineid) == MACHINE_SNACK)
		return 0;

	if(GetVendingMachineType(machineid) == MACHINE_SODA)
	{
		#if defined Streamer_IncludeFileVersion
		SetDynamicObjectMaterial(g_eVendingData[machineid][E_VENDING_ID], 0, -1, "none", "none", color);
		SetDynamicObjectMaterial(g_eVendingData[machineid][E_VENDING_ID], 1, -1, "none", "none", color);
		#else
		SetObjectMaterial(g_eVendingData[machineid][E_VENDING_ID], 0, -1, "none", "none", color);
		SetObjectMaterial(g_eVendingData[machineid][E_VENDING_ID], 1, -1, "none", "none", color);
		#endif
	}
	else
	{
		#if defined Streamer_IncludeFileVersion
		SetDynamicObjectMaterial(g_eVendingData[machineid][E_VENDING_ID], 0, -1, "none", "none", color);
		#else
		SetObjectMaterial(g_eVendingData[machineid][E_VENDING_ID], 0, -1, "none", "none", color);
		#endif
	}

	g_eVendingData[machineid][E_VENDING_COLOR] = color;
	return 1;
}

//------------------------------------------------------------------------------

stock GetVendingMachineColor(machineid)
	return g_eVendingData[machineid][E_VENDING_COLOR];

//------------------------------------------------------------------------------

stock GetVendingMachineObjectType()
{
	#if defined Streamer_IncludeFileVersion
	return OBJECT_TYPE_DYNAMIC;
	#else
	return OBJECT_TYPE_NORMAL;
	#endif
}

//------------------------------------------------------------------------------
stock IsValidVendingMachine(machineid)
	return !(g_eVendingData[machineid][E_VENDING_X] == 0.0 && g_eVendingData[machineid][E_VENDING_Y] == 0.0);

//------------------------------------------------------------------------------

stock GetVendingMachineType(machineid)
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

stock SetVendingMachinePos(machineid, Float:x, Float:y, Float:z)
{
	if(!IsValidVendingMachine(machineid))
		return 0;

	g_eVendingData[machineid][E_VENDING_X] = x;
	g_eVendingData[machineid][E_VENDING_Y] = y;
	g_eVendingData[machineid][E_VENDING_Z] = z;

	#if defined Streamer_IncludeFileVersion
	SetDynamicObjectPos(g_eVendingData[machineid][E_VENDING_ID], x, y, z);
	#else
	SetObjectPos(g_eVendingData[machineid][E_VENDING_ID], x, y, z);
	#endif
	return 1;
}

//------------------------------------------------------------------------------

stock SetVendingMachineRot(machineid, Float:rx, Float:ry, Float:rz)
{
	if(!IsValidVendingMachine(machineid))
		return 0;

	g_eVendingData[machineid][E_VENDING_RX] = rx;
	g_eVendingData[machineid][E_VENDING_RY] = ry;
	g_eVendingData[machineid][E_VENDING_RZ] = rz;


	#if defined Streamer_IncludeFileVersion
	SetDynamicObjectRot(g_eVendingData[machineid][E_VENDING_ID], rx, ry, rz);
	#else
	SetObjectRot(g_eVendingData[machineid][E_VENDING_ID], rx, ry, rz);
	#endif
	return 1;
}

//------------------------------------------------------------------------------

stock GetVendingMachinePos(machineid, &Float:x, &Float:y, &Float:z)
{
	if(!IsValidVendingMachine(machineid))
		return 0;

	x = g_eVendingData[machineid][E_VENDING_X];
	y = g_eVendingData[machineid][E_VENDING_Y];
	z = g_eVendingData[machineid][E_VENDING_Z];
	return 1;
}

//------------------------------------------------------------------------------

stock GetVendingMachineRot(machineid, &Float:rx, &Float:ry, &Float:rz)
{
	if(!IsValidVendingMachine(machineid))
		return 0;

	rx = g_eVendingData[machineid][E_VENDING_RX];
	ry = g_eVendingData[machineid][E_VENDING_RY];
	rz = g_eVendingData[machineid][E_VENDING_RZ];
	return 1;
}

//------------------------------------------------------------------------------

stock GetVendingMachineObjectID(machineid)
	return g_eVendingData[machineid][E_VENDING_ID];

//------------------------------------------------------------------------------

GetXYInFrontOfVending(machineid, &Float:x, &Float:y, Float:distance)
{
	new Float:a, Float:z;
	GetVendingMachineRot(machineid, x, y, a);
	GetVendingMachinePos(machineid, x, y, z);

	a += 180.0;
	if(a > 359.0) a -= 359.0;

	x += (distance * floatsin(-a, degrees));
	y += (distance * floatcos(-a, degrees));
}

//------------------------------------------------------------------------------

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if((newkeys & KEY_SECONDARY_ATTACK) && (GetPlayerAnimationIndex(playerid) != 1660))
	{
		for(new i; i < MAX_MACHINES; i++)
		{
			if(!IsValidVendingMachine(i))
				continue;

			new Float:x, Float:y;
			GetXYInFrontOfVending(i, x, y, 0.5);
			if(!IsPlayerInRangeOfPoint(playerid, g_eVendingData[i][E_VENDING_RADIUS], x, y, g_eVendingData[i][E_VENDING_Z]))
				continue;

			SetPlayerFacingAngle(playerid, g_eVendingData[i][E_VENDING_RZ]);
			if(OnPlayerUseVendingMachine(playerid, i) != 0)
			{
				ApplyAnimation(playerid, "VENDING", "VEND_USE", 10.0, 0, 0, 0, 0, 0, 1);
				if(g_eVendingData[i][E_VENDING_TYPE] == MACHINE_SNACK) PlayerPlaySound(playerid, 42601, 0.0, 0.0, 0.0);
				else PlayerPlaySound(playerid, 42600, 0.0, 0.0, 0.0);
			
				#if !defined NO_SPRUNK_ACTION
				if(g_eVendingData[i][E_VENDING_TYPE] == MACHINE_SPRUNK) SetTimerEx("ven_SetPlayerAnimation", 2500, false, "i", playerid);
				#endif
			}
			else ClearAnimations(playerid);
		}
	}
	#if !defined NO_SPRUNK_ACTION
	else if((newkeys & KEY_FIRE) && (GetPVarInt(playerid, "ven_lastDrink") < GetTickCount()) && (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_DRINK_SPRUNK))
	{
		OnPlayerDrinkSprunk(playerid);
		SetPVarInt(playerid, "ven_lastDrink", GetTickCount() + 2350);
	}
	#endif
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

#if !defined NO_SPRUNK_ACTION
public ven_SetPlayerAnimation(playerid)
{
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);
	return 1;
}
#endif

//------------------------------------------------------------------------------

public OnPlayerConnect(playerid)
{
	for(new i; i < sizeof(sc_VendingMachines); i++)
	{
		RemoveBuildingForPlayer(playerid, sc_VendingMachines[i][e_Model], sc_VendingMachines[i][e_PosX], sc_VendingMachines[i][e_PosY], sc_VendingMachines[i][e_PosZ], 0.25);
	}
	#if defined inc_Ven_OnPlayerConnect
		return inc_Ven_OnPlayerConnect(playerid);
	#else
		return 0;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect inc_Ven_OnPlayerConnect
#if defined inc_Ven_OnPlayerConnect
	forward inc_Ven_OnPlayerConnect(playerid);
#endif

//------------------------------------------------------------------------------

#if defined FILTERSCRIPT
	public OnFilterScriptInit()
	{
		for(new i; i < sizeof(sc_VendingMachines); i++)
		{
			CreateVendingMachine(sc_VendingMachines[i][e_Model], sc_VendingMachines[i][e_PosX], sc_VendingMachines[i][e_PosY], sc_VendingMachines[i][e_PosZ], sc_VendingMachines[i][e_RotX], sc_VendingMachines[i][e_RotY], sc_VendingMachines[i][e_RotZ]);
		}

		#if defined inc_Ven_OnFilterScriptInit
			return inc_Ven_OnFilterScriptInit();
		#else
			return 0;
		#endif
	}
	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif

	#define OnFilterScriptInit inc_Ven_OnFilterScriptInit
	#if defined inc_Ven_OnFilterScriptInit
		forward inc_Ven_OnFilterScriptInit();
	#endif

	public OnFilterScriptExit()
	{
		for(new i; i < sizeof(sc_VendingMachines); i++)
		{
			DestroyVendingMachine(i);
		}

		#if defined inc_Ven_OnFilterScriptExit
			return inc_Ven_OnFilterScriptExit();
		#else
			return 0;
		#endif
	}
	#if defined _ALS_OnFilterScriptExit
		#undef OnFilterScriptExit
	#else
		#define _ALS_OnFilterScriptExit
	#endif

	#define OnFilterScriptExit inc_Ven_OnFilterScriptExit
	#if defined inc_Ven_OnFilterScriptExit
		forward inc_Ven_OnFilterScriptExit();
	#endif
#else
	public OnGameModeInit()
	{
		for(new i; i < sizeof(sc_VendingMachines); i++)
		{
			CreateVendingMachine(sc_VendingMachines[i][e_Model], sc_VendingMachines[i][e_PosX], sc_VendingMachines[i][e_PosY], sc_VendingMachines[i][e_PosZ], sc_VendingMachines[i][e_RotX], sc_VendingMachines[i][e_RotY], sc_VendingMachines[i][e_RotZ]);
		}

		#if defined inc_Ven_OnGameModeInit
			return inc_Ven_OnGameModeInit();
		#else
			return 0;
		#endif
	}
	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif

	#define OnGameModeInit inc_Ven_OnGameModeInit
	#if defined inc_Ven_OnGameModeInit
		forward inc_Ven_OnGameModeInit();
	#endif
#endif

//------------------------------------------------------------------------------
