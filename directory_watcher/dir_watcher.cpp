#include <iostream>
#include <sys/inotify.h>
#include <string.h>
#include <poll.h>
#include <unistd.h>

static void handle_events(int fd, int* wd, int argc, char* argv[])
{
    char buf[4096];
    const struct inotify_event *event;
    ssize_t len;
    while(true)
    {
        len = read(fd, buf, sizeof(buf));
        if(len == -1 && errno != EAGAIN)
        {
            perror("Error while reading");
            exit(EXIT_FAILURE);
        }
        if(len <= 0)
            break;

        for(char* ptr = buf; ptr < buf + len; ptr += sizeof(struct inotify_event) + event->len)
        {
            event = (const struct inotify_event *) ptr;
            if (event->mask & IN_OPEN)
                printf("IN_OPEN: ");
            if (event->mask & IN_CLOSE_NOWRITE)
               printf("IN_CLOSE_NOWRITE: ");
            if (event->mask & IN_CLOSE_WRITE)
                printf("IN_CLOSE_WRITE: ");    
            if (event->mask & IN_ACCESS)
                printf("Accces: ");
            if (event->mask & IN_CREATE)
                printf("Created: ");
            if (event->mask & IN_DELETE)
                printf("Deleted: ");
            if (event->mask & IN_MODIFY)
                printf("Modified: ");
            if (event->mask & IN_MOVE_SELF)
                printf("Moved self: ");

            for(int i = 1; i < argc; ++i)
            {
                if(wd[i] == event->wd)
                {
                    std::cout << argv[i];
                    break;
                }
            }             
            if(event->len)
            {
                std::cout << event->name;
            }                  
            if(event->mask & IN_ISDIR)
            {
                printf(" [directory]\n");
            }
            else
            {
                printf(" [file]\n");
            }
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout << "Not enough arguments\n";
        exit(EXIT_FAILURE);
    }
        int inotify_fd = inotify_init();
        if(inotify_fd == -1)
        {
            perror("inotify_init");
            exit(EXIT_FAILURE);
        }
        int* wd = new int[argc - 1];
        if(wd == nullptr)
        {
            perror("Error why allocating memeory");
            exit(EXIT_FAILURE);
        }
        for(int i = 1; i < argc; ++i)
        {
            wd[i] = inotify_add_watch(inotify_fd, argv[i], IN_ACCESS | IN_OPEN | IN_CLOSE | IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVE_SELF);
            if(wd[i] == -1)
            {
                fprintf(stderr, "Cannot watch '%s' : %s\n", argv[i], strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
        nfds_t nfds = argc - 1;
        struct pollfd fds[2];
        fds[0].fd = STDIN_FILENO;
        fds[0].events = POLLIN;
        fds[1].fd = inotify_fd;
        fds[1].events = POLLIN;
        
        char buf;
        std::cout << "Listening for events.\n";
        while(true)
        {
            int poll_num = poll(fds, nfds, -1);
            if(poll_num == -1)
            {
                if(errno == EINTR)
                    continue;
                perror("poll");
                exit(EXIT_FAILURE);
            }
            if(poll_num > 0)
            {
                if(fds[0].revents & POLLIN)
                {
                    while(read(STDIN_FILENO, &buf, 1) > 0 && buf != '\n')
                        continue;
                    break;
                }
                if(fds[1].revents & POLLIN)
                {
                    handle_events(inotify_fd, wd, argc, argv);                   
                }
            }
        }
        std::cout << "Stopped listening\n";
        close(inotify_fd);
        delete [] wd;
}