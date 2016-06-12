#include <stdio.h>
#include <stdlib.h>		// exit()
#include <sys/types.h>
#include <signal.h>
#include <string.h>		// strerror()
#include <errno.h>		// errno

int main(int argc, char **argv)
{
  // Use strtol() to convert string to integer is a good manner
  //long int strtol(const char *nptr, char **endptr, int base);

  //int kill(pid_t pid, int sig);
  if (kill(atoi(argv[1]), atoi(argv[2])) < 0)
  {
    fprintf(stderr, "Send signal %s to %s failed: %s\n", argv[2], argv[1], strerror(errno));
    exit(1);
  }

  fprintf(stdout, "Sent signal %s to %s successfully.\n", argv[2], argv[1]);

  return 0;
}
