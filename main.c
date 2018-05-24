/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "systick_frt.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "button.h"
#include "button_task.h"

/*****************************    Defines    *******************************/
#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3


/*****************************   Constants   *******************************/

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
  button_init();

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


  // Queues
  xQueueHandle xQueueButton = xQueueCreate(16, sizeof(INT8U));

  // Start the tasks.
  // ----------------
  xTaskCreate( button_task, /* Pointer to the function that implements the task. */
              "Button task",/* Text name for the task. This is to facilitate debugging only. */
              USERTASK_STACK_SIZE, /* Stack depth - small microcontrollers will use much less stack than this. */
              NULL, /* This example does not use the task parameter. */
               1, /* This task will run at priority 1. */
              NULL ); /* This example does not use the task handle. */

  // Start the scheduler.
  // --------------------
  vTaskStartScheduler();

  // Will only get here, if there was insufficient memory.
  // -----------------------------------------------------
  return 1;
}

/****************************** End Of Module *******************************/
