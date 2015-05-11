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

/// DANS LE FUTUR, LUI FAIRE CHARGER DES FICHIERS

void loadMap(Sysvar *sys, Map *map, SDL_Surface **bg, int numMap)
{
    int i, j;
    char str[60] = "";
    FILE *fic = NULL;

    sprintf (str, "Maps/map%d.map", numMap);
    fic = fopen(str, "r");

    if (fic == NULL)
    {
        fic = fopen("Maps/map0.map", "r");
    }

    fgets(str, 60, fic);

    map->sizeX = atoi(str);

    fgets(str, 60, fic);

    map->sizeY = atoi(str);
    map->map = (int**) malloc(map->sizeX * sizeof(int*));

    fgets(str, 60, fic);
    str[strlen(str) - 1] = '\0';
    loadMusic(sys, str);
    map->music = (char*) malloc (100 * sizeof(char));
    sprintf (map->music, "%s", str);

    fgets(str, 60, fic);
    str[strlen(str) - 1] = '\0';

    *bg = IMG_Load(str);
    map->background = (char*) malloc (100 * sizeof(char));
    sprintf (map->background, "%s", str);

    for (i = 0 ; i < map->sizeX ; i++)
    {
        map->map[i] = (int*) malloc(map->sizeY * sizeof(int));

        for (j = 0 ; j < map->sizeY ; j++)
        {
            fscanf(fic, "%s", str);
            map->map[i][j] = atoi(str);
        }
    }

    fclose(fic);
}
