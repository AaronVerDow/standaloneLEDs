// This file devifines LED strips, their pins, lenghts, and groupings.
// Duplicate everything when adding more strips
// See link below for an extreme example
// https://github.com/RedWagon/fifty_leds/blob/master/led_data.h

const int led_pin = 10;

// Use measure animation to find strip length
const int led_length = 518;

CRGB led_pixels[led_length];

Strip leds = {led_pixels, led_length, led_pin};

// This needs to match how many strips are defined
const int all_length = 1;
Strip all_strips[all_length];
Group all = {all_strips, all_length};

void led_data_setup() {
    FastLED.addLeds<NEOPIXEL, led_pin>(led_pixels, led_length);
    all_strips[0] = leds;
}
