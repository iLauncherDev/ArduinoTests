#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <speaker.h>

Adafruit_SSD1306 display(128, 64);
char string[256];

uint8_t a = 5;

void setup()
{
    display.begin(2, 0x3C);
    sp_add_device(a, 20000000, 10, 16);
}

void loop()
{
    while (true)
        sp_write(a, rand() / 10000);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(1);
    display.print("Usb Data: ");
    display.print(itoa(digitalRead(0), string, DEC));
    display.print(", ");
    display.print(itoa(digitalRead(1), string, DEC));
    display.print(";\n");
    display.display();
}