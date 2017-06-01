// mode_task.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
// Assignment: Custom Project - Room Detector - Master Application
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_MASTER_MODE_TASK_H
#define CUSTOM_MASTER_MODE_TASK_H

#include "task.h"
#include <stdint.h>

/// \brief Modes for the device to be in
typedef enum {
    DEVICE_MODE_DETECTION, //!< Device is in detection mode
    DEVICE_MODE_TRAINING, //!< Device is in training mode
} device_mode_t;

/// \brief Task for the mode state machine
extern task_t mode_task;

/// \brief Mode the device is in
extern device_mode_t device_mode;

#endif // CUSTOM_MASTER_MODE_TASK_H
