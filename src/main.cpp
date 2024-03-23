#include <Arduino.h>

IRAM_ATTR const int r = 26, g = 25, b = 17, hsync = 16, vsync = 27, led0 = 12, button0 = 13;
IRAM_ATTR bool vsync_int = false, hsync_int = false;

float sound_speed = 1.0f;
float sound_speed_max = 512.0f;
hw_timer_t *vsyncINT_timer, *hsyncINT_timer;

IRAM_ATTR uint64_t vga_mode[] = {
    (uint64_t)(16.7f * 1000.0f), (uint64_t)(31.8f),
    (uint64_t)(64.0f), (uint64_t)(4.00f), 640, 480};

uint32_t sound_tones[] = {
    300,
    1000,

    299,
    500,

    298,
    100,

    297,
    800,

    296,
    700,

    295,
    2000,

    294,
    6000,

    293,
    8500,

    292,
    2500,

    0,
};

IRAM_ATTR static inline void MicroDelay(uint64_t us)
{
    uint64_t start = micros(), end = start + us;
    while (start < end)
        NOP();
}

IRAM_ATTR void vsyncINT()
{
    vsync_int = true;
    digitalWrite(vsync, 1);
    digitalWrite(vsync, 0);
}

IRAM_ATTR void hsyncINT()
{
    hsync_int = true;
    digitalWrite(hsync, 1);
    digitalWrite(hsync, 0);
}

void vga_task(void *)
{
start:
    while (vsync_int)
    {
        vsync_int = false;
        analogWrite(r, rand());
        analogWrite(g, rand());
        analogWrite(b, rand());
    }
    goto start;
}

void setup()
{
    pinMode(r, OUTPUT);
    pinMode(g, OUTPUT);
    pinMode(b, OUTPUT);
    pinMode(hsync, OUTPUT);
    pinMode(vsync, OUTPUT);
    pinMode(led0, OUTPUT);
    pinMode(button0, INPUT_PULLDOWN);
    vsyncINT_timer = timerBegin(2, 80, true);
    timerAttachInterrupt(vsyncINT_timer, vsyncINT, true);
    timerAlarmWrite(vsyncINT_timer, vga_mode[0], true);
    timerAlarmEnable(vsyncINT_timer);
    hsyncINT_timer = timerBegin(3, 80, true);
    timerAttachInterrupt(hsyncINT_timer, hsyncINT, true);
    timerAlarmWrite(hsyncINT_timer, vga_mode[1], true);
    timerAlarmEnable(hsyncINT_timer);
    xTaskCreate(vga_task, "vga loop", 1024, NULL, 1, NULL);
}

void play_audio(uint8_t pin, void *tones, uint8_t bits, float *speed)
{
    if (!pin || !tones || !bits || !speed)
        return;
    size_t i = 0;
    while (true)
    {
        uint32_t tone_value, tone_delay;
        float cache_speed = *speed;
        float tmp = 0;
        switch (bits)
        {
        case 8:
            tone_delay = ((uint8_t *)tones)[i];
            break;
        case 16:
            tone_delay = ((uint16_t *)tones)[i];
            break;
        default:
            tone_delay = ((uint32_t *)tones)[i];
            break;
        }
        if (!tone_delay)
            break;
        switch (bits)
        {
        case 8:
            tone_value = ((uint8_t *)tones)[i + 1];
            break;
        case 16:
            tone_value = ((uint16_t *)tones)[i + 1];
            break;
        default:
            tone_value = ((uint32_t *)tones)[i + 1];
            break;
        }
        tmp = (uint32_t)((float)tone_delay / (cache_speed ? cache_speed : 1.0f));
        tone(pin, tone_value, 0);
        delay(tmp ? tmp : 1);
        i += 2;
    }
}

void loop()
{
    if (digitalRead(button0))
        sound_speed = sound_speed_max;
    play_audio(led0, sound_tones, 32, &sound_speed);
    if (sound_speed < sound_speed_max)
        sound_speed += 0.01f;
}