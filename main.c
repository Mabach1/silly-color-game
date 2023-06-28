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

const char *get_color_label(Color color) {
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
        fprintf(stdout, "%s ", get_color_label(secret[i]));
    }
    fprintf(stdout, "\n");
}

usize get_color_index(String label) {
    for (Color color = 0; color < num_colors(); ++color) {
        if (string_equal(label, string_str_lit(get_color_label(color)))) {
            return color;
        }
    }
    return Undefined;
}

bool check_input(String input, StringArr inputs, usize guessed) {
    for (usize i = 0; i < guessed; ++i) {
        if (string_equal(input, inputs.arr[i])) {
            return false;
        }
    }

    for (Color color = 0; color < num_colors(); ++color) {
        if (string_equal(input, string_str_lit(get_color_label(color)))) {
            return true;
        }
    }

    return false;
}

bool player_guess(Color guess_buffer[NUM_GUESS]) {
    Arena scratch; 

    arena_init(&scratch);

    StringArr inputs = stringarr_alloc(&scratch, NUM_GUESS);

    for (usize i = 0; i < NUM_GUESS; ++i) {
        fprintf(stdout, "%llu. color: ", i + 1);

        String input = string_from_stdin(&scratch);

        if (!check_input(input, inputs, i)) {
            fprintf(stdout, "Error: Incorrect input!\n");
            --i;
            continue;
        }

        stringarr_push(&inputs, input);
    }

    for (usize i = 0; i < NUM_GUESS; ++i) {
        guess_buffer[i] = get_color_index(inputs.arr[i]);
    }

    arena_deinit(&scratch);

    return true;
}

i32 main(void) {
    Arena arena;    

    arena_init(&arena);

    Color secret[NUM_GUESS];

    secret_init(secret);

    hint(secret);

    Color guess_buffer[NUM_GUESS];

    player_guess(guess_buffer);

    for (usize i = 0; i < NUM_GUESS; ++i) {
        fprintf(stdout, "%s ", get_color_label(guess_buffer[i]));
    }

    u64 correct = 0;
    u64 matched = 0;

    typedef enum { Wrong, Correct } Guess;

    Guess result[NUM_GUESS] = { Wrong };

    for (usize i = 0; i < NUM_GUESS; ++i) {
        if (guess_buffer[i] == secret[i]) {
            result[i] = Correct;
            correct++;
            continue;
        }

        for (usize j = 0; j < NUM_GUESS; ++j) {
            if (Correct != result[j] && guess_buffer[i] == secret[j]) {
                matched++;
            }
        }
    }

    fprintf(stdout, "\nmatched: %llu, correct: %llu", matched, correct);

    arena_deinit(&arena);

    return 0;
}
