#ifndef PIXEL

    #define PIXEL

    #include <SDL/SDL.h>
    #include "constantes.h"

    Uint32 getPixel(SDL_Surface* surface, int x, int y); // get les pixel de l'image de la map
    void setPixel(SDL_Surface *surface, int x, int y, Uint32 pixel); //fonction de dessin pix par pix

#endif
