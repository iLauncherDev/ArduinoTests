#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <speaker.h>

Adafruit_SSD1306 display(128, 64);
char string[256];

uint8_t a = 9;

void setup()
{
    display.begin(2, 0x3C);
    sp_add_device(a, 20000000, 10, 16);
    sp_write(a, 1000);
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
    display.display();
}