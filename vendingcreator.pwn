//------------------------------------------------------------------------------

/*
	SA-MP vending machine creator
	
	Description:
		This filterscript provide code to create and save vending machine in game.

	License:
		The MIT License (MIT)
		Copyright (c) 2014 WiRR-
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
		WiRR

	Contributors:
		Y_Less - GetXYInFrontOfPlayer function

	Version:
		1.0.1
*/

//------------------------------------------------------------------------------

#define FILTERSCRIPT

#include <a_samp>
#include "vending.inc"

//------------------------------------------------------------------------------

#define DIALOG_UPDATES		2357
#define DIALOG_EDITOR		2358
#define DIALOG_CAPTION		"Machine Editor 1.0.1"
#define DIALOG_INFO			"1.\tCreate a Machine\n2.\tEdit nearest machine\n3.\tDelete nearest machine\n4.\tGo to machine\n5.\tExport nearest machine\n6.\tExport all machine\n7.\tUpdates"

#define COLOR_INFO			0x00a4a7ff
#define COLOR_ERROR			0xff4040ff

#define PlaySelectSound(%0)	PlayerPlaySound(%0,1083,0.0,0.0,0.0)
#define PlayCancelSound(%0)	PlayerPlaySound(%0,1084,0.0,0.0,0.0)
#define PlayErrorSound(%0)	PlayerPlaySound(%0,1085,0.0,0.0,0.0)

//------------------------------------------------------------------------------

enum E_VC_PLAYER
{
	E_VC_PLAYER_VENDING_ID,
	bool:E_VC_PLAYER_IS_EDITING
}
new gPlayerData[MAX_PLAYERS][E_VC_PLAYER];

//------------------------------------------------------------------------------

public OnFilterScriptInit()
{
	printf("- Machine Creator loaded.");
	SendClientMessageToAll(COLOR_INFO, "* /machine to open machine editor.");
	for(new i; i < MAX_PLAYERS; i++)
	{
		if(!IsPlayerConnected(i))
			continue;

		ResetPlayerVars(i);
	}
	return 1;
}

//------------------------------------------------------------------------------

public OnFilterScriptExit()
{
	for(new i; i < MAX_MACHINES; i++)
		DestroyMachine(i);
	return 1;
}

//------------------------------------------------------------------------------

public OnPlayerSpawn(playerid)
{
	SendClientMessage(playerid, COLOR_INFO, "* /machine to open machine editor.");
	return 1;
}

//------------------------------------------------------------------------------

public OnPlayerCommandText(playerid, cmdtext[])
{
    if(!strcmp(cmdtext, "/machine", true))
    {
    	if(gPlayerData[playerid][E_VC_PLAYER_IS_EDITING])
    		return SendClientMessage(playerid, COLOR_ERROR, "* You are editing a machine already!");

        ShowPlayerDialog(playerid, DIALOG_EDITOR, DIALOG_STYLE_LIST, DIALOG_CAPTION, DIALOG_INFO, "Select", "Cancel");
        PlaySelectSound(playerid);
        return 1;
    }
    return 0;
}

