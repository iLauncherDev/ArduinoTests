#include <speaker.h>

speaker_t *sp_device = (speaker_t *)NULL;

uint32_t sp_convert8888_XXXX(uint32_t value, uint16_t type)
{
    uint32_t new_value = 0;
    for (uint8_t bits = 0; type; bits += 8)
    {
        new_value |= ((value >> bits) & 0xff) >> (8 - (type % 10));
        type /= 10, bits += 8;
    }
    return new_value;
}

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

uint8_t sp_bit2size(uint8_t bits, uint8_t bits_inc)
{
    uint8_t size = 0;
    for (size = 0; size < bits; size += bits_inc)
        if (bits + bits_inc > bits)
            break;
    return size;
}

uint32_t sp_read(uint8_t port)
{
    speaker_t *tmp = sp_find_node(port);
    if (!tmp)
        return 0;
    uint32_t hz = 0;
    for (uint8_t bits = 0; bits < tmp->bits; bits += tmp->port_bits)
    {
        if (bits + tmp->port_bits > tmp->bits)
            break;
        hz |= (analogRead(port++) << bits) & tmp->max_value;
    }
    return hz;
}

void sp_write(uint8_t port, uint32_t hz)
{
    speaker_t *tmp = sp_find_node(port);
    if (!tmp)
        return;
    uint32_t value = hz ? min(F_CPU, tmp->frequency) / hz : 0;
    for (uint8_t bits = 0; bits < tmp->bits; bits += tmp->port_bits)
    {
        if (bits + tmp->port_bits > tmp->bits)
            break;
        analogWrite(port++, (value >> bits) & tmp->max_value);
    }
}

void sp_add_device(uint16_t port, uint32_t frequency, uint8_t port_bits, uint8_t bits)
{
    speaker_t *tmp = sp_device;
    if (!sp_device)
    {
        sp_device = (speaker_t *)malloc(sizeof(speaker_t));
        memset(sp_device, 0, sizeof(speaker_t));
        sp_device->port = port;
        sp_device->frequency = frequency;
        sp_device->port_bits = port_bits;
        sp_device->bits = bits;
        sp_device->max_value = (1 << port_bits) - 1;
    }
    else
    {
        while (tmp->next)
        {
            if (port >= tmp->port && port < tmp->port + sp_bit2size(tmp->bits, tmp->port_bits))
                return;
            tmp = tmp->next;
        }
        tmp->next = (speaker_t *)malloc(sizeof(speaker_t));
        memset(tmp->next, 0, sizeof(speaker_t));
        tmp->next->port = port;
        tmp->next->frequency = frequency;
        tmp->next->port_bits = port_bits;
        tmp->next->bits = bits;
        tmp->next->max_value = (1 << port_bits) - 1;
        tmp->next->prev = tmp;
    }
}