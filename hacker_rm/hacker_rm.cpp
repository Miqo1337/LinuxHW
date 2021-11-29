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
#include <memory>



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
        exit(EXIT_FAILURE);
    }
    auto buf = std::make_unique<char[]>(stbuf.st_size);
    //auto buf = std::unique_ptr<T>(new T)
    memset(buf.get(), '\0', stbuf.st_size);
    write(fd, buf.get(), stbuf.st_size);
    close(fd);
    remove(argv[1]);
}