#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/types.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KETAN");

/*dev will contain device number within kernel*/
static dev_t dev_no;
/*count will be number of devices required*/
static int count = 1;

struct scull_dev{
	/*Private data members*/
	struct cdev cdev;	/*This is character device structure*/
};

struct scull_dev *dev;

int scull_open(struct inode *inode, struct file *filp)
{
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

static int scull_init(void)
{
	int ret;
	int firstminor = 0;
	int scull_major;
	/*Name of device driver*/
	const char *name = "scull\n";
	/*Request dynamic allocation of major number*/
	ret = alloc_chrdev_region(&dev_no, firstminor, count, name);
	scull_major = MAJOR(dev_no);
	if (ret <  0)
		pr_warning("Scull didn't get major number %d \n", scull_major);
	else
		pr_alert("We get major number %d \n", scull_major);
	/*Device registration in scull*/
	cdev_init(&dev->cdev, &scull_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops   = &scull_fops;
	ret = cdev_add(&dev->cdev, dev_no, count);
	if (ret)
		pr_notice("Error %d adding cdev\n",ret);
	return 0;
}

static void scull_clear(void)
{
	cdev_del(&dev->cdev);
	unregister_chrdev_region(dev_no, count);
        pr_debug("Bye\n");
}

module_init(scull_init);
module_exit(scull_clear);
