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
#include "mapStruct.h"
#include "gameParams.h"
#include "anim.h"

/*

    MODE :

        0 : solo
        1 : multi
        2 : network

*/

GameParams setParams(Sysvar *sys, int mode)
{

    TTF_Font *police = TTF_OpenFont("Font/verdana.ttf", 48);
    SDL_Surface *bg = IMG_Load("Pictures/Vorassic_bg.bmp");
    SDL_Surface *logo = IMG_Load("Pictures/vorassicpark.bmp");
    SDL_Surface *item[5] = {NULL};
    SDL_Surface *values[4] = {NULL};
    int continuer = 1;
    int transp = 0;
    SDL_Color couleur = {230, 230, 230}, couleurHover = {230, 0, 0}, couleurUnavailable = {128, 128, 128};
    int isCaught = 0;
    int i;
    int selector = -1, exSelector;
    char tab[5][20] = {"Players :", "IA Players :", "Mode :", "Extension Pack :", "Launch Game"};
    char val[4][20] = {"1", "0", "Snake", "Dino"};
    SDL_Surface *left = IMG_Load("Pictures/left_arrow.bmp");
    SDL_Surface *right = IMG_Load("Pictures/right_arrow.bmp");
    int stock[4] = {1, 0, 0, 1};
    GameParams params;

    if (mode != 0)
    {
        stock[0] = 2;
        sprintf (val[0], "2");
    }

    for (i = 0 ; i < 4 ; i++)
    {
        item[i] = TTF_RenderText_Blended(police, tab[i], couleur);
        values[i] = TTF_RenderText_Blended(police, val[i], couleur);
    }

    item[4] = TTF_RenderText_Blended(police, tab[4], couleur);

    SDL_SetColorKey(logo, SDL_SRCCOLORKEY, SDL_MapRGB(sys->screen->format, 0, 255, 0));
    SDL_SetColorKey(left, SDL_SRCCOLORKEY, SDL_MapRGB(sys->screen->format, 0, 255, 0));
    SDL_SetColorKey(right, SDL_SRCCOLORKEY, SDL_MapRGB(sys->screen->format, 0, 255, 0));

    loadSound(sys, "Sound/SLAB3.WAV");

    while (continuer && sys->continuer)
    {
        while (SDL_PollEvent(&sys->event))
        {
            switch (sys->event.type)
            {
                case SDL_QUIT:
                    sys->continuer = 0;
                    break;

                case SDL_MOUSEMOTION:
                    selector = -1;
                    for (i = 0 ; i < 4 && selector == -1; i++)
                    {
                        if(sys->event.motion.x > (LARGEUR_FENETRE - values[i]->w) / 2 - left->w + 50 && sys->event.motion.x < (LARGEUR_FENETRE - values[i]->w) / 2 + 50 && sys->event.motion.y > 370 + 60 * i && sys->event.motion.y < 370 + 60 * i + left->h)
                            selector = i * 2;

                        if(sys->event.motion.x > (LARGEUR_FENETRE + values[i]->w) / 2 + 150 && sys->event.motion.x < (LARGEUR_FENETRE + values[i]->w) / 2 + 150 + right->w && sys->event.motion.y > 370 + 60 * i && sys->event.motion.y < 370 + 60 * i + right->h)
                            selector = i * 2 + 1;

                    }

                    if (selector == -1)
                    {
                        if(sys->event.motion.x > (LARGEUR_FENETRE - item[4]->w) / 2 && sys->event.motion.x < (LARGEUR_FENETRE - item[4]->w) / 2 + item[4]->w && sys->event.motion.y > 350 + 60 * 5 && sys->event.motion.y < 350 + 60 * 5 + item[4]->h)
                            selector = 8;
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
                                    if (mode != 0 && stock[0] > 2)
                                    {
                                        stock[0]--;

                                        sprintf (val[0], "%d", stock[0]);

                                    }
                                    break;

                                case 1:
                                    if (mode != 0 && stock[0] < 4)
                                    {
                                        stock[0]++;

                                        sprintf (val[0], "%d", stock[0]);

                                    }
                                    break;

                                case 2:
                                    if (stock[1] > 0)
                                    {
                                        stock[1]--;

                                        sprintf (val[1], "%d", stock[1]);

                                    }
                                    break;

                                case 3:
                                    if (stock[1] < 3)
                                    {
                                        stock[1]++;

                                        sprintf (val[1], "%d", stock[1]);

                                    }
                                    break;

                                case 4:
                                    stock[2]+=3;
                                    stock[2] %= 2;
                                    if (stock[2] != 1)
                                    {
                                        sprintf (val[2], "Snake");
                                    }

                                    else
                                    {
                                        sprintf (val[2], "Octopus");
                                    }
                                    break;

                                case 5:
                                    stock[2]++;
                                    stock[2] %= 2;
                                    if (stock[2] != 1)
                                    {
                                        sprintf (val[2], "Snake");
                                    }

                                    else
                                    {
                                        sprintf (val[2], "Octopus");
                                    }
                                    break;

                                case 6:
                                    stock[3]+= 7;
                                    stock[3] %= 4;
                                    switch(stock[3])
                                    {
                                        case 0:
                                            sprintf (val[3], "Default");
                                            break;
                                        case 1:
                                            sprintf (val[3], "Dino");
                                            break;
                                        case 2:
                                            sprintf (val[3], "Exam");
                                            break;
                                        case 3:
                                            sprintf (val[3], "Troll It");
                                            break;
                                    }
                                    break;

                                case 7:
                                    stock[3]++;
                                    stock[3] %= 4;
                                    switch(stock[3])
                                    {
                                        case 0:
                                            sprintf (val[3], "Default");
                                            break;
                                        case 1:
                                            sprintf (val[3], "Dino");
                                            break;
                                        case 2:
                                            sprintf (val[3], "Exam");
                                            break;
                                        case 3:
                                            sprintf (val[3], "Troll It");
                                            break;
                                    }
                                    break;

                                case 8:
                                    if (stock[0] + stock[1] < 5 && stock[0] + stock[1] > 1)
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

        else if (transp < 128)
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
                if (mode != 0)
                    playSound(sys);

                else if (selector > 1)
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

        for (i = 0 ; i < 4 ; i++)
        {
            if (i == 0 && mode == 0)
            {
                SDL_SetAlpha(left, SDL_SRCALPHA, 128);
                SDL_SetAlpha(right, SDL_SRCALPHA, 128);

                SDL_FreeSurface(values[i]);

                values[i] = TTF_RenderText_Blended(police, val[i], couleurUnavailable);
            }

            else if (selector == i*2)
            {
                SDL_SetAlpha(left, SDL_SRCALPHA, 255);
                SDL_SetAlpha(right, SDL_SRCALPHA, 128);

                SDL_FreeSurface(values[i]);

                values[i] = TTF_RenderText_Blended(police, val[i], couleurHover);
            }

            else if (selector == i*2 + 1)
            {
                SDL_SetAlpha(left, SDL_SRCALPHA, 128);
                SDL_SetAlpha(right, SDL_SRCALPHA, 255);

                SDL_FreeSurface(values[i]);

                values[i] = TTF_RenderText_Blended(police, val[i], couleurHover);
            }

            else
            {
                SDL_SetAlpha(left, SDL_SRCALPHA, 128);
                SDL_SetAlpha(right, SDL_SRCALPHA, 128);

                SDL_FreeSurface(values[i]);

                values[i] = TTF_RenderText_Blended(police, val[i], couleur);
            }

            sys->position.x = (LARGEUR_FENETRE - values[i]->w) / 2 + 100;
            sys->position.y = 350 + 60 * i;
            SDL_BlitSurface(values[i], NULL, sys->screen, &sys->position);

            sys->position.x = (LARGEUR_FENETRE + values[i]->w) / 2 + 150;
            sys->position.y = 370 + 60 * i;
            SDL_BlitSurface(right, NULL, sys->screen, &sys->position);

            sys->position.x = (LARGEUR_FENETRE - values[i]->w) / 2 - left->w + 50;
            sys->position.y = 370 + 60 * i;
            SDL_BlitSurface(left, NULL, sys->screen, &sys->position);

            sys->position.x = (LARGEUR_FENETRE - values[i]->w) / 2 - left->w - item[i]->w;
            sys->position.y = 350 + 60 * i;
            SDL_BlitSurface(item[i], NULL, sys->screen, &sys->position);
        }

        if (selector != 8)
        {
            SDL_FreeSurface(item[4]);
            item[4] = TTF_RenderText_Blended(police, tab[4], couleur);

        }

        else
        {
            SDL_FreeSurface(item[4]);
            item[4] = TTF_RenderText_Blended(police, tab[4], couleurHover);
        }

        sys->position.x = (LARGEUR_FENETRE - item[4]->w) / 2;
        sys->position.y = 350 + 60 * 5;
        SDL_BlitSurface(item[4], NULL, sys->screen, &sys->position);

        blitCursor(sys);

        SDL_Flip(sys->screen);
    }

    for (i = 0 ; i < 5 ; i++)
    {
        SDL_FreeSurface(item[i]);
    }

    for (i = 0 ; i < 4 ; i++)
    {
        SDL_FreeSurface(values[i]);
    }

    SDL_FreeSurface(bg);
    SDL_FreeSurface(logo);
    TTF_CloseFont(police);
    SDL_FreeSurface(left);
    SDL_FreeSurface(right);

    params.nbJoueurs = stock[0];
    params.nbBot = stock[1];
    params.mode = stock[2];
    params.extensionPack = stock[3];
    params.nbMap = -1;

    if (mode != 2)
        params.isNetwork = 1;

    else
        params.isNetwork = 0;

    blackScreen(sys, 1, 5, 255, 0);

    return params;

}

void loadExtendElem(Sysvar *sys, SDL_Surface *pions[4], SDL_Surface **bg, GameParams params)
{
    int i;

    switch (params.extensionPack)
    {

        case 0 : /// DEFAULT
            pions[0] = IMG_Load("Pictures/Pions/pion_J1.bmp");
            pions[1] = IMG_Load("Pictures/Pions/pion_J2.bmp");
            pions[2] = IMG_Load("Pictures/Pions/pion_J3.bmp");
            pions[3] = IMG_Load("Pictures/Pions/pion_J4.bmp");
            break;

        case 1 : /// DINO
            pions[0] = IMG_Load("Pictures/Pions/pion_triceratops.bmp");
            pions[1] = IMG_Load("Pictures/Pions/pion_trex.bmp");
            pions[2] = IMG_Load("Pictures/Pions/pion_pyroraptor.bmp");
            pions[3] = IMG_Load("Pictures/Pions/pion_pterodactyle.bmp");
            break;

        case 2 : /// EXAM
            pions[0] = IMG_Load("Pictures/Pions/pion_sananes.bmp");
            pions[1] = IMG_Load("Pictures/Pions/pion_baudoin.bmp");
            pions[2] = IMG_Load("Pictures/Pions/pion_abid.bmp");
            pions[3] = IMG_Load("Pictures/Pions/pion_peter.bmp");

            SDL_FreeSurface(*bg);

            *bg = IMG_Load("Pictures/jurassic_park1.jpg");

            loadMusic(sys, "Music/This_is_Sparta.mp3");
            break;

        case 3 : /// TROLL IT
            pions[0] = IMG_Load("Pictures/Pions/pion_sparta.bmp");
            pions[1] = IMG_Load("Pictures/Pions/pion_pedobear.bmp");
            pions[2] = IMG_Load("Pictures/Pions/pion_nyan.bmp");
            pions[3] = IMG_Load("Pictures/Pions/pion_troll.bmp");

            SDL_FreeSurface(*bg);

            *bg = IMG_Load("Pictures/dinosaur-titanic-Clive-Palmer-jurassic-park.jpg");

            loadMusic(sys, "Music/Peanut_Butter_Jelly_Time.mp3");
            break;

    }

    for (i = 0 ; i < 4 ; i++)
    {
        SDL_SetColorKey(pions[i], SDL_SRCCOLORKEY, SDL_MapRGB(pions[i]->format, 0, 255, 0));
    }
}
