// mode_task.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
// Assignment: Custom Project - Room Detector - Compute Application
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_COMPUTE_MODE_TASK_H
#define CUSTOM_COMPUTE_MODE_TASK_H

#include "model.h"
#include "task.h"
#include <avr/eeprom.h>
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

/// \brief Model to use for detection and training
extern model_t model;

/// \brief Location to store the model in EEPROM
extern EEMEM model_t model_eeprom;

#endif // CUSTOM_COMPUTE_MODE_TASK_H
