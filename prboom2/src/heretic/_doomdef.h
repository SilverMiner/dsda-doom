//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 1993-2008 Raven Software
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//

// DoomDef.h

#ifndef __DOOMDEF__
#define __DOOMDEF__
#include <stdio.h>
#include <string.h>
//haleyjd: removed WATCOMC
#include <limits.h>

#define HERETIC_VERSION 130
#define HERETIC_VERSION_TEXT "v1.3"

// if rangecheck is undefined, most parameter validation debugging code
// will not be compiled
//#define RANGECHECK

// all external data is defined here
#include "doomdata.h"

// all important printed strings
#include "dstrings.h"

// header generated by multigen utility
#include "info.h"

// WAD file access
#include "w_wad.h"

// fixed_t
#include "m_fixed.h"

// angle_t 
#include "tables.h"

// events
#include "d_event.h"

// gamemode/mission
#include "d_mode.h"

// ticcmd_t
#include "d_ticcmd.h"

#include "d_loop.h"

#define	SAVEGAMENAME "hticsav"

/*
===============================================================================

						GLOBAL TYPES

===============================================================================
*/

#define NUMARTIFCTS	28
#define MAXPLAYERS	4

typedef enum
{
    wipe_0,
    wipe_1,
    wipe_2,
    wipe_3,
    wipe_4,
    NUMWIPES,
    wipe_random
} wipe_t;

/*
===============================================================================

							MAPOBJ DATA

===============================================================================
*/

// think_t is a function pointer to a routine to handle an actor
typedef void (*think_t) ();

typedef struct thinker_s
{
    struct thinker_s *prev, *next;
    think_t function;
} thinker_t;

typedef union
{
    int i;
    struct mobj_s *m;
} specialval_t;

struct player_s;

// each sector has a degenmobj_t in it's center for sound origin purposes
typedef struct
{
    thinker_t thinker;          // not used for anything
    fixed_t x, y, z;
} degenmobj_t;

typedef enum
{
    key_yellow,
    key_green,
    key_blue,
    NUMKEYS
} keytype_t;

typedef enum
{
    wp_staff,
    wp_goldwand,
    wp_crossbow,
    wp_blaster,
    wp_skullrod,
    wp_phoenixrod,
    wp_mace,
    wp_gauntlets,
    wp_beak,
    NUMWEAPONS,
    wp_nochange
} weapontype_t;

typedef enum
{
    am_goldwand,
    am_crossbow,
    am_blaster,
    am_skullrod,
    am_phoenixrod,
    am_mace,
    NUMAMMO,
    am_noammo                   // staff, gauntlets
} ammotype_t;

typedef struct
{
    ammotype_t ammo;
    int upstate;
    int downstate;
    int readystate;
    int atkstate;
    int holdatkstate;
    int flashstate;
} weaponinfo_t;

extern weaponinfo_t wpnlev1info[NUMWEAPONS];
extern weaponinfo_t wpnlev2info[NUMWEAPONS];

typedef enum
{
    arti_none,
    arti_invulnerability,
    arti_invisibility,
    arti_health,
    arti_superhealth,
    arti_tomeofpower,
    arti_torch,
    arti_firebomb,
    arti_egg,
    arti_fly,
    arti_teleport,
    NUMARTIFACTS
} artitype_t;

#define	INVULNTICS (30*35)
#define	INVISTICS (60*35)
#define	INFRATICS (120*35)
#define	IRONTICS (60*35)
#define WPNLEV2TICS (40*35)
#define FLIGHTTICS (60*35)

#define CHICKENTICS (40*35)

#define MESSAGETICS (4*35)
#define BLINKTHRESHOLD (4*32)

#define NUMINVENTORYSLOTS	14
typedef struct
{
    int type;
    int count;
} inventory_t;

/*
================
=
= player_t
=
================
*/

