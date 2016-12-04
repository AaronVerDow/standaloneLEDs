#include <FastLED.h>
#include "datastructures.h"
#include "led_data.h"

const int turn_left_button = 25;
const int turn_right_button = 28;
const int animation_button = 8;
//const int party_hard_button = 27;

int hue = 0;
int saturation = 255;
int brightness = 255;
int counter = 0;
int hue_counter = 0;
int transition_step = 6;

const int dim = 80;

int rand_strip = 0;
int rand_pixel = 0;

const int party_speed = 2;
const int party_blinks = 30;
int party_hard_count = party_speed;
int party_hard_blink = 0;

const CRGB party_on = CRGB::White;
const CRGB party_off = CRGB::Black;
CRGB party_hard_color = party_off;

Cycle_Actions animation = {animation_button, 0, false, false};

void setup() {
    led_data_setup();

    //pinMode(party_hard_button, INPUT);
    pinMode(animation_button, INPUT);

    //do something on power up
    //turn_left_blink = turn_blinks;
    //turn_right_blink = turn_blinks;
    delay(1000);
}

void loop() { 
    hue_counter--;
    if (hue_counter < 1) {
        hue_counter = 2;
        hue++;
        if (hue > 255 ) {
            hue = 0;
        }
    }
    counter--;
    if (counter < 1) {
        counter = 6;
        rand_strip = random(0,all.length-1);
        rand_pixel = random(0,all.strips[rand_strip].length-1);
    }

    write_animation();
    write_party();

    FastLED.show(); 
    delay(10);
}


void write_animation() {
    check_for_button_presses(animation);
    if (animation.changed) {
        fade_out();
    } else if (brightness < 255) {
        fade_in();
    }
    if (brightness == 0) {
        animation.changed = false;
        ++animation.counter;
    }
    switch (animation.counter) {
        case 0:
            //multi rainbow
            delay_strip_and_pixel(all, hue, saturation, brightness, 1, 2);
            break;
        case 1:
            //solid color rainbow
            delay_strip(all, hue, saturation, brightness );
            break;
        case 2:
            //dim white
            write_group(all, CRGB(dim,dim,dim));
            break;
        case 3:
            //off
            write_group(all, CRGB::Black);
            break;
        default:
            animation.counter = 0;
    }
}

void fade_in() {
    if (brightness < 255) {
        brightness = brightness + transition_step;
        if (brightness > 255) {
            brightness = 255;
        }
        FastLED.setBrightness(brightness);
    }
}

void fade_out() {
    if (brightness > 0) {
        brightness = brightness - transition_step;
        if (brightness < 0) {
            brightness = 0;
        }
        FastLED.setBrightness(brightness);
    }
}

void write_party() {
    //if (digitalRead(party_hard_button) == HIGH) {
        //party_hard_blink = party_blinks;
    //}

    if (party_hard_blink > 0) {
        if (party_hard_count == 0) {
            if (party_hard_color == party_on) {
                party_hard_color = party_off;
            } else {
                party_hard_color = party_on;
            }
            party_hard_count = party_speed;
            party_hard_blink--;
        } else {
            party_hard_count--;
        }
        if (party_hard_color == party_on) {
            write_group(all, party_hard_color);
        }
    }
}


void check_for_button_presses(Cycle_Actions &action) {
    if (digitalRead(action.button) == HIGH) {
        if (action.lock == false) {
            action.lock = true;
            action.changed = true;
        }
    } else {
        action.lock = false;
    }
}


void write_end(Strip strip, int count, CRGB color) {
    for (int i=1; i<=count; i++) {
        strip.pixels[strip.length-i] = color;
    }
}

void write_begin(Strip strip, int count, CRGB color) {
    for (int i=0; i<count; i++) {
        strip.pixels[i] = color;
    }
}

void write_blinky_white() {
    random_pixel(all, CRGB::White);
    random_pixel(all, CRGB::Black);
    random_pixel(all, CRGB::Black);
}

void write_random_pixels() {
    random_pixel(all, CHSV(random(0,255), 255, random(0,255)));
    random_pixel(all, CHSV(random(0,255), 255, random(0,255)));
    random_pixel(all, CHSV(random(0,255), 255, random(0,255)));
    random_pixel(all, CRGB::Black);
    random_pixel(all, CRGB::Black);
    random_pixel(all, CRGB::Black);
    random_pixel(all, CRGB::Black);
    random_pixel(all, CRGB::Black);
    random_pixel(all, CRGB::Black);
}

void write_strip(Strip strip, CRGB color) {
    for (int i = 0; i < strip.length; i++) {
        strip.pixels[i] = color;
    }
}

void write_pixel(Group group, int strip, int pixel, CRGB color) {
    group.strips[strip].pixels[pixel] = color;
}

void random_pixel(Group group, CRGB color) {
    int strip = random(0,group.length-1);
    int pixel = random(0,group.strips[strip].length-1);
    group.strips[strip].pixels[pixel] = color;
}

void measure_group(Group group) {
    for (int strip = 0; strip < group.length; strip++) {
        for (int pixel = 0; pixel < group.strips[strip].length; pixel++) {
            CRGB color = CRGB::Green;
            if (pixel % 10 == 0) {
                color = CRGB::Blue;
            }
            if (pixel % 100 == 0) {
                color = CRGB::Red;
            }
            group.strips[strip].pixels[pixel] = color;
        }
    }
}

void write_group(Group group, CRGB color) {
    for (int strip = 0; strip < group.length; strip++) {
        for (int pixel = 0; pixel < group.strips[strip].length; pixel++) {
            group.strips[strip].pixels[pixel] = color;
        }
    }
}

void delay_strip(Group group, int h, int s, int b) {
    for (int strip = 0; strip < group.length; strip++) {
        for (int pixel = 0; pixel < group.strips[strip].length; pixel++) {
            int my_hue = h + (strip * 15);
            group.strips[strip].pixels[pixel] = CHSV(my_hue, s, b);
        }
    }
}

void delay_strip_and_pixel(Group group, int h, int s, int b, int strip_multiplier, int pixel_multiplier) {
    for (int strip = 0; strip < group.length; strip++) {
        for (int pixel = 0; pixel < group.strips[strip].length; pixel++) {
            int my_hue = h + (strip * strip_multiplier) + (pixel * pixel_multiplier);
            group.strips[strip].pixels[pixel] = CHSV(my_hue, s, b);
        }
    }
}