//------------------------------------------------------------------------------

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch(dialogid)
	{
		case DIALOG_EDITOR:
		{
			if(!response)
				return PlayCancelSound(playerid);

			switch(listitem)
			{
				case 0: // Create a vending
				{
					new Float:X, Float:Y, Float:Z;
					GetPlayerPos(playerid, X, Y, Z);
					GetXYInFrontOfPlayer(playerid, X, Y, 5.0);
					gPlayerData[playerid][E_VC_PLAYER_VENDING_ID] = CreateMachine(MACHINE_SPRUNK, X, Y, Z, 0.00, 0.00, 180.00);
					gPlayerData[playerid][E_VC_PLAYER_IS_EDITING] = true;

					EditObject(playerid, GetMachineObjectID(gPlayerData[playerid][E_VC_PLAYER_VENDING_ID]));
					SendClientMessage(playerid, COLOR_INFO, "* Edit the machine position and save.");
					PlaySelectSound(playerid);
					return 1;
				}
				case 1: //Edit nearest vending
				{
					new
						machineid = INVALID_MACHINE_ID,
						Float:distance = 20.0,
						Float:X,
						Float:Y,
						Float:Z;

					for(new i; i < MAX_MACHINES; i++)
					{
						if(!IsValidMachine(i))
							continue;

						GetMachinePos(i, X, Y, Z);
						if(GetPlayerDistanceFromPoint(playerid, X, Y, Z) < distance)
						{
							distance = GetPlayerDistanceFromPoint(playerid, X, Y, Z);
							machineid = i;
						}
					}

					if(machineid == INVALID_MACHINE_ID)
					{
						SendClientMessage(playerid, COLOR_ERROR, "* No machines near you!");
						ShowPlayerDialog(playerid, DIALOG_EDITOR, DIALOG_STYLE_LIST, DIALOG_CAPTION, DIALOG_INFO, "Select", "Cancel");
						PlayErrorSound(playerid);
						return 1;
					}

					gPlayerData[playerid][E_VC_PLAYER_VENDING_ID]	= machineid;
					gPlayerData[playerid][E_VC_PLAYER_IS_EDITING]	= true;
					EditObject(playerid, GetMachineObjectID(gPlayerData[playerid][E_VC_PLAYER_VENDING_ID]));
					PlaySelectSound(playerid);
					return 1;
				}
				case 2: // Delete nearest vending
				{
					new
						machineid = INVALID_MACHINE_ID,
						Float:distance = 20.0,
						Float:X,
						Float:Y,
						Float:Z;

					for(new i; i < MAX_MACHINES; i++)
					{
						if(!IsValidMachine(i))
							continue;

						GetMachinePos(i, X, Y, Z);
						if(GetPlayerDistanceFromPoint(playerid, X, Y, Z) < distance)
						{
							distance = GetPlayerDistanceFromPoint(playerid, X, Y, Z);
							machineid = i;
						}
					}

					if(machineid == INVALID_MACHINE_ID)
					{
						SendClientMessage(playerid, COLOR_ERROR, "* No machine near you!");
						ShowPlayerDialog(playerid, DIALOG_EDITOR, DIALOG_STYLE_LIST, DIALOG_CAPTION, DIALOG_INFO, "Select", "Cancel");
						PlayErrorSound(playerid);
						return 1;
					}

					DestroyMachine(machineid);
					PlaySelectSound(playerid);
					return 1;
				}
				case 3: //Go to vending
				{
					new
						Float:X,
						Float:Y,
						Float:Z;

					new dialogList[2048];

					for(new i; i < MAX_MACHINES; i++)
					{
						if(!IsValidMachine(i))
							continue;

						GetMachinePos(i, X, Y, Z);

						new machineInfo[40];
						format(machineInfo, 40, "MachineID: %d\tDistance: %.2f\n", i, GetPlayerDistanceFromPoint(playerid, X, Y, Z));
						strins(dialogList, machineInfo, strlen(dialogList));
					}

					if(strlen(dialogList) < 1)
					{
						SendClientMessage(playerid, COLOR_ERROR, "* No machine created!");
						ShowPlayerDialog(playerid, DIALOG_EDITOR, DIALOG_STYLE_LIST, DIALOG_CAPTION, DIALOG_INFO, "Select", "Cancel");
						PlayErrorSound(playerid);
						return 1;
					}

					ShowPlayerDialog(playerid, DIALOG_EDITOR+1, DIALOG_STYLE_LIST, DIALOG_CAPTION, dialogList, "Go", "Back");
					PlaySelectSound(playerid);
					return 1;
				}
				case 4: //Export nearest vending
				{
					new
						machineid = INVALID_MACHINE_ID,
						Float:distance = 20.0,
						Float:X,
						Float:Y,
						Float:Z,
						Float:rX,
						Float:rY,
						Float:rZ;

					for(new i; i < MAX_MACHINES; i++)
					{
						if(!IsValidMachine(i))
							continue;

						GetMachinePos(i, X, Y, Z);
						if(GetPlayerDistanceFromPoint(playerid, X, Y, Z) < distance)
						{
							distance = GetPlayerDistanceFromPoint(playerid, X, Y, Z);
							machineid = i;
						}
					}

					if(machineid == INVALID_MACHINE_ID)
					{
						SendClientMessage(playerid, COLOR_ERROR, "* No machine near you!");
						ShowPlayerDialog(playerid, DIALOG_EDITOR, DIALOG_STYLE_LIST, DIALOG_CAPTION, DIALOG_INFO, "Select", "Cancel");
						PlayErrorSound(playerid);
						return 1;
					}

					GetMachinePos(machineid, X, Y, Z);
					GetMachineRot(machineid, rX, rY, rZ);

					new machineName[32];
					switch(GetMachineObjectID(machineid))
					{
						case 955:
							machineName = "MACHINE_SPRUNK";
						case 956:
							machineName = "MACHINE_CANDY";
						case 1302:
							machineName = "MACHINE_SODA";
					}

					new textToSave[128];
					new File:vendingFile = fopen("vending.txt", io_append);
			        format(textToSave, 256, "CreateMachine(%s, %f, %f, %f, %f, %f, %f);", machineName, X, Y, Z, rX, rY, rZ);
			        fwrite(vendingFile, textToSave);
			        fclose(vendingFile);

			        PlaySelectSound(playerid);
			        SendClientMessage(playerid, COLOR_INFO, "* Nearest machine saved to scriptfiles/vending.txt");
			        return 1;
				}
				case 5: // Export all vendings
				{
					new count;
					for(new i; i < MAX_MACHINES; i++)
					{
						if(!IsValidMachine(i))
							continue;

						count++;

						new Float:X, Float:Y, Float:Z, Float:rX, Float:rY, Float:rZ;
						GetMachinePos(i, X, Y, Z);
						GetMachineRot(i, rX, rY, rZ);

						new machineName[32];
						switch(GetMachineObjectID(i))
						{
							case 955:
								machineName = "MACHINE_SPRUNK";
							case 956:
								machineName = "MACHINE_CANDY";
							case 1302:
								machineName = "MACHINE_SODA";
						}

						new textToSave[128];
						new File:vendingFile = fopen("vending.txt", io_append);
				        format(textToSave, 256, "CreateMachine(%s, %f, %f, %f, %f, %f, %f);", machineName, X, Y, Z, rX, rY, rZ);
			        	fwrite(vendingFile, textToSave);
			        	fclose(vendingFile);
					}

					if(count != 0)
					{
						PlaySelectSound(playerid);
			        	SendClientMessage(playerid, COLOR_INFO, "* All machines saved to scriptfiles/vending.txt");						
					}
					else
					{
						PlayErrorSound(playerid);
			        	SendClientMessage(playerid, COLOR_ERROR, "* No machines created!");	
			        	ShowPlayerDialog(playerid, DIALOG_EDITOR, DIALOG_STYLE_LIST, DIALOG_CAPTION, DIALOG_INFO, "Select", "Cancel");
					}
					return 1;
				}
				case 6: // Updates
				{
					ShowPlayerDialog(playerid, DIALOG_UPDATES, DIALOG_STYLE_MSGBOX, DIALOG_CAPTION, "Vending Creator & Vending Include created by WiRR\n\nFor new updates or report a bug/suggestion go to:\nhttps://github.com/WiRR-/SA-MP-Vending-Machine", "Back", "");
					PlaySelectSound(playerid);
					return 1;
				}
			}
		}
		case DIALOG_EDITOR+1:
		{
			if(!response)
			{
				ShowPlayerDialog(playerid, DIALOG_EDITOR, DIALOG_STYLE_LIST, DIALOG_CAPTION, DIALOG_INFO, "Select", "Cancel");
				PlayCancelSound(playerid);
				return 1;
			}

			new machineidList[MAX_MACHINES], count;
			for(new i; i < MAX_MACHINES; i++)
			{
				if(!IsValidMachine(i))
					continue;

				machineidList[count] = i;
				count++;
			}

			new	Float:X, Float:Y, Float:Z;
			GetMachinePos(machineidList[listitem], X, Y, Z);

			SetPlayerPos(playerid, X+1.0, Y+1.0, Z+1.0);

			PlaySelectSound(playerid);
			return 1;
		}
		case DIALOG_UPDATES:
		{
			ShowPlayerDialog(playerid, DIALOG_EDITOR, DIALOG_STYLE_LIST, DIALOG_CAPTION, DIALOG_INFO, "Select", "Cancel");
			PlaySelectSound(playerid);
			return 1;
		}
	}
	return 0;
}

