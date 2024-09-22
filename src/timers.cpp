#include <MillisTimer.h>
#include <timers.h>


MillisTimer Timer_EffectPeriodCompare;
MillisTimer Timer_InhaleEffects;
MillisTimer Timer_VentilationEffects;
MillisTimer Timer_StillEffects;
MillisTimer Timer_Calibration;
MillisTimer Timer_Temp;

MillisTimer Timer_MeasureBattery;
MillisTimer Timer_BatteryCharged;
MillisTimer Timer_Charging;
MillisTimer Timer_CriticalBattery;

MillisTimer Timer_Lighters;

void InitMillisCalls(){
    Timer_EffectPeriodCompare.begin();
    Timer_InhaleEffects.begin();
    Timer_VentilationEffects.begin();
    Timer_StillEffects.begin();
    Timer_Calibration.begin();
    Timer_Temp.begin();

    Timer_MeasureBattery.begin();
    Timer_BatteryCharged.begin();
    Timer_Charging.begin();
    Timer_CriticalBattery.begin();

    Timer_Lighters.begin();
}