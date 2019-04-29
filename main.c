#include <msp430.h>
#include "ADC.h"
void main(void)
     {
        WDTCTL = WDTPW | WDTHOLD;                                //Stop watchdog timer
        PM5CTL0 &= ~LOCKLPM5;                                   //Disable the GPIO power-on default high-impedance mode
        //Read_ADC_Bits(1.4, 12, 2.0);                         //prints ADC value in bits(pinNum, resolution, voltage)
        //Read_ADC_Volts(1.4, 12, 0.8);                       //prints ADC value in volts(pinNum, resolution, voltage)
        //printer(ADC_Bits(1.4, 12, 0.8));                   //returns ADC value in bits()
        //printer(ADC_Volts(1.4, 12, 0.8));                 //returns ADC value in volts()
          Custom_Read_ADC_Volts(1.4, 12, 1.4, 0.8);        //prints ADC value in volts with custom min voltage(pinNum, resolution, voltage)
        //Custom_Read_ADC_Bits(1.4, 12, 1.4);             //prints ADC value in volts with custom min voltage(pinNum, resolution, voltage)
        //printer(Custom_ADC_Bits(1.4, 12, 1.4));        //prints ADC value in volts with custom min voltage(pinNum, resolution, voltage)
        //printer(Custom_ADC_Volts(1.4, 12, 1.4, 0.8)); //prints ADC value in volts with custom min voltage(pinNum, resolution, voltage)
    }
