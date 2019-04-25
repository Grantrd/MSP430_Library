#include <msp430.h>
#include <stdio.h>
#include <stdbool.h>


void printer(float var)
{
    volatile float vint;
    vint = var - (int)var;
    if(vint == 0.00)
    {
        printf("%i\n", (int)var);
    }
    else
    {
        printf("%3.3f\n",var);
    }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


void Read_ADC_Bits(int res, float volts)
{
    volatile int i;
    volatile float AD;
    volatile bool extref;
    P1SEL0 &= ~BIT4;
    P1SEL1 |= BIT4;
    extref = false;
    int volt = (int)(volts * 10);
    if(volt == 12)
    {
        REFCTL0 |= REFVSEL_0;     /* Configure internal 1.2V reference. */
        ADC12MCTL5 |= ADC12VRSEL_1;
    }
    else if(volt == 20)
    {
        REFCTL0  |= REFVSEL_1;      /* Configure internal 2.0V reference. */
        ADC12MCTL5 |= ADC12VRSEL_1;
    }
    else if(volt == 25)
    {
        REFCTL0  |= REFVSEL_2;      /* Configure internal 2.5V reference. */
        ADC12MCTL5 |= ADC12VRSEL_1;
    }
    else if(volt == 33)
    {
        ADC12MCTL5 |= ADC12VRSEL_0;  // Configure internal 3.3V reference. */
    }
    else
    {
        extref = true;
    }
    REFCTL0  |= REFON;

    if(res == 8)
    {
        ADC12CTL2 &= ADC12RES_0; //resolution 8 bit

    }
    else if(res == 10)
    {
        ADC12CTL2 &= ADC12RES_1; //resolution resolution 10 bit
    }
    else
    {
        ADC12CTL2 &= ADC12RES_2; //resolution 12 bit
    }

    __delay_cycles(100);                    // delay to allow Ref to settle
    ADC12CTL0 |= ADC12SHT0_0 | ADC12ON; /* Turn on ADC12, 4 ADC12CLK cycles for sampling */
    if (extref)
    {
        ADC12CTL1 = ADC12SHP;
        ADC12MCTL0 = ADC12VRSEL_4 | ADC12INCH_4;   // Vr+ = VeREF+ (ext) and Vr-=VeREF-

    }
    else
    {
        ADC12CTL3 |= ADC12CSTARTADD_5;      /* select memory buffer, currently choose 5*/
        ADC12MCTL5 |= ADC12INCH_4; // Memory control register - Currently control memory 5 -> Channel: A
    }

    ADC12CTL0 |= ADC12ENC;              // Enable conversion

    for(;;)
    {
        ADC12CTL0 &= ~ADC12SC;                      // clear the start bit
        ADC12CTL0 |= ADC12SC;                      // Sampling and conversion start
        __delay_cycles(100);                      // delay to allow Ref to settle
        if(extref)
        {
            AD = ADC12MEM0;     //external ref conversion
        }
        else
        {
            AD = ADC12MEM5;   // Read in results if conversion
        }
        __delay_cycles(100);                    // delay to allow Ref to settle
        printer(AD);
        i = 200;                          // SW Delay
        do i--;
        while(i != 0);
    }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


void Read_ADC_Volts(int res, float volts)
{
    volatile int resolution;
    volatile int i;
    volatile float AD;
    volatile bool extref;
    P1SEL0 &= ~BIT4;
    P1SEL1 |= BIT4;
    extref = false;
    int volt = (int)(volts * 10);
    if(volt == 12)
    {
        REFCTL0 |= REFVSEL_0;     /* Configure internal 1.2V reference. */
        ADC12MCTL5 |= ADC12VRSEL_1;
    }
    else if(volt == 20)
    {
        REFCTL0  |= REFVSEL_1;      /* Configure internal 2.0V reference. */
        ADC12MCTL5 |= ADC12VRSEL_1;
    }
    else if(volt == 25)
    {
        REFCTL0  |= REFVSEL_2;      /* Configure internal 2.5V reference. */
        ADC12MCTL5 |= ADC12VRSEL_1;
    }
    else if(volt == 33)
    {
        ADC12MCTL5 |= ADC12VRSEL_0;  // Configure internal 3.3V reference. */
    }
    else
    {
        extref = true;
    }
    REFCTL0  |= REFON;

    if(res == 8)
    {
        ADC12CTL2 &= ADC12RES_0; //resolution 8 bit
        resolution = 255;

    }
    else if(res == 10)
    {
        ADC12CTL2 &= ADC12RES_1; //resolution resolution 10 bit
        resolution = 1023;
    }
    else
    {
        ADC12CTL2 &= ADC12RES_2; //resolution 12 bit
        resolution = 4095;
    }

    __delay_cycles(100);                    // delay to allow Ref to settle
    ADC12CTL0 |= ADC12SHT0_0 | ADC12ON; /* Turn on ADC12, 4 ADC12CLK cycles for sampling */
    if (extref)
    {
        ADC12CTL1 = ADC12SHP;
        ADC12MCTL0 = ADC12VRSEL_4 | ADC12INCH_4;   // Vr+ = VeREF+ (ext) and Vr-=VeREF-

    }
    else
    {
        ADC12CTL3 |= ADC12CSTARTADD_5;      /* select memory buffer, currently choose 5*/
        ADC12MCTL5 |= ADC12INCH_4; // Memory control register - Currently control memory 5 -> Channel: A
    }
    ADC12CTL0 |= ADC12ENC;              // Enable conversion
    for(;;)
    {
        ADC12CTL0 &= ~ADC12SC;                      // clear the start bit
        ADC12CTL0 |= ADC12SC;                      // Sampling and conversion start
        __delay_cycles(100);                      // delay to allow Ref to settle
        if(extref)
        {
            AD = ADC12MEM0;     //external ref conversion
        }
        else
        {
            AD = ADC12MEM5;   // Read in results if conversion
        }
        __delay_cycles(100);                    // delay to allow Ref to settle
        AD = AD * (float)(volts/resolution);
        printer(AD);
        i = 200;                          // SW Delay
        do i--;
        while(i != 0);
    }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


float ADC_Volts(int res, float volts)
{
    volatile int resolution;
    volatile float AD;
    volatile bool extref;
    P1SEL0 &= ~BIT4;
    P1SEL1 |= BIT4;
    extref = false;
    int volt = (int)(volts * 10);
    if(volt == 12)
    {
        REFCTL0 |= REFVSEL_0;     /* Configure internal 1.2V reference. */
        ADC12MCTL5 |= ADC12VRSEL_1;
    }
    else if(volt == 20)
    {
        REFCTL0  |= REFVSEL_1;      /* Configure internal 2.0V reference. */
        ADC12MCTL5 |= ADC12VRSEL_1;
    }
    else if(volt == 25)
    {
        REFCTL0  |= REFVSEL_2;      /* Configure internal 2.5V reference. */
        ADC12MCTL5 |= ADC12VRSEL_1;
    }
    else if(volt == 33)
    {
        ADC12MCTL5 |= ADC12VRSEL_0;  // Configure internal 3.3V reference. */
    }
    else
    {
        extref = true;
    }
    REFCTL0  |= REFON;

    if(res == 8)
    {
        ADC12CTL2 &= ADC12RES_0; //resolution 8 bit
        resolution = 255;

    }
    else if(res == 10)
    {
        ADC12CTL2 &= ADC12RES_1; //resolution resolution 10 bit
        resolution = 1023;
    }
    else
    {
        ADC12CTL2 &= ADC12RES_2; //resolution 12 bit
        resolution = 4095;
    }

    __delay_cycles(100);                    // delay to allow Ref to settle
    ADC12CTL0 |= ADC12SHT0_0 | ADC12ON; /* Turn on ADC12, 4 ADC12CLK cycles for sampling */
    if (extref)
    {
        ADC12CTL1 = ADC12SHP;
        ADC12MCTL0 = ADC12VRSEL_4 | ADC12INCH_4;   // Vr+ = VeREF+ (ext) and Vr-=VeREF-

    }
    else
    {
        ADC12CTL3 |= ADC12CSTARTADD_5;      /* select memory buffer, currently choose 5*/
        ADC12MCTL5 |= ADC12INCH_4; // Memory control register - Currently control memory 5 -> Channel: A
    }

    ADC12CTL0 |= ADC12ENC;              // Enable conversion
    ADC12CTL0 &= ~ADC12SC;                      // clear the start bit
    ADC12CTL0 |= ADC12SC;                      // Sampling and conversion start
    __delay_cycles(100);                      // delay to allow Ref to settle
    if(extref)
    {
        AD = ADC12MEM0;     //external ref conversion
    }
    else
    {
        AD = ADC12MEM5;   // Read in results if conversion
    }
    __delay_cycles(100);                    // delay to allow Ref to settle
    AD = AD * (float)(volts/resolution);
    return AD;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


int ADC_Bits(int res, float volts)
{
    volatile int AD;
    volatile bool extref;
    P1SEL0 &= ~BIT4;
    P1SEL1 |= BIT4;
    extref = false;
    int volt = (int)(volts * 10);
    if(volt == 12)
    {
        REFCTL0 |= REFVSEL_0;     /* Configure internal 1.2V reference. */
        ADC12MCTL5 |= ADC12VRSEL_1;
    }
    else if(volt == 20)
    {
        REFCTL0  |= REFVSEL_1;      /* Configure internal 2.0V reference. */
        ADC12MCTL5 |= ADC12VRSEL_1;
    }
    else if(volt == 25)
    {
        REFCTL0  |= REFVSEL_2;      /* Configure internal 2.5V reference. */
        ADC12MCTL5 |= ADC12VRSEL_1;
    }
    else if(volt == 33)
    {
        ADC12MCTL5 |= ADC12VRSEL_0;  // Configure internal 3.3V reference. */
    }
    else
    {
        extref = true;
    }
    REFCTL0  |= REFON;

    if(res == 8)
    {
        ADC12CTL2 &= ADC12RES_0; //resolution 8 bit

    }
    else if(res == 10)
    {
        ADC12CTL2 &= ADC12RES_1; //resolution resolution 10 bit
    }
    else
    {
        ADC12CTL2 &= ADC12RES_2; //resolution 12 bit
    }

    __delay_cycles(100);                    // delay to allow Ref to settle
    ADC12CTL0 |= ADC12SHT0_0 | ADC12ON; /* Turn on ADC12, 4 ADC12CLK cycles for sampling */
    if (extref)
    {
        ADC12CTL1 = ADC12SHP;
        ADC12MCTL0 = ADC12VRSEL_4 | ADC12INCH_4;   // Vr+ = VeREF+ (ext) and Vr-=VeREF-

    }
    else
    {
        ADC12CTL3 |= ADC12CSTARTADD_5;      /* select memory buffer, currently choose 5*/
        ADC12MCTL5 |= ADC12INCH_4; // Memory control register - Currently control memory 5 -> Channel: A
    }
    ADC12CTL0 |= ADC12ENC;              // Enable conversion
    ADC12CTL0 &= ~ADC12SC;                      // clear the start bit
    ADC12CTL0 |= ADC12SC;                      // Sampling and conversion start
    __delay_cycles(100);                      // delay to allow Ref to settle
    if(extref)
    {
        AD = ADC12MEM0;     //external ref conversion
    }
    else
    {
        AD = ADC12MEM5;   // Read in results if conversion
    }
    __delay_cycles(100);                    // delay to allow Ref to settle
    return AD;
}
void Custom_Read_ADC_Volts(int res, float volts, float min)
{
    float minVolts = min;
    volatile int resolution;
    volatile int i;
    volatile float AD;
    volatile bool extref;
    P1SEL0 &= ~BIT4;
    P1SEL1 |= BIT4;
    extref = false;
    int volt = (int)(volts * 10);
    if(volt == 12)
    {
        REFCTL0 |= REFVSEL_0;     /* Configure internal 1.2V reference. */
        ADC12MCTL5 |= ADC12VRSEL_7;
    }
    else if(volt == 20)
    {
        REFCTL0  |= REFVSEL_1;      /* Configure internal 2.0V reference. */
        ADC12MCTL5 |= ADC12VRSEL_7;
    }
    else if(volt == 25)
    {
        REFCTL0  |= REFVSEL_2;      /* Configure internal 2.5V reference. */
        ADC12MCTL5 |= ADC12VRSEL_7;
    }
    else if(volt == 33)
    {
        ADC12MCTL5 |= ADC12VRSEL_12;  // Configure internal 3.3V reference, and sets selectable voltage */
    }
    else
    {
        extref = true;
    }
    REFCTL0  |= REFON;

    if(res == 8)
    {
        ADC12CTL2 &= ADC12RES_0; //resolution 8 bit
        resolution = 255;

    }
    else if(res == 10)
    {
        ADC12CTL2 &= ADC12RES_1; //resolution resolution 10 bit
        resolution = 1023;
    }
    else
    {
        ADC12CTL2 &= ADC12RES_2; //resolution 12 bit
        resolution = 4095;
    }

    __delay_cycles(100);                    // delay to allow Ref to settle
    ADC12CTL0 |= ADC12SHT0_0 | ADC12ON; /* Turn on ADC12, 4 ADC12CLK cycles for sampling */
    if (extref)
    {
        ADC12CTL1 = ADC12SHP;
        ADC12MCTL0 = ADC12VRSEL_14 | ADC12INCH_4;   // Vr+ = VeREF+ (ext) and Vr-=VeREF-
    }
    else
    {
        ADC12CTL3 |= ADC12CSTARTADD_5;      /* select memory buffer, currently choose 5*/
        ADC12MCTL5 |= ADC12INCH_4; // Memory control register - Currently control memory 5 -> Channel: A
    }
    ADC12CTL0 |= ADC12ENC;              // Enable conversion
    for(;;)
    {
        ADC12CTL0 &= ~ADC12SC;                      // clear the start bit
        ADC12CTL0 |= ADC12SC;                      // Sampling and conversion start
        __delay_cycles(100);                      // delay to allow Ref to settle
        if(extref)
        {
            AD = ADC12MEM0;     //external ref conversion
        }
        else
        {
            AD = ADC12MEM5;   // Read in results if conversion
        }
        __delay_cycles(100);                    // delay to allow Ref to settle
        AD = (AD * (float)((volts-minVolts)/resolution)) + minVolts;
        printer(AD);
        i = 200;                          // SW Delay
        do i--;
        while(i != 0);
    }
}

void main(void)
     {
        WDTCTL = WDTPW | WDTHOLD;                    //Stop watchdog timer
        PM5CTL0 &= ~LOCKLPM5;                       //Disable the GPIO power-on default high-impedance mode
        //Read_ADC_Bits(12, 0.8);                  //prints ADC value in bits(resolution, voltage)
        //Read_ADC_Volts(12, 0.8);                //prints ADC value in volts(resolution, voltage)
        //printer(ADC_Bits(12, 0.8));            //returns ADC value in bits()
        //printer(ADC_Volts(12, 0.8));          //returns ADC value in volts()
        Custom_Read_ADC_Volts(12, 1.4, 0.8);   //prints ADC value in volts with custom min voltage(resolution, voltage)
    }
