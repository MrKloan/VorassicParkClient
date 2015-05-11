#ifndef GAME_H
#define GAME_H

char* strpart(const char*, const char*);
Map loadMap(const char*);
void freeMap(Map*);

#endif // GAME_H
