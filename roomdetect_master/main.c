// main.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
// Assignment: Custom Project - Room Detector - Master Application
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "detection_task.h"
#include "mode_task.h"
#include "training_task.h"
#include "lcd_task.h"
#include "schedule.h"
#include "usart_packet_rx_task.h"
#include "usart_rx_task.h"
#include "usart_tx_task.h"
#include <stddef.h>

int main()
{
    schedule_task(&lcd_task);
    schedule_task(&usart_tx_task);
    schedule_task(&usart_rx_task);
    schedule_task(&usart_packet_rx_task);
    schedule_task(&mode_task);
    schedule_task(&detection_task);
    schedule_task(&training_task);
    run_schedule();
}
