# Nicholas LaMonica  
# CSCI 3537 Programming Assignment 2  

## test.c
My test file for reading and writing to the device file through system calls.
w, r, s are commands for writing to, reading from, seeking in the device file.  And e is to exit the program.

## simple_char_driver.c
This the my implementation for a simple device driver. It is loaded into the kernel as a loadable kernel module

## Makefile
Contains the simple_char_driver.c object executable file.
run


## Instructions on running :
1. Making the device file :
	- sudo mknod –m 777 /dev/simple_character_device c 270 0
2. Running the make file to build the kernel module:
	- sudo make -C /lib/modules/$(uname -r)/build M=$PWD
3. Installing the kernel module:
	- sudo insmod simple_char_driver.ko
4. Compiling and running the test file:
	- gcc -o test test.c
	- ./test
5. Uninstalling the kernel module:
	- sudo rmmod simple_character_driver  


Must be on a linux distrobution
