#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define warning(FUNC)	printf("SDLNet_"#FUNC": %s\n", SDLNet_GetError())
#define error(FUNC)	{ warning(FUNC);  exit(1); }

void server(int port) {
    IPaddress ip;
    if (-1 == SDLNet_ResolveHost(&ip, 0, port)) error(ResolveHost);

    TCPsocket server;
    if (!(server = SDLNet_TCP_Open(&ip)))		error(TCP_Open);

    SDLNet_SocketSet servers;
    if (!(servers = SDLNet_AllocSocketSet(2)))		error(AllocSocketSet);
    if (SDLNet_TCP_AddSocket(servers, server) < 0)	error(AddSocket);

    printf("listening for connections on port %i\n", port);

    for (;;) {
        SDLNet_CheckSockets(servers, ~0); 
        if (SDLNet_SocketReady(server)) { 
            TCPsocket client;
            if (!(client = SDLNet_TCP_Accept(server))) { 
                warning(TCP_Accept);
                continue;
            }

            IPaddress *remoteip;
            if (!(remoteip = SDLNet_TCP_GetPeerAddress(client))) warning(TCP_GetPeerAddress);

            unsigned char *h = (void *)&remoteip->host, *p = (void *)&remoteip->port;
            printf("Connection accepted from %i.%i.%i.%i:%hu\n", h[0], h[1], h[2], h[3], p[0] * 256 + p[1]);

            while (1) {
                char *data = malloc(sizeof(*data) * 1024);
                int inlen;
                if ((inlen = SDLNet_TCP_Recv(client, data, sizeof(data) - 1)) <= 0) {
                    warning(TCP_Recv);
                } 
                printf("Received: %.*s\n", inlen, data);
                if (strcmp("q", data) == 0) {
                    printf("client connection closed\n");
                    SDLNet_TCP_Close(client); 
                    break;
                }
                if (strcmp("Q", data) == 0) {
                    printf("client connection closed\n");
                    printf("server connection closed\n");
                    SDLNet_TCP_Close(server); 
                    break;
                }
            }
        }
        break;
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: %s port\n", argv[0]);
        exit(1);
    }

    if (SDLNet_Init() == -1) error(Init);

    server(atoi(argv[1]));

    SDLNet_Quit();

    return 0;
}
