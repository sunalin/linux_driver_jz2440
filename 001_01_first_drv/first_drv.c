#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>


volatile unsigned long *gpfcon = NULL;
volatile unsigned long *gpfdat = NULL;


static int first_drv_open(struct inode *inode, struct file *file)
{
    //printk("first_drv_open\n");
    /* ÅäÖÃGPF4,5,6ÎªÊä³ö */
    *gpfcon &= ~((0x3<<(4*2)) | (0x3<<(5*2)) | (0x3<<(6*2)));
    *gpfcon |= ((0x1<<(4*2)) | (0x1<<(5*2)) | (0x1<<(6*2)));
    return 0;
}

static ssize_t first_drv_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
    int val;

    //printk("first_drv_write\n");

    copy_from_user(&val, buf, count);
    //copy_to_user();

    if (val == 1)
    {
        // µãµÆ
        *gpfdat &= ~((1<<4) | (1<<5) | (1<<6));
    }
    else
    {
        // ÃðµÆ
        *gpfdat |= (1<<4) | (1<<5) | (1<<6);
    }

    return 0;
}

static struct file_operations first_drv_fops =
{
    .owner  =   THIS_MODULE,
    .open   =   first_drv_open,     
    .write  =   first_drv_write,
};


static struct class*        firstdrv_class;
static struct class_device* firstdrv_class_dev;
static int major;
static int first_drv_init(void)
{
    major = register_chrdev(0, "first_drv", &first_drv_fops); // ×¢²á, ¸æËßÄÚºË

    firstdrv_class = class_create(first_drv_fops.owner, "firstdrv");

    firstdrv_class_dev = class_device_create(firstdrv_class, NULL, MKDEV(major, 0), NULL, "xyz"); /* /dev/xyz */

    gpfcon = (volatile unsigned long *)ioremap(0x56000050, 16);
    gpfdat = gpfcon + 1;

    return 0;
}

static void first_drv_exit(void)
{
    unregister_chrdev(major, "first_drv"); // Ð¶ÔØ

    class_device_unregister(firstdrv_class_dev);
    class_destroy(firstdrv_class);
    iounmap(gpfcon);
}

module_init(first_drv_init);
module_exit(first_drv_exit);
MODULE_LICENSE("GPL");


