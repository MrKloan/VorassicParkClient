#include <string.h>
#include <fmod/fmod.h>

#include "mapStruct.h"
#include "constantes.h"
#include "sysvars.h"
#include "socket.h"

short connectToServer(Sysvar *sys, const char *host)
{
    if(SDLNet_Init() < 0)
	{
		fprintf(stderr, "%s\n", SDLNet_GetError());
		return 0;
	}

	if(SDLNet_ResolveHost(&sys->ip, host, SERVER_PORT) < 0)
	{
		fprintf(stderr, "%s\n", SDLNet_GetError());
		return 0;
	}

	if(!(sys->socket = SDLNet_TCP_Open(&sys->ip)))
	{
		fprintf(stderr, "%s\n", SDLNet_GetError());
		return 0;
	}

	sys->packet = (char*)malloc(PACKET_MAX_SIZE*sizeof(char));
	return 1;
}

void disconnectFromServer(Sysvar *sys)
{
    free(sys->packet);
    SDLNet_TCP_Close(sys->socket);
    SDLNet_Quit();
}

short sendPacket(Sysvar *sys)
{
    //Si vrai, la connexion avec le client est perdue. Sinon, le message a bien été envoyé.
    if(SDLNet_TCP_Send(sys->socket, (void*)sys->packet, strlen(sys->packet)+1) < strlen(sys->packet)+1)
    {
        fprintf(stderr, "%s\n", SDLNet_GetError());
        return 0;
    }
    SDL_Delay(PACKET_TIMEOUT);
    return 1;
}

void recvPacketLoop(Sysvar *sys)
{
    while(!recvPacket(sys));
}

short recvPacket(Sysvar *sys)
{
    if(SDLNet_TCP_Recv(sys->socket, sys->packet, PACKET_MAX_SIZE) > 0)
        return 1;
    else
        return 0;
}
