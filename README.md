# Kernel module dev

```shell
cd ./kernel_modules
make
sudo insmod ./shstk_status.ko
sudo mount -t debugfs none /sys/kernel/debug
sudo dmesg -W

sudo -i
cd /sys/kernel/debug/shstk_status/
cat status
```
