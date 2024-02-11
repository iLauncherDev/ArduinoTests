#include <Arduino.h>

const int r = 26, g = 25, b = 17, hsync = 16, vsync = 27;

void IRAM_ATTR vsyncINT()
{
    digitalWrite(vsync, HIGH);
    digitalWrite(vsync, LOW);
}

void IRAM_ATTR hsyncINT()
{
    digitalWrite(hsync, HIGH);
    digitalWrite(hsync, LOW);
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(r, OUTPUT);
    pinMode(g, OUTPUT);
    pinMode(b, OUTPUT);
    pinMode(hsync, OUTPUT);
    pinMode(vsync, OUTPUT);
    hw_timer_t *vsyncINT_timer = timerBegin(0, 80, true);
    timerAttachInterrupt(vsyncINT_timer, vsyncINT, true);
    timerAlarmWrite(vsyncINT_timer, 16 * 1000, true);
    timerAlarmEnable(vsyncINT_timer);
    hw_timer_t *hsyncINT_timer = timerBegin(1, 80, true);
    timerAttachInterrupt(hsyncINT_timer, hsyncINT, true);
    timerAlarmWrite(hsyncINT_timer, 32, true);
    timerAlarmEnable(hsyncINT_timer);
}

void loop()
{
}