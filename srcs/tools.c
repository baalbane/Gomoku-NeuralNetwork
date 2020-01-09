
#include "gomoku.h"

void	Write(int fd, void *value, size_t size) {
	if (write(fd, value, size) != size) {
		printf("ERROR TRYING TO WRITE TO FD\n");
		exit(0);
	}
}


void	Read(int fd, void *buf, size_t size) {
	if (read(fd, buf, size) != size) {
		printf("ERROR TRYING TO READ TO FD\n");
		exit(0);
	}
}









