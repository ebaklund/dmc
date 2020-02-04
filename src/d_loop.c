//
// Copyright(C) 1993-1996 Id Software, Inc.
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
// DESCRIPTION:
//     Main loop code.
//

#include <stdlib.h>
#include <string.h>

#include "d_event.h"
#include "d_loop.h"
#include "d_ticcmd.h"

#include "i_system.h"
#include "i_timer.h"
#include "i_video.h"

#include "m_argv.h"
#include "m_fixed.h"

#include "crispy.h"

// The complete set of data for a particular tic.

typedef struct
{
    ticcmd_t cmds[1];
    boolean ingame[1];
} ticcmd_set_t;

// Maximum time that we wait in TryRunTics() for netgame data to be
// received before we bail out and render a frame anyway.
// Vanilla Doom used 20 for this value, but we use a smaller value
// instead for better responsiveness of the menu when we're stuck.
#define MAX_NETGAME_STALL_TICS  5

//
// gametic is the tic about to (or currently being) run
// maketic is the tic that hasn't had control made for it yet
// recvtic is the latest tic received from the server.
//
// a gametic cannot be run until ticcmds are received for it
// from all players.
//
#define BACKUPTICS 128
static ticcmd_set_t ticdata[BACKUPTICS];

// The index of the next tic to be made (with a call to BuildTiccmd).

static int maketic;

// The number of tics that have been run (using RunTic) so far.

int gametic;
int oldleveltime; // [crispy] check if leveltime keeps tickin'

// When set to true, a single tic is run each time TryRunTics() is called.
// This is used for -timedemo mode.

boolean singletics = false;

// Index of the local player.

static int localplayer;

// Reduce the bandwidth needed by sampling game input less and transmitting
// less.  If ticdup is 2, sample half normal, 3 = one third normal, etc.

int		ticdup = 1;

// Amount to offset the timer for game sync.

fixed_t         offsetms;

// Use new client syncronisation code

static boolean  new_sync = true;

// Callback functions for loop code.

static loop_interface_t *loop_interface = NULL;

// Current players in the multiplayer game.
// This is distinct from playeringame[] used by the game code, which may
// modify playeringame[] when playing back multiplayer demos.

static boolean local_playeringame[1];

// 35 fps clock adjusted by offsetms milliseconds

static int GetAdjustedTime(void)
{
    int time_ms;

    time_ms = I_GetTimeMS();

    return (time_ms * TICRATE) / 1000;
}

extern void D_ProcessEvents (void); // d_main.c
extern void M_Ticker (void); // m_menu.c
void G_BuildTiccmd (ticcmd_t* cmd, int maketic);

static boolean BuildNewTic(void)
{
    int	gameticdiv;
    ticcmd_t cmd;

    gameticdiv = gametic/ticdup;

    I_StartTic ();
    //loop_interface->ProcessEvents();
    D_ProcessEvents();

    // Always run the menu
    // loop_interface->RunMenu();
    M_Ticker();

    if (new_sync)
    {
       // If playing single player, do not allow tics to buffer
       // up very far

       if (maketic - gameticdiv > 2)
           return false;

       // Never go more than ~200ms ahead

       if (maketic - gameticdiv > 8)
           return false;
    }
    else
    {
       if (maketic - gameticdiv >= 5)
           return false;
    }

    //printf ("mk:%i ",maketic);
    memset(&cmd, 0, sizeof(ticcmd_t));
    //loop_interface->BuildTiccmd(&cmd, maketic);
    G_BuildTiccmd(&cmd, maketic);

    ticdata[maketic % BACKUPTICS].cmds[localplayer] = cmd;
    ticdata[maketic % BACKUPTICS].ingame[localplayer] = true;

    ++maketic;

    return true;
}

int      lasttime;

//
// Start game loop
//
// Called after the screen is set but before the game starts running.
//

void D_StartGameLoop(void)
{
    lasttime = GetAdjustedTime() / ticdup;
}

static int GetLowTic(void)
{
    int lowtic;

    lowtic = maketic;

    return lowtic;
}

// When using ticdup, certain values must be cleared out when running
// the duplicate ticcmds.

static void TicdupSquash(ticcmd_set_t *set)
{
    ticcmd_t *cmd;
    unsigned int i;

    for (i = 0; i < 1 ; ++i)
    {
        cmd = &set->cmds[i];
        cmd->chatchar = 0;
        if (cmd->buttons & BT_SPECIAL)
            cmd->buttons = 0;
    }
}

// When running in single player mode, clear all the ingame[] array
// except the local player.

static void SinglePlayerClear(ticcmd_set_t *set)
{
    unsigned int i;

    for (i = 0; i < 1; ++i)
    {
        if (i != localplayer)
        {
            set->ingame[i] = false;
        }
    }
}


static void RunTic(ticcmd_t *cmds, boolean *ingame)
{
    extern void D_DoAdvanceDemo(void);
    extern boolean advancedemo;
    extern ticcmd_t* netcmds;
    extern void G_Ticker(void);

    netcmds = cmds;

    // check that there are players in the game.  if not, we cannot
    // run a tic.

    if (advancedemo)
        D_DoAdvanceDemo ();

    G_Ticker ();
}

