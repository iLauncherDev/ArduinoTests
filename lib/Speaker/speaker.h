#pragma once
#ifndef SPEAKER_H
#define SPEAKER_H
#define SPEAKER_FLAGS_HEADPHONE (1 << 0)
#define SPEAKER_FLAGS_MICROPHONE (1 << 1)
#include <Arduino.h>

typedef struct speaker
{
    uint16_t port;
    uint32_t frequency;
    uint8_t bits;
    struct speaker *prev;
    struct speaker *next;
} speaker_t;

uint32_t sp_read(uint8_t port);
void sp_write(uint8_t port, uint32_t hz);
void sp_add_device(uint16_t port, uint32_t frequency, uint8_t bits);
#endif