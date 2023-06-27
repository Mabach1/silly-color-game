#include "base/base.h"

i32 main(void) {
    Arena arena;    

    arena_init(&arena);

    arena_deinit(&arena);

    return 0;
}
