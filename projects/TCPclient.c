#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define warning(FUNC)	printf("SDLNet_"#FUNC": %s\n", SDLNet_GetError())
#define error(FUNC)	{ warning(FUNC);  exit(1); }

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Error: Incorrect number of inputs where given\n");
        return -1;
    }
    char *host = strdup(argv[1]);
    int port = atoi(argv[2]);

    if (SDLNet_Init() == -1) error(Init);

    IPaddress ip;
    if (-1 == SDLNet_ResolveHost(&ip, host, port)) error(ResolveHost);

    TCPsocket socket;
    if(!(socket = SDLNet_TCP_Open(&ip))) error(TCP_Open);
    printf("Connectedto %s:%i\n", host, port);

    char data[1024];
    int inlen;
    if((inlen = SDLNet_TCP_Recv(socket, data, sizeof(data))) <= 0) error(TCP_Recv);

    printf("recived: %.*s\n", inlen, data);

    SDLNet_TCP_Close(socket);
    SDLNet_Quit();
    return 0;
}
