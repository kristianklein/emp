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

typedef enum {WELCOME, PAYMENT, CASH, ACCOUNT, PIN, PRODUCT, PUMP_READY, FUELING_DONE} UI_STATE_T;

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

// Helper functions
void ui_print_fuel(INT32U milliliters);
void ui_print_summary(INT32U milliliters, INT64U price);
void ui_clear();

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
/***************************** Functions *************************************/
void ui_task(void *pvParameters)
{
    UI_STATE_T state = WELCOME;

    INT32U milliliters_fuelled;
    INT8U trash; // TODO: Remove when done debugging
    GAS_T fuel_type = GAS_92; // TODO: Change this when fuel selection is implemented

    INT8U keypad_received, digi_received, account_length = 0, pin_length = 0;
    INT16U Cash_amount = 0;
    INT8U account_number[ACCOUNT_SIZE], pin_number[PIN_SIZE];

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
                    pumpready_text();
                    fuel_type = GAS_92;
                    state = PUMP_READY;
                }
                else if(keypad_received == '2')
                {
                    pumpready_text();
                    fuel_type = GAS_95;
                    state = PUMP_READY;
                }
                else if(keypad_received == '3')
                {
                    pumpready_text();
                    fuel_type = GAS_E10;
                    state = PUMP_READY;
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
                gfprintf(LCD, "Welcome!");

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
    gfprintf(LCD, "Fuel: %02d.%03d", milliliters/1000, milliliters%1000);
    file_put(LCD, 0x0A);
    gfprintf(LCD, "Price: %03d.", price/100);
    gfprintf(LCD, "%02d", price%100);
    gfprintf(UART0, "%08d", price);
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
    file_put(LCD, 0x00);
    gfprintf(LCD, "Pump is ready");
    file_put(LCD, 0x0A);
    gfprintf(LCD, "Handle is on");
}

