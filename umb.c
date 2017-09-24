/******************************************************************************************************
* UMB - Universal Modified  Bus Driver - simple USB driver for debugging  
* created date: 1 Feb 2013  
* updated on : 25 Feb 2014
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* 
#--------------------------------------------------------------------------------------------------------
#Copyleft (Copyright+):
#Srinivasan Kannan (alias) Ka.Shrinivaasan (alias) Shrinivas Kannan
#Ph: 9791499106, 9003082186
#Krishna iResearch Open Source Products Profiles:
#http://sourceforge.net/users/ka_shrinivaasan,
#https://github.com/shrinivaasanka,
#https://www.openhub.net/accounts/ka_shrinivaasan
#Personal website(research): https://sites.google.com/site/kuja27/
#emails: ka.shrinivaasan@gmail.com, shrinivas.kannan@gmail.com,
#kashrinivaasan@live.com
#--------------------------------------------------------------------------------------------------------
********************************************************************************************************/

/*
*##########################################################################################
* References, copyright attributions and quotes : 
*  1. Linux Device Driver 3rd edition example USB driver source code
*  2. Understanding Linux Kernel 3rd edition
*  3. Various USB debugging resources
*  
* Copyright attribution from Linux Device Driver 3rd edition :
* USB Skeleton driver - 2.0
* Copyright (C) 2001-2004 Greg Kroah-Hartman (greg@kroah.com)
*
*	This program is free software; you can redistribute it and/or
*	modify it under the terms of the GNU General Public License as
*	published by the Free Software Foundation, version 2.
*
* This driver is based on the 2.6.3 version of drivers/usb/usb-skeleton.c 
* but has been rewritten to be easy to read and use, as no locks are now
* needed anymore.
************************************************************************************************************
*/

/* 
 * Changes done for Linux Kernel 3.2.0 and 3.7.8:
 * 1.Removed BigKernelLock(BKL) and replaced it with a mutex
 * 2.Replaced usb_buffer_alloc() with usb_buffer_coherent
 * 3.Build errors fixed
 */

/*
#include <linux/config.h>
#include <linux/smp_lock.h>
*/
#include <umb.h>
#include <linux/mutex.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kref.h>
#include <linux/usb.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>

#define to_umb_driver(d) container_of(d, struct usb_umb, kref)

/*
#define UMB_VENDOR_ID	0xcde0
#define UMB_PRODUCT_ID	0xabc0
*/

/*
* SanDisk 
*--------
*/

/*
#define UMB_VENDOR_ID 0x0781
#define UMB_PRODUCT_ID 0x5567
*/

#define UMB_VENDOR_ID 0x12d1
#define UMB_PRODUCT_ID 0x140b

struct mutex umb_mutex;

MODULE_LICENSE("GPL");

static struct usb_device_id umb_table[] = {
	{ USB_DEVICE(UMB_VENDOR_ID, UMB_PRODUCT_ID)},
	/*{.driver_info = 42},*/
	{ }
};

/*MODULE_DEVICE_TABLE(usb, umb_table);*/

static ssize_t umb_read(struct file* f, char __user *buffer, size_t count, loff_t* ppos);
static ssize_t umb_write(struct file *f, const char __user *user_buffer, size_t count, loff_t *ppos);
static int umb_probe(struct usb_interface *interface, const struct usb_device_id* id);
static void umb_disconnect(struct usb_interface *interface);
static int umb_release(struct inode* inode, struct file* file);
static void umb_delete(struct kref* kref);
static int umb_open(struct inode* inode, struct file* file);
static char* umb_devnode(struct device *dev, mode_t *mode);

#define USB_UMB_MINOR_BASE	378	

static struct usb_driver umb_driver = {
	.name  = "umb",
	.id_table = umb_table,
	.probe = umb_probe,
	.disconnect = umb_disconnect
};

static struct file_operations umb_fops = {
	.owner = THIS_MODULE,
	.read = umb_read,
	.write = umb_write,
	.open = umb_open,
	.release = umb_release
};

/*
 * umb_devnode callback added
 * - 12 Feb 2013 ka.shrinivaasan@gmail.com
 */
