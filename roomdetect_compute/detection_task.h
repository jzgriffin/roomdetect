// detection_task.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
// Assignment: Custom Project - Room Detector - Compute Application
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_COMPUTE_DETECTION_TASK_H
#define CUSTOM_COMPUTE_DETECTION_TASK_H

#include "task.h"
#include <stdint.h>

/// \brief Task for the detection state machine
extern task_t detection_task;

/// \brief Number of the room that was detected
extern uint8_t detected_room;

#endif // CUSTOM_COMPUTE_DETECTION_TASK_H
