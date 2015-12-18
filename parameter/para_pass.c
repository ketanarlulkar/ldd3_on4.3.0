#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KETAN");

static char *whom = "World";
static int howmany = 1;

module_param(howmany, int, S_IRUGO);
module_param(whom, charp, S_IRUGO);

static int hello_init(void)
{
	int i;
	for (i = 0; i < howmany; ++i)
	{
		pr_alert("Hello %s!\n",whom);
	}
	return 0;
}

static void hello_clear(void)
{
	pr_debug("Bye\n");
}

module_init(hello_init);
module_exit(hello_clear);