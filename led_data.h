const int led_pin = 10;
const int led2_pin = 11;

const int led_length = 518;

CRGB led_pixels[led_length];
//CRGB led2_pixels[led_length];

Strip leds = {led_pixels, led_length, led_pin};
//Strip leds2 = {led2_pixels, led_length, led2_pin};

const int all_length = 1;
Strip all_strips[all_length];
Group all = {all_strips, all_length};

void led_data_setup() {
    FastLED.addLeds<NEOPIXEL, led_pin>(led_pixels, led_length);
    //FastLED.addLeds<NEOPIXEL, led2_pin>(led2_pixels, led_length);
    all_strips[0] = leds;
    //all_strips[1] = leds2;
}
