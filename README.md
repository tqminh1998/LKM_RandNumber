# LKM_RandNumber
## Set up
### Install linux header
```
sudo apt-get update
apt-cache search linux-headers-$(uname -r)
```
* After doing those command, get the version of linux header and install it: 
```
sudo apt-get install <linux-header+version>
```
## Compile and run
* Open this repository in terminal
* Make file:
```
make
```
* Insert module
```
sudo insmod randmodule.ko  
```
* Run user app to get random number
```
./test
```
* Remove module from the kernel
```
sudo rmmod randmodule
```
## View process
* See printk() message
```
dmesg
```
* View all module
```
lsmod
```
* View all device file
```
cd /dev
ls
```
* View major, minor, type of device...
```
ls -l /dev/<devicefile_name>/
```
