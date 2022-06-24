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

void test_times()
{
    struct tms t;
    long tt=times(&t);
    printf("times:%l %l %l %l \n",t.utime,t.stime,t.cutime,t.cstime);
    printf("return:%l\n",tt);
}

int main(void) {
    printf("hello\n");
	test_getppid();
    test_times();
	exit(0);
    return 0;
}
