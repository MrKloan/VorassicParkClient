#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <fmod/fmod.h>

#include "sysvars.h"
#include "constantes.h"
#include "loadSound.h"
#include "menuLan.h"
#include "anim.h"
#include "socket.h"

short menuLan(Sysvar *sys)
{
    short loop = 1;
    TTF_Font *police = TTF_OpenFont("Font/verdana.ttf", 30), *policeJurassic = TTF_OpenFont("Font/Jurassic Park.ttf", 55), *policeJurassicHover = TTF_OpenFont("Font/Jurassic Park.ttf", 72);;
    SDL_Surface *bg = IMG_Load("Pictures/Vorassic_bg.bmp");
    SDL_Surface *logo = IMG_Load("Pictures/vorassicpark.bmp");
    SDL_Surface *input = IMG_Load("Pictures/input.jpg");
    SDL_Color color = {230, 230, 230}, colorHover = {230, 0, 0};
    short i;
    int transp = 0;
    int isCaught = -1;
    int selector = -1, exSelector;
    char tab[2][10] = {"Adresse", "Connexion"};
    SDL_Surface *item[3] = {NULL};
    char host[MAX_LAN_HOST_LENGTH+1] = "127.0.0.1\0";
    unsigned short hostLength = strlen(host);
    short connected = 0;

    playMusic(sys);
//    FSOUND_SetFrequency(1, 44100 * 1.2);

    SDL_SetColorKey(logo, SDL_SRCCOLORKEY, SDL_MapRGB(sys->screen->format, 0, 255, 0));

    sys->position.x = 0;
    sys->position.y = 0;
    SDL_BlitSurface(bg, NULL, sys->screen, &sys->position);

    while(loop && sys->continuer)
    {
        while(SDL_PollEvent(&sys->event))
        {
            switch(sys->event.type)
            {
                case SDL_QUIT:
                    sys->continuer = 0;
                    break;

                case SDL_KEYDOWN:
                    if(sys->event.key.keysym.sym == SDLK_ESCAPE)
                        loop = 0;

                    //Saisie de l'hote
                    //Pavé numérique
                    if(hostLength < MAX_LAN_HOST_LENGTH && ((sys->event.key.keysym.sym >= SDLK_KP0 && sys->event.key.keysym.sym <= SDLK_KP9) || sys->event.key.keysym.sym == SDLK_KP_PERIOD))
                    {
                        host[hostLength] = SDL_GetKeyName(sys->event.key.keysym.sym)[1];
                        host[hostLength+1] = '\0';
                        hostLength++;
                    }
                    //Suppression
                    if(hostLength > 0 && sys->event.key.keysym.sym == SDLK_BACKSPACE)
                    {
                        hostLength--;
                        host[hostLength] = '\0';
                    }

                    //Connexion
                    if(hostLength > 0 && (sys->event.key.keysym.sym == SDLK_RETURN || sys->event.key.keysym.sym == SDLK_KP_ENTER))
                    {
                        if(connectToServer(sys, host))
                        {
                            connected = 1;
                            loop = 0;
                        }
                    }

                    break;

                case SDL_MOUSEMOTION:
                    selector = -1;
                    if(selector == -1 && isCaught != -1)
                    {
                        if(sys->event.motion.x > (LARGEUR_FENETRE - item[1]->w) / 2 && sys->event.motion.x < (LARGEUR_FENETRE - item[1]->w) / 2 + item[1]->w && sys->event.motion.y > HAUTEUR_FENETRE - 150 && sys->event.motion.y < HAUTEUR_FENETRE - 150 + item[1]->h)
                            selector = 1;
                    }

                    break;

                case SDL_MOUSEBUTTONDOWN:
                    switch (sys->event.button.button)
                    {
                        case SDL_BUTTON_LEFT:
                            switch (selector)
                            {
                                case 1:
                                    //Bouton connexion
                                    if(connectToServer(sys, host))
                                    {
                                        connected = 1;
                                        loop = 0;
                                    }
                                    break;
                            }
                            break;

                        default:
                            break;
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

        sys->position.x = (LARGEUR_FENETRE - input->w) / 2;;
        sys->position.y = 300;
        SDL_BlitSurface(input, NULL, sys->screen, &sys->position);

        for (i = 0 ; i < 2 && isCaught != -1 ; i++)
        {
            SDL_FreeSurface(item[i]);

            if (selector != i)
                item[i] = TTF_RenderText_Blended(policeJurassic, tab[i], color);
            else
                item[i] = TTF_RenderText_Blended(policeJurassicHover, tab[i], colorHover);

            sys->position.x = (LARGEUR_FENETRE - item[i]->w) / 2;
            if(i == 0)
                sys->position.y = 265;
            else
                sys->position.y = HAUTEUR_FENETRE - 150;

            SDL_BlitSurface(item[i], NULL, sys->screen, &sys->position);
        }

        SDL_FreeSurface(item[2]);

        if(hostLength == 0)
            item[2] = TTF_RenderText_Blended(police, "Saisir l'adresse IP du serveur", color);
        else
            item[2] = TTF_RenderText_Blended(police, host, color);

        sys->position.x = (LARGEUR_FENETRE - item[2]->w) / 2;
        sys->position.y = 320;
        SDL_BlitSurface(item[2], NULL, sys->screen, &sys->position);

        blitCursor(sys);

        SDL_Flip(sys->screen);
    }

    for (i = 0 ; i < 3 ; i++)
        SDL_FreeSurface(item[i]);

    SDL_FreeSurface(input);
    SDL_FreeSurface(bg);
    SDL_FreeSurface(logo);
    TTF_CloseFont(police);
    TTF_CloseFont(policeJurassic);
    TTF_CloseFont(policeJurassicHover);

    return connected;
}
