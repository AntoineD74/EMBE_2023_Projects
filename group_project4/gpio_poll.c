#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <time.h>

int main()
{
    //Enable gpio17 and gpio18
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    write(fd, "17", 2);
    write(fd, "18", 2);
    close(fd);

    //Set gpio17 as input
    fd = open("/sys/class/gpio/gpio17/direction", O_WRONLY);
    write(fd, "in", 2);
    close(fd);

    //Set gpio18 as output
    fd = open("/sys/class/gpio/gpio18/direction", O_WRONLY);
    write(fd, "out", 3);
    close(fd);

    //Set gpio17 interrupt
    fd = open("/sys/class/gpio/gpio17/edge", O_WRONLY);
    //write(fd, "falling", 7);
    write(fd, "both", 4);
    close(fd);

    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLPRI;

    struct timespec start_time, end_time;

    while(1){
        //Wait for event
        fd = open("/sys/class/gpio/gpio17/value", O_RDONLY);       
        int ret = poll(&pfd, 1, 3000);
        char c;
        read(fd, &c, 1);
        close(fd);

        if(ret == 0){
            printf("Timeout\n");
        }
            
        else
            if(c == '0') {
                printf("Pin state is LOW\n");
            }
            else {
                clock_gettime(CLOCK_MONOTONIC, &start_time);
                printf("Pin state is HIGH");
                fd = open("/sys/class/gpio/gpio18/value", O_RDONLY);    //setting gpio18 as HIGH
                write(fd, "1", 1);
                close(fd);
                clock_gettime(CLOCK_MONOTONIC, &end_time);
                break;
            }  
    }

    long elapsed_ns = (end_time.tv_sec - start_time.tv_sec) * 1e9 + (end_time.tv_nsec - start_time.tv_nsec);
    printf("\nTime elapsed: %ld nanoseconds\n", elapsed_ns);

    //Disable gpio17 and gpio18
    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    write(fd, "17", 2);
    write(fd, "18", 2);
    close(fd);

    return(0);
}