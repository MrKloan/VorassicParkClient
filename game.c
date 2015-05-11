#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fmod.h>

#include "sysvars.h"
#include "constantes.h"
#include "loadSound.h"
#include "mapStruct.h"
#include "game.h"

/**
 * Utilisé par loadMap().
 * Renvoie la sous-chaîne présente avant la première occurence de subject.
 * @param const char *str
 * @param const char *subject
 * @return char*
 */


 /*
char* strpart(const char *str, const char *subject)
{
    char *result = strstr(str, subject);

    if(result)
    {
        char *ret = (char*)malloc(4 * sizeof (char));
        strncpy(ret, str, strlen(str) - strlen(result));
        return ret;
    }
    else
        return NULL;
}

Map loadMap(const char* name)
{
    Map map;
    FILE *file;
    char path[50] = "Maps/";

    strcat(path, name);
    map.map = NULL;
    map.sizeY = 0;
    map.sizeX = 0;

    //Si le fichier existe
    if((file = fopen(path, "r")))
    {
        //char buffer[128];
        char *buffer = (char*)malloc(128*sizeof(char));
        short i,j;
        short flag = 0;

        //Enregistrement de la taille de la grille
        fgets(buffer, 128, file);
        map.sizeX = map.sizeY = atoi(buffer);

        map.map = (int**)malloc(map.sizeX*sizeof(int*));
        for(i=0 ; i < map.sizeY ; i++)
            map.map[i] = (int*)malloc(map.sizeY*sizeof(int));

        //Tant qu'il y a des lignes à lire
        i = 0;
        while(fgets(buffer, 128, file) != NULL)
        {
            char* part;

            printf ("%s\n", buffer);

            if(strcmp("begginMap\n", buffer) == 0)
                continue;
            else if(strcmp("endMap\n", buffer) == 0)
            {
                flag = i = 1;
                continue;
            }

            if(!flag)
            {
                int beg = 0, end;
                char str[30] = "";

                for(j=0 ; buffer[j] != '\n' ; j++)
                {
                    if (buffer[j] != ',')
                    {
                        int k;

                        end = j - 1;

                        for (k = 0 ; k < end - beg ; k++)
                        {
                            str[k] = buffer[beg + k];
                            str[k+1] = '\0';
                        }

                        beg = end + 1;

                        map.map[i][j] = atoi(str);
                    }
                }
            }
            else if(i == 1)
                map.background = buffer;
            else if(i == 2)
                map.music = buffer;

            i++;
        }

        free(buffer);
        fclose(file);
    }
    else
    {
        fprintf(stderr, "La map %s n'est pas présente sur le serveur.", path);
        exit(EXIT_FAILURE);
    }

    return map;
}
*/
void freeMap(Map *map)
{
    short i;

    for(i=0 ; i < map->sizeX ; i++)
        free(map->map[i]);
    free(map->map);

}
