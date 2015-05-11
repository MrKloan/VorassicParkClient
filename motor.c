#include <stdio.h>
#include <stdlib.h>
#include "motor.h"

short canPlay(short player, char **positions, short **lastPos, short gridSizeX, short gridSizeY , short gamemode)
{
    char c = player == 1 ? 'r' : player == 2 ? 'j' : player == 3 ? 'v' : player == 4 ? 'b' : '\0';
    short i, j;

    //Conditions de jouabilité
    switch(gamemode)
    {
        //Octopus
        case 1:
            for(i=0 ; i < gridSizeX ; i++)
            {
                for(j=0 ; j < gridSizeY ; j++)
                {
                    //Pour chaque case appartenant à ce joueur...
                    if(positions[i][j] == c)
                    {
                        //... on vérifie que l'une de ses adjacentes est vide. Si oui, renvoie vrai.
                        if((i <= gridSizeX-2 && positions[i+1][j] == '\0')
                        || (i > 0 && positions[i-1][j] == '\0')
                        || (j <= gridSizeY-2 && positions[i][j+1] == '\0')
                        || (j > 0 && positions[i][j-1] == '\0')
                        || (i <= gridSizeX-2 && j <= gridSizeY-2 && positions[i+1][j+1] == '\0')
                        || (i <= gridSizeY-2 && j > 0 && positions[i+1][j-1] == '\0')
                        || (i > 0 && j > 0 && positions[i-1][j-1] == '\0')
                        || (i > 0 && j <= gridSizeY-2 && positions[i-1][j+1] == '\0'))
                            return 1;
                    }
                }
            }
            break;

        //Snake
        case 0:
            if((lastPos[player-1][0] <= gridSizeX-2 && positions[lastPos[player-1][0]+1][lastPos[player-1][1]] == '\0')
            || (lastPos[player-1][0] > 0 && positions[lastPos[player-1][0]-1][lastPos[player-1][1]] == '\0')
            || (lastPos[player-1][1] <= gridSizeY-2 && positions[lastPos[player-1][0]][lastPos[player-1][1]+1] == '\0')
            || (lastPos[player-1][1] > 0 && positions[lastPos[player-1][0]][lastPos[player-1][1]-1] == '\0'))
                return 1;
            break;
    }
    //Par défaut, on admet que le joueur ne peut pas jouer
    return 0;
}

short canPlace(short player, char **positions, short **lastPos, short gridSizeX, short gridSizeY, short gamemode, short col, short row)
{
    char c = player == 1 ? 'r' : player == 2 ? 'j' : player == 3 ? 'v' : player == 4 ? 'b' : '\0';

    if (row < 0 || col < 0 || row > gridSizeX - 1 || col > gridSizeY - 1)
        return 0;

    if(positions[row][col] != '\0')
        return 0;

    switch(gamemode)
    {
        //Octopus
        case 1:
            if((row <= gridSizeX-2 && positions[row+1][col] == c)
            || (row > 0 && positions[row-1][col] == c)
            || (col <= gridSizeY-2 && positions[row][col+1] == c)
            || (col > 0 && positions[row][col-1] == c)
            || (row <= gridSizeX-2 && col <= gridSizeY-2 && positions[row+1][col+1] == c)
            || (row <= gridSizeX-2 && col > 0 && positions[row+1][col-1] == c)
            || (row > 0 && col > 0 && positions[row-1][col-1] == c)
            || (row > 0 && col <= gridSizeY-2 && positions[row-1][col+1] == c))
                return 1;
            break;
        //Snake
        case 0:
            if((row+1 == lastPos[player-1][0] && col == lastPos[player-1][1])
            || (row-1 == lastPos[player-1][0] && col == lastPos[player-1][1])
            || (col+1 == lastPos[player-1][1] && row == lastPos[player-1][0])
            || (col-1 == lastPos[player-1][1] && row == lastPos[player-1][0]))
                return 1;
            break;
    }
    //Par défaut, on admet que la position est invalide
    return 0;
}

void printMap(int **grid, char **positions, short gridSize)
{
    short i, j;

    for(i=0 ; i < gridSize ; i++)
    {
        for(j=0 ; j < gridSize ; j++)
        {
            if(positions[i][j] != '\0')
                printf("%c", positions[i][j]);
            else
                printf("%hi", grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
