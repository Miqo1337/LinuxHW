#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <errno.h>
//#include <stdlib.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
//#include <ftw.h>
#include <string>
#include <memory>

#define PROMPT_ERROR(msg, errno_backup) \
{ \
	int errno_backup = errno_backup; \
	fprintf(stderr, "%s", msg); \
	fprintf(stderr, "file: %s, line: %d\n", __FILE__, __LINE__); \
    perror(strerror(errno_backup)); \
} 


int main(int argc, const char** argv)
{
    int fd = open("exclusive_file.log", O_RDWR | O_CREAT, S_IRWXU);
    int new_fd = dup(fd);
    if(fd < 0 || new_fd < 0)
    {
        int errno_copy = errno;
        std::ostringstream err;
        err << "error occured during opening the file: exclusive_file.log" << "\n";
        PROMPT_ERROR(err.str().c_str(), errno_copy);
        return EXIT_FAILURE;
    }
    else
    {
        if(new_fd < 0)
        {
            int errno_copy = errno;
            std::ostringstream err;
            err << "error occured during opening the file: exclusive_file.log" << "\n";
            PROMPT_ERROR(err.str().c_str(), errno_copy);
            return EXIT_FAILURE;
        }
        
    std::string text;
    std::cin >> text;
    write(fd, text.c_str(), text.size());
    std::string text1;
    std::cin >> text1;
    write(new_fd, "\n", 1);
    write(new_fd, text1.c_str(), text1.size());
    }
    close(new_fd);
    close(fd);
}
