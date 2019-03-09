//
//  myCharDevice.c
//  
//
//  Created by Kaister Wang on 2019/3/5.
//

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kdev_t.h>
#include <linux/init.h>
#include <linux/uaccess.h>

MODULE_AUTHOR("WYK");
MODULE_LICENSE("GPL");

char buffer[1024];
char devName[] = "myCharDevice";
int majorNumber = 456;

static ssize_t chrdev_read(struct file*, char*, size_t, loff_t *);
static ssize_t chrdev_write(struct file*, const char*, size_t, loff_t*);
static int chrdev_open(struct inode *inode, struct file *fp);
static int chrdev_release(struct inode *inode, struct file *fp);
/*结构体file_operations在头文件linux/fs.h中定义，用来存储驱动内核模块提供的对设备进行各种操作的指针。
 该结构体的每个域都对应着驱动内核模块用来处理某个被请求的事物的函数的地址。
 设备“chrdev”e的基本入口点结构变量chrdev_fops
 */
struct file_operations chrdev_fops =
{
    .open = chrdev_open,
    .release = chrdev_release,
    .read = chrdev_read,
    //用来从设备中获取数据，在这个位置的一个空指针导致 read 系统调用以-EINVAL("Invalid argument")失败，一个非负返回值代表了成功读取的字节数（返回值是一个”signed size“类型，常常是目标平台本地的类型。
    .write = chrdev_write,
    //发送z数据给设备，如果NULL，-EINVAL返回给调用 write 系统调用的程序，如果非负，返回值代表成功写的字节数。
};

int init_module(void)//初始化
{
    printk("register : %s\n", devName);
    if ((majorNumber = register_chrdev(0, devName, &chrdev_fops)) < 0)
    {
        printk("failed.\n");
        return -1;
    }
    printk("success.\n");
    printk("device number : %d\n", majorNumber);
    return 0;
}

void cleanup_module(void)//退出
{
    unregister_chrdev(majorNumber, devName);
    printk("unregistered.\n");
}

int chrdev_open(struct inode *inode, struct file *fp)
{
    if(try_module_get(THIS_MODULE) == 0)
    {
        printk("open device failed!\n");
        return -2;
    }
    printk("open device success!\n");
   return 0;
}

int chrdev_release(struct inode *inode, struct file *fp)
{
    module_put(THIS_MODULE);
    printk("release device success!\n");
    return 0;
}

ssize_t chrdev_read(struct file *file, char __user *user, size_t t, loff_t *f)
{
    if(copy_to_user(user, buffer, 1024))
        return -3;
    return 1024;
}

ssize_t chrdev_write(struct file *file, const char __user *user, size_t t, loff_t *f)
{
    if(copy_from_user(buffer, user, 1024))
        return -4;
    return 1024;
}
