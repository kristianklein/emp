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
#define ACCOUNT_SIZE            7
#define PIN_SIZE                4

/***************************** Typedefs **************************************/
typedef enum {WELCOME, PAYMENT, CASH, ACCOUNT, PIN, FULETYPE, PUMP_READY, FUELING_DONE} UI_STATE_T;

/***************************** Variables *************************************/

/***************************** Helper dec. ***********************************/
void welcome_text();
void payment_text();
void cash_text();
void account_text();
void fuletype_text();
void pumpready_text();
/***************************** Functions *************************************/
void ui_task(void *pvParameters)
{
    UI_STATE_T state = WELCOME;

    extern xQueueHandle xQueueKeypad;
    extern xQueueHandle xQueueDigi_switch;
    INT8U keypad_received, digi_received, fuletype = 0, account_length = 0, pin_length = 0;
    INT16U Cash_amount = 0;
    INT8U account_number[ACCOUNT_SIZE], pin_number[PIN_SIZE];

    welcome_text();
    while (1)
    {
        keypad_received = 0;
        digi_received = 0;
        xQueueReceive(xQueueKeypad, &keypad_received,0);
        xQueueReceive(xQueueDigi_switch, &digi_received,0);

        switch (state)
        {
/*************************** Welcome state ***********************************/
            case WELCOME:
                if(keypad_received || digi_received)
                {
                    state = PAYMENT;
                    payment_text();
                }
                break;

/*************************** Payment state ***********************************/
            case PAYMENT:
                if(keypad_received == '1')
                {
                    state =  CASH;
                    cash_text();
                }
                else if (keypad_received == '2')
                {
                    state =  ACCOUNT;
                    account_text();

                }
                else if(keypad_received == '*')
                {
                    welcome_text();
                    state = WELCOME;
                }
                break;

/*************************** Cash state ***********************************/
            case CASH:                                                                          //Warning! It fucks up when Cash_amount = 100

                if(digi_received == 1)
                {
                    Cash_amount = Cash_amount + 50;
                    file_put(LCD, 0x00);
                    gfprintf(LCD, " Cash amount");
                    file_put(LCD, 0x0A);
                    gfprintf(LCD, "%d kr", Cash_amount);
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
                    fuletype_text();
                    state = FULETYPE;
                }
                else if(keypad_received == '*')
                {
                    welcome_text();
                    state = WELCOME;
                    Cash_amount = 0;
                }
                break;

/*************************** Account state ***********************************/
            case ACCOUNT:
                if (!keypad_received)
                {
                    //If no value has been received then do nothing.
                }
                else if(keypad_received == '*')
                {
                    welcome_text();
                    state = WELCOME;
                    account_length = 0;
                }
                else if(keypad_received == '#' && account_length == ACCOUNT_SIZE)
                {
                    file_put(LCD, 0x00);
                    gfprintf(LCD, " Pin code");
                    file_put(LCD, 0x0A);
                    account_length = 0;
                    state = PIN;
                }
                else
                {
                    if(account_length < ACCOUNT_SIZE)
                    {
                        gfprintf(LCD, "%d",(keypad_received-48));                                               //Warning! 0 don't work
                        account_number[account_length] = keypad_received-48;
                        account_length++;
                    }
                }
                break;

/*************************** Pin state ***********************************/
            case PIN:
                if (!keypad_received)
                {
                    //If no value has been received then do nothing.
                }
                else if(keypad_received == '*')
                {
                    welcome_text();
                    state = WELCOME;
                    pin_length = 0;
                }
                else if(keypad_received == '#' && pin_length == PIN_SIZE)
                {
                    fuletype_text();
                    pin_length = 0;
                    state = FULETYPE;
                }
                else
                {
                    if(pin_length < PIN_SIZE)
                    {
                        gfprintf(LCD, "*");
                        account_number[pin_length] = keypad_received-48;
                        pin_length++;
                    }
                }
                break;

/*************************** Fule type state ***********************************/
            case FULETYPE:
                if(keypad_received == '1')
                {
                    pumpready_text();
                    fuletype = 1;
                    state = PUMP_READY;
                }
                else if(keypad_received == '2')
                {
                    pumpready_text();
                    fuletype = 2;
                    state = PUMP_READY;
                }
                else if(keypad_received == '3')
                {
                    pumpready_text();
                    fuletype = 3;
                    state = PUMP_READY;
                }
                else if(keypad_received == '*')
                {
                    fuletype = 0;
                    welcome_text();
                    state = WELCOME;
                }
                break;

/*************************** Pump ready state ***********************************/
            case PUMP_READY:
                break;

/*************************** Fueling done state ***********************************/
            case FUELING_DONE:
                break;

/*************************** default  state ***********************************/
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


void payment_text()
{
    file_put(LCD, 0x00);
    gfprintf(LCD, " Press 1: Cash");
    file_put(LCD, 0x0A);
    gfprintf(LCD, "Press 2: Account");
}


void cash_text()
{
    file_put(LCD, 0x00);
    gfprintf(LCD, " Cash amount");
    file_put(LCD, 0x0A);
    gfprintf(LCD, "0 kr.");
}



void account_text()
{
    file_put(LCD, 0x00);
    gfprintf(LCD, " Account number");
    file_put(LCD, 0x0A);
}


void fuletype_text()
{
    file_put(LCD, 0x00);
    gfprintf(LCD, " Choose fule type");
    file_put(LCD, 0x0A);
    gfprintf(LCD, "1:E10 2:92 3:95");
}

void pumpready_text()
{
    file_put(LCD, 0x00);
    gfprintf(LCD, " Pump is ready");
    file_put(LCD, 0x0A);
    gfprintf(LCD, "Handle is on");
}





