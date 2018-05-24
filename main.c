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

/*****************************    Defines    *******************************/
#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define ALTERNATE_STACK_SIZE 512
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
  rgb_init(); // Initialize RGB LED on the Tiva board
  uart0_init(9600, 8, 1, 0); // Initialize UART0
  file_init(); // Initialize files, to easily interact with UART0, LCD, Keypad and Buttons

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

  // Start the tasks.
  // ----------------
  xTaskCreate(button1_task, "Button1 task", USERTASK_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(button2_task, "Button2 task", USERTASK_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(keypad_task, "Keypad_task_name", USERTASK_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(rtc_task, "RTC task", USERTASK_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(uart_rx_task, "UART RX task", USERTASK_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(uart_tx_task, "UART TX task", USERTASK_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(comm_task, "Comm task", 640, NULL, 1, NULL);


  // Start the scheduler.
  // --------------------
  vTaskStartScheduler();

  // Will only get here, if there was insufficient memory.
  // -----------------------------------------------------
  return 1;
}

/****************************** End Of Module *******************************/
