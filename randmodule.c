#include <linux/init.h>
#include <linux/module.h>     
#include <linux/device.h>   
#include <linux/kernel.h>     
#include <linux/fs.h>      
#include <linux/uaccess.h>    
#include <linux/random.h>
#include <linux/libc-compat.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TRAN QUANG MINH");
MODULE_DESCRIPTION("A simple Linux character device for open and read a random number");
MODULE_VERSION("0.1"); 

#define DEVICE_NAME "randmodule"

static int major;
static int number_Opens = 0;
static struct class *randmoduleClass=NULL;
static struct device *randmoduleDev=NULL;

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);

static struct file_operations fops ={
        .open = dev_open,
        .read = dev_read,
        .release = dev_release,
};

static int __init randmodule_init(void){
    major = register_chrdev(0,DEVICE_NAME,&fops);
    if (major < 0){
        printk(KERN_ALERT"RANDMODULE: failed to register major number");
        return major;
    }

    printk(KERN_INFO"RANDMODULE: registered succesfully, major = %d", major);

    randmoduleClass = class_create(THIS_MODULE, "randmoduleClass");
    if (IS_ERR(randmoduleClass)){
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(randmoduleClass);
    }

    printk(KERN_INFO "RANDMODULE: device class registered correctly\n");

    randmoduleDev = device_create(randmoduleClass, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(randmoduleDev))
    {                        
        class_destroy(randmoduleClass); 
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(randmoduleDev);
    }
    printk(KERN_INFO "RANDMODULE: device class created correctly\n");
    return 0;
}

static void __exit randmodule_exit(void)
{
    device_destroy(randmoduleClass, MKDEV(major, 0)); 
    class_unregister(randmoduleClass);                      
    class_destroy(randmoduleClass);        
    unregister_chrdev(major, DEVICE_NAME);     

    printk(KERN_INFO "randmodule: Goodbye World");
}

static int dev_open(struct inode *inodep, struct file *filep)
{
    number_Opens++;
    printk(KERN_INFO "RANDMODULE: Device has been opened %d time(s)\n", number_Opens);
    return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
    char rand[8];
    get_random_bytes(rand, sizeof(rand));

    
    int error_count = 0;

    error_count = copy_to_user(buffer, rand, sizeof(rand));
    
    if (error_count == 0)
    { 
        printk(KERN_INFO "MODULERAND: Sent %d characters to the user\n", sizeof(rand));
        return 1;
    }
    else
    {
        printk(KERN_INFO "MODULERAND: Failed to send %d characters to the user\n", error_count);
        return -EFAULT; 
    }
}

static int dev_release(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "RANDMODULE: Device successfully closed\n");
    return 0;
}


module_init(randmodule_init);
module_exit(randmodule_exit);