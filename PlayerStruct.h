#ifndef PLAYER_STRUCT

    #define PLAYER_STRUCT

    struct Player
    {
        int score;
        Position pos;
        Position *visited;
        int nbVisited;
    };

    typedef struct Player Player;
#endif