typedef struct player_s
{
    mobj_t *mo;
    playerstate_t playerstate;
    ticcmd_t cmd;

    fixed_t viewz;              // focal origin above r.z
    fixed_t viewheight;         // base height above floor for viewz
    fixed_t deltaviewheight;    // squat speed
    fixed_t bob;                // bounded/scaled total momentum

    int flyheight;
    int lookdir;
    boolean centering;
    int health;                 // only used between levels, mo->health
    // is used during levels
    int armorpoints, armortype; // armor type is 0-2

    inventory_t inventory[NUMINVENTORYSLOTS];
    artitype_t readyArtifact;
    int artifactCount;
    int inventorySlotNum;
    int powers[NUMPOWERS];
    boolean keys[NUMKEYS];
    boolean backpack;
    signed int frags[MAXPLAYERS];       // kills of other players
    weapontype_t readyweapon;
    weapontype_t pendingweapon; // wp_nochange if not changing
    boolean weaponowned[NUMWEAPONS];
    int ammo[NUMAMMO];
    int maxammo[NUMAMMO];
    int attackdown, usedown;    // true if button down last tic
    int cheats;                 // bit flags

    int refire;                 // refired shots are less accurate

    int killcount, itemcount, secretcount;      // for intermission
    const char *message;        // hint messages
    int messageTics;            // counter for showing messages
    int damagecount, bonuscount;        // for screen flashing
    int flamecount;             // for flame thrower duration
    mobj_t *attacker;           // who did damage (NULL for floors)
    int extralight;             // so gun flashes light up areas
    int fixedcolormap;          // can be set to REDCOLORMAP, etc
    int colormap;               // 0-3 for which color to draw player
    pspdef_t psprites[NUMPSPRITES];     // view sprites (gun, etc)
    boolean didsecret;          // true if secret level has been done
    int chickenTics;            // player is a chicken if > 0
    int chickenPeck;            // chicken peck countdown
    mobj_t *rain1;              // active rain maker 1
    mobj_t *rain2;              // active rain maker 2

    // [crispy] show centered "Secret Revealed!" message
    const char *centerMessage;
    int centerMessageTics;            // counter for showing centered messages

    // [AM] Previous position of viewz before think.
    //      Used to interpolate between camera positions.
    angle_t		oldviewz;
} player_t;

#define CF_NOCLIP		1
#define	CF_GODMODE		2
#define	CF_NOMOMENTUM	4       // not really a cheat, just a debug aid
#define CF_SHOWFPS      8       // [crispy]

#define	SBARHEIGHT	(42 << crispy->hires)      // status bar height at bottom of screen


/*
===============================================================================

					GLOBAL VARIABLES

===============================================================================
*/

#define TELEFOGHEIGHT (32*FRACUNIT)

extern gameaction_t gameaction;

extern boolean paused;

extern GameMode_t gamemode;

extern boolean ExtendedWAD;     // true if main WAD is the extended version

extern boolean nomonsters;      // checkparm of -nomonsters

extern boolean respawnparm;     // checkparm of -respawn

extern boolean debugmode;       // checkparm of -debug

extern boolean usergame;        // ok to save / end game

extern boolean ravpic;          // checkparm of -ravpic

extern boolean altpal;          // checkparm to use an alternate palette routine

extern boolean cdrom;           // true if cd-rom mode active ("-cdrom")

extern boolean deathmatch;      // only if started as net death

extern boolean netgame;         // only true if >1 player

extern boolean playeringame[MAXPLAYERS];

extern int consoleplayer;       // player taking events and displaying

extern int displayplayer;

extern int viewangleoffset;     // ANG90 = left side, ANG270 = right

extern player_t players[MAXPLAYERS];

extern boolean DebugSound;      // debug flag for displaying sound info

extern int GetWeaponAmmo[NUMWEAPONS];

extern boolean demorecording;
extern boolean demoplayback;
extern boolean demoextend;      // allow demos to persist through exit/respawn
extern int skytexture;

// Truncate angleturn in ticcmds to nearest 256.
// Used when recording Vanilla demos in netgames.
extern boolean lowres_turn;

