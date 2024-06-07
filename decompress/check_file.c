#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "big.h"

int open_file_reading(char str[]) {
	int fd = open(str, O_RDONLY);
	if(fd == -1) {
		printf("failed to open file\n");
		return -1;
	}
	return fd;
}


int main(int argc, char *argv[]) {
	int fd = open_file_reading(argv[1]);
	if(fd == -1) {
		return 1;
	}
	unsigned char ch;
	while(read(fd, &ch, 1)) {
		printf("%c, <%d>\n", ch, ch);
	}
	//close(fd);

	lseek(fd , -1 , SEEK_CUR); // curr
	read(fd, &ch, 1);
        printf("-----------%c, <%d>\n", ch, ch);
        


	close(fd);
	return 0;
}
