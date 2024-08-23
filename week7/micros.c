#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

unsigned long micros(void)
{
    static struct timeval epoch;
    if (!epoch.tv_sec) gettimeofday(&epoch, 0);
    struct timeval tod;
    gettimeofday(&tod, 0);
    long sec = tod.tv_sec - epoch.tv_sec;
    long usec = (long)tod.tv_usec - (long)epoch.tv_usec;
    return sec * 1000000 + usec;
}

int main()
{
    unsigned long start = micros();
    sleep(1);
    unsigned long stop = micros();
    printf("%lu\n", stop - start);

    return 0;
}


