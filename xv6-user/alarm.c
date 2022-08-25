#include "xv6-user/user.h"

int main() {
  printf("Alarm testing\n");

  alarm(5);         // send SIGALARM to calling process after 5 seconds, which means terminating it
  while(1)	// process suspended, waiting for signals to wake up
    printf("unreachable!");

  exit(0);
}