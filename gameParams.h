#ifndef GAMEPARAMS

    #define GAMEPARAMS

    /*struct GameParams
    {
        int nbJoueurs;
        int nbBot;
        int mode;
        int extensionPack;
        int nbMap;
        int isNetwork;
    };*/

    typedef struct {
        //Configs
        short nbJoueurs; //1-4
        short mode; //0 = octopus, 1 = snake
        //char* netmap; //Serialized map

        //Game values
        short pass; // compteur de tours passés quand le joueur ne peut pas jouer
        short playing; // joueur en train de jouer
        short *scores;
        char **positions;
        short **lastPos;
        Map map;

        // Runtime Values
        int nbBot;
        int extensionPack;
        int nbMap;
        int isNetwork;
    } GameParams;

#endif
