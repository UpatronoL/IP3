#include "libio.h"

int main() {

    for (;;) {
        char *message = input();
        if (!message) break;
        output(message);
    }

    return 0; 
}
