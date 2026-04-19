#pragma once

#include "matrix.h"

static const uint8_t MATRIX_ROW_LENGTHS[MATRIX_ROWS] = {14, 14, 13, 12, 8};

static uint8_t get_row_length(uint8_t row) {
    return MATRIX_ROW_LENGTHS[row];
}

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
    while (p.x >= MATRIX_ROW_LENGTHS[p.y]) {
        p.x -= MATRIX_ROW_LENGTHS[p.y];
        p.y += 1;
    }
    return p;
}

uint8_t point_into_index(point_t p) {
    uint8_t idx = p.x;
    for (uint8_t i = 0; i < p.y; ++i) {
        idx += MATRIX_ROW_LENGTHS[i];
    }
    return idx;
}

#define INPUT_BUFFER_SIZE 3
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

static matrix_row_t prev_matrix[MATRIX_ROWS];

static void update_matrix_state(void) {
    for (uint8_t i = 0; i < MATRIX_ROWS; ++i) {
        prev_matrix[i] = matrix_get_row(i);
    }
}

static bool key_is_pressed(point_t p) {
    matrix_row_t current = matrix_get_row(p.y);
    matrix_row_t prev    = prev_matrix[p.y];
    matrix_row_t mask    = (matrix_row_t)1 << p.x;
    return (current & mask) && !(prev & mask);
}
