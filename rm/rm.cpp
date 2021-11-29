#include <iostream>
#include <chrono>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ftw.h>
#include <string>

#define PROMT_ERROR(msg, errno_backup) \
{ \
	int errno_backup = errno_backup; \
	fprintf(stderr, "%s", msg); \
	fprintf(stderr, "file: %s, line: %d\n", __FILE__, __LINE__); \
    perror(strerror(errno_backup)); \
} 

int apply_to_file(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    remove(fpath);
}



int main(int argc, const char** argv)
{
    //std::cout << argv[1];
    if(strcmp(argv[1], "-r") == 0)
    {
        for(int i = 2; i < argc; ++i)
        {
            nftw(argv[i], apply_to_file, 100, FTW_DEPTH);
        }
    }
    else
    {
        for(int i = 1; i < argc; ++i)
        {
            int fd = open(argv[i], O_RDONLY);
            if(fd < 0)
	        {
		        int errno_copy = errno;
		        std::ostringstream err;
                err << "error occured during opening the file: " << argv[i] << "\n";
		        PROMT_ERROR(err.str().c_str(), errno_copy);
	        }
            struct stat buffer;
            int status = fstat(fd, &buffer);
            if(S_ISDIR(buffer.st_mode))
            {
                int errno_copy = errno;
		        std::ostringstream err;
                err << "error occured during opening the file: " << argv[i] << "\n";
		        PROMT_ERROR(err.str().c_str(), errno_copy);
            }
            if(S_ISREG(buffer.st_mode))
            {
                unlink(argv[i]);
            }
        }
    }
    //nftw(argv[1], apply_to_file, 100, FTW_DEPTH);
}