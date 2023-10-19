#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#define SPEAKER_BITS 16
#define SPEAKER_BITS_INC 8
#define SPEAKER_FREQUENCY_HZ 20000000
#define SPEAKER_FREQUENCY_KHZ (SPEAKER_FREQUENCY_HZ / 1000)
#define SPEAKER_FREQUENCY_MHZ (SPEAKER_FREQUENCY_HZ / 1000000)

uint8_t speaker_volume = 1.00f;

Adafruit_SSD1306 display(128, 64);
char string[256];

void speaker_set_volume(uint8_t percent)
{
    speaker_volume = 101 - (percent & 100);
}

uint32_t speaker_read(uint8_t port)
{
    uint32_t value = 0;
    for (uint8_t bits = 0; bits < SPEAKER_BITS; bits += SPEAKER_BITS_INC)
        value |= analogRead(port++) << bits;
    return value ? SPEAKER_FREQUENCY_HZ / value : 0;
}

void speaker_write(uint8_t port, uint32_t hz)
{
    uint32_t div = hz ? SPEAKER_FREQUENCY_HZ / (int16_t)round((float)hz * speaker_volume) : 0;
    for (uint8_t bits = 0; bits < SPEAKER_BITS; bits += SPEAKER_BITS_INC)
        analogWrite(port++, (div >> bits) & 0xff);
}

void setup()
{
    display.begin(2, 0x3C);
    speaker_set_volume(100);
}

void loop()
{
    speaker_write(2, 1000);
    speaker_write(4, 1000);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(1);
    display.print("Usb Data: ");
    display.print(itoa(digitalRead(0), string, DEC));
    display.print(", ");
    display.print(itoa(digitalRead(1), string, DEC));
    display.print(";\n");
    display.print("Speaker Data: ");
    display.print(itoa(speaker_read(2), string, DEC));
    display.print(", ");
    display.print(itoa(speaker_read(4), string, DEC));
    display.print(", ");
    display.print(itoa(SPEAKER_BITS, string, DEC));
    display.print(";\n");
    display.display();
}