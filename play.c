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
#include "mapping.h"
#include "loadMap.h"
#include "anim.h"
#include "gameParams.h"
#include "params.h"
#include "menuLan.h"
#include "motor.h"
#include "socket.h"
#include "IA.h"

/**

0 : SNAKE
1 : OCTOPUS
2 : LAN
3 : LOADED

**/

void saveGame(GameParams);
short loadGame(GameParams*);

void play(Sysvar *sys, int mode)
{
    int i, j;
    SDL_Surface *elements[9] = {NULL};
    TTF_Font *police = TTF_OpenFont("Font/verdana.ttf", 24);
    TTF_Font *police2 = TTF_OpenFont("Font/Jurassic Park.ttf", 48);
    SDL_Surface *bg = NULL;
    int continuer = 1;
    GameParams params;
    SDL_Surface *pions[4] = {NULL};
    int selection = 0;
    SDL_Surface *selectorT = NULL, *selectorW = NULL;
    SDL_Rect pos;
    SDL_Surface *scores[4] = {NULL};
    SDL_Color couleur2;
    char str[6] = "";
    SDL_Surface *interf = IMG_Load("Pictures/interf.bmp");
SDL_FreeSurface(scores[0]);
    selectorT = IMG_Load("Pictures/selectT.bmp");
    selectorW = IMG_Load("Pictures/selectW.bmp");
    int hover = 0;

    SDL_SetColorKey(selectorT, SDL_SRCCOLORKEY, SDL_MapRGB(selectorT->format, 0, 255, 0));
    SDL_SetColorKey(selectorW, SDL_SRCCOLORKEY, SDL_MapRGB(selectorW->format, 0, 255, 0));
    SDL_SetColorKey(interf, SDL_SRCCOLORKEY, SDL_MapRGB(interf->format, 0, 255, 0));
    SDL_SetAlpha(interf, SDL_SRCALPHA, 128);

    loadElements(elements);
    SDL_SetColorKey(selectorT, SDL_SRCCOLORKEY, SDL_MapRGB(selectorT->format, 0, 255, 0));
    SDL_SetAlpha(elements[1], SDL_SRCALPHA, 96);

    if(mode != 2) // 2 = LAN GAME
    {
        if(mode != 3) // 3 = LOADED
        {
            params = setParams(sys, mode);

            loadMap(sys, &params.map, &bg, rand() % NB_MAP);
            loadExtendElem(sys, pions, &bg, params);
            SDL_SetColorKey(selectorT, SDL_SRCCOLORKEY, SDL_MapRGB(selectorT->format, 0, 255, 0));

            playMusic(sys);

            freeScreen(sys);

            sys->position.x = 0;
            sys->position.y = 0;
            SDL_BlitSurface(bg, NULL, sys->screen, &sys->position);

            mapGame(sys, params.map, elements, police);

            //Allocation des grilles
            params.scores = (short*)malloc((params.nbJoueurs + params.nbBot)*sizeof(short));
            params.positions = (char**)malloc(params.map.sizeX*sizeof(char*));
            params.lastPos = (short**)malloc((params.nbJoueurs + params.nbBot)*sizeof(short*));

            //Initialisation des scores à 0 et allocation des positions
            for(i=0 ; i < params.nbJoueurs + params.nbBot ; i++)
            {
                params.scores[i] = 0;
                params.lastPos[i] = (short*)malloc(2*sizeof(short)); // 2 -> row, col
            }

            for(i=0 ; i < params.map.sizeX ; i++)
            {
                params.positions[i] = (char*)malloc(params.map.sizeX*sizeof(char));

                for(j=0 ; j < params.map.sizeY ; j++)
                {
                    switch (params.map.map[i][j])
                    {
                        case LIMIT_PTS_MIN - 2:
                            if (params.nbJoueurs + params.nbBot > 0)
                            {

                                params.positions[i][j] = 'r';
                                params.lastPos[0][0] = i;
                                params.lastPos[0][1] = j;

                            }

                            else
                            {
                                params.map.map[i][j] = 0;
                                params.positions[i][j] = '\0';
                            }
                            break;

                        case LIMIT_PTS_MIN - 3:
                            if (params.nbJoueurs + params.nbBot > 1)
                            {

                                params.positions[i][j] = 'j';
                                params.lastPos[1][0] = i;
                                params.lastPos[1][1] = j;

                            }

                            else
                            {
                                params.map.map[i][j] = 0;
                                params.positions[i][j] = '\0';
                            }
                            break;

                        case LIMIT_PTS_MIN - 4:
                            if (params.nbJoueurs + params.nbBot > 2)
                            {

                                params.positions[i][j] = 'v';
                                params.lastPos[2][0] = i;
                                params.lastPos[2][1] = j;

                            }

                            else
                            {
                                params.map.map[i][j] = 0;
                                params.positions[i][j] = '\0';
                            }
                            break;

                        case LIMIT_PTS_MIN - 5:
                            if (params.nbJoueurs + params.nbBot > 3)
                            {

                                params.positions[i][j] = 'b';
                                params.lastPos[3][0] = i;
                                params.lastPos[3][1] = j;

                            }
                            else
                            {
                                params.map.map[i][j] = 0;
                                params.positions[i][j] = '\0';
                            }
                            break;

                        case LIMIT_PTS_MIN - 7: case LIMIT_PTS_MIN - 1:
                        /// UNCODED AREAS ARE BELOW
                        case LIMIT_PTS_MAX + 1: case LIMIT_PTS_MAX + 2: case LIMIT_PTS_MAX + 3: case LIMIT_PTS_MAX + 4: case LIMIT_PTS_MAX + 5: case LIMIT_PTS_MAX + 6: case LIMIT_PTS_MAX + 7:
                            params.positions[i][j] = 'x'; // x is blocking area
                            break;

                        default :
                            params.positions[i][j] = '\0'; // \0 is passing area
                            break;
                    }

                }
            }

            params.playing = 1;
            params.pass = 0;
        }
        else //Si on charge une partie
        {
            if(loadGame(&params))
            {
                loadMusic(sys, params.map.music);
                bg = IMG_Load(params.map.background);

                loadExtendElem(sys, pions, &bg, params);
                SDL_SetColorKey(selectorT, SDL_SRCCOLORKEY, SDL_MapRGB(selectorT->format, 0, 255, 0));

                playMusic(sys);
                freeScreen(sys);

                sys->position.x = 0;
                sys->position.y = 0;
                SDL_BlitSurface(bg, NULL, sys->screen, &sys->position);

                mapGame(sys, params.map, elements, police);
            }
        }

        whiteScreen(sys, 1, 5, 0, 255);

        while (continuer && sys->continuer) /// GERER AFFICHAGE FIN DE PARTIE
        {
            if(!canPlay(params.playing, params.positions, params.lastPos, params.map.sizeX, params.map.sizeY , params.mode))
            {
                printf("Le joueur %d passe son tour !\n", params.playing);
                params.playing = (params.playing % (params.nbJoueurs + params.nbBot)) + 1;
                params.pass++;
                if(params.pass >= params.nbJoueurs + params.nbBot)
                    continuer = 0;
                continue;
            }

            /*if (params.pass > params.nbJoueurs + params.nbBot)
                continuer = 0;*/

            if (params.playing > params.nbJoueurs) // si c'est un bot qui joue
            {
                /// IA
                ia(50, &params, 100, params.playing, 50);
            }

            if(params.pass > 0)
                params.pass--;

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

                            case SDLK_s:
                                saveGame(params);
                                break;

                            default:
                                break;
                        }
                        break;

                    case SDL_MOUSEBUTTONDOWN:
                        switch (sys->event.button.button)
                        {
                            case SDL_BUTTON_LEFT:
                                if (params.playing <= params.nbJoueurs && selection)
                                {
                                    /// PLACE HERE
                                    params.positions[pos.y][pos.x] = params.playing == 1 ? 'r' : params.playing == 2 ? 'j' : params.playing == 3 ? 'v' : params.playing == 4 ? 'b' : '\0';

                                    if (params.map.map[pos.y][pos.x] <= LIMIT_PTS_MAX && params.map.map[pos.y][pos.x] >= LIMIT_PTS_MIN) /// SI CASE A POINTS
                                    {
                                        params.scores[params.playing-1] += params.map.map[pos.y][pos.x];
                                    }

                                    params.map.map[pos.y][pos.x] = 0;
                                    params.lastPos[params.playing-1][0] = pos.y;
                                    params.lastPos[params.playing-1][1] = pos.x;
                                    params.playing = (params.playing % (params.nbJoueurs + params.nbBot)) + 1;
                                    printMap(params.map.map, params.positions, params.map.sizeX);
                                }
                                break;
                        }
                        break;

                    default :
                        break;
                }
            }

            SDL_Delay(1000 / FRAMERATE);

            freeScreen(sys);

            sys->position.x = 0;
            sys->position.y = 0;
            SDL_BlitSurface(bg, NULL, sys->screen, &sys->position);

            pos = mapGame(sys, params.map, elements, police);

            if (pos.x != -1 && pos.y != -1 && params.playing <= params.nbJoueurs + params.nbBot)
                selection = canPlace(params.playing, params.positions, params.lastPos, params.map.sizeX, params.map.sizeY, params.mode, pos.x, pos.y);

            else
                selection = 0;

            sys->position.x = (LARGEUR_FENETRE - params.map.sizeY * elements[1]->w) / 2 + pos.x * elements[1]->w;
            sys->position.y = (HAUTEUR_FENETRE - params.map.sizeX * elements[1]->h) / 2 + pos.y * elements[1]->h;

            if (params.playing <= params.nbJoueurs && selection == 1 && sys->position.x >= (LARGEUR_FENETRE - params.map.sizeY * elements[1]->w) / 2 && sys->position.x < (LARGEUR_FENETRE - params.map.sizeY * elements[1]->w) / 2 + params.map.sizeY * elements[1]->w && sys->position.y >= (HAUTEUR_FENETRE - params.map.sizeX * elements[1]->h) / 2 && sys->position.y < (HAUTEUR_FENETRE - params.map.sizeX * elements[1]->h) / 2 + params.map.sizeX * elements[1]->h)
                SDL_BlitSurface(selectorT, NULL, sys->screen, &sys->position);

            else if (params.playing <= params.nbJoueurs && sys->position.x >= (LARGEUR_FENETRE - params.map.sizeY * elements[1]->w) / 2 && sys->position.x < (LARGEUR_FENETRE - params.map.sizeY * elements[1]->w) / 2 + params.map.sizeY * elements[1]->w && sys->position.y >= (HAUTEUR_FENETRE - params.map.sizeX * elements[1]->h) / 2 && sys->position.y < (HAUTEUR_FENETRE - params.map.sizeX * elements[1]->h) / 2 + params.map.sizeX * elements[1]->h)
                SDL_BlitSurface(selectorW, NULL, sys->screen, &sys->position);

            for (i = 0 ; i < params.nbJoueurs + params.nbBot ; i++)
            {
                sprintf (str, "%d", params.scores[i]);

                couleur2.r = 230 * (i == 0 || i == 1);
                couleur2.g = 230 * (i == 1 || i == 2);
                couleur2.b = 230 * (i == 3);

                scores[i] = TTF_RenderText_Blended(police, str, couleur2);

                if (i == params.playing - 1)
                    SDL_SetAlpha(interf, SDL_SRCALPHA, 255);

                else
                    SDL_SetAlpha(interf, SDL_SRCALPHA, 128);

                sys->position.x = (LARGEUR_FENETRE - interf->w - 20) * (i % 2) + 10;
                sys->position.y = (HAUTEUR_FENETRE - interf->h - 75) * (i >= 2) + 50;
                SDL_BlitSurface (interf, NULL, sys->screen, &sys->position);

                sys->position.x = (LARGEUR_FENETRE - pions[i]->w - 170) * (i % 2) + 40;
                sys->position.y = (HAUTEUR_FENETRE - pions[i]->h - 65) * (i >= 2) + 40;
                SDL_BlitSurface (pions[i], NULL, sys->screen, &sys->position);

                sys->position.x = (LARGEUR_FENETRE - scores[i]->w - 200) * (i % 2) + 150;
                sys->position.y = (HAUTEUR_FENETRE - scores[i]->h - 85) * (i >= 2) + 60;
                SDL_BlitSurface (scores[i], NULL, sys->screen, &sys->position);

                SDL_FreeSurface(scores[i]);
            }

            for(i=0 ; i < params.map.sizeX ; i++)
            {
                for(j=0 ; j < params.map.sizeY ; j++)
                {
                    sys->position.x = (LARGEUR_FENETRE - params.map.sizeY * elements[1]->w) / 2 + j * pions[0]->w;
                    sys->position.y = (HAUTEUR_FENETRE - params.map.sizeX * elements[1]->h) / 2 + i * pions[0]->h;

                    switch (params.positions[i][j])
                    {
                        case 'r':
                            SDL_BlitSurface(pions[0], NULL, sys->screen, &sys->position);
                            break;

                        case 'j':
                            SDL_BlitSurface(pions[1], NULL, sys->screen, &sys->position);
                            break;

                        case 'v':
                            SDL_BlitSurface(pions[2], NULL, sys->screen, &sys->position);
                            break;

                        case 'b':
                            SDL_BlitSurface(pions[3], NULL, sys->screen, &sys->position);
                            break;
                    }

                }
            }

            blitCursor(sys);
            SDL_Flip(sys->screen);
        }
    }
    else /// LAN GAME
    {
        if(menuLan(sys)) // True si connexion réussie
        {
            FILE *netmap;
            unsigned short listen = 1;

            printf("[CONNECTED]\n");

            //On récupère et on charge la map de jeu
            printf("Waiting for map...\n");
            recvPacketLoop(sys);

            printf("Loading netmap...\n%s\n", sys->packet);
            netmap = fopen("Maps/map99.map", "w");
            fprintf(netmap, "%s", sys->packet);
            fclose(netmap);
            loadMap(sys, &params.map, &bg, 99);

            //On récupère et on charge le nombre de joueurs
            printf("Waiting for game infos...\n");
            recvPacketLoop(sys);
            params.nbJoueurs = atoi(sys->packet); //nombre de joueurs
            recvPacketLoop(sys);
            params.mode = atoi(sys->packet); //gamemode
            printf("Gamemode : %hi\n", params.mode);
            recvPacketLoop(sys);
            params.playing = atoi(sys->packet); //numéro de joueur pour la fonction canPlace
            printf("Player number : %hi\n", params.playing);

            printf("Initializing game...\n");
            params.scores = (short*)malloc(params.nbJoueurs*sizeof(short));
            params.positions = (char**)malloc(params.map.sizeX*sizeof(char*));
            params.lastPos = (short**)malloc(params.nbJoueurs*sizeof(short*));

            for(i=0 ; i < params.nbJoueurs ; i++)
            {
                params.scores[i] = 0;
                params.lastPos[i] = (short*)malloc(2*sizeof(short));
            }

            for(i=0 ; i < params.map.sizeX ; i++)
            {
                params.positions[i] = (char*)malloc(params.map.sizeY*sizeof(char));
                for(j=0 ; j < params.map.sizeY ; j++)
                    params.positions[i][j] = '\0';
            }
            params.extensionPack = 1;
            params.nbBot = 0;

            printf("Loading resources...\n");
            loadExtendElem(sys, pions, &bg, params);
            SDL_SetColorKey(selectorT, SDL_SRCCOLORKEY, SDL_MapRGB(selectorT->format, 0, 255, 0));

            playMusic(sys);
            freeScreen(sys);

            sys->position.x = 0;
            sys->position.y = 0;
            SDL_BlitSurface(bg, NULL, sys->screen, &sys->position);

            printf("First rendering map...\n");
            mapGame(sys, params.map, elements, police);

            printf("Starting game loop...\n");
            //Boucle de jeu
            while(continuer && sys->continuer)
            {
                short type;

                //Si ce n'est pas au tour du joueur, on écoute le serveur
                if(listen)
                {
                    printf("Waiting for packet...\n");

                    if(recvPacket(sys))
                    {
                        char c;
                        short player, row, col;
                        type = atoi(sys->packet);

                        switch(type)
                        {
                            //Début du tour
                            case PACKET_PLAY:
                                printf("[Packet] PACKET_PLAY\n");
                                listen = 0;
                                break;

                            //Mise à jour d'une position
                            case PACKET_UPDATE:
                                printf("[Packet] PACKET_UPDATE\n");

                                recvPacketLoop(sys);
                                player = atoi(sys->packet);
                                printf("Get player : %d\n", player);
                                recvPacketLoop(sys);
                                row = atoi(sys->packet);
                                printf("Get row : %d\n", row);
                                recvPacketLoop(sys);
                                col = atoi(sys->packet);
                                printf("Get col : %d\n", col);

                                c = player == 1 ? 'r' : player == 2 ? 'j' : player == 3 ? 'v' : player == 4 ? 'b' : '\0';

                                params.map.map[row][col] = 0;
                                params.positions[row][col] = c;
                                params.lastPos[player-1][0] = row;
                                params.lastPos[player-1][1] = col;
                                break;

                            //Mise à jour des scores
                            case PACKET_SCORES:
                                printf("[Packet] PACKET_SCORES\n");
                                for(i=0 ; i < params.nbJoueurs ; i++)
                                {
                                    recvPacketLoop(sys);
                                    params.scores[i] = atoi(sys->packet);
                                }
                                break;

                            //Fin de la partie
                            case PACKET_END:
                                printf("[Packet] PACKET_END\n");
                                continuer = 0;
                                break;

                            default:
                                break;
                        }
                    }

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

                                    case SDLK_s:
                                        saveGame(params);
                                        break;

                                    default:
                                        break;
                                }
                                break;

                            default:
                                break;
                        }
                    }
                }
                //Sinon, on traite les events de clic
                else
                {
                    while(SDL_PollEvent(&sys->event))
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

                                    case SDLK_s:
                                        saveGame(params);
                                        break;

                                    default:
                                        break;
                                }
                                break;

                            case SDL_MOUSEBUTTONDOWN:
                                switch (sys->event.button.button)
                                {
                                    case SDL_BUTTON_LEFT:
                                        if(params.playing <= params.nbJoueurs && selection)
                                        {
                                            /*params.positions[pos.y][pos.x] = params.playing == 1 ? 'r' : params.playing == 2 ? 'j' : params.playing == 3 ? 'v' : params.playing == 4 ? 'b' : '\0';

                                            if(params.map.map[pos.y][pos.x] <= LIMIT_PTS_MAX && params.map.map[pos.y][pos.x] >= LIMIT_PTS_MIN) /// SI CASE A POINTS
                                                params.scores[params.playing-1] += params.map.map[pos.y][pos.x];

                                            params.map.map[pos.y][pos.x] = 0;
                                            params.lastPos[params.playing-1][0] = pos.y;
                                            params.lastPos[params.playing-1][1] = pos.x;*/

                                            sprintf(sys->packet, "%hi", pos.y);
                                            sendPacket(sys);
                                            sprintf(sys->packet, "%hi", pos.x);
                                            sendPacket(sys);

                                            listen = 1; //On précise que notre tour de jeu est terminé et on retourne en mode d'écoute
                                        }
                                        break;
                                }
                                break;

                            default :
                                break;
                        }
                    }
                }

                SDL_Delay(1000 / FRAMERATE);

            freeScreen(sys);

            sys->position.x = 0;
            sys->position.y = 0;
            SDL_BlitSurface(bg, NULL, sys->screen, &sys->position);

            pos = mapGame(sys, params.map, elements, police);

            if (pos.x != -1 && pos.y != -1 && params.playing <= params.nbJoueurs + params.nbBot)
                selection = canPlace(params.playing, params.positions, params.lastPos, params.map.sizeX, params.map.sizeY, params.mode, pos.x, pos.y);

            else
                selection = 0;

            sys->position.x = (LARGEUR_FENETRE - params.map.sizeY * elements[1]->w) / 2 + pos.x * elements[1]->w;
            sys->position.y = (HAUTEUR_FENETRE - params.map.sizeX * elements[1]->h) / 2 + pos.y * elements[1]->h;

            if (params.playing <= params.nbJoueurs && selection == 1 && sys->position.x >= (LARGEUR_FENETRE - params.map.sizeY * elements[1]->w) / 2 && sys->position.x < (LARGEUR_FENETRE - params.map.sizeY * elements[1]->w) / 2 + params.map.sizeY * elements[1]->w && sys->position.y >= (HAUTEUR_FENETRE - params.map.sizeX * elements[1]->h) / 2 && sys->position.y < (HAUTEUR_FENETRE - params.map.sizeX * elements[1]->h) / 2 + params.map.sizeX * elements[1]->h)
                SDL_BlitSurface(selectorT, NULL, sys->screen, &sys->position);

            else if (params.playing <= params.nbJoueurs && sys->position.x >= (LARGEUR_FENETRE - params.map.sizeY * elements[1]->w) / 2 && sys->position.x < (LARGEUR_FENETRE - params.map.sizeY * elements[1]->w) / 2 + params.map.sizeY * elements[1]->w && sys->position.y >= (HAUTEUR_FENETRE - params.map.sizeX * elements[1]->h) / 2 && sys->position.y < (HAUTEUR_FENETRE - params.map.sizeX * elements[1]->h) / 2 + params.map.sizeX * elements[1]->h)
                SDL_BlitSurface(selectorW, NULL, sys->screen, &sys->position);

            for (i = 0 ; i < params.nbJoueurs + params.nbBot ; i++)
            {
                sprintf (str, "%d", params.scores[i]);

                couleur2.r = 230 * (i == 0 || i == 1);
                couleur2.g = 230 * (i == 1 || i == 2);
                couleur2.b = 230 * (i == 3);

                scores[i] = TTF_RenderText_Blended(police, str, couleur2);

                if (i == params.playing - 1)
                    SDL_SetAlpha(interf, SDL_SRCALPHA, 255);

                else
                    SDL_SetAlpha(interf, SDL_SRCALPHA, 128);

                sys->position.x = (LARGEUR_FENETRE - interf->w - 20) * (i % 2) + 10;
                sys->position.y = (HAUTEUR_FENETRE - interf->h - 75) * (i >= 2) + 50;
                SDL_BlitSurface (interf, NULL, sys->screen, &sys->position);

                sys->position.x = (LARGEUR_FENETRE - pions[i]->w - 170) * (i % 2) + 40;
                sys->position.y = (HAUTEUR_FENETRE - pions[i]->h - 65) * (i >= 2) + 40;
                SDL_BlitSurface (pions[i], NULL, sys->screen, &sys->position);

                sys->position.x = (LARGEUR_FENETRE - scores[i]->w - 200) * (i % 2) + 150;
                sys->position.y = (HAUTEUR_FENETRE - scores[i]->h - 85) * (i >= 2) + 60;
                SDL_BlitSurface (scores[i], NULL, sys->screen, &sys->position);

                SDL_FreeSurface(scores[i]);
            }

            for(i=0 ; i < params.map.sizeX ; i++)
            {
                for(j=0 ; j < params.map.sizeY ; j++)
                {
                    sys->position.x = (LARGEUR_FENETRE - params.map.sizeY * elements[1]->w) / 2 + j * pions[0]->w;
                    sys->position.y = (HAUTEUR_FENETRE - params.map.sizeX * elements[1]->h) / 2 + i * pions[0]->h;

                    switch (params.positions[i][j])
                    {
                        case 'r':
                            SDL_BlitSurface(pions[0], NULL, sys->screen, &sys->position);
                            break;

                        case 'j':
                            SDL_BlitSurface(pions[1], NULL, sys->screen, &sys->position);
                            break;

                        case 'v':
                            SDL_BlitSurface(pions[2], NULL, sys->screen, &sys->position);
                            break;

                        case 'b':
                            SDL_BlitSurface(pions[3], NULL, sys->screen, &sys->position);
                            break;
                    }

                }
            }

            blitCursor(sys);
            SDL_Flip(sys->screen);
            }

            disconnectFromServer(sys);
            params.pass = 5; //De manière à forcer l'affichage des scores
        }
        else return;
    }

    SDL_SetAlpha(interf, SDL_SRCALPHA, 255);

    /// FAIRE AFFICHAGE DES SCORES
    if (params.pass >= params.nbJoueurs + params.nbBot)
        continuer = 1;

    while (continuer && sys->continuer)
    {

        freeScreen(sys);

        sys->position.x = 0;
        sys->position.y = 0;
        SDL_BlitSurface(bg, NULL, sys->screen, &sys->position);

        if (hover)
        {
            couleur2.r = 230;
            couleur2.g = 0;
            couleur2.b = 0;
        }

        else
        {
            //SDL_FreeSurface(scores[0]);
            couleur2.r = 230;
            couleur2.g = 230;
            couleur2.b = 230;
            scores[0] = TTF_RenderText_Blended(police2, "Quitter", couleur2);
        }

        sys->position.x = (LARGEUR_FENETRE - scores[0]->w) / 2;
        sys->position.y = HAUTEUR_FENETRE - 100;
        SDL_BlitSurface(scores[0], NULL, sys->screen, &sys->position);

        SDL_FreeSurface(scores[0]);

        couleur2.r = 230;
        couleur2.g = 0;
        couleur2.b = 0;

        scores[0] = TTF_RenderText_Blended(police2, "PARTIE TERMINEE", couleur2);

        sys->position.x = (LARGEUR_FENETRE - scores[0]->w) / 2;
        sys->position.y = 50;
        SDL_BlitSurface(scores[0], NULL, sys->screen, &sys->position);

        SDL_FreeSurface(scores[0]);

        for (i = 0 ; i < params.nbBot + params.nbJoueurs ; i++)
        {
            int nbInf = 0;

            sprintf (str, "%d", params.scores[i]);

            scores[i] = TTF_RenderText_Blended(police, str, couleur2);

            for (j = 0 ; j < params.nbBot + params.nbJoueurs ; j++)
            {
                if (params.scores[i] >= params.scores[j])
                {
                    nbInf++;
                }
            }

            sys->position.x = (LARGEUR_FENETRE - interf->w) / 2 + i * 100 - 200;
            sys->position.y = (HAUTEUR_FENETRE - interf->h) / 2  - (((nbInf - 1) % 4) - 2) * 100;
            SDL_BlitSurface (interf, NULL, sys->screen, &sys->position);

            sys->position.x = (LARGEUR_FENETRE - interf->w) / 2 + 30 + i * 100 - 200;
            sys->position.y = (HAUTEUR_FENETRE - interf->h) / 2 - 10 - (((nbInf - 1) % 4) - 2) * 100;
            SDL_BlitSurface (pions[i], NULL, sys->screen, &sys->position);

            sys->position.x = (LARGEUR_FENETRE - interf->w) / 2 + 140 + i * 100 - 200;
            sys->position.y = (HAUTEUR_FENETRE - interf->h) / 2 + 10 - (((nbInf - 1) % 4) - 2) * 100;
            SDL_BlitSurface (scores[i], NULL, sys->screen, &sys->position);

            SDL_FreeSurface(scores[i]);
        }

        blitCursor(sys);

        SDL_Flip(sys->screen);

        SDL_WaitEvent(&sys->event);

        scores[0] = TTF_RenderText_Blended(police2, "Quitter", couleur2);

        if (sys->event.type == SDL_QUIT)
            sys->continuer = 0;

        if (sys->event.motion.x > (LARGEUR_FENETRE - scores[0]->w) / 2 && sys->event.motion.x < (LARGEUR_FENETRE - scores[0]->w) / 2 + scores[0]->w && sys->event.motion.y > HAUTEUR_FENETRE - 100 && sys->event.motion.y < HAUTEUR_FENETRE - 100 + scores[0]->h)
        {
            if (hover == 0)
                playSound(sys);

            hover = 1;
            if (sys->event.button.button == SDL_BUTTON_LEFT)
                continuer = 0;
        }

        else
            hover = 0;
    }

    /// FREE
    for (i = 0 ; i < 9 ; i++)
        SDL_FreeSurface(elements[i]);

    for (i = 0 ; i < params.map.sizeX ; i++)
        free(params.map.map[i]);

    for (i = 0 ; i < 4 ; i++)
        SDL_FreeSurface(pions[i]);

    free(params.map.map);

    free (params.map.background);
    free (params.map.music);

    SDL_FreeSurface(bg);
    SDL_FreeSurface(interf);

    TTF_CloseFont(police);
    TTF_CloseFont(police2);

    blackScreen(sys, 1, 5, 255, 0);
}

