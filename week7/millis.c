#include <stdio.h>
#include <unistd.h>

#include <sys/time.h>

unsigned long millis(void)
{
  struct timeval tv;
  static unsigned long epoch = 0;
  gettimeofday(&tv, 0);
  if (!epoch) epoch = tv.tv_sec * 1000L + tv.tv_usec / 1000;
  return tv.tv_sec * 1000L + tv.tv_usec / 1000 - epoch;
}

int main()
{
    unsigned long start = millis();
    sleep(1);
    unsigned long stop = millis();
    printf("%lu\n", stop - start);

    return 0;
}


