// task.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_COMMON_TASK_H
#define CUSTOM_COMMON_TASK_H

#include <stdint.h>

/// \brief Type for storing task periods in milliseconds
typedef uint16_t task_period_t;

/// \brief Type capable of storing any task state
typedef uint16_t task_state_t;

/// \brief Signature for task tick functions
typedef task_state_t task_tick_t(task_state_t);

/// \brief Task meant for use with the scheduler
typedef struct {
    task_state_t state; //!< Current state of the task
    task_tick_t *tick; //!< Tick function for the task
    task_period_t period; //!< Period of the task in milliseconds
    task_period_t elapsed; //!< Elapsed milliseconds since the last tick
} task_t;

#endif // CUSTOM_COMMON_TASK_H
