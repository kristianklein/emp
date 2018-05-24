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
#include "keypad_task.h"
#include "queue.h"

/*****************************    Defines    *******************************/
#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3


//User defines
#define KEYPAD_Q_SIZE 10


/*****************************   Queue Declarations   *******************************/
xQueueHandle xQueueKeypad;


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

    //Switch: GIPO Port F bit 4 - base: 0x4002-500
    //LED's: GIPO Port F bit 1-3 - base: 0x4002-500

    //Enalble the GPIO port
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;

    //Set the direction (0=input and 1=output)
    GPIO_PORTF_DIR_R = 0x0E;        //Switch 1 (bit 4) to 0 and LED (bit 1-3) to 1

    //Enable the GPIO pins
    GPIO_PORTF_DEN_R = 0x1E;

    //Enable internal pull-up
    GPIO_PORTF_PUR_R = 0x10;


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


  // Start the tasks.
  // ----------------
  xTaskCreate(keypad_task,          //Task function pointer
              "Keypad_task_name",   //Task name
              USERTASK_STACK_SIZE,  //Stack size
              NULL,                 //Input parameter
              1,                    //Priority
              NULL);                //Task Handle



  // Start the scheduler.
  // --------------------
  vTaskStartScheduler();

  // Will only get here, if there was insufficient memory.
  // -----------------------------------------------------
  return 1;
}

/****************************** End Of Module *******************************/
