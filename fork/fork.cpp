#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(int argc, char** argv)
{
    if(argc > 2)
    {
        std::cout << "Too many arguments\n";
        exit(EXIT_FAILURE);
    }
    int new_process = fork();
    if(new_process < 0)
    {

    }
    else if(new_process == 0)
    {
        if(execlp("ls", "-a", "-l", NULL) < 0)
        {
            perror(strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    wait(nullptr);
}