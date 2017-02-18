// This file devifines LED strips, their pins, lenghts, and groupings.
// Duplicate everything when adding more strips
// See link below for an extreme example
// https://github.com/RedWagon/fifty_leds/blob/master/led_data.h

const int one_pin = 15;
const int two_pin = 16;

// Use measure animation to find strip length
const int default_led_length = 290;
const int one_length = default_led_length;
const int two_length = default_led_length;

CRGB one_pixels[one_length];
CRGB two_pixels[two_length];

CHSV one_colors[one_length];
CHSV two_colors[two_length];

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
Strip two = {two_pixels, two_colors, two_length, two_pin};

// This needs to match how many strips are defined
const int all_length = 2;
Strip all_strips[all_length];
Strip inner_strips[1];
Strip outer_strips[1];
Group all = {all_strips, all_length};
Group inner = {inner_strips, 1};
Group outer = {outer_strips, 1};

void led_data_setup() {
    FastLED.addLeds<NEOPIXEL, one_pin>(one_pixels, one_length);
    FastLED.addLeds<NEOPIXEL, two_pin>(two_pixels, two_length);
    all_strips[0] = one;
    all_strips[1] = two;
    inner_strips[0] = one;
    outer_strips[0] = two;
}