static struct usb_class_driver umb_class = {
		/* 
			_class_register() requires name to be set due to printk in drivers/base/class.c
			- Ka.Shrinivaasan 27 February 2014
		*/
		.name = "usb/umb%d",
		.fops = &umb_fops,
		/*.mode = S_IFCHR | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH,*/
		/*.devnode = umb_devnode,*/
		/*.minor_base = USB_UMB_MINOR_BASE,*/
};

struct usb_umb {
	struct usb_device* udev;
	struct usb_interface* interface;
	unsigned char* bulk_in_buffer;
	/* 
	 * bulk_out_buffer added
	 * - ka.shrinivaasan 8 Feb 2013 
	 */
	unsigned char* bulk_out_buffer;
	size_t bulk_in_size;
	__u8 bulk_in_endpointAddr;
	__u8 bulk_out_endpointAddr;
	struct kref kref;
};

static ssize_t umb_read(struct file* f, char __user *buffer, size_t count, loff_t* ppos)
{
	struct usb_umb *dev;
        int retval = 0;
	int ulcount = (int)count;
        printk(KERN_INFO "umb_read(): before usb_bulk_msg\n");
        dev = (struct usb_umb*)f->private_data;
        dev->bulk_in_buffer=kstrdup(virgo_kernel_analytics_conf[0].value,GFP_ATOMIC);
        retval = usb_bulk_msg(dev->udev,
                                                usb_rcvbulkpipe(dev->udev, dev->bulk_in_endpointAddr),
                                                dev->bulk_in_buffer,
                                                min(dev->bulk_in_size, count),
                                                &ulcount,
                                                HZ*10
                                                );
        printk(KERN_INFO "umb_read(): by default returns virgo_kernel_analytics_conf[0].value, exported by kernel_analytics module\n");
        printk(KERN_INFO "umb_read(): before copy_to_user(): user buffer=%s, dev->bulk_in_buffer=%s\n", buffer, dev->bulk_in_buffer);
        copy_to_user(buffer, dev->bulk_in_buffer, strlen(dev->bulk_in_buffer));
        printk(KERN_INFO "umb_read(): after copy_to_user(): user buffer=%s, dev->bulk_in_buffer=%s\n", buffer, dev->bulk_in_buffer);
        return retval;
}

static void umb_write_bulk_callback(struct urb *urb)
{
	printk(KERN_INFO "umb_write_bulk_callback(): in function %s and urb status is %d \n", urb->status);
}

static ssize_t umb_write(struct file *f, const char __user *user_buffer, size_t count, loff_t *ppos)
{
	struct usb_umb *dev;
	int retval = 0;
	struct urb *urb = NULL;
	char *buf = NULL;
	
	printk(KERN_INFO "umb_write(): before usb_alloc_urb\n");
	printk(KERN_INFO "umb_write(): user_buffer = %s\n", user_buffer);
	dev = (struct usb_umb*)f->private_data;
	
	if (count == 0)
		return -1;
	
	/*alloc urb and buffer, copy from userspace*/
	urb = usb_alloc_urb(0, GFP_ATOMIC);
	/*buf = usb_buffer_alloc(dev->udev, count, GFP_ATOMIC, &urb->transfer_dma);*/
	buf = usb_alloc_coherent(dev->udev, count, GFP_ATOMIC, &urb->transfer_dma);
	printk(KERN_INFO "umb_write(): before copy_from_user(): buf=%s\n", buf);
	copy_from_user(buf, user_buffer, count);
	printk(KERN_INFO "umb_write(): after copy_from_user(): buf=%s\n", buf);
	
	/*fill urb and submit */
	usb_fill_bulk_urb(urb, dev->udev, usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
						buf, count, umb_write_bulk_callback, dev);
	urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
	retval = usb_submit_urb(urb, GFP_ATOMIC);
	usb_free_urb(urb);
	return count;
}

static char* umb_devnode(struct device *dev, mode_t *mode)
{
	return "mode";
}

static int umb_init(void)
{
	int result;
	result = usb_register(&umb_driver);
	printk(KERN_INFO "umb_init(): in init ...after usb_register(), result %d\n", result);
	return result;
}

static void umb_exit(void)
{
	usb_deregister(&umb_driver);
	printk(KERN_INFO "umb_exit(): in exit\n");
	return 0;
}

