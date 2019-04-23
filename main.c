#include <msp430.h>
#include <stdio.h>
void printer(int var)
{
        printf("%d\n",var);
}

int adc(int p,int res, float volts)
{
    volatile int AD;
    P1SEL0 &= ~BIT4;
    P1SEL1 |= BIT4;
    int volt = (int)(volts * 10);

    if(volt == 12)
    {
        REFCTL0 |= REFVSEL_0;     /* Configure internal 1.2V reference. */
        ADC12MCTL5 |= ADC12VRSEL_1;
        printer(volt);
    }
    else if(volt == 20)
    {
        REFCTL0  |= REFVSEL_1;      /* Configure internal 2.0V reference. */
        ADC12MCTL5 |= ADC12VRSEL_1;
        printer(volt);
    }
    else if(volt == 25)
    {
        REFCTL0  |= REFVSEL_2;      /* Configure internal 2.5V reference. */
        ADC12MCTL5 |= ADC12VRSEL_1;
        printer(volt);
    }
    else
    {
        ADC12MCTL5 |= ADC12VRSEL_0;  // Configure internal 3.3V reference. */
        printer(volt);
    }
    //ADC12MCTL5 |= ADC12VRSEL_1;
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
    ADC12CTL3 |= ADC12CSTARTADD_5;      /* select memory buffer, currently choose 5*/
    ADC12MCTL5 |= ADC12INCH_4; // Memory control register - Currently control memory 5 -> Channel: A
    ADC12CTL0 |= ADC12ENC;              // Enable conversion
    if(p)
    {
        volatile int i;
        for(;;)
        {
            ADC12CTL0 &= ~ADC12SC;  // clear the start bit
            ADC12CTL0 |= ADC12SC;       // Sampling and conversion start
            __delay_cycles(100);                    // delay to allow Ref to settle
            AD = ADC12MEM5;      // Read in results if conversion
            __delay_cycles(100);                    // delay to allow Ref to settle
            printf("%d\n",AD);
            i = 200;                          // SW Delay
            do i--;
            while(i != 0);
        }
    }
    else
    {
        ADC12CTL0 &= ~ADC12SC;  // clear the start bit
        ADC12CTL0 |= ADC12SC;       // Sampling and conversion start
        __delay_cycles(100);                    // delay to allow Ref to settle
        AD = ADC12MEM5;      // Read in results if conversion
        __delay_cycles(100);                    // delay to allow Ref to settle
        printer(AD);
    }
    return AD;
}


void main(void) {
    WDTCTL = WDTPW | WDTHOLD;                //Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   //Disable the GPIO power-on default high-impedance mode
    adc(1, 12, 3.3);                      //adc(print, resolution, internal voltage reference, )
    }


