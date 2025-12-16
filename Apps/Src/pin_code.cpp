#include "pin_code.hpp"

static char pin_code[6] {};

void PIN_CODE::setPinCode(const char* pin)
{
    for (int i = 0; i < 6; i++)
    {
        pin_code[i] = pin[i];
    }
}

bool PIN_CODE::verifyPinCode(const char* pin)
{
    bool result = true;
    for (int i = 0; i < 6; i++)
    {
        if (pin_code[i] != pin[i])
            result = false;
    }
    return result;
}
