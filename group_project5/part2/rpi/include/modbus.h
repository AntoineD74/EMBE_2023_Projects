#ifndef _MODBUS_H_
#define _MODBUS_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "crc.h"

uint8_t readHoldingRegisters(char* response_buffer, uint8_t fd, uint8_t address,
                             uint16_t first_register, uint16_t register_count);

uint8_t writeSingleRegister(uint8_t fd, uint8_t address,
                            uint16_t register_address, uint16_t value);

uint8_t handleExceptionCode(uint8_t fd, uint16_t first_argument,
                            uint16_t second_argument);

#endif  // _MODBUS_H_
