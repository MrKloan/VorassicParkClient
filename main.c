#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <fmod/fmod.h>
#include <time.h>

#include "sysvars.h"
#include "constantes.h"
#include "loadSound.h"
#include "freeProg.h"
#include "present.h"
#include "menuP.h"
#include "mapStruct.h"
#include "gameParams.h"
#include "params.h"

int main (int argc, char **argv)
{

    Sysvar sys;

    sys.screen = NULL;
    sys.music = NULL;
    sys.sound = NULL;
    sys.continuer = 1;
    sys.cursor = IMG_Load("Cursors/MB-EyeOfTheDamned_th.bmp");

    SDL_Init(SDL_INIT_VIDEO);
//    FSOUND_Init(SOUND_FREQ, SOUND_Q, 0);
    TTF_Init();

    srand(time(NULL));

    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetColorKey(sys.cursor, SDL_SRCCOLORKEY, SDL_MapRGB(sys.cursor->format, 0, 255, 0));

    #ifdef RELEASE
        sys.screen = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, RESOLUTION, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWACCEL | SDL_FULLSCREEN);
        SDL_WM_SetCaption("Vorassic Park", NULL);
        present(&sys);
    #else
        #ifdef DEBUG
            sys.screen = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, RESOLUTION, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWACCEL);
            SDL_WM_SetCaption("Vorassic Park [DEBUG]", NULL);
        #endif
    #endif

    SDL_Delay(1000);

    menuP(&sys);

    freeProg(&sys);

    return EXIT_SUCCESS;
}
