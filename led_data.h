// This file devifines LED strips, their pins, lenghts, and groupings.
// Duplicate everything when adding more strips
// See link below for an extreme example
// https://github.com/RedWagon/fifty_leds/blob/master/led_data.h

const int one_pin = 15;

// Use measure animation to find strip length
const int default_led_length = 290;
const int one_length = default_led_length;

CRGB one_pixels[one_length];

CHSV one_colors[one_length];

//uint8_t one_hue[one_length];
//uint8_t two_hue[two_length];
//uint8_t three_hue[three_length];
//
//uint8_t one_sat[one_length];
//uint8_t two_sat[two_length];
//uint8_t three_sat[three_length];
//
//uint8_t one_val[one_length];
//uint8_t two_val[two_length];
//uint8_t three_val[three_length];

//Strip one = {one_pixels, one_hue, one_sat, one_val, one_length, one_pin};
//Strip two = {two_pixels, two_hue, two_sat, two_val, two_length, two_pin};
//Strip three = {three_pixels, three_hue, three_sat, three_val, three_length, three_pin};
//Strip four = {one_pixels, one_hue, one_sat, one_val, four_length, four_pin};
//Strip five = {two_pixels, two_hue, two_sat, two_val, five_length, five_pin};

Strip one = {one_pixels, one_colors, one_length, one_pin};

// This needs to match how many strips are defined
const int all_length = 1;
Strip all_strips[all_length];
Group all = {all_strips, all_length};

void led_data_setup() {
    FastLED.addLeds<NEOPIXEL, one_pin>(one_pixels, one_length);
    all_strips[0] = one;
}
