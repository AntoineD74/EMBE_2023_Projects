#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int file, count;

    if (argc != 2) {
        printf("Please enter a state when launching\n");
        return -2;
    }

    if ((file = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY)) < 0) {
        perror("UART: Failed to open the file.\n");
        return -1;
    }

    struct termios options;

    tcgetattr(file, &options);

    cfmakeraw(&options);

    // 9600 Same as the Arduino
    options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
    options.c_iflag = IGNPAR | ICRNL;
    tcflush(file, TCIFLUSH);
    tcsetattr(file, TCSANOW, &options);

    int value;
    if (strcmp(argv[1], "HIGH") == 0) {
        value = 1;
    }
    else if (strcmp(argv[1], "LOW") == 0){
        value = 2;
    }
    else {
        perror("Wrong value entered\n");
        return -1;
    }

    if ((count = write(file, &value, sizeof(value))) < 0) {
        return -1;
    }
        

    usleep(100000);

    unsigned char receive[100];

    if ((count = read(file, (void *)receive, 100)) < 0) {
        perror("Failed to read from the input\n");
        return -1;
    }

    if (count == 0)
        printf("There was no data available to read!\n");
    else {
        receive[count] = 0;  // There is no null character sent by the Arduino
        printf("The following was read in [%d]: %s\n", count, receive);
    }

    close(file);
    return 0;
}