//
// TryRunTics
//

void TryRunTics (void)
{
    int	i;
    int	lowtic;
    int	entertic;
    static int oldentertics;
    int realtics;
    int	availabletics;
    int	counts;

    // [AM] If we've uncapped the framerate and there are no tics
    //      to run, return early instead of waiting around.
    extern int leveltime;
    #define return_early (crispy->uncapped && counts == 0 && leveltime > oldleveltime && screenvisible)

    // get real tics
    entertic = I_GetTime() / ticdup;
    realtics = entertic - oldentertics;
    oldentertics = entertic;

    // in singletics mode, run a single tic every time this function
    // is called.

    BuildNewTic();

    lowtic = GetLowTic();

    availabletics = lowtic - gametic/ticdup;

    // decide how many tics to run

    if (new_sync)
    {
	      counts = availabletics;

        // [AM] If we've uncapped the framerate and there are no tics
        //      to run, return early instead of waiting around.
        if (return_early)
            return;
    }
    else
    {
        // decide how many tics to run
        if (realtics < availabletics-1)
            counts = realtics+1;
        else if (realtics < availabletics)
            counts = realtics;
        else
            counts = availabletics;

        // [AM] If we've uncapped the framerate and there are no tics
        //      to run, return early instead of waiting around.
        if (return_early)
            return;

        if (counts < 1)
            counts = 1;
    }

    if (counts < 1)
	    counts = 1;

    // wait for new tics if needed
    while (lowtic < gametic/ticdup + counts)
    {
        lowtic = GetLowTic();

  	  if (lowtic < gametic/ticdup)
  	    I_Error ("TryRunTics: lowtic < gametic");

        // Still no tics to run? Sleep until some are available.
        if (lowtic < gametic/ticdup + counts)
        {
            // If we're in a netgame, we might spin forever waiting for
            // new network data to be received. So don't stay in here
            // forever - give the menu a chance to work.
            if (I_GetTime() / ticdup - entertic >= MAX_NETGAME_STALL_TICS)
            {
                return;
            }

            I_Sleep(1);
        }
    }

    // run the count * ticdup dics
    while (counts--)
    {
        ticcmd_set_t *set;

        set = &ticdata[(gametic / ticdup) % BACKUPTICS];

        SinglePlayerClear(set);

    	for (i=0 ; i<ticdup ; i++)
    	{
            if (gametic/ticdup > lowtic)
                I_Error ("gametic>lowtic");

            memcpy(local_playeringame, set->ingame, sizeof(local_playeringame));

            // loop_interface->RunTic(set->cmds, set->ingame);
            RunTic(set->cmds, set->ingame);
    	    gametic++;

    	    // modify command for duplicated tics

            TicdupSquash(set);
    	}
    }
}

void D_RegisterLoopCallbacks(loop_interface_t *i)
{
    loop_interface = i;
}

// TODO: Move nonvanilla demo functions into a dedicated file.
#include "m_misc.h"
#include "w_wad.h"

static boolean StrictDemos(void)
{
    //!
    // @category demo
    //
    // When recording or playing back demos, disable any extensions
    // of the vanilla demo format - record demos as vanilla would do,
    // and play back demos as vanilla would do.
    //
    return M_ParmExists("-strictdemos");
}

// If the provided conditional value is true, we're trying to record
// a demo file that will include a non-vanilla extension. The function
// will return true if the conditional is true and it's allowed to use
// this extension (no extensions are allowed if -strictdemos is given
// on the command line). A warning is shown on the console using the
// provided string describing the non-vanilla expansion.
boolean D_NonVanillaRecord(boolean conditional, const char *feature)
{
    if (!conditional || StrictDemos())
    {
        return false;
    }

    printf("Warning: Recording a demo file with a non-vanilla extension "
           "(%s). Use -strictdemos to disable this extension.\n",
           feature);

    return true;
}

// Returns true if the given lump number corresponds to data from a .lmp
// file, as opposed to a WAD.
static boolean IsDemoFile(int lumpnum)
{
    char *lower;
    boolean result;

    lower = M_StringDuplicate(lumpinfo[lumpnum]->wad_file->path);
    M_ForceLowercase(lower);
    result = M_StringEndsWith(lower, ".lmp");
    free(lower);

    return result;
}

// If the provided conditional value is true, we're trying to play back
// a demo that includes a non-vanilla extension. We return true if the
// conditional is true and it's allowed to use this extension, checking
// that:
//  - The -strictdemos command line argument is not provided.
//  - The given lumpnum identifying the demo to play back identifies a
//    demo that comes from a .lmp file, not a .wad file.
//  - Before proceeding, a warning is shown to the user on the console.
boolean D_NonVanillaPlayback(boolean conditional, int lumpnum,
                             const char *feature)
{
    if (!conditional || StrictDemos())
    {
        return false;
    }

    if (!IsDemoFile(lumpnum))
    {
        printf("Warning: WAD contains demo with a non-vanilla extension "
               "(%s)\n", feature);
        return false;
    }

    printf("Warning: Playing back a demo file with a non-vanilla extension "
           "(%s). Use -strictdemos to disable this extension.\n",
           feature);

    return true;
}