static int umb_probe(struct usb_interface *interface, const struct usb_device_id* id)
{
	struct usb_umb* dev=NULL;
	struct usb_host_interface* iface_desc;
	struct usb_endpoint_descriptor* endpoint;
	size_t buffer_size;
	int i;
	int retval = -ENOMEM;

	printk(KERN_INFO "umb_probe(): before probing\n"); 		
	dev=kzalloc(sizeof(struct usb_umb), GFP_ATOMIC);
	memset(dev, 0x0, sizeof(struct usb_umb));
	printk(KERN_INFO "umb_probe(): before kref_init\n");
	kref_init(&dev->kref);
	
	printk(KERN_INFO "umb_probe(): before usb_get_dev\n");
	dev->udev = usb_get_dev(interface_to_usbdev(interface));
	dev->interface = interface;
	
	iface_desc = interface->cur_altsetting;
	for(i=0; i < iface_desc->desc.bNumEndpoints; i++)
	{
		printk(KERN_INFO "umb_probe(): looping over interface endpoints: i= %d\n",i);
		endpoint = &iface_desc->endpoint[i].desc;
		if(!dev->bulk_in_endpointAddr &&
				(endpoint->bEndpointAddress & USB_DIR_IN) &&
				((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK)
				 == USB_ENDPOINT_XFER_BULK))
		{
			printk(KERN_INFO "umb_probe(): before initializing dev bulk in data\n");
			buffer_size = endpoint->wMaxPacketSize;
			dev->bulk_in_size = buffer_size;
			dev->bulk_in_endpointAddr = endpoint->bEndpointAddress;
			dev->bulk_in_buffer = kzalloc(buffer_size, GFP_ATOMIC);
		}
		if(!dev->bulk_out_endpointAddr &&
						(endpoint->bEndpointAddress & USB_DIR_OUT) &&
						((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK)
						 == USB_ENDPOINT_XFER_BULK))
		{
			printk(KERN_INFO "umb_probe(): before allocating dev bulk out data\n");
			dev->bulk_out_buffer = kzalloc(buffer_size,GFP_ATOMIC);
			dev->bulk_out_endpointAddr= endpoint->bEndpointAddress;
		}
	}
	printk(KERN_INFO "umb_probe(): before usb_set_intfdata\n");
	usb_set_intfdata(interface, dev);
	printk(KERN_INFO "umb_probe(): before usb_register_dev, interface=%p, umb_class=%p\n", interface, &umb_class);
	usb_register_dev(interface, &umb_class);
	return 0;
}

static void umb_disconnect(struct usb_interface *interface)
{
	struct usb_umb *dev;
	int minor = interface->minor;

	/*
	commenting mutex lock and unlock due to crash in mutex_lock_slow_path which is already an open
	bug filed in a previous linux version. Could be related.
	-Ka.Shrinivaasan 5March2014
	*/
	
	/*lock_kernel();*/
	/*mutex_lock(&umb_mutex);*/
	printk(KERN_INFO "umb_disconnect(): before deregistering device");
	dev = usb_get_intfdata(interface);
	usb_set_intfdata(interface,NULL);
	usb_deregister_dev(interface, &umb_class);
	/*unlock_kernel();*/
	/*mutex_unlock(&umb_mutex);*/
	kref_put(&dev->kref, umb_delete);
}

static int umb_open(struct inode* inode, struct file* file)
{
	struct usb_umb *dev;
	struct usb_interface *interface;
	int subminor;
	int retval = 0;
	
	subminor = iminor(inode);

	printk(KERN_INFO "umb_open(): subminor = %d\n",subminor);
	
	interface = usb_find_interface(&umb_driver, subminor);
	dev = usb_get_intfdata(interface);
	
	kref_get(&dev->kref);
	
	file->private_data = dev;
	printk(KERN_INFO "umb_open(): file->private_data = %p, interface = %p\n",file->private_data,interface);
	return retval;
}

static int umb_release(struct inode* inode, struct file* file)
{
	struct usb_umb *dev;
	dev = (struct usb_umb*)file->private_data;
	printk(KERN_INFO "umb_release(): dev=%p\n",dev);
	kref_put(&dev->kref, umb_delete);
}

static void umb_delete(struct kref* kref)
{
	struct usb_umb* dev = to_umb_driver(kref);
	printk(KERN_INFO "umb_delete(): dev=%p\n",dev); 
	usb_put_dev(dev->udev);
	kfree(dev->bulk_in_buffer);
	kfree(dev);
}

module_init(umb_init);
module_exit(umb_exit);