extern gamestate_t gamestate;
extern skill_t gameskill;
extern boolean respawnmonsters;
extern int gameepisode;
extern int gamemap;
extern int prevmap;
extern int totalkills, totalitems, totalsecret; // for intermission
extern int levelstarttic;       // gametic at level start
extern int leveltime;           // tics in game play for par
extern int totalleveltimes; // [crispy] total time for all completed levels

extern boolean finalintermission; // [crispy] track intermission at end of episode

extern ticcmd_t *netcmds;

#define SAVEGAMESIZE 0x30000*16
#define SAVESTRINGSIZE 24

extern mapthing_t *deathmatch_p;
extern mapthing_t deathmatchstarts[10];
extern mapthing_t playerstarts[MAXPLAYERS];
extern boolean playerstartsingame[MAXPLAYERS];

extern int mouseSensitivity;

extern boolean precache;        // if true, load all graphics at level load

extern boolean singledemo;      // quit after playing a demo from cmdline

extern int bodyqueslot;
extern skill_t startskill;
extern int startepisode;
extern int startmap;
extern boolean autostart;

extern boolean testcontrols;
extern int testcontrols_mousespeed;

extern int vanilla_savegame_limit;
extern int vanilla_demo_limit;

/*
===============================================================================

					GLOBAL FUNCTIONS

===============================================================================
*/

#include "z_zone.h"

//----------
//BASE LEVEL
//----------
void D_DoomMain(void);
void IncThermo(void);
void InitThermo(int max);
void tprintf(const char *string, int initflag);
// not a globally visible function, just included for source reference
// calls all startup code
// parses command line options
// if not overrided, calls N_AdvanceDemo

void D_DoomLoop(void);
// not a globally visible function, just included for source reference
// called by D_DoomMain, never exits
// manages timing and IO
// calls all ?_Responder, ?_Ticker, and ?_Drawer functions
// calls I_GetTime, I_StartFrame, and I_StartTic

//---------
//SYSTEM IO
//---------
byte *I_AllocLow(int length);
// allocates from low memory under dos, just mallocs under unix

// haleyjd: was WATCOMC, preserved for historical interest.
// This is similar to the -control structure in DOOM v1.4 and Strife.
#if 0
extern boolean useexterndriver;

#define EBT_FIRE			1
#define EBT_OPENDOOR 		2
#define EBT_SPEED			4
#define EBT_STRAFE			8
#define EBT_MAP				0x10
#define EBT_INVENTORYLEFT 	0x20
#define EBT_INVENTORYRIGHT 	0x40
#define EBT_USEARTIFACT		0x80
#define EBT_FLYDROP			0x100
#define EBT_CENTERVIEW		0x200
#define EBT_PAUSE			0x400
#define EBT_WEAPONCYCLE 	0x800

typedef struct
{
    short vector;               // Interrupt vector

    signed char moveForward;    // forward/backward (maxes at 50)
    signed char moveSideways;   // strafe (maxes at 24)
    short angleTurn;            // turning speed (640 [slow] 1280 [fast])
    short angleHead;            // head angle (+2080 [left] : 0 [center] : -2048 [right])
    signed char pitch;          // look up/down (-110 : +90)
    signed char flyDirection;   // flyheight (+1/-1)
    unsigned short buttons;     // EBT_* flags
} externdata_t;
#endif

//----
//GAME
//----

void G_DeathMatchSpawnPlayer(int playernum);

void G_InitNew(skill_t skill, int episode, int map);

void G_DeferedInitNew(skill_t skill, int episode, int map);
// can be called by the startup code or M_Responder
// a normal game starts at map 1, but a warp test can start elsewhere

void G_DeferedPlayDemo(const char *demo);

void G_LoadGame(char *name);
// can be called by the startup code or M_Responder
// calls P_SetupLevel or W_EnterWorld
void G_DoLoadGame(void);

void G_SaveGame(int slot, char *description);
// called by M_Responder

