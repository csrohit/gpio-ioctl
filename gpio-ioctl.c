#include<linux/module.h>



static __init int gpio_init(void){
    printk("[%s] gpio_init() called.\n", THIS_MODULE->name);


    return 0;
}

static __exit void gpio_exit(void){
    printk("[%s] gpio_exit() called.\n", THIS_MODULE->name);
}


module_init(gpio_init);
module_exit(gpio_exit);

MODULE_AUTHOR("Rohit Nimakr <nehalnimkar@gmail.com>");
MODULE_DESCRIPTION("Toggle led using ioctl commands");
MODULE_LICENSE("GPL");