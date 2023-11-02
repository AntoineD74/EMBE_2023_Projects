#include <fcntl.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "../include/modbus.h"

#define MAKE_16(higher, lower) (((uint16_t)higher << 8) | (uint16_t)lower)

int main(int argc, char *argv[]) {
    int fd;
    char response_buffer[2] = {0};

    uint8_t device = atoi(argv[1]);
    printf("Device modbus adress: %d\n", device);

    unsigned char command = argv[2][0];
    printf("Command entered: %c\n", command);
    
    uint16_t value;
    if (strcmp(argv[3], "HIGH") == 0) {
        value = 1;
    }
    else if (strcmp(argv[3], "LOW") == 0){
        value = 2;
    }
    else {
        perror("Wrong value entered\n");
        return -1;
    }
    printf("Value to send: %d\n\n", value);
    
    if ((fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY)) < 0) {
        perror("UART: Failed to open the fd.\n");
        return -1;
    }

    struct termios options;
    tcgetattr(fd, &options);

    options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
    options.c_iflag = IGNPAR | ICRNL;
    cfmakeraw(&options);

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);
    switch (command) {
        case 'r':
            if (readHoldingRegisters(response_buffer, fd, device, 0x01, 0x01) != 0) {
                printf("Error on read command\n");
            } else {
                printf("Read %04x\n", MAKE_16(response_buffer[0], response_buffer[1]));
            }
            break;

        case 'w':
            writeSingleRegister(fd, device, 0x01, value);
            break;

        default:
            printf("Wrong call of the program");
            return -1;
    }

    close(fd);

    return 0;
}
