#include "base/base.h"

#include <stdlib.h>
#include <string.h>

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

const u64 num_colors(void) {
    u64 result = 0;

    for (Color color = 0; color < Undefined; ++color) {
        ++result;
    }

    return result;
}

const usize row_max_len(void) {
    usize result = 0;

    usize sizes[num_colors()];

    for (Color c = 0; c < num_colors(); ++c) {
        sizes[c] = strlen(get_color_label(c));
    }

    bool swaped = false;
    
    do {
        swaped = false;

        for (usize i = 0; i < num_colors() - 1; ++i) {
            if (sizes[i] > sizes[i + 1]) {
                usize temp = sizes[i];
                sizes[i] = sizes[i + 1];
                sizes[i + 1] = temp;

                swaped = true;
            }
        }
    } while (swaped);

    for (usize i = num_colors() - NUM_GUESS; i < num_colors(); ++i) {
        result += sizes[i];
    }

    return result;
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

void player_guess(Color guess_buffer[NUM_GUESS]) {
    Arena scratch; 

    arena_init(&scratch);

    StringArr colors;
    bool all_good = true;

    while (true) {
        fprintf(stdout, "Your guess(%llu colors): ", NUM_GUESS);
        String input = string_from_stdin(&scratch);

        colors = string_parse(&scratch, &input, " ");

        if (colors.len > NUM_GUESS || colors.len < NUM_GUESS) {
            fprintf(stdout, "Incorrect number of colors!\n");
            continue;
        }

        for (usize i = 0; i < NUM_GUESS; ++i) {
            if (!check_input(colors.arr[i], colors, i)) {
                string_print_format("Uknown color: %\n", colors.arr[i]);
                all_good = false;  
            }
        }

        if (all_good) { break; }

        all_good = true;
    }

    for (usize i = 0; i < NUM_GUESS; ++i) {
        guess_buffer[i] = get_color_index(colors.arr[i]);
    }

    arena_deinit(&scratch);
}

void game_over(Color secret[]) {
    ProgressCell *current_cell = &global_table.table[global_table.tries - 1];

    bool over = false;

    if (global_table.tries >= NUM_TRIES) {
        fprintf(stdout, "You loss :(");
        over = true;
    }

    if (NUM_GUESS == current_cell->correct) {
        fprintf(stdout, "You won :)");
        over = true;
    }

    if (!over) { return; }

    fprintf(stdout, "\nCorrect combination: ");

    for (usize i = 0; i < NUM_GUESS; ++i) {
        fprintf(stdout, "%s ", get_color_label(secret[i]));
    }

    exit(EXIT_SUCCESS);
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

        game_over(secret);

        system("cls");
    }

    return 0;
}
