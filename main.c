#include "base/base.h"

#include <stdlib.h>

#define _HINT

#define NUM_GUESS   (4)
#define NUM_TRIES   (8)

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

typedef struct {
    Color guessed_colors[NUM_GUESS];
    u64 correct;
    u64 matched;
} ProgressCell;

typedef struct {
    usize tries;
    ProgressCell table[NUM_TRIES];
} ProgressTable;

ProgressTable global_table = { .tries = 0 };

void push_guess(Color guess[], u64 correct, u64 matched) {
    ProgressCell currect_cell;

    currect_cell.correct = correct;
    currect_cell.matched = matched;

    for (usize i = 0; i < NUM_GUESS; ++i) {
        currect_cell.guessed_colors[i] = guess[i];
    }

    global_table.table[global_table.tries++] = currect_cell;
}

void print_cell(ProgressCell cell) {
    fprintf(stdout, "| ");

    for (usize i = 0; i < NUM_GUESS; ++i) {
        fprintf(stdout, "%s ", get_color_label(cell.guessed_colors[i]));
    }

    fprintf(stdout, "| correct: %llu, matched: %llu |\n", cell.correct, cell.matched);
}

void display_progress(void) {
    for (usize i = 0; i < global_table.tries; ++i) {
        print_cell(global_table.table[i]);
    }
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

void game_over() {
    ProgressCell *current_cell = &global_table.table[global_table.tries - 1];


    if (global_table.tries >= NUM_TRIES) {
        fprintf(stdout, "You loss :(");
        exit(EXIT_SUCCESS);
    }

    if (NUM_GUESS == current_cell->correct) {
        fprintf(stdout, "You won :)");
        exit(EXIT_SUCCESS);
    }
}

i32 main(void) {
    Color secret[NUM_GUESS];

    secret_init(secret);

#ifdef HINT
    hint(secret);
#endif 

    while (true) {
        display_progress();

        Color guess_buffer[NUM_GUESS];

        player_guess(guess_buffer);

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

        push_guess(guess_buffer, correct, matched);

        game_over();

        system("cls");
    }

    return 0;
}
