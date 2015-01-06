//------------------------------------------------------------------------------

/*
	SA-MP vending machine creator
	
	Description:
		This filterscript provide code to create and save vending machine in game.

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
		Y_Less - GetXYInFrontOfPlayer function

	Version:
		1.6
*/

//------------------------------------------------------------------------------

#define FILTERSCRIPT

#include <a_samp>
#include <vending>

//------------------------------------------------------------------------------

#define DIALOG_MACHINE		2356
#define DIALOG_EDITOR		2358
#define DIALOG_COLOR		2360
#define DIALOG_CAPTION		"Machine Editor 1.6"
#define DIALOG_INFO			"1.\tCreate a Machine\n2.\tEdit nearest machine pos\n3.\tEdit nearest machine color\n4.\tDelete nearest machine\n5.\tGo to machine\n6.\tExport nearest machine\n7.\tExport all machine"

#define COLOR_WHITE			0xffffffff
#define COLOR_INFO			0x67ff22ff
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
	SendClientMessageToAll(COLOR_WHITE, "* {67ff22}/machine{ffffff} to open machine editor.");
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
		DestroyVendingMachine(i);
	return 1;
}

//------------------------------------------------------------------------------

public OnPlayerSpawn(playerid)
{
	SendClientMessage(playerid, COLOR_WHITE, "* {67ff22}/machine{ffffff} to open machine editor.");
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
		case DIALOG_MACHINE:
		{
			if(!response)
			{
				ShowPlayerDialog(playerid, DIALOG_EDITOR, DIALOG_STYLE_LIST, DIALOG_CAPTION, DIALOG_INFO, "Select", "Cancel");
				PlayCancelSound(playerid);
				return 1;
			}

			new machinetype;
			if(listitem == 0) machinetype = MACHINE_SPRUNK;
			else if(listitem == 1) machinetype = MACHINE_SNACK;
			else if(listitem == 2) machinetype = MACHINE_SODA;

			new Float:X, Float:Y, Float:Z;
			GetPlayerPos(playerid, X, Y, Z);
			GetXYInFrontOfPlayer(playerid, X, Y, 5.0);
			gPlayerData[playerid][E_VC_PLAYER_VENDING_ID] = CreateVendingMachine(machinetype, X, Y, Z, 0.00, 0.00, 180.00);
			gPlayerData[playerid][E_VC_PLAYER_IS_EDITING] = true;

			EditObject(playerid, GetVendingMachineObjectID(gPlayerData[playerid][E_VC_PLAYER_VENDING_ID]));
			SendClientMessage(playerid, COLOR_WHITE, "* {67ff22}Edit{ffffff} the machine position and {67ff22}save{ffffff}.");
			PlaySelectSound(playerid);
		}
		case DIALOG_EDITOR:
		{
			if(!response)
				return PlayCancelSound(playerid);

			switch(listitem)
			{
				case 0: // Create a vending
				{
					ShowPlayerDialog(playerid, DIALOG_MACHINE, DIALOG_STYLE_LIST, DIALOG_CAPTION, "1.\tSprunk Machine\n2.\tSnack Machine\n3.\tSoda Machine", "Select", "Back");
					PlaySelectSound(playerid);
					return 1;
				}
				case 1: // Edit nearest vending pos
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

						GetVendingMachinePos(i, X, Y, Z);
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

					gPlayerData[playerid][E_VC_PLAYER_VENDING_ID]	= machineid;
					gPlayerData[playerid][E_VC_PLAYER_IS_EDITING]	= true;
					EditObject(playerid, GetVendingMachineObjectID(gPlayerData[playerid][E_VC_PLAYER_VENDING_ID]));
					PlaySelectSound(playerid);
					return 1;
				}
				case 2: // Edit nearest vending color
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

						GetVendingMachinePos(i, X, Y, Z);
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

					else if(GetVendingMachineType(machineid) == MACHINE_SNACK)
					{
						SendClientMessage(playerid, COLOR_ERROR, "* Can't change color of MACHINE_SNACK type!");
						ShowPlayerDialog(playerid, DIALOG_EDITOR, DIALOG_STYLE_LIST, DIALOG_CAPTION, DIALOG_INFO, "Select", "Cancel");
						PlayErrorSound(playerid);
						return 1;
					}

					gPlayerData[playerid][E_VC_PLAYER_VENDING_ID]	= machineid;
					ShowPlayerDialog(playerid, DIALOG_COLOR, DIALOG_STYLE_LIST, DIALOG_CAPTION, "1.\tOriginal\n2.\tBlue\n3.\tGreen\n4.\tYellow\n5.\tRed\n6.\tOrange\n7.\tPurple\n8.\tCyan\n9.\tPink\n10.\tCustom ({67ff22}ARGB{ffffff})", "Select", "Back");
					PlaySelectSound(playerid);
					return 1;
				}
				case 3: // Delete nearest vending
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

						GetVendingMachinePos(i, X, Y, Z);
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

					DestroyVendingMachine(machineid);
					PlaySelectSound(playerid);
					return 1;
				}
				case 4: //Go to vending
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

						GetVendingMachinePos(i, X, Y, Z);

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
				case 5: //Export nearest vending
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

						GetVendingMachinePos(i, X, Y, Z);
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

					GetVendingMachinePos(machineid, X, Y, Z);
					GetVendingMachineRot(machineid, rX, rY, rZ);

					new machineName[32];
					switch(GetVendingMachineType(machineid))
					{
						case MACHINE_SPRUNK:
							machineName = "MACHINE_SPRUNK";
						case MACHINE_SNACK:
							machineName = "MACHINE_SNACK";
						case MACHINE_SODA:
							machineName = "MACHINE_SODA";
					}

					new textToSave[128];
					new File:vendingFile = fopen("vending.txt", io_append);
			        format(textToSave, 256, "CreateVendingMachine(%s, %f, %f, %f, %f, %f, %f);\n", machineName, X, Y, Z, rX, rY, rZ);
			        fwrite(vendingFile, textToSave);
			        fclose(vendingFile);

			        PlaySelectSound(playerid);
			        SendClientMessage(playerid, COLOR_WHITE, "* {67ff22}Nearest machine saved{ffffff} to scriptfiles/vending.txt.");
			        return 1;
				}
				case 6: // Export all vendings
				{
					new count;
					for(new i; i < MAX_MACHINES; i++)
					{
						if(!IsValidMachine(i))
							continue;

						count++;

						new Float:X, Float:Y, Float:Z, Float:rX, Float:rY, Float:rZ;
						GetVendingMachinePos(i, X, Y, Z);
						GetVendingMachineRot(i, rX, rY, rZ);

						new machineName[32];
						switch(GetVendingMachineType(i))
						{
							case MACHINE_SPRUNK:
								machineName = "MACHINE_SPRUNK";
							case MACHINE_SNACK:
								machineName = "MACHINE_SNACK";
							case MACHINE_SODA:
								machineName = "MACHINE_SODA";
						}

						new textToSave[128];
						new File:vendingFile = fopen("vending.txt", io_append);
				        format(textToSave, 256, "CreateVendingMachine(%s, %f, %f, %f, %f, %f, %f);\n", machineName, X, Y, Z, rX, rY, rZ);
			        	fwrite(vendingFile, textToSave);
			        	fclose(vendingFile);
					}

					if(count != 0)
					{
						PlaySelectSound(playerid);
			        	SendClientMessage(playerid, COLOR_WHITE, "* {67ff22}All machines saved{ffffff} to scriptfiles/vending.txt");						
					}
					else
					{
						PlayErrorSound(playerid);
			        	SendClientMessage(playerid, COLOR_ERROR, "* No machines created!");	
			        	ShowPlayerDialog(playerid, DIALOG_EDITOR, DIALOG_STYLE_LIST, DIALOG_CAPTION, DIALOG_INFO, "Select", "Cancel");
					}
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
			GetVendingMachinePos(machineidList[listitem], X, Y, Z);

			SetPlayerPos(playerid, X+1.0, Y+1.0, Z+1.0);

			PlaySelectSound(playerid);
			return 1;
		}
		case DIALOG_COLOR:
		{
			if(!response)
			{
				ShowPlayerDialog(playerid, DIALOG_EDITOR, DIALOG_STYLE_LIST, DIALOG_CAPTION, DIALOG_INFO, "Select", "Cancel");
				PlayCancelSound(playerid);
				return 1;
			}

			PlaySelectSound(playerid);

			switch(listitem)
			{
				case 0: SetVendingMachineColor(gPlayerData[playerid][E_VC_PLAYER_VENDING_ID], 0); // Original
				case 1: SetVendingMachineColor(gPlayerData[playerid][E_VC_PLAYER_VENDING_ID], 0xff00b9e8); // Blue
				case 2: SetVendingMachineColor(gPlayerData[playerid][E_VC_PLAYER_VENDING_ID], 0xff67ff22); // Green
				case 3: SetVendingMachineColor(gPlayerData[playerid][E_VC_PLAYER_VENDING_ID], 0xffffd700); // Yellow
				case 4: SetVendingMachineColor(gPlayerData[playerid][E_VC_PLAYER_VENDING_ID], 0xfff63752); // Red
				case 5: SetVendingMachineColor(gPlayerData[playerid][E_VC_PLAYER_VENDING_ID], 0xffff8100); // Orange
				case 6: SetVendingMachineColor(gPlayerData[playerid][E_VC_PLAYER_VENDING_ID], 0xff704992); // Purple
				case 7: SetVendingMachineColor(gPlayerData[playerid][E_VC_PLAYER_VENDING_ID], 0xff00e5ee); // Cyan
				case 8: SetVendingMachineColor(gPlayerData[playerid][E_VC_PLAYER_VENDING_ID], 0xffee00e5); // Pink
				case 9: // Custom (ARGB)
				{
					ShowPlayerDialog(playerid, DIALOG_COLOR+1, DIALOG_STYLE_INPUT, DIALOG_CAPTION, "{ffffff}Insert the color (AARRGGBB):\n({ffffff}It is {ff0000}A{67ff22}RGB{ffffff} not {67ff22}RGB{ff0000}A{ffffff})", "Select", "Back");
					PlaySelectSound(playerid);
					return 1;
				}
			}
		}
		case DIALOG_COLOR+1:
		{
			if(!response)
			{
				ShowPlayerDialog(playerid, DIALOG_COLOR, DIALOG_STYLE_LIST, DIALOG_CAPTION, "1.\tOriginal\n2.\tBlue\n3.\tGreen\n4.\tYellow\n5.\tRed\n6.\tOrange\n7.\tPurple\n8.\tCyan\n9.\tPink\n10.\tCustom ({67ff22}ARGB{ffffff})", "Select", "Back");
				PlayCancelSound(playerid);
				return 1;
			}

			if(strlen(inputtext) != 8)
			{
				SendClientMessage(playerid, COLOR_ERROR, "* Invalid format. (AARRGGBB)");
				ShowPlayerDialog(playerid, DIALOG_COLOR+1, DIALOG_STYLE_INPUT, DIALOG_CAPTION, "{ffffff}Insert the color (AARRGGBB):\n({ffffff}It is {ff0000}A{67ff22}RGB{ffffff} not {67ff22}RGB{ff0000}A{ffffff})", "Select", "Back");
				PlayErrorSound(playerid);
				return 1;
			}

			SetVendingMachineColor(gPlayerData[playerid][E_VC_PLAYER_VENDING_ID], HexToInt(inputtext));
			PlaySelectSound(playerid);
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
		if(objectid == GetVendingMachineObjectID(gPlayerData[playerid][E_VC_PLAYER_VENDING_ID]))
		{
			SetVendingMachinePos(gPlayerData[playerid][E_VC_PLAYER_VENDING_ID], fX, fY, fZ);
			SetVendingMachineRot(gPlayerData[playerid][E_VC_PLAYER_VENDING_ID], fRotX, fRotY, fRotZ);
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
	if(GetPlayerMoney(playerid) < 1)
	{
		SendClientMessage(playerid, COLOR_ERROR, "* You don't have enough money.");
		return 0;
	}

	new Float:health;
	GetPlayerHealth(playerid, health);

	if((health + 10.0) > 100.0) health = 100.0;
	else health += 10.0;

	SetPlayerHealth(playerid, health);
	GivePlayerMoney(playerid, -1);

	new message[128];
	format(message, 128, "* You've used the machineid {67ff22}%d{ffffff}.", machineid);
	SendClientMessage(playerid, COLOR_WHITE, message);
	return 1;
}

//------------------------------------------------------------------------------

public OnPlayerDrinkSprunk(playerid)
{
	new Float:health;
	GetPlayerHealth(playerid, health);

	if((health + 10.0) > 100.0) health = 100.0;
	else health += 10.0;

	SetPlayerHealth(playerid, health);
	SendClientMessage(playerid, COLOR_WHITE, "* You've drank the {67ff22}sprunk{ffffff}. ({67ff22}+{ffffff}10HP)");
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

stock HexToInt(string[])
{// Made by Dracoblue
	new i = 0;
	new cur = 1;
	new res = 0;
	for (i = strlen(string); i > 0; i--)
	{
		string[i-1] = toupper(string[i-1]);
		if (string[i-1] < 58) res = res + cur*(string[i-1] - 48); else res = res + cur*(string[i-1] - 65 + 10);
		cur = cur*16;
	}
	return res;
}
