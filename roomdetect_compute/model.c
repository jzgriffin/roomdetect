// model.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
// Assignment: Custom Project - Room Detector - Compute Application
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "model.h"
#include "numeric.h"
#include <math.h>
#include <avr/eeprom.h>

void clear_vector(vector_t *v)
{
    v->samples = 0;
    v->mean = 0K;
    v->var = 0K;
    v->stdev = 0K;
}

void update_vector(vector_t *v, accum x)
{
    size_t n = v->samples + 1;
    accum delta = x - v->mean;
    accum mean = v->mean + delta / (accum)n;
    accum var = v->var + delta * (x - mean);

    v->samples = n;
    v->mean = mean;
    v->var = var;
    if (n < 2) {
        v->stdev = (accum)sqrt((double)(var / (accum)n));
    }
    else {
        v->stdev = (accum)sqrt((double)(var / (accum)(n - 1)));
    }
}

accum score_vector(const vector_t *v, accum x)
{
    if (v->samples == 0 || v->stdev == 0K) {
        return 0K;
    }
    return (x - v->mean) / v->stdev;
}

void clear_room(room_t *r)
{
    for (size_t i = 0; i < MODEL_VECTOR_SIZE; ++i) {
        clear_vector(&r->vectors[i]);
    }
    r->samples = 0;
}

void update_room(room_t *r, const accum *x)
{
    for (size_t i = 0; i < MODEL_VECTOR_SIZE; ++i) {
        update_vector(&r->vectors[i], x[i]);
    }
    ++r->samples;
}

accum score_room(const room_t *r, const accum *x)
{
    if (r->samples == 0) {
        return 0K;
    }
    accum z = 0K;
    for (size_t i = 0; i < MODEL_VECTOR_SIZE; ++i) {
        accum zi = score_vector(&r->vectors[i], x[i]);
        z += zi * zi;
    }
    return (accum)sqrt((double)(z / (accum)MODEL_VECTOR_SIZE));
}

void clear_model(model_t *m)
{
    for (size_t i = 0; i < MODEL_ROOMS_MAX; ++i) {
        clear_room(&m->rooms[i]);
    }
}

void write_model_eeprom(const model_t *m, model_t *a)
{
    eeprom_update_block(m, a, sizeof(*m));
}

void read_model_eeprom(model_t *m, const model_t *a)
{
    eeprom_read_block(m, a, sizeof(*m));
}

size_t classify_model(const model_t *m, const accum *x)
{
    size_t i_close = 0xFF;
    accum z_close = ACCUM_MAX;
    for (size_t i = 0; i < MODEL_ROOMS_MAX; ++i) {
        if (m->rooms[i].samples == 0) {
            continue;
        }

        accum z = score_room(&m->rooms[i], x);
        if (z < z_close) {
            i_close = i;
            z_close = z;
        }
    }
    return i_close;
}
