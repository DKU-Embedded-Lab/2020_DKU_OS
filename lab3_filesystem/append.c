#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
        int fd, page = atoi(argv[2]);
	char *path = argv[1], *msg = argv[3], ent = 10;
        
	fd = open(path, O_RDWR|O_SYNC);

	lseek(fd, (page - 1) * 4096, SEEK_SET);
	write(fd, msg, strlen(msg));
	write(fd, &ent, 1);

#if _GNU_SOURCE
	syncfs(fd);
#endif
#if _XOPEN_SOURCE >= 500 || _DEFAULT_SOURCE || _BSD_SOURCE
	sync();
#endif

	close(fd);
	return 0;
}

