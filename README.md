SA-MP-Vending-Machine
=====================

A SA-MP include that creates server-sided vending machines that gives you full control of the in-game machines. You can create machines everywhere you want.

**This will automatic replace the single-player machines.**

Creating Machine
================

You can use vending creator filterscript to create and export vending machines.

![](http://i58.tinypic.com/2ztd2cl.jpg)
![](http://i61.tinypic.com/k996s5.jpg)
![](http://i61.tinypic.com/2rc0f81.jpg)
![](http://i57.tinypic.com/11gpwrk.jpg)

Example code
============

```c
new gVending;

main()
{
	gVending = CreateMachine(MACHINE_SPRUNK, 1755.348144, -2113.468750, 12.692808, 0.000000, 0.000000, 180.000000);
}

public OnPlayerUseVendingMachine(playerid, machineid)
{ // Called when the player use the machine.
	if(GetPlayerMoney(playerid) < 1)
	{// If the player has no money.
		SendClientMessage(playerid, COLOR_ERROR, "* You don't have enough money.");
		return 0;
	}
	
	// Restore 10% of player's health and takes 1$.
	new Float:health;
	GetPlayerHealth(playerid, health);

	// Avoid player having more than 100.0 health.
	if((health + 10.0) > 100.0) health = 100.0;
	else health += 10.0;

	// Give player stats
	SetPlayerHealth(playerid, health);
	GivePlayerMoney(playerid, -1);
	return 1;
}


public OnPlayerDrinkSprunk(playerid)
{// Called when the player drink from the can.
	new Float:health;
	GetPlayerHealth(playerid, health);

	if((health + 10.0) > 100.0) health = 100.0;
	else health += 10.0;

	SetPlayerHealth(playerid, health);
	SendClientMessage(playerid, COLOR_INFO, "* You drank the sprunk. (+10HP)");
	return 1;
}
```

Check wiki for full documentation.
