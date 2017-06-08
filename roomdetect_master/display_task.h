// display_task.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
// Assignment: Custom Project - Room Detector - Master Application
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_MASTER_DISPLAY_TASK_H
#define CUSTOM_MASTER_DISPLAY_TASK_H

#include "task.h"
#include <stdbool.h>

/// \brief Task for the display state machine
extern task_t display_task;

/// \brief Whether or not the EEPROM save message should be displayed
///
/// This variable is used as input to the display task.
extern bool should_display_save;

/// \brief Whether or not the EEPROM reset message should be displayed
///
/// This variable is used as input to the display task.
extern bool should_display_reset;

/// \brief Whether or not the EEPROM erase message should be displayed
///
/// This variable is used as input to the display task.
extern bool should_display_erase;

#endif // CUSTOM_MASTER_DISPLAY_TASK_H
