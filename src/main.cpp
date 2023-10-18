#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#define SPEAKER_PORT 2
#define SPEAKER_BITS 16
#define SPEAKER_BITS_INC 8
#define SPEAKER_FREQUENCY_HZ 25000000
#define SPEAKER_FREQUENCY_KHZ (SPEAKER_FREQUENCY_HZ / 1000)
#define SPEAKER_FREQUENCY_MHZ (SPEAKER_FREQUENCY_HZ / 1000000)

Adafruit_SSD1306 display(128, 64);
char string[256];

uint32_t speaker_read()
{
    uint8_t port = SPEAKER_PORT;
    uint32_t value = 0;
    for (uint8_t bits = 0; bits < SPEAKER_BITS; bits += SPEAKER_BITS_INC)
        value |= analogRead(port++) << bits;
    return value ? SPEAKER_FREQUENCY_HZ / value : 0;
}

void speaker_write(uint32_t hz)
{
    uint8_t port = SPEAKER_PORT;
    uint32_t div = hz ? SPEAKER_FREQUENCY_HZ / hz : 0;
    for (uint8_t bits = 0; bits < SPEAKER_BITS; bits += SPEAKER_BITS_INC)
        analogWrite(port++, (div >> bits) & 0xff);
}

void setup()
{
    display.begin(2, 0x3C);
    speaker_write(1000);
}

void loop()
{
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
    display.print(itoa(speaker_read(), string, DEC));
    display.print(", ");
    display.print(itoa(SPEAKER_BITS, string, DEC));
    display.print(";\n");
    display.display();
}