//------------------------------------------------------------------------------

public OnPlayerEditObject(playerid, playerobject, objectid, response, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ)
{
	new Float:oldX, Float:oldY, Float:oldZ, Float:oldRotX, Float:oldRotY, Float:oldRotZ;
	GetObjectPos(objectid, oldX, oldY, oldZ);
	GetObjectRot(objectid, oldRotX, oldRotY, oldRotZ);

	if(!playerobject)
	{
	    if(!IsValidObject(objectid)) return 1;

	    SetObjectPos(objectid, fX, fY, fZ);		          
        SetObjectRot(objectid, fRotX, fRotY, fRotZ);
	}
 
	if(response == EDIT_RESPONSE_FINAL)
	{
		if(objectid == GetMachineObjectID(gPlayerData[playerid][E_VC_PLAYER_VENDING_ID]))
		{
			SetMachinePos(gPlayerData[playerid][E_VC_PLAYER_VENDING_ID], fX, fY, fZ);
			SetMachineRot(gPlayerData[playerid][E_VC_PLAYER_VENDING_ID], fRotX, fRotY, fRotZ);
		}
		gPlayerData[playerid][E_VC_PLAYER_IS_EDITING] = false;
		PlaySelectSound(playerid);
	}
 
	if(response == EDIT_RESPONSE_CANCEL)
	{
		if(!playerobject)
		{
			SetObjectPos(objectid, oldX, oldY, oldZ);
			SetObjectRot(objectid, oldRotX, oldRotY, oldRotZ);
		}
		else
		{
			SetPlayerObjectPos(playerid, objectid, oldX, oldY, oldZ);
			SetPlayerObjectRot(playerid, objectid, oldRotX, oldRotY, oldRotZ);
		}
	}
	return 1;
}

