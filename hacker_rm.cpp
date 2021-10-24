#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstring>
#include <string.h>
#include <sstream>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>



#define PROMT_ERROR(msg, errno_backup) \
{ \
	int errno_backup = errno_backup; \
	fprintf(stderr, "%s", msg); \
	fprintf(stderr, "file: %s, line: %d\n", __FILE__, __LINE__); \
    perror(strerror(errno_backup)); \
}


int main(int argc, char const *argv[])
{
    struct stat stbuf;
    stat(argv[1], &stbuf);
    int fd = open(argv[1], O_WRONLY);
    if (fd < 0)
    {
        int errno_copy = errno;
		std::ostringstream err;
        err << "error occured during opening the file: " << argv[1] << "\n";
		PROMT_ERROR(err.str().c_str(), errno_copy);
    }
    char *buf= new char[stbuf.st_size];
    memset(buf, '\0', stbuf.st_size);

    write(fd, buf, stbuf.st_size);

    close(fd);

    delete[] buf; 

    remove(argv[1]);
}