#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <fmod/fmod.h>
#include <time.h>

#include "sysvars.h"
#include "constantes.h"
#include "mapStruct.h"
#include "gameParams.h"
#include "motor.h"
#include "tri.h"

int ia(int prof, GameParams *params, int errorMargin, int onTurn, int profBeg)
{
    int tab[4] = {-99999, -99999, -99999, -99999};
    int tab2[4] = {-99999, -99999, -99999, -99999};
    int senseX, senseY;
    int x[4] = { -1, -1, -1, -1}, y[4] = { -1, -1, -1, -1};
    int x1_2[4] = { -1, -1, -1, -1}, y1_2[4] = { -1, -1, -1, -1};
    int x2[4] = { -1, -1, -1, -1}, y2[4] = { -1, -1, -1, -1};
    int x2_2[4] = { -1, -1, -1, -1}, y2_2[4] = { -1, -1, -1, -1};
    int i;
    int playing;
    GameParams pTemp;
    int j, k;
    int l, m; /// FOR TRIPLE LOOP OF OCTOPUS MODE
    char c;

    printf ("Calling ia (prof = %d / %d)\n", prof, profBeg);

    if (prof == 0)
    {
        return -99999; /// SIGNAL D'ARRET
    }

    else
    {
        params->playing = (params->playing % (params->nbJoueurs + params->nbBot)) + 1;

        for (playing = 0 ; playing < params->nbBot + params->nbJoueurs ; playing++)
        {

            if (canPlay(params->playing, params->positions, params->lastPos, params->map.sizeX, params->map.sizeY , params->mode))
            {
                if (params->mode == 0) /// SNAKE
                {

                    for (i = 0 ; i < 4 ; i++)
                    {
                        switch (i)
                        {
                            case 0:/// HAUT
                                senseX = 0;
                                senseY = -1;
                                break;

                            case 1: /// BAS
                                senseX = 0;
                                senseY = 1;
                                break;

                            case 2: /// DROITE
                                senseX = 1;
                                senseY = 0;
                                break;

                            case 3: /// GAUCHE
                                senseX = -1;
                                senseY = 0;
                                break;
                        }

                        if (canPlace(params->playing, params->positions, params->lastPos, params->map.sizeX, params->map.sizeY, params->mode, params->lastPos[params->playing - 1][1] + senseY, params->lastPos[params->playing - 1][0] + senseX))
                        {

                            if (tab[i] == -99999)
                                tab[i] = 0;

                            if (params->map.map[params->lastPos[params->playing - 1][0] + senseX][params->lastPos[params->playing - 1][1] + senseY] >= LIMIT_PTS_MIN &&  params->map.map[params->lastPos[params->playing - 1][0] + senseX][params->lastPos[params->playing - 1][1] + senseY] <= LIMIT_PTS_MAX )
                            {
                                if (params->playing == onTurn)
                                {

                                    x[i] = params->lastPos[params->playing - 1][0] + senseX;
                                    y[i] = params->lastPos[params->playing - 1][1] + senseY;

                                    /// ACTUALISATION DES PARAMS DE JEU
                                    //pTemp = *params;
                                    pTemp.nbJoueurs = params->nbJoueurs; //1-4
                                    pTemp.mode = params->mode; //0 = octopus, 1 = snake
                                    //char* netmap; //Serialized map

                                    //Game values
                                    pTemp.pass = params->pass; // compteur de tours passés quand le joueur ne peut pas jouer
                                    pTemp.playing = params->playing; // joueur en train de jouer

                                    pTemp.map.sizeX = params->map.sizeX;
                                    pTemp.map.sizeY = params->map.sizeY;

                                    pTemp.map.map = (int**)malloc(pTemp.map.sizeX*sizeof(int*));

                                    //Initialisation des scores à 0 et allocation des positions
                                    for(j=0 ; j < pTemp.map.sizeX ; j++)
                                    {
                                        pTemp.map.map[j] = (int*)malloc(pTemp.map.sizeY*sizeof(int));
                                        for (k = 0 ; k < pTemp.map.sizeY ; k++)
                                        {
                                            pTemp.map.map[j][k] = params->map.map[j][k];
                                        }
                                    }

                                    // Runtime Values
                                    pTemp.nbBot = params->nbBot;
                                    pTemp.extensionPack = params->extensionPack;
                                    pTemp.nbMap = params->nbMap;
                                    pTemp.isNetwork = params->isNetwork;

                                    //Allocation des grilles
                                    pTemp.positions = (char**)malloc(pTemp.map.sizeX*sizeof(char*));
                                    pTemp.lastPos = (short**)malloc((pTemp.nbJoueurs + pTemp.nbBot)*sizeof(short*));

                                    //Initialisation des scores à 0 et allocation des positions
                                    for(j=0 ; j < pTemp.nbJoueurs + pTemp.nbBot ; j++)
                                    {
                                        pTemp.lastPos[j] = (short*)malloc(2*sizeof(short)); // 2 -> row, col
                                        for (k = 0 ; k < 2 ; k++)
                                        {
                                            pTemp.lastPos[j][k] = params->lastPos[j][k];
                                        }
                                    }

                                    for(j=0 ; j < pTemp.map.sizeX ; j++)
                                    {
                                        pTemp.positions[j] = (char*)malloc(pTemp.map.sizeY*sizeof(char));


                                        for(k=0 ; k < pTemp.map.sizeY ; k++)
                                        {
                                            pTemp.positions[j][k] = params->positions[j][k];
                                        }

                                    }

                                    /// PLACE HERE
                                    pTemp.positions[x[i]][y[i]] = params->playing == 1 ? 'r' : params->playing == 2 ? 'j' : params->playing == 3 ? 'v' : params->playing == 4 ? 'b' : '\0';

                                    if (params->map.map[x[i]][y[i]] <= LIMIT_PTS_MAX && params->map.map[x[i]][y[i]] >= LIMIT_PTS_MIN) /// SI CASE A POINTS
                                    {
                                        tab[i] += params->map.map[params->lastPos[params->playing - 1][0] + senseX][params->lastPos[params->playing - 1][1] + senseY];
                                    }

                                    pTemp.map.map[x[i]][y[i]] = 0;
                                    pTemp.lastPos[params->playing-1][0] = x[i];
                                    pTemp.lastPos[params->playing-1][1] = y[i];

                                    tab[i] += ia (prof-1, &pTemp, errorMargin, onTurn, profBeg);

                                    for (j = 0 ; j < 2 ; j++)
                                        free(pTemp.lastPos[j]);

                                    free(pTemp.lastPos);

                                    for (j = 0 ; j < pTemp.map.sizeX ; j++)
                                    {
                                        free(pTemp.positions[j]);
                                        free(pTemp.map.map[j]);
                                    }

                                    free(pTemp.positions);
                                    free(pTemp.map.map);
                                }

                                else
                                {
                                    x2[i] = params->lastPos[params->playing - 1][0] + senseX;
                                    y2[i] = params->lastPos[params->playing - 1][1] + senseY;

                                    /// ACTUALISATION DES PARAMS DE JEU
                                    //pTemp = *params;
                                    pTemp.nbJoueurs = params->nbJoueurs; //1-4
                                    pTemp.mode = params->mode; //0 = octopus, 1 = snake
                                    //char* netmap; //Serialized map

                                    //Game values
                                    pTemp.pass = params->pass; // compteur de tours passés quand le joueur ne peut pas jouer
                                    pTemp.playing = params->playing; // joueur en train de jouer

                                    pTemp.map.sizeX = params->map.sizeX;
                                    pTemp.map.sizeY = params->map.sizeY;

                                    pTemp.map.map = (int**)malloc(pTemp.map.sizeX*sizeof(int*));

                                    //Initialisation des scores à 0 et allocation des positions
                                    for(j=0 ; j < pTemp.map.sizeX ; j++)
                                    {
                                        pTemp.map.map[j] = (int*)malloc(pTemp.map.sizeY*sizeof(int));
                                        for (k = 0 ; k < pTemp.map.sizeY ; k++)
                                        {
                                            pTemp.map.map[j][k] = params->map.map[j][k];
                                        }
                                    }

                                    // Runtime Values
                                    pTemp.nbBot = params->nbBot;
                                    pTemp.extensionPack = params->extensionPack;
                                    pTemp.nbMap = params->nbMap;
                                    pTemp.isNetwork = params->isNetwork;

                                    //Allocation des grilles
                                    pTemp.positions = (char**)malloc(pTemp.map.sizeX*sizeof(char*));
                                    pTemp.lastPos = (short**)malloc((pTemp.nbJoueurs + pTemp.nbBot)*sizeof(short*));

                                    //Initialisation des scores à 0 et allocation des positions
                                    for(j=0 ; j < pTemp.nbJoueurs + pTemp.nbBot ; j++)
                                    {
                                        pTemp.lastPos[j] = (short*)malloc(2*sizeof(short)); // 2 -> row, col
                                        for (k = 0 ; k < 2 ; k++)
                                        {
                                            pTemp.lastPos[j][k] = params->lastPos[j][k];
                                        }
                                    }

                                    for(j=0 ; j < pTemp.map.sizeX ; j++)
                                    {
                                        pTemp.positions[j] = (char*)malloc(pTemp.map.sizeY*sizeof(char));

                                        for(k=0 ; k < pTemp.map.sizeY ; k++)
                                        {

                                            pTemp.positions[j][k] = params->positions[j][k];
                                        }

                                    }

                                    /// PLACE HERE
                                    pTemp.positions[x2[i]][y2[i]] = params->playing == 1 ? 'r' : params->playing == 2 ? 'j' : params->playing == 3 ? 'v' : params->playing == 4 ? 'b' : '\0';

                                    if (params->map.map[x2[i]][y2[i]] <= LIMIT_PTS_MAX && params->map.map[x2[i]][y2[i]] >= LIMIT_PTS_MIN) /// SI CASE A POINTS
                                    {
                                        tab[i] -= params->map.map[params->lastPos[params->playing - 1][0] + senseX][params->lastPos[params->playing - 1][1] + senseY] / (pTemp.nbBot + pTemp.nbJoueurs - 1);
                                    }

                                    pTemp.map.map[x2[i]][y2[i]] = 0;
                                    pTemp.lastPos[params->playing-1][0] = x2[i];
                                    pTemp.lastPos[params->playing-1][1] = y2[i];

                                    for (j = 0 ; j < 2 ; j++)
                                        free(pTemp.lastPos[j]);

                                    free(pTemp.lastPos);

                                    for (j = 0 ; j < pTemp.map.sizeX ; j++)
                                    {
                                        free(pTemp.positions[j]);
                                        free(pTemp.map.map[j]);
                                    }

                                    free(pTemp.positions);
                                    free(pTemp.map.map);
                                }

                            }
                        }
                    }
                }

                else /// OCTOPUS
                {
                    c = params->playing == 1 ? 'r' : params->playing == 2 ? 'j' : params->playing == 3 ? 'v' : params->playing == 4 ? 'b' : '\0';

                    for (l = 0 ; l < params->map.sizeX ; l++)
                    {
                        for (m = 0 ; m < params->map.sizeY ; m++)
                        {

                            if (params->map.map[l][m] == c)
                            {

                                for (i = 0 ; i < 4 ; i++)
                                {
                                    switch (i)
                                    {
                                        case 0:/// HAUT
                                            senseX = 0;
                                            senseY = -1;
                                            break;

                                        case 1: /// BAS
                                            senseX = 0;
                                            senseY = 1;
                                            break;

                                        case 2: /// DROITE
                                            senseX = 1;
                                            senseY = 0;
                                            break;

                                        case 3: /// GAUCHE
                                            senseX = -1;
                                            senseY = 0;
                                            break;
                                    }

                                    //tab2[i] = -99999;

                                    if (canPlace(params->playing, params->positions, params->lastPos, params->map.sizeX, params->map.sizeY, params->mode, params->positions[m][l] + senseY, params->lastPos[m][l] + senseX))
                                    {

                                        if (tab2[i] == -99999)
                                            tab2[i] = 0;

                                        if (params->map.map[params->lastPos[params->playing - 1][0] + senseX][params->lastPos[params->playing - 1][1] + senseY] >= LIMIT_PTS_MIN &&  params->map.map[params->lastPos[params->playing - 1][0] + senseX][params->lastPos[params->playing - 1][1] + senseY] <= LIMIT_PTS_MAX )
                                        {
                                            if (params->playing == onTurn)
                                            {

                                                x1_2[i] = params->lastPos[params->playing - 1][0] + senseX;
                                                y1_2[i] = params->lastPos[params->playing - 1][1] + senseY;

                                                /// ACTUALISATION DES PARAMS DE JEU
                                                //pTemp = *params;
                                                pTemp.nbJoueurs = params->nbJoueurs; //1-4
                                                pTemp.mode = params->mode; //0 = octopus, 1 = snake
                                                //char* netmap; //Serialized map

                                                //Game values
                                                pTemp.pass = params->pass; // compteur de tours passés quand le joueur ne peut pas jouer
                                                pTemp.playing = params->playing; // joueur en train de jouer

                                                pTemp.map.sizeX = params->map.sizeX;
                                                pTemp.map.sizeY = params->map.sizeY;

                                                pTemp.map.map = (int**)malloc(pTemp.map.sizeX*sizeof(int*));

                                                //Initialisation des scores à 0 et allocation des positions
                                                for(j=0 ; j < pTemp.map.sizeX ; j++)
                                                {
                                                    pTemp.map.map[j] = (int*)malloc(pTemp.map.sizeY*sizeof(int));
                                                    for (k = 0 ; k < pTemp.map.sizeY ; k++)
                                                    {
                                                        pTemp.map.map[j][k] = params->map.map[j][k];
                                                    }
                                                }

                                                // Runtime Values
                                                pTemp.nbBot = params->nbBot;
                                                pTemp.extensionPack = params->extensionPack;
                                                pTemp.nbMap = params->nbMap;
                                                pTemp.isNetwork = params->isNetwork;

                                                //Allocation des grilles
                                                pTemp.positions = (char**)malloc(pTemp.map.sizeX*sizeof(char*));
                                                pTemp.lastPos = (short**)malloc((pTemp.nbJoueurs + pTemp.nbBot)*sizeof(short*));

                                                //Initialisation des scores à 0 et allocation des positions
                                                for(j=0 ; j < pTemp.nbJoueurs + pTemp.nbBot ; j++)
                                                {
                                                    pTemp.lastPos[j] = (short*)malloc(2*sizeof(short)); // 2 -> row, col
                                                    for (k = 0 ; k < 2 ; k++)
                                                    {
                                                        pTemp.lastPos[j][k] = params->lastPos[j][k];
                                                    }
                                                }

                                                for(j=0 ; j < pTemp.map.sizeX ; j++)
                                                {
                                                    pTemp.positions[j] = (char*)malloc(pTemp.map.sizeY*sizeof(char));


                                                    for(k=0 ; k < pTemp.map.sizeY ; k++)
                                                    {
                                                        pTemp.positions[j][k] = params->positions[j][k];
                                                    }

                                                }

                                                /// PLACE HERE
                                                pTemp.positions[x1_2[i]][y1_2[i]] = params->playing == 1 ? 'r' : params->playing == 2 ? 'j' : params->playing == 3 ? 'v' : params->playing == 4 ? 'b' : '\0';

                                                if (params->map.map[x1_2[i]][y1_2[i]] <= LIMIT_PTS_MAX && params->map.map[x1_2[i]][y1_2[i]] >= LIMIT_PTS_MIN) /// SI CASE A POINTS
                                                {
                                                    tab2[i] += params->map.map[params->lastPos[params->playing - 1][0] + senseX][params->lastPos[params->playing - 1][1] + senseY];
                                                }

                                                pTemp.map.map[x1_2[i]][y1_2[i]] = 0;
                                                pTemp.lastPos[params->playing-1][0] = x1_2[i];
                                                pTemp.lastPos[params->playing-1][1] = y1_2[i];

                                                tab2[i] += ia (prof-1, &pTemp, errorMargin, onTurn, profBeg);

                                                for (j = 0 ; j < 2 ; j++)
                                                    free(pTemp.lastPos[j]);

                                                free(pTemp.lastPos);

                                                for (j = 0 ; j < pTemp.map.sizeX ; j++)
                                                {
                                                    free(pTemp.positions[j]);
                                                    free(pTemp.map.map[j]);
                                                }

                                                free(pTemp.positions);
                                                free(pTemp.map.map);
                                            }

                                            else
                                            {
                                                x2_2[i] = params->lastPos[params->playing - 1][0] + senseX;
                                                y2_2[i] = params->lastPos[params->playing - 1][1] + senseY;

                                                /// ACTUALISATION DES PARAMS DE JEU
                                                //pTemp = *params;
                                                pTemp.nbJoueurs = params->nbJoueurs; //1-4
                                                pTemp.mode = params->mode; //0 = octopus, 1 = snake
                                                //char* netmap; //Serialized map

                                                //Game values
                                                pTemp.pass = params->pass; // compteur de tours passés quand le joueur ne peut pas jouer
                                                pTemp.playing = params->playing; // joueur en train de jouer

                                                pTemp.map.sizeX = params->map.sizeX;
                                                pTemp.map.sizeY = params->map.sizeY;

                                                pTemp.map.map = (int**)malloc(pTemp.map.sizeX*sizeof(int*));

                                                //Initialisation des scores à 0 et allocation des positions
                                                for(j=0 ; j < pTemp.map.sizeX ; j++)
                                                {
                                                    pTemp.map.map[j] = (int*)malloc(pTemp.map.sizeY*sizeof(int));
                                                    for (k = 0 ; k < pTemp.map.sizeY ; k++)
                                                    {
                                                        pTemp.map.map[j][k] = params->map.map[j][k];
                                                    }
                                                }

                                                // Runtime Values
                                                pTemp.nbBot = params->nbBot;
                                                pTemp.extensionPack = params->extensionPack;
                                                pTemp.nbMap = params->nbMap;
                                                pTemp.isNetwork = params->isNetwork;

                                                //Allocation des grilles
                                                pTemp.positions = (char**)malloc(pTemp.map.sizeX*sizeof(char*));
                                                pTemp.lastPos = (short**)malloc((pTemp.nbJoueurs + pTemp.nbBot)*sizeof(short*));

                                                //Initialisation des scores à 0 et allocation des positions
                                                for(j=0 ; j < pTemp.nbJoueurs + pTemp.nbBot ; j++)
                                                {
                                                    pTemp.lastPos[j] = (short*)malloc(2*sizeof(short)); // 2 -> row, col
                                                    for (k = 0 ; k < 2 ; k++)
                                                    {
                                                        pTemp.lastPos[j][k] = params->lastPos[j][k];
                                                    }
                                                }

                                                for(j=0 ; j < pTemp.map.sizeX ; j++)
                                                {
                                                    pTemp.positions[j] = (char*)malloc(pTemp.map.sizeY*sizeof(char));

                                                    for(k=0 ; k < pTemp.map.sizeY ; k++)
                                                    {

                                                        pTemp.positions[j][k] = params->positions[j][k];
                                                    }

                                                }

                                                /// PLACE HERE
                                                pTemp.positions[x2_2[i]][y2_2[i]] = params->playing == 1 ? 'r' : params->playing == 2 ? 'j' : params->playing == 3 ? 'v' : params->playing == 4 ? 'b' : '\0';

                                                if (params->map.map[x2_2[i]][y2_2[i]] <= LIMIT_PTS_MAX && params->map.map[x2_2[i]][y2_2[i]] >= LIMIT_PTS_MIN) /// SI CASE A POINTS
                                                {
                                                    tab2[i] -= params->map.map[params->lastPos[params->playing - 1][0] + senseX][params->lastPos[params->playing - 1][1] + senseY] / (pTemp.nbBot + pTemp.nbJoueurs - 1);
                                                }

                                                pTemp.map.map[x2_2[i]][y2_2[i]] = 0;
                                                pTemp.lastPos[params->playing-1][0] = x2_2[i];
                                                pTemp.lastPos[params->playing-1][1] = y2_2[i];

                                                for (j = 0 ; j < 2 ; j++)
                                                    free(pTemp.lastPos[j]);

                                                free(pTemp.lastPos);

                                                for (j = 0 ; j < pTemp.map.sizeX ; j++)
                                                {
                                                    free(pTemp.positions[j]);
                                                    free(pTemp.map.map[j]);
                                                }

                                                free(pTemp.positions);
                                                free(pTemp.map.map);
                                            }

                                        }
                                    }
                                }
                            }

                            for(i = 0 ; i < 4 ; i++)
                            {
                                if (tab2[i] >= tab[3])
                                {
                                    tab[3] = tab2[i];
                                    x[3] = x1_2[i];
                                    y[3] = y1_2[i];
                                    tribub(tab, x, y);
                                }
                            }

                        }
                    }
                }
            }

            params->playing = (params->playing % (params->nbJoueurs + params->nbBot)) + 1;
        }

        if (prof != profBeg)
        {
            playing = 0;

            for (i = 1 ; i < 4 ; i++) /// MAX
            {
                if (tab[playing] < tab[i])
                    playing = i;
            }

            return tab[playing];
        }

        else
        {
            /*playing = 0;

            for (i = 1 ; i < 4 ; i++) /// MAX
            {
                if (tab[playing] < tab[i])
                    playing = i;
            }
            /// PLACE HERE
            printf ("playing = %d\n", playing);
            printf ("tab = { %d, %d, %d, %d }\n", tab[0], tab[1], tab[2], tab[3]);
            printf ("x[playing] = %d\n", x[playing]);
            printf ("y[playing] = %d\n\n", y[playing]); */

            k = rand() % 1000;

            tribub(tab, x, y);

            if (k < 1000 - errorMargin)
            {
                playing = 0;
            }

            else if (k < 1000 - errorMargin + 3 * (1000 - errorMargin) / 4)
            {
                playing = 1;

                if(x[playing] == -1 || y[playing] == -1)
                {
                    playing = 0;
                }
            }

            else if (k < 1000 - errorMargin + 3 * (1000 - errorMargin) / 4 + 9 * (1000 - errorMargin) / 16)
            {
                playing = 2;

                if(x[playing] == -1 || y[playing] == -1)
                {
                    playing = 1;
                }

                if(x[playing] == -1 || y[playing] == -1)
                {
                    playing = 0;
                }
            }

            else
            {
                playing = 3;

                if(x[playing] == -1 || y[playing] == -1)
                {
                    playing = 2;
                }

                if(x[playing] == -1 || y[playing] == -1)
                {
                    playing = 1;
                }

                if(x[playing] == -1 || y[playing] == -1)
                {
                    playing = 0;
                }
            }

            if (x[playing] != -1 || y[playing] != -1)
            {
                params->positions[x[playing]][y[playing]] = onTurn == 1 ? 'r' : onTurn == 2 ? 'j' : onTurn == 3 ? 'v' : onTurn == 4 ? 'b' : '\0';

                if (params->map.map[x[playing]][y[playing]] <= LIMIT_PTS_MAX && params->map.map[x[playing]][y[playing]] >= LIMIT_PTS_MIN) /// SI CASE A POINTS
                {
                    params->scores[onTurn-1] += params->map.map[x[playing]][y[playing]];
                }

                params->map.map[x[playing]][y[playing]] = 0;
                params->lastPos[onTurn-1][0] = x[playing];
                params->lastPos[onTurn-1][1] = y[playing];

                params->playing = (onTurn % (params->nbJoueurs + params->nbBot)) + 1;

                return -99999;
            }

            params->playing = (onTurn % (params->nbJoueurs + params->nbBot)) + 1;

            return tab[playing];
        }
    }

    /*int i, j, k, l;
    GameParams pTemp;
    int values[4] = {-99999, -99999, -99999, -99999};
    int out, temp, temp2, temp3, temp4;
    int senseX, senseY;
    char c;
    int x[4] = {-1, -1, -1, -1}, y[4] = {-1, -1, -1, -1};
    int tempx, tempy;

    if (prof == 0)
        return 0;

    switch (params->playing)
    {
        case 1:
            c = 'r';
            break;

        case 2:
            c = 'j';
            break;

        case 3:
            c = 'v';
            break;

        case 4:
            c = 'b';
            break;
    }

    if (params->mode != 1) /// SNAKE
    {
        for (i = 0 ; i < 4 ; i++)
        {
            switch(i)
            {
                case 0: /// HAUT
                    senseX = 0;
                    senseY = -1;
                    break;

                case 1: /// BAS
                    senseX = 0;
                    senseY = 1;
                    break;

                case 2: /// DROITE
                    senseX = 1;
                    senseY = 0;
                    break;

                case 3: /// GAUCHE
                    senseX = -1;
                    senseY = 0;
                    break;
            }

            if (canPlay(params->playing, params->positions, params->lastPos, params->map.sizeX, params->map.sizeY , params->mode)
                && canPlace(params->playing, params->positions, params->lastPos, params->map.sizeX, params->map.sizeY, params->mode, params->lastPos[params->playing - 1][1] + senseY, params->lastPos[params->playing - 1][0] + senseX)
                && params->map.map[params->lastPos[params->playing - 1][0] + senseX] [params->lastPos[params->playing - 1][1] + senseY] != -36)
            {

                values[i] = params->map.map[params->lastPos[params->playing - 1][0] + senseX] [params->lastPos[params->playing - 1][1] + senseY];

                /// ACTUALISATION DES PARAMS DE JEU
                //pTemp = *params;
                pTemp.nbJoueurs = params->nbJoueurs; //1-4
                pTemp.mode = params->mode; //0 = octopus, 1 = snake
                //char* netmap; //Serialized map

                //Game values
                pTemp.pass = params->pass; // compteur de tours passés quand le joueur ne peut pas jouer
                pTemp.playing = params->playing; // joueur en train de jouer
                pTemp.map = params->map;

                // Runtime Values
                pTemp.nbBot = params->nbBot;
                pTemp.extensionPack = params->extensionPack;
                pTemp.nbMap = params->nbMap;
                pTemp.isNetwork = params->isNetwork;

                //Allocation des grilles
                pTemp.scores = (short*)malloc((pTemp.nbJoueurs + pTemp.nbBot)*sizeof(short));
                pTemp.positions = (char**)malloc(pTemp.map.sizeX*sizeof(char*));
                pTemp.lastPos = (short**)malloc((pTemp.nbJoueurs + pTemp.nbBot)*sizeof(short*));

                //Initialisation des scores à 0 et allocation des positions
                for(j=0 ; j < pTemp.nbJoueurs + pTemp.nbBot ; j++)
                {
                    pTemp.scores[j] = params->scores[j];
                    pTemp.lastPos[j] = (short*)malloc(2*sizeof(short)); // 2 -> row, col
                    for (k = 0 ; k < 2 ; k++)
                    {
                        pTemp.lastPos[j][k] = params->lastPos[j][k];
                    }
                }

                for(j=0 ; j < pTemp.map.sizeX ; j++)
                {
                    pTemp.positions[j] = (char*)malloc(pTemp.map.sizeX*sizeof(char));


                    for(k=0 ; k < pTemp.map.sizeY ; k++)
                    {

                        pTemp.positions[j][k] = params->positions[j][k];
                    }

                }

                pTemp.lastPos[params->playing - 1][0] += senseX;
                pTemp.lastPos[params->playing - 1][1] += senseY;

                switch (params->playing)
                {
                    case 1:
                        pTemp.positions[pTemp.lastPos[params->playing - 1][0]][pTemp.lastPos[params->playing - 1][1]] = 'r';
                        if (prof == profBeg)
                        {
                            x[i] = pTemp.lastPos[params->playing - 1][0];
                            y[i] = pTemp.lastPos[params->playing - 1][1];
                        }
                        break;

                    case 2:
                        pTemp.positions[pTemp.lastPos[params->playing - 1][0]][pTemp.lastPos[params->playing - 1][1]] = 'j';
                        if (prof == profBeg)
                        {
                            x[i] = pTemp.lastPos[params->playing - 1][0];
                            y[i] = pTemp.lastPos[params->playing - 1][1];
                        }
                        break;

                    case 3:
                        pTemp.positions[pTemp.lastPos[params->playing - 1][0]][pTemp.lastPos[params->playing - 1][1]] = 'v';
                        if (prof == profBeg)
                        {
                            x[i] = pTemp.lastPos[params->playing - 1][0];
                            y[i] = pTemp.lastPos[params->playing - 1][1];
                        }
                        break;

                    case 4:
                        pTemp.positions[pTemp.lastPos[params->playing - 1][0]][pTemp.lastPos[params->playing - 1][1]] = 'b';
                        if (prof == profBeg)
                        {
                            x[i] = pTemp.lastPos[params->playing - 1][0];
                            y[i] = pTemp.lastPos[params->playing - 1][1];
                        }
                        break;
                }

                pTemp.playing = (pTemp.playing % (params->nbJoueurs + params->nbBot)) + 1; /// EVITE LE CALCUL DES ABSENTS QUI RENVERRA 0 A COUP SUR

                values[i] += ia(prof - 1, &pTemp, errorMargin, onTurn, profBeg);

                free(pTemp.scores);

                for (i = 0 ; i < 2 ; i++)
                    free(pTemp.lastPos[i]);

                free(pTemp.lastPos);

                for (i = 0 ; i < pTemp.map.sizeX ; i++)
                {
                    free(pTemp.positions[i]);
                }

            }
        }

        /// TRI A BULLES
        tribub(values, x, y);

        /// OPPOSANT = NEGATIF
        for (i = 0 ; i < 4 && (params->playing == onTurn || params->playing == onTurn % (params->nbJoueurs + params->nbBot) + 1) ; i++)
        {
            values[i] = -(values[i] / (params->nbJoueurs + params->nbBot - 1)); /// MOYENNE POUR LES ADVERSAIRES
        }
    }

    else /// OCTOPUS
    {

        for(i=0 ; i < params->map.sizeX ; i++)
            {
                for(j=0 ; j < params->map.sizeY ; j++)
                {
                    //Pour chaque case appartenant à ce joueur...
                    if(params->positions[i][j] == c)
                    {
                        //... on vérifie que l'une de ses adjacentes est vide. Si oui, renvoie vrai.
                        if((i <= params->map.sizeX-2 && params->positions[i+1][j] == '\0')
                        || (i > 0 && params->positions[i-1][j] == '\0')
                        || (j <= params->map.sizeX-2 && params->positions[i][j+1] == '\0')
                        || (j > 0 && params->positions[i][j-1] == '\0')
                        || (i <= params->map.sizeX-2 && j <= params->map.sizeY-2 && params->positions[i+1][j+1] == '\0')
                        || (i <= params->map.sizeX-2 && j > 0 && params->positions[i+1][j-1] == '\0')
                        || (i > 0 && j > 0 && params->positions[i-1][j-1] == '\0')
                        || (i > 0 && j <= params->map.sizeX-2 && params->positions[i-1][j+1] == '\0'))
                        {
                            for (k = 0 ; k < 4 ; k++)
                            {
                                switch(k)
                                {
                                    case 0: /// HAUT
                                        senseX = 0;
                                        senseY = -1;
                                        break;

                                    case 1: /// BAS
                                        senseX = 0;
                                        senseY = 1;
                                        break;

                                    case 2: /// DROITE
                                        senseX = 1;
                                        senseY = 0;
                                        break;

                                    case 3: /// GAUCHE
                                        senseX = -1;
                                        senseY = 0;
                                        break;
                                }

                                if (canPlay(params->playing, params->positions, params->lastPos, params->map.sizeX, params->map.sizeY , params->mode)
                                    && canPlace(params->playing, params->positions, params->lastPos, params->map.sizeX, params->map.sizeY, params->mode, i + senseX, j + senseY)
                                    && params->map.map[i + senseX][j + senseY] != -36)
                                {
                                    temp = params->map.map[params->lastPos[params->playing - 1][0] + senseX] [params->lastPos[params->playing - 1][1] + senseY];

                                    /// ACTUALISATION DES PARAMS DE JEU
                                    pTemp = *params;

                                    pTemp.lastPos[params->playing - 1][0] = i + senseX;
                                    pTemp.lastPos[params->playing - 1][1] = j + senseY;

                                    switch (params->playing)
                                    {
                                        case 1:
                                            pTemp.positions[pTemp.lastPos[params->playing - 1][0]][pTemp.lastPos[params->playing - 1][1]] = 'r';
                                            break;

                                        case 2:
                                            pTemp.positions[pTemp.lastPos[params->playing - 1][0]][pTemp.lastPos[params->playing - 1][1]] = 'j';
                                            break;

                                        case 3:
                                            pTemp.positions[pTemp.lastPos[params->playing - 1][0]][pTemp.lastPos[params->playing - 1][1]] = 'v';
                                            break;

                                        case 4:
                                            pTemp.positions[pTemp.lastPos[params->playing - 1][0]][pTemp.lastPos[params->playing - 1][1]] = 'b';
                                            break;
                                    }

                                    pTemp.playing = (pTemp.playing % (params->nbJoueurs + params->nbBot)) + 1; /// EVITE LE CALCUL DES ABSENTS QUI RENVERRA 0 A COUP SUR

                                    temp += ia(prof - 1, &pTemp, errorMargin, onTurn, profBeg);

                                    tempx = pTemp.lastPos[params->playing - 1][0];
                                    tempy = pTemp.lastPos[params->playing - 1][1];

                                    for (l = 0 ; l < 4 ; l++)
                                    {
                                        if (temp > values[l])
                                        {
                                            temp2 = values[l];
                                            values[l] = temp;
                                            temp = temp2;
                                            temp3 = x[l];
                                            x[l] = tempx;
                                            tempx = temp3;
                                            temp4 = y[l];
                                            y[l] = tempy;
                                            tempy = temp4;

                                        }
                                    }

                                }
                            }
                        }
                    }
                }
            }
    }

    /// GET DE LA MARGE D'ERREUR
    out = rand() % 100;

    printf ("%d laps remaining\n", profBeg - prof);
    printf ("values : %d %d %d %d\n", values[0], values[1], values[2], values[3]);
    printf ("x : %d %d %d %d\n", x[0], x[1], x[2], x[3]);
    printf ("y : %d %d %d %d\n\n", y[0], y[1], y[2], y[3]);

    if (prof == profBeg)
    {
        if (out < errorMargin && x[0] != -1 && y[0] != -1)
        {
            params->positions[x[0]][y[0]] = c;
            params->lastPos[params->playing - 1][0] = x[0];
            params->lastPos[params->playing - 1][1] = y[0];
        }

        else if (out < errorMargin + 3 * (100 - errorMargin) / 4 && x[0] != -1 && y[0] != -1)
        {
            if (x[1] != -1 && y[1] != -1)
            {
                params->positions[x[1]][y[1]] = c;
                params->lastPos[params->playing - 1][0] = x[1];
                params->lastPos[params->playing - 1][1] = y[1];
            }


            else
            {
                params->positions[x[0]][y[0]] = c;
                params->lastPos[params->playing - 1][0] = x[0];
                params->lastPos[params->playing - 1][1] = y[0];
            }
        }

        else if (out < errorMargin + 9 * (100 - errorMargin) / 16 && x[0] != -1 && y[0] != -1)
        {

            if (x[2] != -1 && y[2] != -1)
            {
                params->positions[x[2]][y[2]] = c;
                params->lastPos[params->playing - 1][0] = x[2];
                params->lastPos[params->playing - 1][1] = y[2];
            }

            else if (x[1] != -1 && y[1] != -1)
            {
                params->positions[x[1]][y[1]] = c;
                params->lastPos[params->playing - 1][0] = x[1];
                params->lastPos[params->playing - 1][1] = y[1];
            }

            else
            {
                params->positions[x[0]][y[0]] = c;
                params->lastPos[params->playing - 1][0] = x[0];
                params->lastPos[params->playing - 1][1] = y[0];
            }
        }

        else if(x[0] != -1 && y[0] != -1)
        {
            if (x[3] != -1 && y[3] != -1)
            {
                params->positions[x[3]][y[3]] = c;
                params->lastPos[params->playing - 1][0] = x[3];
                params->lastPos[params->playing - 1][1] = y[3];
            }

            else if (x[2] != -1 && y[2] != -1)
            {
                params->positions[x[2]][y[2]] = c;
                params->lastPos[params->playing - 1][0] = x[2];
                params->lastPos[params->playing - 1][1] = y[2];
            }

            else if (x[1] != -1 && y[1] != -1)
            {
                params->positions[x[1]][y[1]] = c;
                params->lastPos[params->playing - 1][0] = x[1];
                params->lastPos[params->playing - 1][1] = y[1];
            }

            else
            {
                params->positions[x[0]][y[0]] = c;
                params->lastPos[params->playing - 1][0] = x[0];
                params->lastPos[params->playing - 1][1] = y[0];
            }
        }

        if (params->map.map[params->lastPos[params->playing - 1][0]][params->lastPos[params->playing - 1][1]] <= LIMIT_PTS_MAX && params->map.map[params->lastPos[params->playing - 1][0]][params->lastPos[params->playing - 1][1]] >= LIMIT_PTS_MIN) /// SI CASE A POINTS
        {
            params->scores[params->playing-1] += params->map.map[params->lastPos[params->playing - 1][0]][params->lastPos[params->playing - 1][1]];
        }

        return -1; /// AUCUNE SIGNIFICATION PRECISE
    }

    if (out < errorMargin)
    {
        return values[0];
    }

    else if (out < errorMargin + 3 * (100 - errorMargin) / 4)
    {
        if (values[1] != -99999 && values[1] != 99999)
            return values[1];

        return values[0];
    }

    else if (out < errorMargin + 9 * (100 - errorMargin) / 16)
    {
        if (values[2] != -99999 && values[2] != -99999)
            return values[2];

        else if (values[1] != -99999 && values[1] != 99999)
            return values[1];

        return values[0];
    }

    else
    {
        if (values[3] != -99999 && values[3] != -99999)
            return values[3];

        else if (values[2] != -99999 && values[2] != -99999)
            return values[2];

        else if (values[1] != -99999 && values[1] != 99999)
            return values[1];

        return values[0];
    }*/
}