//------------------------------------------------------------------------------

public OnPlayerUseVendingMachine(playerid, machineid)
{
	new Float:health;
	GetPlayerHealth(playerid, health);

	if((health + 10.0) > 100.0) health = 100.0;
	else health += 10.0;

	SetPlayerHealth(playerid, health);
	GivePlayerMoney(playerid, -1);
	return 1;
}

//------------------------------------------------------------------------------

public OnPlayerConnect(playerid)
{
	ResetPlayerVars(playerid);
	return 1;
}

//------------------------------------------------------------------------------

GetXYInFrontOfPlayer(playerid, &Float:x, &Float:y, Float:distance)
{	// Created by Y_Less

	new Float:a;

	GetPlayerPos(playerid, x, y, a);
	GetPlayerFacingAngle(playerid, a);

	if (GetPlayerVehicleID(playerid)) {
	    GetVehicleZAngle(GetPlayerVehicleID(playerid), a);
	}

	x += (distance * floatsin(-a, degrees));
	y += (distance * floatcos(-a, degrees));
}

//------------------------------------------------------------------------------

ResetPlayerVars(playerid)
{
	gPlayerData[playerid][E_VC_PLAYER_VENDING_ID]	= INVALID_MACHINE_ID;	
	gPlayerData[playerid][E_VC_PLAYER_IS_EDITING]	= false;
}

//------------------------------------------------------------------------------
