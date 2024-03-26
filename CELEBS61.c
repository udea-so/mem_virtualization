/* CELEBS61

   This example determines the value of ARG_MAX.

 */
#define _POSIX_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

main() {
  long result;

  errno = 0;
  puts("examining ARG_MAX limit");
  if ((result = sysconf(_SC_ARG_MAX)) == -1)
    if (errno == 0)
      puts("ARG_MAX is not supported.");
    else perror("sysconf() error");
  else
    printf("ARG_MAX is %ld\n", result);
}