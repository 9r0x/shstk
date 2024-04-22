#include <stdio.h>
#include <asm/prctl.h>
#include <sys/syscall.h>
#include <unistd.h>
#define READSSP(val) __asm__ volatile("rdsspq %0" : "=r"(val) : :);

int foo()
{
    unsigned long long ssp2 = 0;
    READSSP(ssp2);
    printf("foo: rdssp returned 0x%016llx\n", ssp2);

    return 0;
}

int main()
{
    // foo();
    unsigned long long ssp = 0;
    READSSP(ssp);
    printf("main: rdssp returned 0x%016llx\n", ssp);
    foo();
    ssp = 0;
    READSSP(ssp);
    printf("main: rdssp returned 0x%016llx\n", ssp);

    return 0;
}