#include "ui_task.h"
#include "semphr.h"
#include "emp_type.h"
#include "string.h"
#include "file.h"
#include "lcd.h"
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

void ui_task(void *pvParameters)
{
    UI_STATE_T state = WELCOME;

    INT32U milliliters_fuelled;
    INT8U trash; // TODO: Remove when done debugging
    GAS_T fuel_type = GAS_92; // TODO: Change this when fuel selection is implemented

    while (1)
    {
        switch (state)
        {

            // Det er tilladt at lave hjælpe funktioner internt i denne fil,
            // for at gøre dette switch-statement lidt mere overskueligt :)

            case WELCOME:
                // DEBUGGING
                if (xQueueReceive(xQueueButton, &trash, 0)) INT8U trash; // TODO: Remove when done debugging
                {
                    state = PUMP_READY;
                }
                // DEBUGGING
                break;
            case PAYMENT:
                break;
            case CASH:
                break;
            case ACCOUNT:
                break;
            case PIN:
                break;
            case PRODUCT:
                break;
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
            default:
                break;
        }
    }
}

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
