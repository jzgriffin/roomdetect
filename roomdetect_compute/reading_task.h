// reading_task.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
// Assignment: Custom Project - Room Detector - Compute Application
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_COMPUTE_READING_TASK_H
#define CUSTOM_COMPUTE_READING_TASK_H

#include "model.h"
#include "task.h"
#include <stdbool.h>
#include <stdfix.h>

/// \brief Task for the reading state machine
extern task_t reading_task;

/// \brief Current reading vector from the sensors
///
/// This variable is used as output by the reading task.
extern accum reading_vector[MODEL_VECTOR_SIZE];

/// \brief Whether or not the sensor reading vector is ready
///
/// This variable is used as output by the reading task.
extern bool are_readings_ready;

/// \brief Whether or not the sensor reading vector should be updated
///
/// This variable is used as input by the reading task.
extern bool should_poll_readings;

#endif // CUSTOM_COMPUTE_READING_TASK_H
