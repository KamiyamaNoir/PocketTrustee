#ifndef PIN_CODE_H
#define PIN_CODE_H

class PIN_CODE
{
public:
    static void setPinCode(const char* pin);
    static bool verifyPinCode(const char* pin);
};

#endif
