#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <fmod/fmod.h>

#include "sysvars.h"
#include "constantes.h"
#include "loadSound.h"
#include "mapStruct.h"

/** elems :

[0;6]  : map elements
{7}    : default box
[8;14] : bonus / malus

**/

SDL_Rect mapGame(Sysvar *sys, Map map, SDL_Surface *elements[9], TTF_Font *police)
{
    int i, j;
    SDL_Surface *txt = NULL;
    char str[5] = "";
    SDL_Color couleur = {230, 0, 0}, couleur2 = {0, 0, 230};
    SDL_Rect pos;

    pos.x = -1;
    pos.y = -1;

    for (i = 0 ; i < map.sizeX ; i++)
    {
        for (j = 0 ; j < map.sizeY ; j++)
        {

            if (sys->event.motion.x >= (LARGEUR_FENETRE - map.sizeY * elements[1]->w) / 2 + j * elements[1]->w
                && sys->event.motion.x < (LARGEUR_FENETRE - map.sizeY * elements[1]->w) / 2 + (j + 1) * elements[1]->w
                && sys->event.motion.y >= (HAUTEUR_FENETRE - map.sizeX * elements[1]->h) / 2 + i * elements[1]->h
                && sys->event.motion.y < (HAUTEUR_FENETRE - map.sizeX * elements[1]->h) / 2 + (i + 1) * elements[1]->h)
                {
                    pos.x = j;
                    pos.y = i;
                }

            if (map.map[i][j] >= LIMIT_PTS_MIN && map.map[i][j] <= LIMIT_PTS_MAX)
            {
                sprintf (str, "%d", map.map[i][j]);

                if (map.map[i][j] > 0)
                    txt = TTF_RenderText_Blended(police, str, couleur2);

                else
                    txt = TTF_RenderText_Blended(police, str, couleur);

                sys->position.x = (LARGEUR_FENETRE - map.sizeY * elements[1]->w) / 2 + j * elements[1]->w;
                sys->position.y = (HAUTEUR_FENETRE - map.sizeX * elements[1]->h) / 2 + i * elements[1]->h;
                SDL_BlitSurface(elements[1], NULL, sys->screen, &sys->position);

                if (map.map[i][j] != 0)
                {
                    sys->position.x = (LARGEUR_FENETRE - map.sizeY * elements[1]->w) / 2 + j * elements[1]->w + (elements[1]->w - txt->w) / 2;
                    sys->position.y = (HAUTEUR_FENETRE - map.sizeX * elements[1]->h) / 2 + i * elements[1]->h + (elements[1]->h - txt->h) / 2;
                    SDL_BlitSurface(txt, NULL, sys->screen, &sys->position);
                }


                SDL_FreeSurface(txt);
            }

            else if (map.map[i][j] < LIMIT_PTS_MIN) /// map elems
            {
                if (map.map[i][j] != LIMIT_PTS_MIN - 7)
                {
                    sys->position.x = (LARGEUR_FENETRE - map.sizeY * elements[1]->w) / 2 + j * elements[1]->w;
                    sys->position.y = (HAUTEUR_FENETRE - map.sizeX * elements[1]->h) / 2 + i * elements[1]->h;
                    SDL_BlitSurface(elements[1], NULL, sys->screen, &sys->position);

                     if(map.map[i][j] == LIMIT_PTS_MIN - 1)
                    {
                        sys->position.x = (LARGEUR_FENETRE - map.sizeY * elements[1]->w) / 2 + j * elements[1]->w + (elements[1]->w - elements[0]->w) / 2;
                        sys->position.y = (HAUTEUR_FENETRE - map.sizeX * elements[1]->h) / 2 + i * elements[1]->h + (elements[1]->h - elements[0]->h) / 2;
                        SDL_BlitSurface(elements[0], NULL, sys->screen, &sys->position);
                    }
                }

            }

            else /// bonus / malus
            {
                sys->position.x = (LARGEUR_FENETRE - map.sizeY * elements[1]->w) / 2 + j * elements[1]->w;
                sys->position.y = (HAUTEUR_FENETRE - map.sizeX * elements[1]->h) / 2 + i * elements[1]->h;
                SDL_BlitSurface(elements[1], NULL, sys->screen, &sys->position);

                sys->position.x = (LARGEUR_FENETRE - map.sizeY * elements[1]->w) / 2 + j * elements[1]->w + (elements[1]->w - elements[2 - LIMIT_PTS_MAX + map.map[i][j]]->w) / 2;
                sys->position.y = (HAUTEUR_FENETRE - map.sizeX * elements[1]->h) / 2 + i * elements[1]->h + (elements[1]->h - elements[2 - LIMIT_PTS_MAX + map.map[i][j]]->h) / 2;
                SDL_BlitSurface(elements[2 - LIMIT_PTS_MAX + map.map[i][j]], NULL, sys->screen, &sys->position);
            }
        }
    }

    return pos;
}

void loadElements(SDL_Surface *elements[9])
{
    int i;

    elements[0] = IMG_Load("Pictures/wall.bmp");
    elements[1] = IMG_Load("Pictures/case.bmp");
    elements[2] = IMG_Load("Pictures/X2.bmp");
    elements[3] = IMG_Load("Pictures/theft.bmp");
    elements[4] = IMG_Load("Pictures/pass.bmp");
    elements[5] = IMG_Load("Pictures/block.bmp");
    elements[6] = IMG_Load("Pictures/switch.bmp");
    elements[7] = IMG_Load("Pictures/hit_supp.bmp");
    elements[8] = IMG_Load("Pictures/switch_score.bmp");

    for (i = 0 ; i < 9 ; i++)
    {
        SDL_SetColorKey(elements[i], SDL_SRCCOLORKEY, SDL_MapRGB(elements[i]->format, 0, 255, 0));
    }
}
