#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
	int dev = open("/dev/mydevice", O_RDONLY);
	if (dev == -1) {
		printf("Opening not possible!\r\n");
		return -1;
	}
	printf("Open succsfull!\r\n");
	return 0;
}
