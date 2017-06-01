// schedule.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "schedule.h"
#include "config.h"
#include "interrupt.h"
#include "numeric.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <stddef.h>

static task_period_t timer_period;
static task_t *tasks[MAXIMUM_TASKS];
static volatile size_t task_count;
static volatile bool is_running;

ISR(TIMER3_COMPA_vect)
{
    // ATMega generates TIMER3_COMPA when TCNT3 == OCR3
    for (size_t i = 0; i < MAXIMUM_TASKS; ++i) {
        task_t *task = tasks[i];
        if (task == NULL) {
            continue;
        }
        if (task->elapsed >= task->period) {
            task->state = task->tick(task->state);
            task->elapsed = 0;
        }
        task->elapsed += timer_period;
    }
}

static void enable_timer()
{
    // COM3A = 00 (normal port operation, disconnect OC3A)
    // COM3B = 00 (normal port operation, disconnect OC3B)
    // WGM3 = 0100 (CTC mode, TOP = OCR3A)
    // ICNC3 = 0 (disable input capture noise canceler)
    // ICES3 = 0 (falling input capture edge trigger)
    // CS3 = 011 (clock select = clk_io/64)
    // ICIE3 = 0 (disable input capture interrupt)
    // OCIE3B = 0 (disable output compare B match interrupt)
    // OCIE3A = 1 (enable output compare A match interrupt)
    // TOIE3 = 0 (disable overflow interrupt)
    TCCR3A =
        (0 << COM3A1) | (0 << COM3A0) |
        (0 << COM3B1) | (0 << COM3B0) |
        (0 << WGM31) | (0 << WGM30);
    TCCR3B =
        (0 << ICNC3) |
        (0 << ICES3) |
        (0 << WGM33) | (1 << WGM32) |
        (0 << CS32) | (1 << CS31) | (1 << CS30);
    TIMSK3 =
        (0 << ICIE3) |
        (0 << OCIE3B) |
        (1 << OCIE3A) |
        (0 << TOIE3);
    OCR3A = timer_period * (F_CPU / 64 / 1000); // T ms using /64 prescaler
    TCNT3 = 0;
    enable_interrupts();
}

static void disable_timer()
{
    disable_interrupts();
    TCCR3A = 0;
    TCCR3B = 0;
    TIMSK3 = 0;
    OCR3A = 0;
}

void run_schedule()
{
    timer_period = 0;
    for (size_t i = 0; i < MAXIMUM_TASKS; ++i) {
        if (tasks[i] == NULL) {
            continue;
        }
        timer_period = gcd(timer_period, tasks[i]->period);
    }

    set_sleep_mode(SLEEP_MODE_IDLE);

    is_running = true;
    enable_timer();
    while (task_count > 0) {
        sleep_mode();
    }
    disable_timer();
    is_running = false;
}

bool schedule_task(task_t *task)
{
    if (is_running) {
        return false;
    }

    for (size_t i = 0; i < MAXIMUM_TASKS; ++i) {
        if (tasks[i] == task) {
            break;
        }
        if (tasks[i] != NULL) {
            continue;
        }
        tasks[i] = task;
        ++task_count;
        return true;
    }
    return false;
}

bool unschedule_task(const task_t *task)
{
    for (size_t i = 0; i < MAXIMUM_TASKS; ++i) {
        if (tasks[i] != task) {
            continue;
        }
        tasks[i] = NULL;
        --task_count;
        return true;
    }
    return false;
}
