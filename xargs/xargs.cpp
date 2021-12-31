#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <cstring>



int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cerr << "Not correct amout of arguments\n" 
            << "In file:" << __FILE__ 
            << "on line:" << __LINE__ 
            << "\nError description:" << strerror(errno) << "\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        std::string args;
        while(std::cin >> args)
        {
            int new_process_id = fork();
            if(new_process_id < 0)
            {
                std::cerr << "Something went wrong when creating new process\n" 
                    << "In file:" << __FILE__ 
                    << "on line:" << __LINE__ 
                    << "\nError description:" << strerror(errno) << "\n";
                exit(EXIT_FAILURE);
            }
            else if(new_process_id == 0)
            {
                //std::string cmnd = "/bin/" + argv[1];
                if(execlp(argv[1], argv[1], args.c_str(), nullptr) < 0)
                {
                    std::cerr << "Can't execute\n" 
                        << "In file:" << __FILE__ 
                        << "on line:" << __LINE__ 
                        << "\nError description:" << strerror(errno) << "\n";
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}