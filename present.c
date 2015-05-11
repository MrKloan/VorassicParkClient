#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <fmod/fmod.h>

#include "sysvars.h"
#include "constantes.h"
#include "present.h"
#include "anim.h"
#include "loadSound.h"

void present(Sysvar *sys)
{
    TTF_Font *police = TTF_OpenFont("Font/Jurassic Park.ttf", 96), *police2 = TTF_OpenFont("Font/Jurassic Park.ttf", 64);
    SDL_Color couleur = {0, 200, 230}, couleur2 = {230, 0, 0};
    SDL_Surface *logoE = IMG_Load("Pictures/Logo-ESGI.bmp");
    SDL_Surface *ad = TTF_RenderText_Blended(police, "Aurelien Duval   ", couleur);
    SDL_Surface *et = TTF_RenderText_Blended(police2, "AND", couleur2);
    SDL_Surface *vf = TTF_RenderText_Blended(police, "Valentin Fries", couleur);
    SDL_Surface *pres = TTF_RenderText_Blended(police, "present", couleur);

    sys->position.x = LARGEUR_FENETRE / 4 - ad->w / 2;
    sys->position.y = (HAUTEUR_FENETRE - ad->h) / 3 - 100;
    SDL_BlitSurface(ad, NULL, sys->screen, &sys->position);

    sys->position.x = LARGEUR_FENETRE / 2 - et->w / 2;
    sys->position.y = (HAUTEUR_FENETRE - et->h) / 3 - 150;
    SDL_BlitSurface(et, NULL, sys->screen, &sys->position);

    sys->position.x = 3 * LARGEUR_FENETRE / 4 - vf->w / 2;
    sys->position.y = (HAUTEUR_FENETRE - vf->h) / 3 - 100;
    SDL_BlitSurface(vf, NULL, sys->screen, &sys->position);

    sys->position.x = (LARGEUR_FENETRE - pres->w) / 2;
    sys->position.y = (HAUTEUR_FENETRE - pres->h) / 2 + 50;
    SDL_BlitSurface(pres, NULL, sys->screen, &sys->position);

    sys->position.x = LARGEUR_FENETRE - logoE->w - 20;
    sys->position.y = HAUTEUR_FENETRE - logoE->h - 20;
    SDL_BlitSurface(logoE, NULL, sys->screen, &sys->position);

    loadMusic(sys, "Music/Asphyxia_-_Without_You.mp3");
    playMusic(sys);

//    FSOUND_SetFrequency(1, 44100 * 1.5);

//    FSOUND_SetVolume(1, 128);

    whiteScreen(sys, 1, 2, 0, 255);

    SDL_Delay(5000);

    blackScreen(sys, 1, 2, 255, 0);

    SDL_FreeSurface(ad);
    SDL_FreeSurface(vf);
    SDL_FreeSurface(logoE);
    TTF_CloseFont(police);
    TTF_CloseFont(police2);
}
