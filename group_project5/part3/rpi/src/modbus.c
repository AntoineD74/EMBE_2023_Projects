#include "../include/modbus.h"

#define HIGHER_8(x) (x >> (8 * sizeof(x) - 8))
#define LOWER_8(x) (x & 0xff)
#define MAKE_16(higher, lower) (((uint16_t)higher << 8) | (uint16_t)lower)

void inline printPacket(char* packet, size_t length) {
    for (size_t i = 0; i < length; i++) {
        printf(" %02x", packet[i]);
    }
    printf("\n");
}

uint8_t readHoldingRegisters(char* response_buffer, uint8_t fd, uint8_t address, uint16_t first_register, uint16_t register_count) {
    char recv_packet[7];
    char packet[8] = {(char)address,
                      '\x03',
                      (char)HIGHER_8(first_register),
                      (char)LOWER_8(first_register),
                      (char)HIGHER_8(register_count),
                      (char)LOWER_8(register_count),
                      0,
                      0};

    setCrc(packet, sizeof(packet));

    printf("Sending packet:");
    printPacket(packet, sizeof(packet));

    if (write(fd, packet, sizeof(packet)) != sizeof(packet)) {
        perror("Failed to write read holding register packet\n");
        return -1;
    }

    if (read(fd, recv_packet, 1) != 1) {
        perror("Failed to read unit address from received packet\n");
        return -1;
    }

    if (recv_packet[0] != address) {
        fprintf(stderr, "Response form wrong address. Expected: %02x, got: %02x\n",
                address, recv_packet[0]);
        return -1;
    }

    if (read(fd, recv_packet + 1, 1) != 1) {
        perror("Failed to read return code from received  packet\n");
        return -1;
    }

    if (recv_packet[1] == '\x83') { // if error
        return handleExceptionCode(fd, first_register, register_count);
    } 
    else if (recv_packet[1] == '\x03') {  // if success

        if (read(fd, recv_packet + 2, 1) != 1) {
            perror("Failed to read number of bytes from received  packet\n");
            return -1;
        }

        printf("n bytes: %d\n", recv_packet[2]);

        // 2 extra bytes for crc
        if (read(fd, recv_packet + 3, recv_packet[2] + 2) !=
            recv_packet[2] + 2) {
            perror("Failed to read bytes in packet packet\n");
            return -1;
        }

        printf("Recieved packet: ");
        printPacket(recv_packet, sizeof(recv_packet));

        if (verifyCrc(recv_packet, sizeof(recv_packet)) != 0) {
            return -1;
        }

        memcpy(response_buffer, recv_packet + 3, recv_packet[2]);

        return 0;
    }

    fprintf(stderr, "Error: unexpected return code 0x%02x\n", recv_packet[0]);
    return 1;
}

uint8_t writeSingleRegister(uint8_t fd, uint8_t address, uint16_t register_address, uint16_t value) {
    uint16_t recv_address, recv_value;

    char recv_packet[8];
    char packet[8] = {(char)address,
                      '\x06',
                      (char)HIGHER_8(register_address),
                      (char)LOWER_8(register_address),
                      (char)HIGHER_8(value),
                      (char)LOWER_8(value),
                      0,
                      0};

    setCrc(packet, sizeof(packet));

    printf("Sending packet: ");
    printPacket(packet, sizeof(packet));

    if (write(fd, packet, sizeof(packet)) != sizeof(packet)) {
        perror("Failed to write write single register packet\n");
        return -1;
    }

	usleep(10000);

    if (read(fd, recv_packet, 1) != 1) {
        perror("Failed to read unit address from received packet\n");
        return -1;
    }

    if (recv_packet[0] != address) {
        fprintf(stderr, "Response form wrong address. Expected: %02x, got: %02x\n", address, recv_packet[0]);
        return -1;
    }

    if (read(fd, recv_packet + 1, 1) != 1) {
        perror("Failed to read return code from received packet\n");
        return -1;
    }
    
    if (recv_packet[1] == '\x86') { // if error
        return handleExceptionCode(fd, register_address, value);

    } else if (recv_packet[1] == '\x06') {  // if success

        if (read(fd, recv_packet + 2, 6) != 6) {
            perror("Failed to read contents of received  "
                "packet\n");
            return -1;
        }

        recv_address = MAKE_16(recv_packet[2], recv_packet[3]);
        recv_value = MAKE_16(recv_packet[4], recv_packet[5]);

        if (recv_address != register_address) {
            fprintf(stderr, "Recieved unexpected address after writeSingleRegister, "
                    "received: %04x, expected: %04x\n",
                    recv_address, register_address);
            return 1;
        }

        if (recv_value != value) {
            fprintf(stderr, "Recieved unexpected value after writeSingleRegister, "
                    "received: %04x, expected: %04x\n",
                    recv_value, value);
            return 1;
        }

        printf("Recieved packet: ");
        printPacket(recv_packet, sizeof(recv_packet));

        return verifyCrc(recv_packet, sizeof(recv_packet));
    }

    fprintf(stderr, "Error: unexpected return code %d\n", recv_packet[0]);
    return 1;
}

