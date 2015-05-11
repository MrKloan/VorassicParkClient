#ifndef MAPSTRUCT

    #define MAPSTRUCT

    struct Map
    {
        int **map;
        int sizeX;
        int sizeY;
        char *background;
        char *music;
    };

    typedef struct Map Map;

#endif
