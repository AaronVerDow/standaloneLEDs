typedef struct {
    CRGB* pixels;
    int length;
    int pin;
} Strip;

typedef struct {
    Strip* strips;
    int length;
} Group;

typedef struct {
    int button;
    int counter;
    bool lock;
    bool changed;
} Cycle_Actions;

typedef struct {
    int duration;
    int cycle;
    CRGB color;
    CRGB on;
    CRGB off;
} Blinker;
