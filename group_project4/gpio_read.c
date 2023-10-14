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

    struct timespec start_time, end_time;
    char state = '0';  

    char c;
    while(1){
        fd = open("/sys/class/gpio/gpio17/value", O_RDONLY);  
        read(fd, &c, 1);
        close(fd);

        if(c == '0'){
            printf("Pin state is LOW\n");
        }
        else{
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