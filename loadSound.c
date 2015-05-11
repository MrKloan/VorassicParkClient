#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <fmod/fmod.h>

#include "sysvars.h"
#include "loadSound.h"

extern Sysvar sys;

int loadSound(Sysvar *sys, char* path)
{
    if (sys->sound != NULL)
    {
//        FSOUND_Sample_Free(sys->sound);
    }

//    sys->sound = FSOUND_Sample_Load(FSOUND_FREE, path, 0, 0, 0);

    if (sys->sound != NULL)
    {
        return 1;
    }

    return 0;
}

void playSound(Sysvar *sys)
{
//    FSOUND_PlaySound(0, sys->sound);
}

int loadMusic(Sysvar *sys, char* path)
{
    if (sys->music != NULL)
    {
//        FSOUND_Stream_Close(sys->music);
    }

//    sys->music = FSOUND_Stream_Open(path, FSOUND_LOOP_NORMAL, 0, 0);

//    FSOUND_Stream_SetLoopCount(sys->music, -1);

    if (sys->music != NULL)
    {
        return 1;
    }

    return 0;
}

void playMusic(Sysvar *sys)
{
//    FSOUND_Stream_Play(1, sys->music);
}
