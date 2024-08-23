#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define warning(FUNC)	printf("SDLNet_"#FUNC": %s\n", SDLNet_GetError())
#define error(FUNC)	{ warning(FUNC);  exit(1); }

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Error: Incorrect number of inputs where given\n");
        return -1;
    }
    char *host = strdup(argv[1]);
    int port = atoi(argv[2]);
    char *message = malloc(sizeof(*message) * 1024);

    if (SDLNet_Init() == -1) error(Init);

    IPaddress ip;
    if (-1 == SDLNet_ResolveHost(&ip, host, port)) error(ResolveHost);

    TCPsocket socket;
    if(!(socket = SDLNet_TCP_Open(&ip))) error(TCP_Open);
    printf("Connected to %s:%i\n", host, port);

    while (1) {
        printf("enter message: ");
        fgets(message, 1024, stdin);
        int outlen = strlen(message);
        if (outlen > 0 && message[outlen-1] == '\n') {
            message[outlen-1] = '\0';
        }
        if (SDLNet_TCP_Send(socket, message, outlen) < outlen) warning(TCP_Send);
        if (strcmp("q", message) == 0 || strcmp("Q", message) == 0) {
            SDLNet_TCP_Close(socket); 
            break;
        }
    }
    SDLNet_Quit();
    return 0;
}
