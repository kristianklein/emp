#include "price.h"

INT16U price_92 = 1000; // 10.00 DKK
INT16U price_95 = 1125; // 11.25 DKK
INT16U price_E10 = 1250; // 12.50 DKK


void price_set(GAS_T type, INT16U new_price)
{
    switch (type)
    {
        case GAS_92:
            price_92 = new_price;
            break;
        case GAS_95:
            price_95 = new_price;
            break;
        case GAS_E10:
            price_E10 = new_price;
            break;
        default:
            break;
    }
}


INT16U price_get(GAS_T type)
{
    INT16U price;

    switch (type)
        {
            case GAS_92:
                price = price_92;
                break;
            case GAS_95:
                price = price_95;
                break;
            case GAS_E10:
                price = price_E10;
                break;
            default:
                break;
        }

    return price;
}