void saveGame(GameParams params)
{
    short i, j;
    FILE *save;
    char savePath[30] = "Saves/last.save";

    //sprintf(savePath, "Saves/%d.save", (int)time(NULL));
    save = fopen(savePath, "w");

    //Map
    fprintf(save, "%d ", params.map.sizeX);
    fprintf(save, "%d ", params.map.sizeY);

    for(i=0 ; i < params.map.sizeX ; i++)
        for(j=0 ; j < params.map.sizeY ; j++)
            fprintf(save, "%d ", params.map.map[i][j]);

    fprintf(save, "%s ", params.map.background);
    fprintf(save, "%s ", params.map.music);

    //Game struct
    fprintf(save, "%d ", params.nbJoueurs);
    fprintf(save, "%d ", params.nbBot);
    fprintf(save, "%d ", params.mode);
    fprintf(save, "%d ", params.pass);
    fprintf(save, "%d ", params.playing);

    for(i=0 ; i < params.nbJoueurs + params.nbBot ; i++)
        fprintf(save, "%d ", params.scores[i]);

    for(i=0 ; i < params.map.sizeX ; i++)
    {
        for(j=0 ; j < params.map.sizeY ; j++)
        {
            if(params.positions[i][j] != '\0')
                fprintf(save, "%c ", params.positions[i][j]);
            else
                fprintf(save, "n ");
        }
    }

    for(i=0 ; i < params.nbJoueurs + params.nbBot ; i++)
        for(j=0 ; j < 2 ; j++)
            fprintf(save, "%hi ", params.lastPos[i][j]);

    fprintf(save, "%d ", params.extensionPack);
    fprintf(save, "%d ", params.nbMap);
    fprintf(save, "%d", params.isNetwork);

    fclose(save);
}

