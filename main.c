#include "base/base.h"

#define NUM_GUESS   (4)

typedef enum {
    Red,
    Green, 
    Blue,
    Cyan,
    Magenta,
    Yellow,
    White,
    Black,
    Undefined
} Color;

const char *color_label(Color color) {
    switch (color) {
        case Red: return "red";
        case Green: return "green";
        case Blue: return "blue";
        case Cyan: return "cyan";
        case Magenta: return "magenta";
        case Yellow: return "yellow";
        case White: return "white";
        case Black: return "black";

        default: return "undefined";
    }

    return "undefined";
}

u64 num_colors(void) {
    u64 result = 0;

    for (Color color = 0; color < Undefined; ++color) {
        ++result;
    }

    return result;
}

void secret_init(Color secret_buffer[NUM_GUESS]) {
    u64 random_numbers[NUM_GUESS];

    ran_rand_range_arr(random_numbers, NUM_GUESS, 0, num_colors() - 1, true);

    for (usize i = 0; i < NUM_GUESS; ++i) {
        secret_buffer[i] = random_numbers[i];
    }
}

void hint(const Color *secret) {
    for (usize i = 0; i < NUM_GUESS; ++i) {
        fprintf(stdout, "%s ", color_label(secret[i]));
    }
}

i32 main(void) {
    Arena arena;    

    arena_init(&arena);

    Color secret[NUM_GUESS];

    secret_init(secret);

    hint(secret);

    arena_deinit(&arena);

    return 0;
}