uint8_t writeStateMachineCommand(uint8_t fd, uint8_t address, uint8_t command, uint16_t register_address, uint16_t value) {
    uint16_t recv_address, recv_value;

    char recv_packet[8];
    char packet[8] = {(char)address,
                      (char)command,
                      (char)HIGHER_8(register_address),
                      (char)LOWER_8(register_address),
                      (char)HIGHER_8(value),
                      (char)LOWER_8(value),
                      0,
                      0};

    setCrc(packet, sizeof(packet));

    printf("Sending packet: ");
    printPacket(packet, sizeof(packet));

    if (write(fd, packet, sizeof(packet)) != sizeof(packet)) {
        perror("Failed to write write single register packet\n");
        return -1;
    }

	usleep(10000);

    if (read(fd, recv_packet, 1) != 1) {
        perror("Failed to read unit address from received packet\n");
        return -1;
    }

    if (recv_packet[0] != address) {
        fprintf(stderr, "Response form wrong address. Expected: %02x, got: %02x\n", address, recv_packet[0]);
        return -1;
    }

    if (read(fd, recv_packet + 1, 1) != 1) {
        perror("Failed to read return code from received packet\n");
        return -1;
    }
    
    if (recv_packet[1] == '\x86') { // if error
        return handleExceptionCode(fd, register_address, value);

    } else if (recv_packet[1] == '\x06') {  // if success

        if (read(fd, recv_packet + 2, 6) != 6) {
            perror("Failed to read contents of received  "
                "packet\n");
            return -1;
        }

        recv_address = MAKE_16(recv_packet[2], recv_packet[3]);
        recv_value = MAKE_16(recv_packet[4], recv_packet[5]);

        if (recv_address != register_address) {
            fprintf(stderr, "Recieved unexpected address after writeSingleRegister, "
                    "received: %04x, expected: %04x\n",
                    recv_address, register_address);
            return 1;
        }

        if (recv_value != value) {
            fprintf(stderr, "Recieved unexpected value after writeSingleRegister, "
                    "received: %04x, expected: %04x\n",
                    recv_value, value);
            return 1;
        }

        printf("Recieved packet: ");
        printPacket(recv_packet, sizeof(recv_packet));

        return verifyCrc(recv_packet, sizeof(recv_packet));
    }

    fprintf(stderr, "Error: unexpected return code %d\n", recv_packet[0]);
    return 1;
}

uint8_t handleExceptionCode(uint8_t fd, uint16_t first_argument, uint16_t second_argument) {

    uint8_t exception_code;
    if (read(fd, &exception_code, 1) != 1) {
        perror("Failed to read return code from received  packet\n");
        return -1;
    }

    switch (exception_code) {
        case 1:
            fprintf(stderr, "Recieved error illegal function\n");
            return 1;
        case 2:
            fprintf(stderr, "Recieved error illegal data address for first "
                    "register %04x and register count %04x\n",
                    first_argument, second_argument);
            return 1;
        case 3:
            fprintf(stderr, "Recieved error illegal data value\n");
            return 1;
        case 4:
            fprintf(stderr, "Recieved error slave device failure\n");
            return 1;
    }
    fprintf(stderr, "Recieved unknown error\n");
    return 1;
}
