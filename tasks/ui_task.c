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
#include "price.h"
#include "rtc.h"

// Queue declarations
extern xQueueHandle xQueueKeypad;
extern xQueueHandle xQueueButton;
extern xQueueHandle xQueueUART_TX;
extern xQueueHandle xQueueUART_RX;
extern xQueueHandle xQueueLCD;
extern xQueueHandle xQueueDigi_switch;
extern xQueueHandle xQueueMaxMilliliters;
extern xQueueHandle xQueueMillilitersFueled;

// Semaphore declarations
extern xSemaphoreHandle xSemaphorePumpActive;
extern xSemaphoreHandle xSemaphoreFuelingDone;


/***************************** Defines ***************************************/
#define ACCOUNT_SIZE            6
#define PIN_SIZE                4

/***************************** Typedefs **************************************/
typedef enum {WELCOME, PAYMENT, CASH, ACCOUNT, PIN, FUELTYPE, PUMP_READY, FUELING_DONE} UI_STATE_T;

/***************************** Variables *************************************/

/***************************** Helper dec. ***********************************/
void welcome_text();
void payment_text();
void cash_text();
void account_text();
void fuletype_text();
void pumpready_text();
void ui_print_fuel(INT32U milliliters);
void ui_print_summary(INT32U milliliters, INT64U price);
void ui_clear();
INT32U calc_max_milliliters(GAS_T fuel_type, INT16U cash_amount);

/***************************** Functions *************************************/
void ui_task(void *pvParameters)
{
    UI_STATE_T state = WELCOME;

    INT32U milliliters_fuelled;
    INT32U max_milliliters;
    INT32U trash;
    GAS_T fuel_type = GAS_92; // TODO: Change this when fuel selection is implemented

    INT8U keypad_received, digi_received, account_length = 0, pin_length = 0;
    INT16U Cash_amount = 0;
    INT8U account_number[ACCOUNT_SIZE], pin_number[PIN_SIZE];

    BOOLEAN paid_cash = 0;

    welcome_text();
		
    while (1)
    {
//        keypad_received = 0;
//        digi_received = 0;
        if(!xQueueReceive(xQueueKeypad, &keypad_received,0))
        {
            keypad_received = 0;
        }
        if(!xQueueReceive(xQueueDigi_switch, &digi_received,0))
        {
            digi_received = 0;
        }

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
                    paid_cash = 1;
                    cash_text();
                }
                else if (keypad_received == '2')
                {
                    state =  ACCOUNT;
                    paid_cash = 0;
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
                    gfprintf(LCD, "Cash amount");
                    file_put(LCD, 0x0A);
                    gfprintf(LCD, "%d kr", Cash_amount);
                }
                else if(digi_received == 2)
                {
                    Cash_amount = Cash_amount + 100;
                    file_put(LCD, 0x00);
                    gfprintf(LCD, "Cash amount");
                    file_put(LCD, 0x0A);
                    gfprintf(LCD, "%d kr",Cash_amount);
                }
                else if(keypad_received == '#')
                {
                    fuletype_text();
                    state = FUELTYPE;
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
                    gfprintf(LCD, "Pin code");
                    file_put(LCD, 0x0A);
                    account_length = 0;
                    state = PIN;
                }
                else if(!(keypad_received == '#'))
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
                    state = FUELTYPE;
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
            case FUELTYPE:
                if(keypad_received == '1')
                {
                    /*
                    // Calculate max fuel and push it to queue
                    if (paid_cash)
                    {
                        max_milliliters = calc_max_milliliters(fuel_type, Cash_amount);
                    }
                    else
                    {
                        max_milliliters = 0;
                    }
                    xQueueReceive(xQueueMaxMilliliters, &trash, 0);
                    xQueueSendToBack(xQueueMaxMilliliters, &max_milliliters, 0);
                    */

                    pumpready_text();
                    fuel_type = GAS_92;
                    state = PUMP_READY;
                    xSemaphoreGive(xSemaphorePumpActive);

                }
                else if(keypad_received == '2')
                {
                    /*
                    // Calculate max fuel and push it to queue
                    if (paid_cash)
                    {
                        max_milliliters = calc_max_milliliters(fuel_type, Cash_amount);
                    }
                    else
                    {
                        max_milliliters = 0;
                    }
                    xQueueReceive(xQueueMaxMilliliters, &trash, 0);
                    xQueueSendToBack(xQueueMaxMilliliters, &max_milliliters, 0);
                    */

                    pumpready_text();
                    fuel_type = GAS_95;
                    state = PUMP_READY;
                    xSemaphoreGive(xSemaphorePumpActive);

                }
                else if(keypad_received == '3')
                {
                    /*
                    // Calculate max fuel and push it to queue
                    if (paid_cash)
                    {
                        max_milliliters = calc_max_milliliters(fuel_type, Cash_amount);
                    }
                    else
                    {
                        max_milliliters = 0;
                    }
                    xQueueReceive(xQueueMaxMilliliters, &trash, 0);
                    xQueueSendToBack(xQueueMaxMilliliters, &max_milliliters, 0);
                    */

                    pumpready_text();
                    fuel_type = GAS_E10;
                    state = PUMP_READY;
                    xSemaphoreGive(xSemaphorePumpActive);


                }
                else if(keypad_received == '*')
                {
                    welcome_text();
                    state = WELCOME;
                }
                break;


/*************************** Pump ready state ***********************************/

            case PUMP_READY:
                // TODO: Set pump ready semaphore in the state that leads to this state

                // Continuously read Litersfueled queue and print it to LCD (once per second)
                if (xQueueReceive(xQueueMillilitersFueled, &milliliters_fuelled, 0))
                {
                    ui_print_fuel(milliliters_fuelled);
                }

                // If fueling done semaphore is available, switch state
                if (xSemaphoreTake(xSemaphoreFuelingDone, 1))
                {
                    state = FUELING_DONE;
                }

                break;

/*************************** Fueling done state ***********************************/
            case FUELING_DONE:;
                // Calculate total price is cents
                INT64U total_price = (price_get(fuel_type) * milliliters_fuelled)/1000;

                // Print summary (liters and price)
                ui_print_summary(milliliters_fuelled, total_price);

                // Save log of purchase
                log_put(rtc_get_hour(), rtc_get_min(), rtc_get_sec(), GAS_92, milliliters_fuelled, total_price, 1, 0, 0);

                vTaskDelay(200*5); // Delay 5 secs before returning to WELCOME
                state = WELCOME;
                ui_clear();
                welcome_text();

                break;

/*************************** default  state ***********************************/
            default:
                break;
        }
    }
}

