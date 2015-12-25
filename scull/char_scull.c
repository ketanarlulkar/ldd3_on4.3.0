#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KETAN");

/*dev will contain device number within kernel*/
static dev_t dev_no = 0;
/*count will be number of devices required*/
static int count = 1;
/*Find class in /sys/class*/
struct class *scull_class;

struct scull_dev{
	/*Private data members*/
	struct cdev cdev;	/*This is character device structure*/
};

struct scull_dev *dev;

int scull_open(struct inode *inode, struct file *filp)
{
	struct scull_dev *dev;
	dev  = container_of(inode->i_cdev, struct scull_dev, cdev);
	filp->private_data = dev;
	return 0;
}
ssize_t scull_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	
	return 0;
}
ssize_t scull_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	return 0;
}
int scull_release(struct inode *inode, struct file *filp)
{
	return 0;
}

struct file_operations scull_fops = {
	.owner   = THIS_MODULE,
	.open    = scull_open,
	.release = scull_release,
	.read    = scull_read,
	.write   = scull_write,
};

static void scull_clear(void)
{
	pr_alert("Cleaning\n");
	if(dev){
		pr_alert("Removing dev\n");
		cdev_del(&dev->cdev);
	}
	pr_alert("Destroying device in class");
	device_destroy(scull_class,dev_no);
	if(scull_class){
		pr_alert("Destroying Class");
		class_destroy(scull_class);
	}
	pr_alert("Deallocating Number\n");
	unregister_chrdev_region(dev_no, count);
        pr_alert("Bye\n");
}

static int scull_init(void)
{
	int ret;
	int firstminor = 0;
	int scull_major;
	struct device *scull_device;
	pr_alert("Request dynamic allocation of major number\n");
	ret = alloc_chrdev_region(&dev_no, firstminor, count, "scull");
	scull_major = MAJOR(dev_no);
	if (ret <  0){
		pr_err("Scull didn't get major number, returns %d \n", ret);
		goto fail;
	}
	else
		pr_alert("We get major number %d \n", scull_major);
	pr_alert("Creating device class for registering in sysfs\n");
	scull_class = class_create(THIS_MODULE, "Char_Driver_Scull");
	if (!scull_class){
		pr_err("Can not creat class, returns %p\n", scull_class);
		goto fail;
	}
	pr_alert("Creating device in Char_drive_Scull class\n");
	scull_device = device_create(scull_class, NULL, dev_no, NULL, "scull");
	if(!scull_device){
		pr_err("Device create failed with %p\n",scull_device);
		goto fail;
	}
	/*Device registration in scull*/
	dev = kmalloc(sizeof(struct scull_dev), GFP_KERNEL);
	if (!dev) {
		ret = -ENOMEM;
		pr_err("memory allocation for scull_dev failed, return %p",dev);
		goto fail;  /* Make this more graceful */
	}
	cdev_init(&dev->cdev, &scull_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops   = &scull_fops;
	ret = cdev_add(&dev->cdev, dev_no, count);
	if (ret){
		pr_err("Error %d adding cdev\n",ret);
		goto fail;
	}
	pr_alert("Done it\n");
	return 0;
fail:	scull_clear();
	return ret;
}

module_init(scull_init);
module_exit(scull_clear);
