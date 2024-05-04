#include <stdio.h>
#include <asm/prctl.h>
#include <sys/syscall.h>
#include <unistd.h>

// Since rdssp requires a register, if rdssp is a NOP on certain arch,
// previous rax(or any other register) value will be used.
#define READSSP(val) __asm__ volatile("rdsspq %0" : "=r"(val) : :);
#define INCSSP(val) __asm__ volatile("incsspq %0" : : "r"(val) :);
#define WRSSQ(val, addr) __asm__ volatile("wrssq %0, %1" : : "r"(val), "m"(addr) :);
#define WRUSSQ(val, addr) __asm__ volatile("wrussq %0, %1" : : "r"(val), "m"(addr) :);

int foo()
{
    unsigned long long ssp2 = 0;

    // Test rdssp
    READSSP(ssp2);
    printf("foo: rdssp returned 0x%016llx\n", ssp2);
    printf("value at ssp2: 0x%016llx\n", *(unsigned long long *)ssp2);

    (*(unsigned long long *)(ssp2 - 8)) = 0xdeadbeef;

    // Test incssp
    INCSSP(3ULL);

    READSSP(ssp2);
    printf("foo: rdssp returned 0x%016llx\n", ssp2);
    printf("value at ssp2: 0x%016llx\n", *(unsigned long long *)ssp2);

    // Test wrssq
    WRSSQ(0xdeadbeefULL, ssp2);
    printf("value at ssp2: 0x%016llx\n", *(unsigned long long *)ssp2);

    printf("foo is going to return\n");
    return 0;

    while (1)
    {
        sleep(1);
    }

    return 0;
}

int main()
{
    // foo();
    unsigned long long ssp = 0;
    READSSP(ssp);
    printf("main: rdssp returned 0x%016llx\n", ssp);
    foo();
    printf("foo returned\n");
    ssp = 0;
    READSSP(ssp);
    printf("main: rdssp returned 0x%016llx\n", ssp);

    return 0;
}