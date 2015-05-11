#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <fmod/fmod.h>

#include "sysvars.h"
#include "freeProg.h"

void freeProg(Sysvar *sys)
{
    SDL_FreeSurface(sys->screen);
//    FSOUND_Stream_Close(sys->music);
//    FSOUND_Sample_Free(sys->sound);
    SDL_FreeSurface(sys->cursor);

    SDL_Quit();
//    FSOUND_Close();
    TTF_Quit();

    exit(EXIT_SUCCESS);
}
