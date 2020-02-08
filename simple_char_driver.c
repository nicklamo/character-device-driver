#include<linux/init.h>
#include<linux/module.h>

#include<linux/fs.h>
#include<linux/sched.h>
#include<linux/slab.h>
#include<linux/uaccess.h>



#define BUFFER_SIZE 1024

/* Define device_buffer and other global data structures you will need here */
//device buffer is an array with the device data of size buffer size

#define major_num 270
#define device_name "simple_character_device"
MODULE_LICENSE("GPL");
char *device_buffer;
int opened = 0;
int closed = 0;


ssize_t simple_char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer to where you are writing the data you want to be read from the device file*/
	/* length is the length of the userspace buffer*/
	/* offset will be set to current position of the opened file after read*/
	/* copy_to_user function: source is device_buffer and destination is the userspace buffer *buffer */
	int num_bytes_read = 0;
	printk(KERN_ALERT "In Read function");
	//need to check that we are not reading out of bounds of the buffer size
	//if we are, read the most that we can without going out of bounds
	if (*offset + length > BUFFER_SIZE){
		// writing out of bounds, need to resize length
		length -= ((*offset+length) - BUFFER_SIZE);
	} // otherwise length is fine

	num_bytes_read = length;
	copy_to_user(buffer, device_buffer + *offset, length);
	printk(KERN_ALERT "Number of bytes read: %d", num_bytes_read);

	// update file pos to last byte read
	*offset = *offset+length;
	return *offset+length;
}



ssize_t simple_char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer where you are writing the data you want to be written in the device file*/
	/* length is the length of the userspace buffer*/
	/* current position of the opened file*/
	/* copy_from_user function: destination is device_buffer and source is the userspace buffer *buffer */
	int num_bytes_write = 0;
	printk(KERN_ALERT "In write function");
	//need to check that we are not writing out of bounds of the buffer size
	//if we are, write the most that we can without going out of bounds
	if (*offset + length > BUFFER_SIZE){
		// writing out of bounds, need to resize length
		length -= ((*offset+length) - BUFFER_SIZE);
	} // otherwise length is fine

	num_bytes_write = length;
	copy_from_user(device_buffer + *offset, buffer, length);
	printk(KERN_ALERT "Number of bytes written to: %d", num_bytes_write);

	//update file position to last byte written
	*offset = *offset+length;

	return *offset+length;
}


int simple_char_driver_open (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is opened and also print the number of times this device has been opened until now*/
	opened += 1;
	printk(KERN_ALERT "Device opened %d times", opened);
	return 0;
}

int simple_char_driver_close (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is closed and also print the number of times this device has been closed until now*/
	closed += 1;
	printk(KERN_ALERT "Device closed %d times", closed);
	return 0;
}

loff_t simple_char_driver_seek (struct file *pfile, loff_t offset, int whence)
{
	/* Update open file position according to the values of offset and whence */
	//returns loff_t
	//check whence and update new_pos accordingly
	loff_t new_pos;
	if (whence == 0){
		new_pos = offset;
	} else if (whence == 1){
		new_pos = pfile->f_pos + offset;
	} else if (whence == 2) {
		new_pos = BUFFER_SIZE - offset;
	} else {
		printk(KERN_ALERT "Invalid whence");
		return pfile->f_pos;
	}
	//if new pos is out of bounds do not update
	if (new_pos < 0 || new_pos > BUFFER_SIZE){
		printk(KERN_ALERT "Seeking out of bounds");
		return pfile->f_pos;
	}
	// now log and return new position and set to f_pos
	printk(KERN_ALERT "New position %lld", new_pos);
	pfile->f_pos = new_pos;
	return pfile->f_pos;
}

struct file_operations simple_char_driver_file_operations = {
	.owner   = THIS_MODULE,
	/* add the function pointers to point to the corresponding file operations. look at the file fs.h in the linux souce code*/
	.open		 = simple_char_driver_open,
	.release = simple_char_driver_close,
	.read		 = simple_char_driver_read,
	.write 	 = simple_char_driver_write,
	.llseek	 = simple_char_driver_seek
};

static int simple_char_driver_init(void)
{
	/* print to the log file that the init function is called.*/
	printk(KERN_ALERT "Init function called.");
	/* register the device and allocate memory for device buffer*/
	register_chrdev(major_num, device_name, &simple_char_driver_file_operations);
	device_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
	//set all the data in the newly allocated to null to get rid of garbage data
	memset(device_buffer, '\0', BUFFER_SIZE);
	return 0;
}

static void simple_char_driver_exit(void)
{
	/* print to the log file that the exit function is called.*/
	printk(KERN_ALERT "Exit function called.");
	/* unregister  the device using the register_chrdev() function and unallocate memory */
	kfree(device_buffer);
	unregister_chrdev(major_num, device_name);
}

/* add module_init and module_exit to point to the corresponding init and exit function*/
module_init(simple_char_driver_init);
module_exit(simple_char_driver_exit);
