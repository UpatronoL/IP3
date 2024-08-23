#include <sys/time.h>

/* return milliseconds elapsed since first call to this function
 */
unsigned long microsecondClock(void)
{
    static unsigned long epoch = 0;
    struct timeval tv;
    gettimeofday(&tv, 0);          // current time in seconds and microseconds
    unsigned long uSecs = tv.tv_sec * 1000000UL + tv.tv_usec;
    if (0 == epoch) epoch = uSecs; // this is the first call to this function
    return uSecs - epoch;          // make time relative to first call
}
