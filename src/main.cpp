#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#define SPEAKER_PORT 0x02

Adafruit_SSD1306 display(128, 64);
char string[64];

uint32_t audio[] = {
    1000,
    50,
    686,
    327,
    436,
    1231,
    (uint32_t)-1,
};

void setup()
{
    display.begin(2, 0x3C);
    pinMode(2, OUTPUT);
}

void speaker_write(uint32_t hz)
{
    uint8_t port = SPEAKER_PORT;
    if (hz)
    {
        uint32_t div = 20000000 / 1000;
        for (uint8_t bits = 0; bits <= 8; bits += 8)
            analogWrite(port++, (div >> bits) & 0xff);
    }
    else
    {
        uint32_t div = 0;
        for (uint8_t bits = 0; bits <= 8; bits += 8)
            analogWrite(port++, (div >> bits) & 0xff);
    }
}

void loop()
{
    goto speaker;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(1);
    display.print("Usb Data: ");
    display.print(itoa(analogRead(0), string, 16));
    display.print(", ");
    display.print(itoa(analogRead(1), string, 16));
    display.print(";\n");
    display.display();
speaker:
    speaker_write(1000);
}
