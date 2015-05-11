#ifndef SYSVARS

    #define SYSVARS
    #include <SDL/SDL_net.h>

    /*****************************************************************************
    **                                                                          **
    ** DESCRIPTION : Fichier contenant les variables systèmes du moteur du jeu  **
    **                                                                          **
    *****************************************************************************/

    typedef struct Sysvar Sysvar;

    struct Sysvar
    {
        SDL_Surface *screen;
        SDL_Event event;
        FSOUND_STREAM *music;
        FSOUND_SAMPLE *sound;
        SDL_Rect position;
        SDL_Surface *cursor;

        TCPsocket socket;
        IPaddress ip;
        char *packet;

        int continuer;
    };

#endif
