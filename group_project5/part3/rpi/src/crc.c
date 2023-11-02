#include "../include/crc.h"

#define HIGHER_8(x) (x >> (8 * sizeof(x) - 8))
#define LOWER_8(x) (x & 0xff)
#define MAKE_16(higher, lower) (((uint16_t)higher << 8) | (uint16_t)lower)

uint16_t ModRTU_CRC(uint8_t buf[], int len) {
    uint16_t crc = 0xFFFF;

    for (int pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)buf[pos];

        for (int i = 8; i != 0; i--) {
            if ((crc & 0x0001) != 0) {
                crc >>= 1;
                crc ^= 0xA001;
            } else
                crc >>= 1;
        }
    }
    return crc;
}

void setCrc(char* packet, size_t length) {
    uint16_t crc;

    crc = ModRTU_CRC((uint8_t*)packet, length - 2);

    // Big endian
    packet[length - 2] = (char)HIGHER_8(crc);
    packet[length - 1] = (char)LOWER_8(crc);
}

uint8_t verifyCrc(char* packet, size_t length) {
    uint16_t crc, recv_crc;

    // Big endian
    recv_crc = MAKE_16(packet[length - 2], packet[length - 1]);

    crc = ModRTU_CRC((uint8_t*)packet, length - 2);

    if (crc != recv_crc) {
        fprintf(stderr, "Crc invalid. Expected: %02x %02x, got: %02x %02x\n",
                HIGHER_8(crc), LOWER_8(crc), HIGHER_8(recv_crc),
                LOWER_8(recv_crc));
        return -1;
    }
    return 0;
}