/***************************** Helper def. ************************************/

void ui_print_fuel(INT32U milliliters)
{
    ui_clear();
    gfprintf(LCD, "Fuel: %02d.%03dL", milliliters/1000, milliliters%1000);
    vTaskDelay(50);
}

void ui_print_summary(INT32U milliliters, INT64U price)
{
    ui_clear();
    gfprintf(LCD, "Fuel: %02d.%03dL", milliliters/1000, milliliters%1000);
    file_put(LCD, 0x0A);
    gfprintf(LCD, "Price: %03d.", price/100);
    gfprintf(LCD, "%02d", price%100);
    gfprintf(UART0, "%08d kr.", price);
}

void ui_clear()
{
    file_put(LCD, 0x00);
    file_put(LCD, 0x00);
}

void welcome_text()
{
    file_put(LCD, 0x00);
    gfprintf(LCD, "Welcome");
    file_put(LCD, 0x0A);
    gfprintf(LCD, "Press any button");
}

void payment_text()
{
    file_put(LCD, 0x00);
    gfprintf(LCD, "Press 1: Cash");
    file_put(LCD, 0x0A);
    gfprintf(LCD, "Press 2: Account");
}

void cash_text()
{
    file_put(LCD, 0x00);
    gfprintf(LCD, "Cash amount");
    file_put(LCD, 0x0A);
    gfprintf(LCD, "0 kr");
}

void account_text()
{
    file_put(LCD, 0x00);
    gfprintf(LCD, "Account number");
    file_put(LCD, 0x0A);
}

void fuletype_text()
{
    file_put(LCD, 0x00);
    gfprintf(LCD, "Choose fuel type");
    file_put(LCD, 0x0A);
    gfprintf(LCD, "1:92 2:95 3:E10");
}

void pumpready_text()
{
    ui_clear();
    gfprintf(LCD, "Pump is ready!");
    file_put(LCD, 0x0A);
    gfprintf(LCD, "Start fueling.");
}

INT32U account_array2int(INT8U *account_array)
{
    INT32U result = 0;
    for (INT8U i = 0; i < ACCOUNT_SIZE; i++) // ACCOUNT_SIZE == 6
    {
        switch (i) // TODO: Make this nicer. Use i instead of switch statement
        {
            case 0:
                result += account_array[i]*100000;
                break;
            case 1:
                result += account_array[i]*10000;
                break;
            case 2:
                result += account_array[i]*1000;
                break;
            case 3:
                result += account_array[i]*100;
                break;
            case 4:
                result += account_array[i]*10;
                break;
            case 5:
                result += account_array[i];
                break;
            default:
                break;
        }
    }
    return result;
}

INT32U calc_max_milliliters(GAS_T fuel_type, INT16U cash_amount)
{
    INT32U max_ml = 0;

    max_ml = ((FP32)cash_amount / (FP32)price_get(fuel_type))*100000;

    return max_ml;

}
