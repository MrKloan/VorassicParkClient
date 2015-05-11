#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <fmod/fmod.h>

#include "sysvars.h"
#include "constantes.h"
#include "loadSound.h"
#include "menuP.h"
#include "anim.h"
#include "play.h"
#include "present.h"

void menuP(Sysvar *sys)
{

    TTF_Font *police = TTF_OpenFont("Font/Jurassic Park.ttf", 48), *policeHover = TTF_OpenFont("Font/Jurassic Park.ttf", 72);
    SDL_Surface *bg = IMG_Load("Pictures/Vorassic_bg.bmp");
    SDL_Surface *logo = IMG_Load("Pictures/vorassicpark.bmp");
    SDL_Surface *item[6] = {NULL};
    int continuer = 1;
    int transp = 0;
    int isCaught = -1;
    SDL_Color couleur = {230, 230, 230}, couleurHover = {230, 0, 0};
    int i;
    int selector = -1, exSelector;
    char tab[6][20] = {"Solo", "Multiplayer", "LAN Game", "Load Game", "Credits", "Quit"};

    for (i = 0 ; i < 6 ; i++)
    {
        item[i] = TTF_RenderText_Blended(police, tab[i], couleur);
    }

    loadSound(sys, "Sound/SLAB3.WAV");

    loadMusic(sys, "Music/Jurassic_Park_-_Theme.mp3");
    playMusic(sys);

//    FSOUND_SetFrequency(1, 44100 * 1.2);

    SDL_SetColorKey(logo, SDL_SRCCOLORKEY, SDL_MapRGB(sys->screen->format, 0, 255, 0));

    sys->position.x = 0;
    sys->position.y = 0;
    SDL_BlitSurface(bg, NULL, sys->screen, &sys->position);

    whiteScreen(sys, 0, 1, 0, 255);

    while (continuer && sys->continuer)
    {
        while (SDL_PollEvent(&sys->event))
        {
            switch (sys->event.type)
            {
                case SDL_QUIT:
                    sys->continuer = 0;
                    break;

                case SDL_KEYDOWN:
                    switch (sys->event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            continuer = 0;
                            break;

                        default:
                            break;
                    }
                    break;

                case SDL_MOUSEMOTION:
                    selector = -1;
                    for (i = 0 ; i < 6 && selector == -1 && isCaught != -1; i++)
                    {
                        if(sys->event.motion.x > (LARGEUR_FENETRE - item[i]->w) / 2 && sys->event.motion.x < (LARGEUR_FENETRE - item[i]->w) / 2 + item[i]->w && sys->event.motion.y > 350 + 60 * i && sys->event.motion.y < 350 + 60 * i + item[i]->h)
                            selector = i;

                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                {
                    switch (sys->event.button.button)
                    {
                        case SDL_BUTTON_LEFT:
                            switch (selector)
                            {
                                case 0:
                                    // SOLO
                                    play(sys, 0);
                                    loadSound(sys, "Sound/SLAB3.WAV");

                                    loadMusic(sys, "Music/Jurassic_Park_-_Theme.mp3");
                                    playMusic(sys);

//                                    FSOUND_SetFrequency(1, 44100 * 1.2);

                                    SDL_SetColorKey(logo, SDL_SRCCOLORKEY, SDL_MapRGB(sys->screen->format, 0, 255, 0));

                                    sys->position.x = 0;
                                    sys->position.y = 0;
                                    SDL_BlitSurface(bg, NULL, sys->screen, &sys->position);

                                    whiteScreen(sys, 1, 5, 0, 255);
                                    break;

                                case 1:
                                    // MULTI
                                    play(sys, 1);
                                    loadSound(sys, "Sound/SLAB3.WAV");

                                    loadMusic(sys, "Music/Jurassic_Park_-_Theme.mp3");
                                    playMusic(sys);

//                                    FSOUND_SetFrequency(1, 44100 * 1.2);

                                    SDL_SetColorKey(logo, SDL_SRCCOLORKEY, SDL_MapRGB(sys->screen->format, 0, 255, 0));

                                    sys->position.x = 0;
                                    sys->position.y = 0;
                                    SDL_BlitSurface(bg, NULL, sys->screen, &sys->position);

                                    whiteScreen(sys, 1, 5, 0, 255);
                                    break;

                                case 2:
                                    // LAN (BONUS)
                                    play(sys, 2);
                                    loadSound(sys, "Sound/SLAB3.WAV");

                                    loadMusic(sys, "Music/Jurassic_Park_-_Theme.mp3");
                                    playMusic(sys);

//                                    FSOUND_SetFrequency(1, 44100 * 1.2);

                                    SDL_SetColorKey(logo, SDL_SRCCOLORKEY, SDL_MapRGB(sys->screen->format, 0, 255, 0));

                                    sys->position.x = 0;
                                    sys->position.y = 0;
                                    SDL_BlitSurface(bg, NULL, sys->screen, &sys->position);

                                    whiteScreen(sys, 1, 5, 0, 255);
                                    break;

                                case 3:
                                    // LOAD GAME
                                    play(sys, 3);
                                    loadSound(sys, "Sound/SLAB3.WAV");

                                    loadMusic(sys, "Music/Jurassic_Park_-_Theme.mp3");
                                    playMusic(sys);

//                                    FSOUND_SetFrequency(1, 44100 * 1.2);

                                    SDL_SetColorKey(logo, SDL_SRCCOLORKEY, SDL_MapRGB(sys->screen->format, 0, 255, 0));

                                    sys->position.x = 0;
                                    sys->position.y = 0;
                                    SDL_BlitSurface(bg, NULL, sys->screen, &sys->position);

                                    whiteScreen(sys, 1, 5, 0, 255);
                                    break;

                                case 4:
                                    // CREDITS
                                    freeScreen(sys);
                                    present(sys);

                                    loadSound(sys, "Sound/SLAB3.WAV");

                                    loadMusic(sys, "Music/Jurassic_Park_-_Theme.mp3");
                                    playMusic(sys);

//                                    FSOUND_SetFrequency(1, 44100 * 1.2);

                                    SDL_SetColorKey(logo, SDL_SRCCOLORKEY, SDL_MapRGB(sys->screen->format, 0, 255, 0));

                                    sys->position.x = 0;
                                    sys->position.y = 0;
                                    SDL_BlitSurface(bg, NULL, sys->screen, &sys->position);

                                    whiteScreen(sys, 1, 5, 0, 255);
                                    break;

                                case 5: // QUIT
                                    continuer = 0;
                                    break;

                                default:
                                    break;
                            }
                            break;

                        default:
                            break;
                    }
                }

                default:
                    break;
            }
        }

        SDL_Delay(1000 / FRAMERATE);

        if (transp > 250)
        {
            isCaught = 1;
            transp -= 5;
        }

        else if (transp < 128 && isCaught != -1)
        {
            isCaught = 0;
            transp += 5;
        }

        else if (isCaught == 1)
        {
            transp -= 5;
        }

        else
        {
            transp += 5;
        }

        if (selector != exSelector)
        {
            exSelector = selector;

            if (selector != -1)
            {
                playSound(sys);
            }
        }

        freeScreen(sys);

        sys->position.x = 0;
        sys->position.y = 0;
        SDL_BlitSurface(bg, NULL, sys->screen, &sys->position);

        sys->position.x = 3 * LARGEUR_FENETRE / 4 - logo->w / 2;
        sys->position.y = (HAUTEUR_FENETRE - logo->h) / 8;
        SDL_SetAlpha(logo, SDL_SRCALPHA, transp);
        SDL_BlitSurface(logo, NULL, sys->screen, &sys->position);

        for (i = 0 ; i < 6 && isCaught != -1 ; i++)
        {
            SDL_FreeSurface(item[i]);

            if (selector != i)
            {
                item[i] = TTF_RenderText_Blended(police, tab[i], couleur);
            }

            else
            {
                item[i] = TTF_RenderText_Blended(policeHover, tab[i], couleurHover);
            }

            sys->position.x = (LARGEUR_FENETRE - item[i]->w) / 2;
            sys->position.y = 350 + 60 * i;
            SDL_BlitSurface(item[i], NULL, sys->screen, &sys->position);
        }

        blitCursor(sys);

        SDL_Flip(sys->screen);
    }

    for (i = 0 ; i < 6 ; i++)
    {
        SDL_FreeSurface(item[i]);
    }

    SDL_FreeSurface(bg);
    SDL_FreeSurface(logo);
    TTF_CloseFont(police);
    TTF_CloseFont(policeHover);
}
