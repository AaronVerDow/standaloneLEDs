// Arduino (check version if using Teensy)
// https://www.arduino.cc/en/Main/OldSoftwareReleases

// Teensy
// http://www.pjrc.com/teensy/td_download.html

// FastLED (If not included by default)
// https://github.com/FastLED/FastLED/releases

// FastLED color reference:
// https://github.com/FastLED/FastLED/wiki/Pixel-reference#colors

#include <FastLED.h>
#include "datastructures.h"
#include "led_data.h"

// Pin for button to cycle through animations
const int animation_button = 8;

// This change in the loop, editing does nothing
int hue = 0;
int hue_counter = 0;
int counter = 0;


// Hue cycle (rainbow)
// Minimum value = 1
// higher = slow down hue change
int hue_cycles_per_change = 2;
// higher = speed up hue change
int hue_changes_per_cycle = 1;
// At least one of these variables must be 1

//                   slow --------------> fast
//cycles_per_change  4   3   2   1   1   1   1
//changes_per_cycle  1   1   1   1   2   3   4

// Default maximum values
int saturation = 255;
int brightness = 255;

// Fade between animations
// higher = fade faster
// lower = fade slower
int fade_speed = 6;

// Value for dimmed white 0-255
const int dim = 80;

// Values for party hard (seizure) mode
// Currently not used
const int party_speed = 2;
const int party_blinks = 30;
int party_hard_count = party_speed;
int party_hard_blink = 0;
const CRGB party_on = CRGB::White;
const CRGB party_off = CRGB::Black;
CRGB party_hard_color = party_off;

Cycle_Actions animation = {animation_button, 0, false, false};

void setup() {
    // Pull in LED data
    led_data_setup();
    // Setup buttons
    pinMode(animation_button, INPUT);
    // Failsafe for Teensy programming
    delay(1000);
}

void loop() { 
    // Rotates hue for rainbows
    hue_counter--;
    if (hue_counter < 1) {
        hue_counter = hue_cycles_per_change;
        hue = hue + hue_changes_per_cycle;
        if (hue > 255 ) {
            hue = 0;
        }
    }
    
    // Default animations
    write_animation();
    
    // Add conditional animations (like party) here to override default

    FastLED.show(); 
    delay(10);
}


// Define animations here.  Make sure case statements are in order
void write_animation() {
    check_for_button_presses(animation);
    fade();
    // Watch the case statement numbers
    switch (animation.counter) {
        case 0:
            //multi rainbow
            delay_strip_and_pixel(all, hue, saturation, brightness, 1, 2);
            break;
        case 1:
            //solid color rainbow
            delay_strip(all, hue, saturation, brightness);
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



// Changes hue by multipliers on strips and pixels  (rainbow)
void delay_strip_and_pixel(Group group, int h, int s, int b, int strip_multiplier, int pixel_multiplier) {
    for (int strip = 0; strip < group.length; strip++) {
        for (int pixel = 0; pixel < group.strips[strip].length; pixel++) {
            int my_hue = h + (strip * strip_multiplier) + (pixel * pixel_multiplier);
            group.strips[strip].pixels[pixel] = CHSV(my_hue, s, b);
        }
    }
}

// Rainbow but solid color per strip
void delay_strip(Group group, int h, int s, int b) {
    for (int strip = 0; strip < group.length; strip++) {
        for (int pixel = 0; pixel < group.strips[strip].length; pixel++) {
            int my_hue = h + (strip * 15);
            group.strips[strip].pixels[pixel] = CHSV(my_hue, s, b);
        }
    }
}

// Solid color
void write_group(Group group, CRGB color) {
    for (int strip = 0; strip < group.length; strip++) {
        for (int pixel = 0; pixel < group.strips[strip].length; pixel++) {
            group.strips[strip].pixels[pixel] = color;
        }
    }
}

// Used to measure strip length
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

// Blinks solid colors
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

// Failure, trying to make lightning
void write_blinky_white() {
    write_random_pixel(all, CRGB::White);
    write_random_pixel(all, CRGB::Black);
    write_random_pixel(all, CRGB::Black);
}

// Failure, trying to do colorful blinky something
void write_random_pixels() {
    write_random_pixel(all, CHSV(random(0,255), 255, random(0,255)));
    write_random_pixel(all, CHSV(random(0,255), 255, random(0,255)));
    write_random_pixel(all, CHSV(random(0,255), 255, random(0,255)));
    write_random_pixel(all, CRGB::Black);
    write_random_pixel(all, CRGB::Black);
    write_random_pixel(all, CRGB::Black);
    write_random_pixel(all, CRGB::Black);
    write_random_pixel(all, CRGB::Black);
    write_random_pixel(all, CRGB::Black);
}

// Write to the end of a strip
void write_end(Strip strip, int count, CRGB color) {
    for (int i=1; i<=count; i++) {
        strip.pixels[strip.length-i] = color;
    }
}

// Write to the beginning of a strip
void write_begin(Strip strip, int count, CRGB color) {
    for (int i=0; i<count; i++) {
        strip.pixels[i] = color;
    }
}

void write_strip(Strip strip, CRGB color) {
    for (int i = 0; i < strip.length; i++) {
        strip.pixels[i] = color;
    }
}

void write_pixel(Group group, int strip, int pixel, CRGB color) {
    group.strips[strip].pixels[pixel] = color;
}

void write_random_pixel(Group group, CRGB color) {
    int strip = random(0,group.length-1);
    int pixel = random(0,group.strips[strip].length-1);
    group.strips[strip].pixels[pixel] = color;
}

// Supporting methods

void fade() {
    if (animation.changed) {
        fade_out();
    } else if (brightness < 255) {
        fade_in();
    }
    if (brightness == 0) {
        animation.changed = false;
        ++animation.counter;
    }   
}

void fade_in() {
    if (brightness < 255) {
        brightness = brightness + fade_speed;
        if (brightness > 255) {
            brightness = 255;
        }
        FastLED.setBrightness(brightness);
    }
}

void fade_out() {
    if (brightness > 0) {
        brightness = brightness - fade_speed;
        if (brightness < 0) {
            brightness = 0;
        }
        FastLED.setBrightness(brightness);
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


