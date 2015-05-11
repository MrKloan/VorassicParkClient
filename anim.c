#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <fmod/fmod.h>

#include "sysvars.h"
#include "constantes.h"
#include "anim.h"
#include "pixel.h"

// nettoyage de l'ecran
void freeScreen(Sysvar *sys)
{
    SDL_FillRect(sys->screen, NULL, SDL_MapRGB(sys->screen->format, 0, 0, 0));
}

// animation disparition dans la transparence
void blackScreen(Sysvar *sys, int soundEffect, int speed, int begin, int limit)
{
    SDL_Surface *CloneEcran = SDL_CreateRGBSurface (SDL_HWSURFACE, LARGEUR_FENETRE, HAUTEUR_FENETRE, RESOLUTION, 0, 0, 0, 0);
    int i;

    sys->position.x = 0;
    sys->position.y = 0;
    SDL_BlitSurface(sys->screen, NULL, CloneEcran, &sys->position);

    for ( i = begin ; i > limit ; i-=speed)
    {
        SDL_PollEvent(&sys->event);

        if (sys->event.key.keysym.sym == SDLK_SYSREQ)
        {
            SDL_WM_IconifyWindow();
        }

        else if (sys->event.type == SDL_QUIT)
        {
            exit(EXIT_SUCCESS);
        }


        SDL_Delay(1000 / FRAMERATE);

        freeScreen(sys);

        SDL_SetAlpha(CloneEcran, SDL_SRCALPHA, i);

        sys->position.x = 0;
        sys->position.y = 0;
        SDL_BlitSurface(CloneEcran, NULL, sys->screen, &sys->position);

        if (soundEffect == 1)
        {
//            FSOUND_SetVolume (FSOUND_ALL, i);
        }

        SDL_Flip(sys->screen);
    }

    SDL_FreeSurface(CloneEcran);
}

// animation apparition dans la transparence : NECESSITE UNE PREPARATION NON BLITTE DE L'ECRAN
void whiteScreen(Sysvar *sys, int soundEffect, int speed, int begin, int limit)
{
    SDL_Surface *CloneEcran = SDL_CreateRGBSurface (SDL_HWSURFACE, LARGEUR_FENETRE, HAUTEUR_FENETRE, RESOLUTION, 0, 0, 0, 0);
    int i;

    sys->position.x = 0;
    sys->position.y = 0;
    SDL_BlitSurface(sys->screen, NULL, CloneEcran, &sys->position);

    for ( i = begin ; i < limit ; i+=speed)
    {

        SDL_PollEvent(&sys->event);

        if (sys->event.key.keysym.sym == SDLK_SYSREQ)
        {
            SDL_WM_IconifyWindow();
        }

        else if (sys->event.type == SDL_QUIT)
        {
            exit(EXIT_SUCCESS);
        }

        SDL_Delay(1000 / FRAMERATE);

        freeScreen(sys);

        SDL_SetAlpha(CloneEcran, SDL_SRCALPHA, i);

        sys->position.x = 0;
        sys->position.y = 0;
        SDL_BlitSurface(CloneEcran, NULL, sys->screen, &sys->position);

        if (soundEffect == 1)
        {
//            FSOUND_SetVolume (FSOUND_ALL, i);
        }

        SDL_Flip(sys->screen);
    }

    SDL_FreeSurface(CloneEcran);
}

SDL_Surface* makePlaceHolder(int w, int h)
{
    SDL_Surface *temp = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, RESOLUTION, 0, 0, 0, 0);

    SDL_FillRect(temp, NULL, 0xffffff);

    return temp;
}

void blitCursor(Sysvar *sys)
{
    int x, y;

    SDL_GetMouseState(&x,&y);

    sys->position.x = x;
    sys->position.y = y;
    SDL_BlitSurface(sys->cursor, NULL, sys->screen, &sys->position);
}
