#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <speaker.h>

Adafruit_SSD1306 display(128, 64);
char string[256];

void setup()
{
    display.begin(2, 0x3C);
    sp_add_device(5, 20000000, 16);
    sp_add_device(6, 20000000, 16);
}

void loop()
{
    sp_write(5, rand());
    sp_write(6, rand());
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