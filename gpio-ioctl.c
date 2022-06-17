#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#define BASE_MINOR 0
#define N_DEVICES 1

// #include<linux/init.h>
// #include<linux/device.h>
static int major = 250, minor = 0;
static dev_t dev_no;
static struct class *pclass;

static struct cdev cdev;
static struct file_operations fops;

static __init int gpio_init(void)
{
    int ret;

    static struct device *pdevice;

    printk("[%s] gpio_init() called.\n", THIS_MODULE->name);

    dev_no = MKDEV(major, minor);
    ret = alloc_chrdev_region(&dev_no, BASE_MINOR, N_DEVICES, "cs_gpio"); // register device driver /proc/
    if (ret < 0)
    {
        printk("[%s] alloc_chrdev_region() failed.\n", THIS_MODULE->name);
        return ret;
    }
    major = MAJOR(dev_no);
    minor = MINOR(dev_no);
    printk("[%s] alloc_chrdev_region() success. major=%d, minor=%d\n", THIS_MODULE->name, major, minor);

    pclass = class_create(THIS_MODULE, "cs_gpio");
    if (IS_ERR(pclass))
    {
        printk("[%s] class_create() failed.\n", THIS_MODULE->name);
        unregister_chrdev_region(dev_no, N_DEVICES); // number of device numbers to unregister
        return -1;
    }
    printk("[%s] class_create() success.\n", THIS_MODULE->name);
    pdevice = device_create(pclass, NULL, dev_no, NULL, "cs_gpio0");
    if (IS_ERR(pclass))
    {
        printk("[%s] device_create() failed.\n", THIS_MODULE->name);
        class_destroy(pclass);
        unregister_chrdev_region(dev_no, N_DEVICES); // number of device numbers to unregister
        return -1;
    }
    printk("[%s] device_create() success.\n", THIS_MODULE->name);

    cdev_init(&cdev, &fops);
    ret = cdev_add(&cdev, dev_no, N_DEVICES);
    if (ret < 0)
    {
        printk("[%s] cdev_add() failed.\n", THIS_MODULE->name);
        device_destroy(pclass, dev_no);
        class_destroy(pclass);
        unregister_chrdev_region(dev_no, N_DEVICES);
        return -1;
    }
    printk("[%s] cdev_add() success: Add char device to system.\n", THIS_MODULE->name);

    return 0;
}

static __exit void gpio_exit(void)
{
    printk("[%s] gpio_exit() called.\n", THIS_MODULE->name);

    cdev_del(&cdev);
    printk("[%s] cdev_del(), remove char device from system.\n", THIS_MODULE->name);
    
    device_destroy(pclass, dev_no);
    printk("[%s] device_destroy(), destroyed device.\n", THIS_MODULE->name);
    
    class_destroy(pclass);
    printk("[%s] class_destroy(), destroyed class.\n", THIS_MODULE->name);

    unregister_chrdev_region(dev_no, N_DEVICES);
    printk("[%s] unregister_chrdev_region(), unregistered driver from system.\n", THIS_MODULE->name);
}

module_init(gpio_init);
module_exit(gpio_exit);

MODULE_AUTHOR("Rohit Nimakr <nehalnimkar@gmail.com>");
MODULE_DESCRIPTION("Toggle led using ioctl commands");
MODULE_LICENSE("GPL");