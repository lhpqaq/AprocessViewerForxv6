#include "xv6-user/user.h"

int main(){
    printf("getmem success\nsize = %dKB\n", getmem());
    exit(0);
    return 0;
}