/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: main.c
*
* PROJECT....: EMP
*
* DESCRIPTION: Assignment 2, main module. No main.h file.
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150215  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "systick_frt.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "button.h"
#include "rtc.h"
#include "taskmodel.h"
#include "rgb.h"
#include "uart0.h"
#include "file.h"
#include "flow.h"
#include "semphr.h"


/*****************************    Defines    *******************************/
#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define MEDIUM_STACK_SIZE 128
#define LARGE_STACK_SIZE 256
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3

//User defines
#define KEYPAD_Q_SIZE 10


/*****************************   Queue Declarations   *******************************/
xQueueHandle xQueueKeypad;
xQueueHandle xQueueButton;
xQueueHandle xQueueUART_TX;
xQueueHandle xQueueUART_RX;
xQueueHandle xQueueLCD;
xQueueHandle xQueueDigi_switch;
xQueueHandle xQueueMaxMilliliters;
xQueueHandle xQueueMillilitersFueled;

/*****************************   Semaphore Declarations   *******************************/
xSemaphoreHandle xSemaphorePumpActive;
xSemaphoreHandle xSemaphoreFuelingDone;

/*****************************   Variables   *******************************/


/*****************************   Functions   *******************************/


int putChar()
/*****************************************************************************
*   Input    :  -
*   Output   :  Result
*   Function :  putChar for FreeRTOS debug functionality.
*****************************************************************************/
{
  return(0);
}

static void setupHardware(void)
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :
*****************************************************************************/
{
  // TODO: Put hardware configuration and initialisation in here
  uart0_init(9600, 8, 1, 0); // Initialize UART0
  rgb_init(); // Initialize RGB LED on the Tiva board
  file_init(); // Initialize files, to easily interact with UART0, LCD, Keypad and Buttons
  flow_init(); // Initialize flow meter (TIMER0A)

  rgb_set(1,1,1); // Set RGB high (all EMP-kit LEDs off)

  // Warning: If you do not initialize the hardware clock, the timings will be inaccurate
  init_systick();
}

int main(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : The super loop.
******************************************************************************/
{
  setupHardware();

  // Open the Q's.
  xQueueKeypad = xQueueCreate(KEYPAD_Q_SIZE,sizeof(INT8U));
  xQueueButton = xQueueCreate(16, sizeof(INT8U));
  xQueueUART_TX = xQueueCreate(128, sizeof(INT8U));
  xQueueUART_RX = xQueueCreate(128, sizeof(INT8U));
  xQueueLCD = xQueueCreate(128, sizeof(INT8U));
  xQueueDigi_switch = xQueueCreate(128, sizeof(INT8U));
  xQueueMaxMilliliters = xQueueCreate(1, sizeof(INT32U));
  xQueueMillilitersFueled = xQueueCreate(1, sizeof(INT32U));

  // Initialize semaphores
  vSemaphoreCreateBinary(xSemaphorePumpActive);
  xSemaphoreTake(xSemaphorePumpActive, 1); // Take semaphore, so it is initialized to zero

  vSemaphoreCreateBinary(xSemaphoreFuelingDone);
  xSemaphoreTake(xSemaphoreFuelingDone, 1); // Take semaphore, so it is initialized to zero

  // Start the tasks.
  // ----------------
  xTaskCreate(button1_task, "Button1", USERTASK_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(button2_task, "Button2", USERTASK_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(digi_task, "Digitask", USERTASK_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(comm_task, "Comm", MEDIUM_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(lcd_task, "LCD task", USERTASK_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(keypad_task, "Keypad", USERTASK_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(uart_rx_task, "UART RX", USERTASK_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(uart_tx_task, "UART TX", USERTASK_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(pump_task, "Pump", USERTASK_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(rtc_task, "RTC task", 128, NULL, 1, NULL);

  // Start the scheduler.
  // --------------------
  vTaskStartScheduler();

  // Will only get here, if there was insufficient memory.
  // -----------------------------------------------------
  return 1;
}

/****************************** End Of Module *******************************/
