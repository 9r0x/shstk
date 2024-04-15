#include <stdio.h>
#include <asm/prctl.h>
#include <sys/syscall.h>
#include <unistd.h>

int main()
{
    long ret = 0;
    long features = 0;

// https://elixir.bootlin.com/linux/latest/source/arch/x86/include/uapi/asm/prctl.h#L33
#define ARCH_SHSTK_ENABLE 0x5001
#define ARCH_SHSTK_STATUS 0x5005

#define ARCH_SHSTK_SHSTK (1ULL << 0)
#define ARCH_SHSTK_WRSS (1ULL << 1)

    ret = syscall(SYS_arch_prctl, ARCH_SHSTK_STATUS, &features);
    printf("arch_prctl(ARCH_SHSTK_STATUS, &features) returned %d\n", ret);
    printf("features = %lx\n", features);

    ret = syscall(SYS_arch_prctl, ARCH_SHSTK_ENABLE, ARCH_SHSTK_SHSTK);
    printf("arch_prctl(ARCH_SHSTK_ENABLE, ARCH_SHSTK_SHSTK) returned %d\n", ret);

    // ret = syscall(SYS_arch_prctl, ARCH_SHSTK_ENABLE, ARCH_SHSTK_WRSS);
    // printf("arch_prctl(ARCH_SHSTK_ENABLE, ARCH_SHSTK_WRSS) returned %d\n", ret);

    return 0;
}