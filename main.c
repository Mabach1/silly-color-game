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

Color *secret_init(Arena *arena) {
    u64 random_numbers[NUM_GUESS];

    ran_rand_range_arr(random_numbers, NUM_GUESS, 0, num_colors() - 1, true);

    Color *result = arena_alloc(arena, NUM_GUESS);

    for (usize i = 0; i < NUM_GUESS; ++i) {
        result[i] = random_numbers[i];
    }

    return result;
}

void hint(const Color *secret) {
    for (usize i = 0; i < NUM_GUESS; ++i) {
        fprintf(stdout, "%s ", color_label(secret[i]));
    }
}

i32 main(void) {
    Arena arena;    

    arena_init(&arena);

    Color *secret = secret_init(&arena);

    hint(secret);

    arena_deinit(&arena);

    return 0;
}
