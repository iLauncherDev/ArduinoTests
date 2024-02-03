#include <Arduino.h>

uint8_t a = 12;

void IRAM_ATTR blink()
{
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    hw_timer_t *blink_timer = timerBegin(1, 80, true);
    timerAttachInterrupt(blink_timer, blink, true);
    timerAlarmWrite(blink_timer, 1000000, true);
    timerAlarmEnable(blink_timer);
}

void loop()
{
    tone(a, rand() % (15 * 1000), 0);
}