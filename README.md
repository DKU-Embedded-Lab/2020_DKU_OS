# 2020_DKU_OS


This is a place for studying Operaing System in Dankook University.
- Professor : Jongmoo Choi
- Assistant : Gunhee Choi (Email : choi_gunhee@dankook.ac.kr)
- 2020 DKU Operating System Course Information [link](http://embedded.dankook.ac.kr/~choijm/course/course.html)


## Download Environment
Virtual Machine Platform: VirtualBox 6.X [link](https://www.virtualbox.org/)

Operating System: Ubuntu 18.04 [link](https://ubuntu.com/download/desktop)

This environment is based on VirtualBox 6.X.
If image execution is not possible, we recommend reinstalling VirtualBox 5.2 version.

[Virtual Box 5.2 Link](https://www.virtualbox.org/wiki/Download_Old_Builds_5_2)


## Clone Git Repository

```
git clone https://github.com/DKU-Embedded-Lab/2020_DKU_OS.git
```

If you get an error there are no commands, install git with following command :

```
sudo apt-get install git
```

## Lab0
lab0 contains information about installing a virtual machine and Ubuntu.

Set the environment according to the documentation.

The documentation for lab0 is at the link below.

The OS image to use for preferences is also on the link below.

Documentation for conducting the assignment can be found at following link :

#### [Lab0 Documentation Link](https://drive.google.com/open?id=1JIwUUl1qL7z290NiIqOBuZLzkXMjQn55)

#### DKU_OS Image Download [link](https://drive.google.com/open?id=1uCVLdL9EdkVZhmvn4egd2p4aErwybT2i)

## Lab1
If you want to proceed to Lab1, go to command below :

```
cd lab1_sched

make

./lab1_sched
```

Documentation for conducting the assignment can be found at following link :

#### [Lab1 Documentation Link](https://drive.google.com/open?id=1LzeJsWAyntJVmKHmIJl5YgBYGpVuNCs0)

## Lab2

If you want to proceed to Lab2, go to command below :

```
cd lab2_sync

make lab2_example

./lab2_example –t 4 –i 1000000 –s o

./lab2_example –t 4 –i 1000000 –s m
```

The above command is an example.

See the documentation for details.

Documentation for conducting the assignment can be found at following link :

#### [Lab2 Documentation Link](https://drive.google.com/open?id=1lLiAKuQeW7PDoqocLSaPB4N2v9GvcSHR)

## Lab3

If you want to proceed to Lab3, go to command below :

```
cd lab3_filesystem

make

insmod ramdisk.ko

mkfs.ext2 /dev/ramdisk

mkdir mnt

mount /dev/ramdisk ./mnt

./create.sh

./apd mnt/A/BC 13 A/BC-13      //A, B, C is your last 3 digit of you student ID : XXXXXABC
```

then you can proceed Lab3

If you want to proceed to Lab3-Bonus, go to command below :

```
umount /dev/ramdisk

rmmod ramdisk

insmod ramdisk.ko

cd os_ext2          //change the source code

make

insmod os_ext2.ko

mkfs.ext2 /dev/ramdisk

mount -t os_ext /dev/ramdisk ../mnt

dmesg | grep os_ext2
```

See the documentation for details.

Documentation for conducting the assignment can be found at following link :

#### [Lab3 Documentation Link](https://drive.google.com/file/d/1k4MYyxDebCe5RiHk3o1-0pL3-mek5ual/view?usp=sharing)