short loadGame(GameParams *params)
{
    short i, j;
    FILE* save;
    char path[30];

    sprintf(path, "Saves/last.save");
    if((save = fopen(path, "r")))
    {
        //Map
        fscanf(save, "%d", &params->map.sizeX);
        fscanf(save, "%d", &params->map.sizeY);

        params->map.map = (int**)malloc(params->map.sizeX*sizeof(int*));
        for(i=0 ; i < params->map.sizeX ; i++)
        {
            params->map.map[i] = (int*)malloc(params->map.sizeY*sizeof(int));

            for(j=0 ; j < params->map.sizeY ; j++)
                fscanf(save, "%d", &params->map.map[i][j]);
        }

        params->map.background = (char*) malloc (100 * sizeof(char));
        fscanf(save, "%s", params->map.background);

        params->map.music = (char*) malloc (100 * sizeof(char));
        fscanf(save, "%s", params->map.music);

        //Game struct
        fscanf(save, "%hi", &params->nbJoueurs);
        fscanf(save, "%d", &params->nbBot);
        fscanf(save, "%hi", &params->mode);
        fscanf(save, "%hi", &params->pass);
        fscanf(save, "%hi", &params->playing);

        params->scores = (short*)malloc((params->nbJoueurs + params->nbBot)*sizeof(short));
        for(i=0 ; i < params->nbJoueurs + params->nbBot ; i++)
            fscanf(save, "%hi", &params->scores[i]);

        params->positions = (char**)malloc(params->map.sizeX*sizeof(char*));
        for(i=0 ; i < params->map.sizeX ; i++)
        {
            params->positions[i] = (char*)malloc(params->map.sizeY*sizeof(char));

            for(j=0 ; j < params->map.sizeY ; j++)
            {
                fscanf(save, "%s", &params->positions[i][j]);
                if(params->positions[i][j] == 'n')
                    params->positions[i][j] = '\0';
            }
        }

        params->lastPos = (short**)malloc((params->nbJoueurs + params->nbBot)*sizeof(short*));
        for(i=0 ; i < params->nbJoueurs + params->nbBot ; i++)
        {
            params->lastPos[i] = (short*)malloc(2*sizeof(short));

            for(j=0 ; j < 2 ; j++)
                fscanf(save, "%hi", &params->lastPos[i][j]);
        }

        fscanf(save, "%d", &params->extensionPack);
        fscanf(save, "%d", &params->nbMap);
        fscanf(save, "%d", &params->isNetwork);

        if(params->extensionPack < 1)
            params->extensionPack = 1;

        fclose(save);
        return 1;
    }
    else
        return 2;
}
