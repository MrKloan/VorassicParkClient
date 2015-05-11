#ifndef ANIM

    #define ANIM

    void freeScreen(Sysvar *sys);
    void blackScreen(Sysvar *sys, int soundEffect, int speed, int begin, int limit);
    void whiteScreen(Sysvar *sys, int soundEffect, int speed, int begin, int limit);
    SDL_Surface* makePlaceHolder(int w, int h);
    void blitCursor(Sysvar *sys);

#endif
