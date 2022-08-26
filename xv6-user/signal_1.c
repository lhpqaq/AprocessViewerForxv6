#include "xv6-user/user.h"

int main(){
  printf("Waiting for alarm to go off\n");
  signal ( SIGALARM, SIG_IGN );
  printf("now reachable\n");
  signal(SIGALARM, SIG_DFL);
  printf("unreachable\n");
  exit(0);
}