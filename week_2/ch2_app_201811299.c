#include <stdio.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define IOCTL_START_NUM 0x80
#define IOCTL_NUM1 IOCTL_START_NUM+1
#define IOCTL_NUM2 IOCTL_START_NUM+2
#define IOCTL_NUM3 IOCTL_START_NUM+3
#define IOCTL_NUM4 IOCTL_START_NUM+4

#define MOD_IOCTL_NUM 'z'
#define GET_IOCTL  _IOWR(MOD_IOCTL_NUM, IOCTL_NUM1, unsigned long)
#define SET_IOCTL  _IOWR(MOD_IOCTL_NUM, IOCTL_NUM2, unsigned long)
#define ADD_IOCTL  _IOWR(MOD_IOCTL_NUM, IOCTL_NUM3, unsigned long)
#define MUL_IOCTL  _IOWR(MOD_IOCTL_NUM, IOCTL_NUM4, unsigned long)

void main(void){
	int dev;
	long result = 200;

	dev = open("/dev/ch2_dev", O_RDWR);

	ioctl(dev, SET_IOCTL, 200);
	result = ioctl(dev, GET_IOCTL, NULL);
	printf("first: %ld\n", result);

	ioctl(dev, ADD_IOCTL, 300);
	result = ioctl(dev, GET_IOCTL, NULL);
        printf("second: %ld\n", result);

	ioctl(dev, MUL_IOCTL, 100);
	result = ioctl(dev, GET_IOCTL, NULL);
        printf("third: %ld\n", result);

	close(dev);
}


