#ifndef POCKETTRUSTEE_DRIVER_ADC_HPP
#define POCKETTRUSTEE_DRIVER_ADC_HPP

class CoreADC
{
public:
    static void Refresh();
    static void RefreshAsync();

    static float GetBatteryVoltage();
    static float GetRSSI();
};

#endif //POCKETTRUSTEE_DRIVER_ADC_HPP