#define SAVE_GAME_TERMINATOR 0x1d
// Support routines for saving games
char *SV_Filename(int slot);
void SV_Open(char *fileName);
void SV_OpenRead(char *fileName);
void SV_Close(char *fileName);
void SV_Write(void *buffer, int size);
void SV_WriteByte(byte val);
void SV_WriteWord(unsigned short val);
void SV_WriteLong(unsigned int val);
void SV_Read(void *buffer, int size);
byte SV_ReadByte(void);
uint16_t SV_ReadWord(void);
uint32_t SV_ReadLong(void);

extern char *savegamedir;

void G_RecordDemo(skill_t skill, int numplayers, int episode, int map,
                  const char *name);
// only called by startup code

void G_PlayDemo(char *name);
void G_TimeDemo(char *name);

void G_ExitLevel(void);
void G_SecretExitLevel(void);

void G_WorldDone(void);

void G_Ticker(void);
boolean G_Responder(event_t * ev);

void G_ScreenShot(void);

//-----
//PLAY
//-----

void P_Ticker(void);
// called by C_Ticker
// can call G_PlayerExited
// carries out all thinking of monsters and players

void P_SetupLevel(int episode, int map, int playermask, skill_t skill);
// called by W_Ticker

void P_Init(void);
// called by startup code

void P_ArchivePlayers(void);
void P_UnArchivePlayers(void);
void P_ArchiveWorld(void);
void P_UnArchiveWorld(void);
void P_ArchiveThinkers(void);
void P_UnArchiveThinkers(void);
void P_ArchiveSpecials(void);
void P_UnArchiveSpecials(void);
// load / save game routines


//-------
//REFRESH
//-------

extern boolean setsizeneeded;

extern boolean BorderNeedRefresh;
extern boolean BorderTopRefresh;

extern int UpdateState;
// define the different areas for the dirty map
#define I_NOUPDATE	0
#define I_FULLVIEW	1
#define I_STATBAR	2
#define I_MESSAGES	4
#define I_FULLSCRN	8

void R_RenderPlayerView(player_t * player);
// called by G_Drawer

void R_Init(void);
// called by startup code

void R_DrawViewBorder(void);
void R_DrawTopBorder(void);
// if the view size is not full screen, draws a border around it

void R_SetViewSize(int blocks, int detail);
// called by M_Responder

int R_FlatNumForName(const char *name);

int R_TextureNumForName(const char *name);
int R_CheckTextureNumForName(const char *name);
// called by P_Ticker for switches and animations
// returns the texture number for the texture name


//----
//MISC
//----
// returns the position of the given parameter in the arg list (0 if not found)

int M_DrawText(int x, int y, boolean direct, char *string);

//----------------------
// Interlude (IN_lude.c)
//----------------------

extern boolean intermission;

void IN_Start(void);
void IN_Ticker(void);
void IN_Drawer(void);

//----------------------
// Chat mode (CT_chat.c)
//----------------------

void CT_Init(void);
void CT_Drawer(void);
boolean CT_Responder(event_t * ev);
void CT_Ticker(void);
char CT_dequeueChatChar(void);

extern boolean chatmodeon;
extern boolean ultimatemsg;

//--------------------
// Finale (F_finale.c)
//--------------------

void F_Drawer(void);
void F_Ticker(void);
void F_StartFinale(void);

//----------------------
// STATUS BAR (SB_bar.c)
//----------------------

void SB_Init(void);
boolean SB_Responder(event_t * event);
void SB_Ticker(void);
void SB_Drawer(void);

//-----------------
// MENU (MN_menu.c)
//-----------------

extern boolean MenuActive;

void MN_Init(void);
void MN_ActivateMenu(void);
void MN_DeactivateMenu(void);
boolean MN_Responder(event_t * event);
void MN_Ticker(void);
void MN_Drawer(void);
void MN_DrTextA(const char *text, int x, int y);
int MN_TextAWidth(const char *text);
void MN_DrTextB(const char *text, int x, int y);
int MN_TextBWidth(const char *text);

#include "sounds.h"

#endif // __DOOMDEF__
