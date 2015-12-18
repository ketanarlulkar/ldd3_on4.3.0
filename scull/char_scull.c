#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KETAN");

static dev_t dev;
static int count = 1;

static int scull_init(void)
{
	int ret;
	int firstminor = 0;
	int scull_major;
	const char *name = "scull\n";
	ret = alloc_chrdev_region(&dev, firstminor, count, name);
	scull_major = MAJOR(dev);
	if (ret <  0)
		pr_warning("Scull didn't get major number %d \n", scull_major);
	else
		pr_alert("We get major number %d \n", scull_major);
        return 0;
}

static void scull_clear(void)
{
	unregister_chrdev_region(dev, count);
        pr_debug("Bye\n");
}

module_init(scull_init);
module_exit(scull_clear);
