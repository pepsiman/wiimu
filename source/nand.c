#include <stdio.h>
#include <gccore.h>
#include <ogc/es.h>
#include <ogc/isfs.h>
#include <ogc/ipc.h>
#include <ogc/ios.h>
#include <wiiuse/wpad.h>
#include <fat.h>

#include "screen.h"
#include "rethandle.h"
#include "nand.h"
#include "yawnd.h"
#include "general.h"

int backupNAND()
{
	printf("\nWould you like to backup your Wii's NAND Flash? Hit A for Yes, or B for no.\n");
	while(1)
	{
		WPAD_ScanPads();
		PAD_ScanPads();
		u32 WPAD_Pressed = WPAD_ButtonsDown(0);
		WPAD_Pressed |= WPAD_ButtonsDown(1);
		WPAD_Pressed |= WPAD_ButtonsDown(2);
		WPAD_Pressed |= WPAD_ButtonsDown(3);

		u32 PAD_Pressed  = PAD_ButtonsDown(0);
		PAD_Pressed  |= PAD_ButtonsDown(1);
		PAD_Pressed  |= PAD_ButtonsDown(2);
		PAD_Pressed  |= PAD_ButtonsDown(3);

		if( (WPAD_Pressed & WPAD_BUTTON_A) || (PAD_Pressed & PAD_BUTTON_A) )
			break;
		if( (WPAD_Pressed & WPAD_BUTTON_B) || (PAD_Pressed & PAD_BUTTON_B) )
			return 0;

	}
	YAWND_Backup();					// Thanks Redbird for releasing the YaWnD source code! 
	return 0;
}

int backupNANDzestig()
{
	printf("\nWould you like to backup your Wii's NAND Flash? Hit A for Yes, or B for no.\n");
	while(1)
	{
		WPAD_ScanPads();
		PAD_ScanPads();
		u32 WPAD_Pressed = WPAD_ButtonsDown(0);
		WPAD_Pressed |= WPAD_ButtonsDown(1);
		WPAD_Pressed |= WPAD_ButtonsDown(2);
		WPAD_Pressed |= WPAD_ButtonsDown(3);

		u32 PAD_Pressed  = PAD_ButtonsDown(0);
		PAD_Pressed  |= PAD_ButtonsDown(1);
		PAD_Pressed  |= PAD_ButtonsDown(2);
		PAD_Pressed  |= PAD_ButtonsDown(3);

		if( (WPAD_Pressed & WPAD_BUTTON_A) || (PAD_Pressed & PAD_BUTTON_A) )
			break;
		if( (WPAD_Pressed & WPAD_BUTTON_B) || (PAD_Pressed & PAD_BUTTON_B) )
			return 0;

	}
	YAWND_Backup_Zestig();					// Thanks Redbird for releasing the YaWnD source code! 
	return 0;
}

/*int restoreNAND()								// MEGA DANGEROUS!!! NOT GOING TO ADD TO _ANY_ MAIN BUILD!!! ADD IN AT YOUR OWN RISK!!!
{
	printf("WARNING!!! THIS FUNCTION MAY PERMANENTLY BRICK YOUR WII!!!\nI (SquidMan) WILL TAKE ABSOLUTELY NO RESPONSIBILITY IF IT DOES!!!\nDO NOT ASK FOR ANY SYMPATHY IF YOUR WII IS PERMANENTLY AND IRREVERSABLY BRICKED!!!\nPress A+B+1+2 to continue, or Home to exit.\n");
	while(1) {
		WPAD_ScanPads();
		int buttonsDown = WPAD_ButtonsDown(0);
		if((buttonsDown & WPAD_BUTTON_A) && (buttonsDown & WPAD_BUTTON_B) && (buttonsDown & WPAD_BUTTON_1) && (buttonsDown & WPAD_BUTTON_2))
			break;
		if(buttonsDown & WPAD_BUTTON_HOME)
			return 0;
	}
	static u8 NAND_RestoreBuffer[NAND_READ_SIZE] ATTRIBUTE_ALIGN(32);
	FILE* sdFd=fopen("/nand/backup.bin","a+b");
	int i;
	s32 nandFd=IOS_Open(NAND_FLASH_DIR,IPC_OPEN_WRITE);
	if(nandFd<0)
		return 1;
	IOS_Seek(nandFd,0,0);
	fseek(sdFd,0,SEEK_SET);
	for(i=0;i<NAND_ACCESSES;i++)
	{
		IOS_Seek(nandFd,i,0);
		fread(NAND_RestoreBuffer,NAND_READ_SIZE, 1, sdFd);
		IOS_Write(nandFd,NAND_RestoreBuffer,NAND_READ_SIZE);
	}
	fclose(sdFd);
	IOS_Close(nandFd);
	return 0;
}*/


