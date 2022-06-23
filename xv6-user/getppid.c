#include "kernel/include/types.h"
#include "kernel/include/stat.h"
#include "xv6-user/user.h"


int test_getppid()
{
    //TEST_START(__func__);
    int pid = getppid();
    //assert(pid >= 0);
    printf("getppid success.\nppid = %d\n", pid);
    //TEST_END(__func__);
    return 0;
}

int main(void) {
    printf("hello\n");
	test_getppid();
	exit(0);
    return 0;
}
