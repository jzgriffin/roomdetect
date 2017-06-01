// schedule.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_COMMON_SCHEDULE_H
#define CUSTOM_COMMON_SCHEDULE_H

#include "task.h"
#include <stdbool.h>

/// \brief Maximum number of tasks supported
#define MAXIMUM_TASKS 16

/// \brief Runs the schedule
///
/// Using timer/counter 3, executes the schedule created using
/// \ref schedule_task until all tasks are removed using \ref unschedule_task.
void run_schedule();

/// \brief Schedules a task to be periodically executed
/// \param task task to schedule
/// \return \c true if the task was added; otherwise \c false
///
/// Tasks cannot be scheduled when \ref MAXIMUM_TASKS tasks are scheduled or
/// the scheduler is running.
bool schedule_task(task_t *task);

/// \brief Unschedules a task
/// \param task task to unschedule
/// \return \c true if the task was removed; otherwise \c false
///
/// If all tasks are unscheduled while the scheduler is running, it will stop.
bool unschedule_task(const task_t *task);

#endif // CUSTOM_COMMON_SCHEDULE_H
