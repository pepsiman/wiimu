/*
 *  pretty.c
 *  SysMenu
 *
 *  Created by Alex Marshall on 15.7.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <string.h>
#include <ogcsys.h>
#include <gcmodplay.h>
#include <stdio.h>
#include <stdlib.h>

#include "pretty.h"
#include "factory_mod.h"
#include "zelda_mod.h"

/*static MODPlay play;
int isPlaying=0;
*/
// Stuff from the WiiLight demo.
struct timespec _wiilight_timeOn;
struct timespec _wiilight_timeOff;
struct timespec _squidlight_timeWait;

int WIILIGHT_IsOn;
int WIILIGHT_Level;

int _squidlight_kill=0;
int _squidlight_original_level=0;

void _wiilight_turn(int enable);
//static void * _wiilight_loop(void *arg);
//static vu32 *_wiilight_reg = (u32*)0xCD0000C0;
lwp_t _wiilight_thread;
lwp_t _squidlight_thread;

void WIILIGHT_Init();
void WIILIGHT_TurnOn();
int WIILIGHT_GetLevel();
int WIILIGHT_SetLevel(int level);

void WIILIGHT_Toggle();
void WIILIGHT_TurnOff();

#ifdef USE_WIILIGHT
void WIILIGHT_Init(){
	_wiilight_timeOn.tv_sec=(time_t)0;
	_wiilight_timeOff.tv_nsec=(time_t)0;
	WIILIGHT_IsOn=0;
	WIILIGHT_SetLevel(0);
}


int WIILIGHT_GetLevel(){
	return WIILIGHT_Level;
}

//
// here is where we set the light intensity
// it turns on the light and 
// waits level*40000 nanoseconds
// (so, if it is 255 it will wait for 10200000 nanoseconds) 
// (that means ~ 1/100secs)
// 
// for the time turned off it is 10200000-level_on
// (so, if level is 255, it will wait for 0 seconds)
// (that means full light intensity)
//

int WIILIGHT_SetLevel(int level){//0 to 255
	long level_on;
	long level_off;
	if(level>255) level=255;
	if(level<0) level=0;
	level_on=level*40000;
	level_off=10200000-level_on;
	_wiilight_timeOn.tv_nsec=level_on;
	_wiilight_timeOff.tv_nsec=level_off;
	WIILIGHT_Level=level;
	return level;
}

//
// just turn one bit
//

void _wiilight_turn(int enable){
	u32 val = (*_wiilight_reg&~0x20);
	if(enable) val |= 0x20;
	*_wiilight_reg=val;
}

void WIILIGHT_TurnOff(){
	
	WIILIGHT_IsOn=false;
	
}

//
// Set the var on, and start the looping thread
//

void WIILIGHT_TurnOn(){
	
	WIILIGHT_IsOn=true;
	LWP_CreateThread(&_wiilight_thread, _wiilight_loop,NULL,NULL,0,80);
	
}

void WIILIGHT_Toggle(){
	
	if(WIILIGHT_IsOn){
		WIILIGHT_TurnOff();
	}else{
		WIILIGHT_TurnOn();
	}

}

// 
// This is the looping thread.. it just loops
// turning on and off the light depending on the timing
// specified by SetLevel()
//

static void * _wiilight_loop(void *arg){
	
	struct timespec timeOn;
	struct timespec timeOff;
	while(WIILIGHT_IsOn){
		timeOn=_wiilight_timeOn;
		timeOff=_wiilight_timeOff;
		_wiilight_turn(1);
		nanosleep(&timeOn);
		if(timeOff.tv_nsec>0) _wiilight_turn(0);
		nanosleep(&timeOff);
	}
	
	return NULL;
	
}

// My stuff now :)

static void *_squidlight_loop(void *arg){
	struct timespec timeWait;
	int i;
	for(i=_squidlight_original_level*-1;i<_squidlight_original_level && !_squidlight_kill;i++)
	{
		timeWait=_squidlight_timeWait;
		nanosleep(&timeWait);
		if(i<0)
			WIILIGHT_SetLevel(i*-1);
		else
			WIILIGHT_SetLevel(i);
	}
	return NULL;
}

void Light_Start()						// Initializes Wiilight and sets maximum intensity.
{
	_squidlight_timeWait.tv_sec=(time_t)0;			// Maximum wait time for light to go down and up is 1/2 second
	_squidlight_timeWait.tv_nsec=(time_t)1000000000/1024;	// Maximum wait time for light to go down and up is 1/2 second
	_squidlight_kill=0;
	_squidlight_original_level=0;
	WIILIGHT_Init();
	WIILIGHT_TurnOn();
	WIILIGHT_SetLevel(255);
}

int Light_MenuSelect(){
	_squidlight_kill=1;
	struct timespec timeWait;
	timeWait=_squidlight_timeWait;
	nanosleep(&timeWait);
	_squidlight_kill=0;
	_squidlight_original_level=WIILIGHT_GetLevel();
	LWP_CreateThread(&_squidlight_thread, _squidlight_loop,NULL,NULL,0,80);
	return 0;
}

#else
void WIILIGHT_Init(){}


int WIILIGHT_GetLevel(){return WIILIGHT_Level;}

//
// here is where we set the light intensity
// it turns on the light and 
// waits level*40000 nanoseconds
// (so, if it is 255 it will wait for 10200000 nanoseconds) 
// (that means ~ 1/100secs)
// 
// for the time turned off it is 10200000-level_on
// (so, if level is 255, it will wait for 0 seconds)
// (that means full light intensity)
//

int WIILIGHT_SetLevel(int level){return 0;}//0 to 255

//
// just turn one bit
//

void _wiilight_turn(int enable){}

void WIILIGHT_TurnOff(){}

//
// Set the var on, and start the looping thread
//

void WIILIGHT_TurnOn(){}

void WIILIGHT_Toggle(){}

// 
// This is the looping thread.. it just loops
// turning on and off the light depending on the timing
// specified by SetLevel()
//

//static void * _wiilight_loop(void *arg){return NULL;}

// My stuff now :)

void Light_Start(){}					// Initializes Wiilight and sets maximum intensity.

int Light_MenuSelect(){return 0;}

#endif //USE_WIILIGHT
/*
void Music_Start()					// Plays music (has some fun too!)
{
	if(isPlaying)
		MODPlay_Stop(&play);
	isPlaying=1;
//	srand(time(NULL));
//	int rate=rand()%100;
	MODPlay_Init(&play);
	MODPlay_SetMOD(&play, factory_mod);
	MODPlay_SetStereo(&play,1);
	MODPlay_SetVolume(&play, 64, 64);
//	if(rate>=80)					// Play "alternate" music.
//		MODPlay_SetMOD(&play, zelda_mod);
//	printf("\n%d\n",rate);
	play.manual_polling=0;
	MODPlay_Start(&play);
}
*/
