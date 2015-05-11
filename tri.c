#include <stdio.h>
#include <stdlib.h>

void tribub(int tab[4], int x[4], int y[4])
{
    int taille = 4;
    int isSorted = 0;
    int temp;
    int i;

    while(!isSorted)
    {
        isSorted = 1;
        for(i=0 ; i < taille-1 ; i++)
        {
            if(tab[i] < tab[i+1])
            {
                temp = tab[i];
                tab[i] = tab[i + 1];
                tab[i + 1] = temp;

                temp = x[i];
                x[i] = x[i + 1];
                x[i + 1] = temp;

                temp = y[i];
                y[i] = y[i + 1];
                y[i + 1] = temp;

                isSorted = 0;
            }
        }
    }
}
