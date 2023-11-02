#ifndef _CRC_H_
#define _CRC_H_

#include <stdint.h>
#include <stdio.h>

uint16_t ModRTU_CRC(uint8_t buf[], int len);
void setCrc(char* packet, size_t length);
uint8_t verifyCrc(char* packet, size_t length);

#endif  // _CRC_H_
