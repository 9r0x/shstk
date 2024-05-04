#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/init.h>
#include <asm/processor.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shukai Ni");
MODULE_DESCRIPTION("A simple Linux driver to read shstk related status.");

#define READSSP(val) __asm__ volatile("rdsspq %0" : "=r"(val) : :);

static void print_cr4_cet(void)
{
    unsigned long cr4_value;
    cr4_value = __read_cr4();
#define CR4_CET_BIT 23
    printk(KERN_INFO "CR4.CET bit: %lu\n", (cr4_value >> CR4_CET_BIT) & 1);
}

static void print_cpl(void)
{
    unsigned long cs;
    unsigned int cpl;

    asm volatile("mov %%cs, %0" : "=r"(cs));
    cpl = cs & 0x3;

    printk(KERN_INFO "Current CPL: %u\n", cpl);
}

// https://i.blackhat.com/asia-19/Thu-March-28/bh-asia-Sun-How-to-Survive-the-Hardware-Assisted-Control-Flow-Integrity-Enforcement.pdf
#define MSR_IA32_S_CET 0x6A2
static void print_IA32_S_CET(void)
{
    unsigned long IA32_S_CET;

    asm volatile("rdmsr" : "=a"(IA32_S_CET) : "c"(MSR_IA32_S_CET));
    printk(KERN_INFO "IA32_S_CET: %lx\n", IA32_S_CET);
}

#define MSR_IA32_U_CET 0x6A0
static void print_IA32_U_CET(void)
{
    unsigned long IA32_U_CET;

    asm volatile("rdmsr" : "=a"(IA32_U_CET) : "c"(MSR_IA32_U_CET));
    printk(KERN_INFO "IA32_U_CET: %lx\n", IA32_U_CET);
}

static void foo(void)
{
    unsigned long long ssp2 = 0;
    READSSP(ssp2);
    printk(KERN_INFO "foo: rdssp returned 0x%016llx\n", ssp2);
}

static ssize_t
write_data(struct file *f, const char __user *buf, size_t count, loff_t *off)
{
    printk(KERN_INFO "write_data\n");
    unsigned long long ssp = 0;
    READSSP(ssp);
    printk(KERN_INFO "ssp: 0x%016llx\n", ssp);
    foo();
    READSSP(ssp);
    printk(KERN_INFO "ssp: 0x%016llx\n", ssp);
    return count;
}

static ssize_t
read_data(struct file *f, char __user *buf, size_t count, loff_t *off)
{
    printk(KERN_INFO "read_data\n");
    print_cpl();
    print_cr4_cet();
    print_IA32_S_CET();
    print_IA32_U_CET();
    return count;
}

static const struct file_operations data_fops = {
    .write = write_data,
    .read = read_data,
};

static struct dentry *root_dir = NULL;
static struct dentry *status_dev = NULL;

static void
cleanup_debugfs(void)
{
    if (status_dev != NULL)
        debugfs_remove(status_dev);
    if (root_dir != NULL)
        debugfs_remove(root_dir);
}

static int debugfs_init(void)
{
    root_dir = debugfs_create_dir("shstk_status", NULL);
    if (!root_dir)
    {
        printk(KERN_ERR "Failed to create debugfs directory\n");
        return -ENODEV;
    }

    status_dev = debugfs_create_file("status", 0600, root_dir, NULL, &data_fops);
    if (!status_dev)
    {
        printk(KERN_ERR "Failed to create debugfs file\n");
        cleanup_debugfs();
        return -ENODEV;
    }

    return 0;
}

static int __init shstk_status_init(void)
{
    printk(KERN_INFO "Starting shstk Status Module\n");
    debugfs_init();
    return 0;
}

static void __exit shstk_status_exit(void)
{
    printk(KERN_INFO "Exiting shstk Status Module\n");
    cleanup_debugfs();
}

module_init(shstk_status_init);
module_exit(shstk_status_exit);