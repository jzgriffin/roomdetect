// model.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
// Assignment: Custom Project - Room Detector - Compute Application
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_COMPUTE_MODEL_H
#define CUSTOM_COMPUTE_MODEL_H

#include <stddef.h>
#include <stdfix.h>

#define MODEL_VECTOR_SIZE 3
#define MODEL_ROOMS_MAX 0x0F

typedef struct {
    size_t samples;
    accum mean;
    accum var;
    accum stdev;
} vector_t;

typedef struct {
    size_t samples;
    vector_t vectors[MODEL_VECTOR_SIZE];
} room_t;

typedef struct {
    room_t rooms[MODEL_ROOMS_MAX];
} model_t;

void clear_vector(vector_t *v);
void update_vector(vector_t *v, accum x);
accum score_vector(const vector_t *v, accum x);

void clear_room(room_t *r);
void update_room(room_t *r, const accum *x);
accum score_room(const room_t *r, const accum *x);

void clear_model(model_t *m);
size_t classify_model(const model_t *m, const accum *x);

#endif // CUSTOM_COMPUTE_MODEL_H
