#ifndef SOCKET_H
#define SOCKET_H

short connectToServer(Sysvar *sys, const char *host);
void disconnectFromServer(Sysvar *sys);
short sendPacket(Sysvar *sys);
void recvPacketLoop(Sysvar *sys);
short recvPacket(Sysvar *sys);

#endif // SOCKET_H
