#ifdef ENABLE_RGB_MATRIX_SNAKE
RGB_MATRIX_EFFECT(SNAKE)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#        include "matrix.h"

static const uint8_t GRID[MATRIX_ROWS] = {14, 14, 13, 12, 8};

typedef enum {
    DIR_LEFT,
    DIR_RIGHT,
    DIR_UP,
    DIR_DOWN,
} direction_t;

typedef struct {
    uint8_t x;
    uint8_t y;
} point_t;

static bool point_eq(point_t rhs, point_t lhs) {
    return rhs.x == lhs.x && rhs.y == lhs.y;
}

static point_t point_from_index(uint8_t i) {
    point_t p = {.x = i, .y = 0};
    while (p.x >= GRID[p.y]) {
        p.x -= GRID[p.y];
        p.y += 1;
    }
    return p;
}

#        define INPUT_BUFFER_SIZE 3
typedef struct {
    point_t data[INPUT_BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t length;
} input_buffer_t;

static input_buffer_t input_buffer_new(void) {
    return (input_buffer_t){
        .data   = {},
        .head   = 0,
        .tail   = 0,
        .length = 0,
    };
}

static void input_buffer_enqueue(input_buffer_t *ib, point_t p) {
    if (ib->length == INPUT_BUFFER_SIZE) {
        ib->tail = (ib->tail + 1) % INPUT_BUFFER_SIZE;
    } else {
        ib->length += 1;
    }
    ib->data[ib->head] = p;
    ib->head           = (ib->head + 1) % INPUT_BUFFER_SIZE;
}

static point_t input_buffer_dequeue(input_buffer_t *ib) {
    point_t p = ib->data[ib->tail];
    ib->tail  = (ib->tail + 1) % INPUT_BUFFER_SIZE;
    ib->length -= 1;
    return p;
}

static bool key_is_pressed(point_t p) {
    return !!(matrix_get_row(p.y) & (1 << p.x));
}

typedef struct {
    direction_t dir;
    point_t     head;
    point_t     body[32];
    uint8_t     length;
} snake_t;

typedef struct {
    uint32_t       last_time;
    input_buffer_t input_buffer;
    snake_t        snake;
    point_t        food;
} snake_game_state_t;

static const point_t KEY_LEFT  = {.x = 10, .y = 4};
static const point_t KEY_RIGHT = {.x = 13, .y = 4};
static const point_t KEY_UP    = {.x = 13, .y = 3};
static const point_t KEY_DOWN  = {.x = 12, .y = 4};

static snake_game_state_t snake_game_state = {0};

static const hsv_t white_hsv      = {.h = 85, .s = 127, .v = 254};
static const hsv_t border_hsv     = {.h = white_hsv.h, .s = white_hsv.s, .v = 127};
static const hsv_t grid_hsv       = {.h = white_hsv.h, .s = white_hsv.s, .v = 16};
static const hsv_t snake_head_hsv = {.h = 85, .s = 200, .v = 254};
static const hsv_t snake_body_hsv = {.h = snake_head_hsv.h, .s = snake_head_hsv.s, .v = 127};
static const hsv_t snake_food_hsv = {.h = 0, .s = 200, .v = 254};

static rgb_t border_rgb     = {0};
static rgb_t grid_rgb       = {0};
static rgb_t snake_head_rgb = {0};
static rgb_t snake_body_rgb = {0};
static rgb_t snake_food_rgb = {0};

static bool snake_contains(snake_t *snake, point_t p) {
    if (point_eq(snake->head, p)) {
        return true;
    }
    for (uint8_t i = 0; i < snake->length; ++i) {
        if (point_eq(snake->body[i], p)) {
            return true;
        }
    }
    return false;
}

static point_t snake_game_get_food(snake_game_state_t *state) {
    point_t p = point_from_index(random8_max(RGB_MATRIX_LED_COUNT));
    while ((p.x == 0 || p.x >= GRID[p.y] - 1 || p.y == 0 || p.y >= MATRIX_ROWS - 1) || snake_contains(&state->snake, p)) {
        p = point_from_index(random8_max(RGB_MATRIX_LED_COUNT));
    }
    return p;
}

static void snake_game_setup(snake_game_state_t *state) {
    state->last_time    = 0;
    state->input_buffer = input_buffer_new();
    state->snake        = (snake_t){
               .dir    = DIR_RIGHT,
               .head   = {.x = 6, .y = 2},
               .body   = {{.x = 5, .y = 2}},
               .length = 1,
    };
    state->food = snake_game_get_food(state);

    border_rgb     = rgb_matrix_hsv_to_rgb(border_hsv);
    grid_rgb       = rgb_matrix_hsv_to_rgb(grid_hsv);
    snake_head_rgb = rgb_matrix_hsv_to_rgb(snake_head_hsv);
    snake_body_rgb = rgb_matrix_hsv_to_rgb(snake_body_hsv);
    snake_food_rgb = rgb_matrix_hsv_to_rgb(snake_food_hsv);
}

static uint32_t snake_game_tick(snake_game_state_t *state) {
    uint32_t tick = 1000 / scale16by8(qadd8(rgb_matrix_config.speed, 16), 16);
    if (g_rgb_timer - state->last_time >= tick) {
        state->last_time = g_rgb_timer;
        return true;
    }
    return false;
}

static void snake_game_update(snake_game_state_t *state) {
    if (key_is_pressed(KEY_LEFT)) {
        input_buffer_enqueue(&state->input_buffer, KEY_LEFT);
    }
    if (key_is_pressed(KEY_RIGHT)) {
        input_buffer_enqueue(&state->input_buffer, KEY_RIGHT);
    }
    if (key_is_pressed(KEY_UP)) {
        input_buffer_enqueue(&state->input_buffer, KEY_UP);
    }
    if (key_is_pressed(KEY_DOWN)) {
        input_buffer_enqueue(&state->input_buffer, KEY_DOWN);
    }

    if (!snake_game_tick(state)) {
        return;
    }

    if (state->input_buffer.length > 0) {
        point_t key = input_buffer_dequeue(&state->input_buffer);
        if (point_eq(key, KEY_LEFT) && state->snake.dir != DIR_RIGHT) {
            state->snake.dir = DIR_LEFT;
        }
        if (point_eq(key, KEY_RIGHT) && state->snake.dir != DIR_LEFT) {
            state->snake.dir = DIR_RIGHT;
        }
        if (point_eq(key, KEY_UP) && state->snake.dir != DIR_DOWN) {
            state->snake.dir = DIR_UP;
        }
        if (point_eq(key, KEY_DOWN) && state->snake.dir != DIR_UP) {
            state->snake.dir = DIR_DOWN;
        }
    }

    point_t prev_pos = state->snake.head;

    switch (state->snake.dir) {
        case DIR_LEFT:
            state->snake.head.x = (state->snake.head.x == 1) ? GRID[state->snake.head.y] - 2 : state->snake.head.x - 1;
            break;
        case DIR_RIGHT:
            state->snake.head.x = (state->snake.head.x == GRID[state->snake.head.y] - 2) ? 1 : state->snake.head.x + 1;
            break;
        case DIR_UP:
            state->snake.head.y = (state->snake.head.y == 1) ? MATRIX_ROWS - 2 : state->snake.head.y - 1;
            if (state->snake.head.x > GRID[state->snake.head.y] - 2) {
                state->snake.head.x = GRID[state->snake.head.y] - 2;
            }
            break;
        case DIR_DOWN:
            state->snake.head.y = (state->snake.head.y == MATRIX_ROWS - 2) ? 1 : state->snake.head.y + 1;
            if (state->snake.head.x > GRID[state->snake.head.y] - 2) {
                state->snake.head.x = GRID[state->snake.head.y] - 2;
            }
            break;
    }

    for (uint8_t i = 0; i < state->snake.length; ++i) {
        point_t temp         = state->snake.body[i];
        state->snake.body[i] = prev_pos;
        prev_pos             = temp;
    }

    if (point_eq(state->snake.head, state->food)) {
        state->snake.body[state->snake.length] = state->snake.body[state->snake.length - 1];
        state->snake.length += 1;
        state->food = snake_game_get_food(state);
    }
}

static bool snake_game_render(effect_params_t *params, snake_game_state_t *state) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        point_t p = point_from_index(i);
        if (point_eq(p, state->snake.head)) {
            rgb_matrix_set_color(i, snake_head_rgb.r, snake_head_rgb.g, snake_head_rgb.b);
        } else if (snake_contains(&state->snake, p)) {
            rgb_matrix_set_color(i, snake_body_rgb.r, snake_body_rgb.g, snake_body_rgb.b);
        } else if (point_eq(p, state->food)) {
            rgb_matrix_set_color(i, snake_food_rgb.r, snake_food_rgb.g, snake_food_rgb.b);
        } else if (p.x == 0 || p.x == GRID[p.y] - 1 || p.y == 0 || p.y == MATRIX_ROWS - 1) {
            rgb_matrix_set_color(i, border_rgb.r, border_rgb.g, border_rgb.b);
        } else {
            rgb_matrix_set_color(i, grid_rgb.r, grid_rgb.g, grid_rgb.b);
        }
    }
    return rgb_matrix_check_finished_leds(led_max);
}

static bool SNAKE(effect_params_t *params) {
    if (params->init) {
        snake_game_setup(&snake_game_state);
    }
    snake_game_update(&snake_game_state);
    return snake_game_render(params, &snake_game_state);
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif
