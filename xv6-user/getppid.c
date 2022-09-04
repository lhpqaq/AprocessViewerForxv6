#include "kernel/include/types.h"
#include "kernel/include/stat.h"
#include "xv6-user/user.h"
#include "kernel/include/param.h"

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
    int i=0;
    while(i<100000000)
    {
        int j=0;
        while(j<100)
        {
            j++;
        }
        i++;
    }
    long tt=times(&t);
    printf("times:%ds %ds %ds %ds \n",t.utime/CLK_FREQ,t.stime/CLK_FREQ,t.cutime/CLK_FREQ,t.cstime/CLK_FREQ);
    printf("return:%ds\n",tt/CLK_FREQ);
}

int main(void) {
    printf("hello\n");
	test_getppid();
    test_times();
	exit(0);
    return 0;
}
