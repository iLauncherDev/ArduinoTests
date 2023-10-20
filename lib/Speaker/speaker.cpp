#include <speaker.h>

speaker_t *sp_device = (speaker_t *)NULL;

speaker_t *sp_find_node(uint16_t port)
{
    if (sp_device)
    {
        speaker_t *tmp = sp_device;
        while (tmp)
        {
            if (tmp->port == port)
                return tmp;
            tmp = tmp->next;
        }
    }
    return (speaker_t *)NULL;
}

uint32_t sp_read(uint8_t port)
{
    speaker_t *tmp = sp_find_node(port);
    if (!tmp)
        return 0;
    uint32_t value = 0;
    for (uint8_t bits = 0; bits < tmp->bits; bits += 10)
        value |= (analogRead(port++) << bits) & 0x3ff;
    return value ? tmp->frequency / value : 0;
}

void sp_write(uint8_t port, uint32_t hz)
{
    speaker_t *tmp = sp_find_node(port);
    if (!tmp)
        return;
    uint32_t div = hz ? tmp->frequency / hz : 0;
    for (uint8_t bits = 0; bits < tmp->bits; bits += 10)
        analogWrite(port++, (div >> bits) & 0x3ff);
}

void sp_add_device(uint16_t port, uint32_t frequency, uint8_t bits)
{
    speaker_t *tmp = sp_device;
    if (!sp_device)
    {
        sp_device = (speaker_t *)malloc(sizeof(speaker_t));
        memset(sp_device, 0, sizeof(speaker_t));
        sp_device->port = port;
        sp_device->frequency = frequency;
        sp_device->bits = (bits / 10) * 10;
    }
    else
    {
        while (tmp->next)
        {
            if (tmp->port == port)
                return;
            tmp = tmp->next;
        }
        tmp->next = (speaker_t *)malloc(sizeof(speaker_t));
        memset(tmp->next, 0, sizeof(speaker_t));
        tmp->next->port = port;
        tmp->next->frequency = frequency;
        tmp->next->bits = (bits / 10) * 10;
        tmp->next->prev = tmp;
    }
}