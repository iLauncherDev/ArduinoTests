#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#define SPEAKER_PORT 2
#define SPEAKER_BITS 16
#define SPEAKER_FREQUENCY_HZ 20000000
#define SPEAKER_FREQUENCY_KHZ (SPEAKER_FREQUENCY_HZ / 1000)
#define SPEAKER_FREQUENCY_MHZ (SPEAKER_FREQUENCY_HZ / 1000000)

Adafruit_SSD1306 display(128, 64);
char string[64];

uint32_t speaker_read()
{
    uint8_t port = SPEAKER_PORT;
    uint32_t value = 0;
    for (uint8_t bits = 0; bits < SPEAKER_BITS; bits += 8)
        value |= analogRead(port++) << bits;
    return SPEAKER_FREQUENCY_HZ * value;
}

void speaker_write(uint32_t hz)
{
    uint8_t port = SPEAKER_PORT;
    if (hz)
    {
        uint32_t div = SPEAKER_FREQUENCY_HZ / hz;
        for (uint8_t bits = 0; bits < SPEAKER_BITS; bits += 8)
            analogWrite(port++, (div >> bits) & 0xff);
    }
    else
    {
        uint32_t div = 0;
        for (uint8_t bits = 0; bits < SPEAKER_BITS; bits += 8)
            analogWrite(port++, (div >> bits) & 0xff);
    }
}

void setup()
{
    display.begin(2, 0x3C);
    pinMode(2, OUTPUT);
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
