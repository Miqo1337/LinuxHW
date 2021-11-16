#ifndef __MAGIC__
#define __MAGIC__
#include <sstream>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define PROMPT_ERROR(msg, errno_backup) \
{ \
	int errno_backup = errno_backup; \
	fprintf(stderr, "%s", msg); \
	fprintf(stderr, "file: %s, line: %d\n", __FILE__, __LINE__); \
    perror(strerror(errno_backup)); \
} 



void do_magic()
{
    int fd = open("new_pts_0", O_RDWR | O_CREAT, S_IRWXU);
    if(fd < 0)
    {
        int errno_copy = errno;
        std::ostringstream err;
        err << "error occured during opening the file: exclusive_file.log" << "\n";
        PROMPT_ERROR(err.str().c_str(), errno_copy);
        exit(EXIT_FAILURE);
    }
    int cin_check = dup2(fd, 0);
    if(cin_check == -1)
    {
        int errno_copy = errno;
        std::ostringstream err;
        err << "error occured during opening the file: exclusive_file.log" << "\n";
        PROMPT_ERROR(err.str().c_str(), errno_copy);
        exit(EXIT_FAILURE);
    }
    int cout_check =  dup2(fd, 1);
    if(cout_check == -1)
    {
        int errno_copy = errno;
        std::ostringstream err;
        err << "error occured during opening the file: exclusive_file.log" << "\n";
        PROMPT_ERROR(err.str().c_str(), errno_copy);
        exit(EXIT_FAILURE);
    }
    close(fd);
}


#endif