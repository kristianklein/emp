/***************************** Included files ********************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "taskmodel.h"
#include "semphr.h"
#include "string.h"
#include "lcd.h"
#include "ui_task.h"
#include "file.h"

/***************************** Defines ***************************************/
typedef enum {WELCOME, PAYMENT, CASH, ACCOUNT, PIN, FULETYPE, PUMP_READY, FUELING_DONE} UI_STATE_T;
/***************************** Constants *************************************/
/***************************** Variables *************************************/

/***************************** Helper dec. ************************************/
void welcome_text();

/***************************** Functions *************************************/
void ui_task(void *pvParameters)
{
    UI_STATE_T state = WELCOME;

    extern xQueueHandle xQueueKeypad;
    extern xQueueHandle xQueueDigi_switch;
    INT8U keypad_received, digi_received, fuletype = 0;
    INT16U Cash_amount = 0;


    welcome_text();
    while (1)
    {

        keypad_received = 0;
        digi_received = 0;

        xQueueReceive(xQueueKeypad, &keypad_received,0);
        xQueueReceive(xQueueDigi_switch, &digi_received,0);

        switch (state)
        {
            case WELCOME:
                if(keypad_received || digi_received)
                {
                    state = PAYMENT;
                    file_put(LCD, 0x00);
                    gfprintf(LCD, " Press 1: Cash");
                    file_put(LCD, 0x0A);
                    gfprintf(LCD, "Press 2: Account");
                }
                break;
            case PAYMENT:
                if(keypad_received == '1')
                {
                    state =  CASH;
                    file_put(LCD, 0x00);
                    gfprintf(LCD, " Cash amount");
                    file_put(LCD, 0x0A);
                    gfprintf(LCD, "0 kr.");
                }
                else if (keypad_received == '2')
                {
                    state =  ACCOUNT;
                    file_put(LCD, 0x00);
                    gfprintf(LCD, " Account number");
                }
                else if(keypad_received == '*')
                {
                    welcome_text();
                    state = WELCOME;
                }
                break;
            case CASH:

                if(digi_received == 1)
                {
                    Cash_amount = Cash_amount + 50;
                    file_put(LCD, 0x00);
                    gfprintf(LCD, " Cash amount");
                    file_put(LCD, 0x0A);
                    gfprintf(LCD, "%d kr.", Cash_amount);
                }
                else if(digi_received == 2)
                {
                    Cash_amount = Cash_amount + 100;
                    file_put(LCD, 0x00);
                    gfprintf(LCD, " Cash amount");
                    file_put(LCD, 0x0A);
                    gfprintf(LCD, "%d kr",Cash_amount);
                }
                else if(digi_received == 3)
                {
                    file_put(LCD, 0x00);
                    gfprintf(LCD, " Choose fule type");
                    file_put(LCD, 0x0A);
                    gfprintf(LCD, "1:E10 2:92 3:95");
                    state = FULETYPE;
                }
                else if(keypad_received == '*')
                {
                    welcome_text();
                    state = WELCOME;
                    Cash_amount = 0;
                }
                break;
            case ACCOUNT:
                break;
            case PIN:
                break;
            case FULETYPE:
                if(keypad_received == '1')
                {
                    fuletype = 1;
                    state = PUMP_READY;
                    // set ind pumpt ready code.
                }
                else if(keypad_received == '2')
                {
                    fuletype = 2;
                    state = PUMP_READY;
                    // set ind pumpt ready code.

                }
                else if(keypad_received == '3')
                {
                    fuletype = 3;
                    state = PUMP_READY;
                    // set ind pumpt ready code.
                }
                else if(keypad_received == '*')
                {
                    fuletype = 0;
                    welcome_text();
                    state = WELCOME;
                }
                break;
            case PUMP_READY:
                break;
            case FUELING_DONE:
                break;
            default:
                break;
        }
    }
}


/***************************** Helper def. ************************************/
void welcome_text()
{
    file_put(LCD, 0x00);
    gfprintf(LCD, " Welcome");
    file_put(LCD, 0x0A);
    gfprintf(LCD, "Press any button");

}



