#include <msp430.h>
#include <Source/adc.h>
#include <Source/oled.h>



volatile  u16 AdcValue=0;
volatile  u16 currentAdcValue=0;
u16 AdcValues[COUNT]={0};
u16 AdcResults[9]={0};
u8 count=0;


double AdcAverage=0;
volatile double ppmAverage=0;
double percentAverage=0;
u8 adcFinished=0;

u8 adcIndex = 8;   //adc采集通道

void init_ADC(void)
{
  
    // Configure ADC A1 pin
    P1DIR &= ~BIT1;
    P1SEL0 |= BIT1;
    P1SEL1 |= BIT1;
    
    P5DIR &= ~BIT0;
    P5SEL0 |= BIT0;
    P5SEL1 |= BIT0;
   
    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    //PM5CTL0 &= ~LOCKLPM5;

    // Configure ADC
    ADCCTL0 |= ADCON | ADCMSC;                                // ADCON
    ADCCTL1 |= ADCSHS_2 | ADCCONSEQ_3;                        // repeat single channel; TB1.1 trig sample start
    ADCCTL2 &= ~ADCRES;                                       // clear ADCRES in ADCCTL
    ADCCTL2 |= ADCRES_2;                                      // 12-bit conversion results
    ADCMCTL0 |= ADCINCH_8 | ADCSREF_0;                        // A1 ADC input select; Vref=1.5V
    ADCIE |= ADCIE0;                                          // Enable ADC conv complete interrupt

    // Configure reference
    PMMCTL0_H = PMMPW_H;                                      // Unlock the PMM registers
    PMMCTL2 |= INTREFEN | REFVSEL_0;                          // Enable internal 1.5V reference
    __delay_cycles(400);                                      // Delay for reference settling

   


    // ADC conversion trigger signal - TimerB1.1 (32ms ON-period)
   // TB1CCR0 = 3277-1;                                         // PWM Period
   // TB1CCR1 = 1639-1;                                          // TB1.1 ADC trigger
    
    
   // TB1CCR0 = 3277-1;                                         // PWM Period
    //'TB1CCR1 = 1639-1; 
    
    
        
    TB1CCR0 = 20-1;   //32*5/8                                      // PWM Period
    TB1CCR1 = 10; 
    
    
    
    TB1CCTL1 = OUTMOD_2;                                      // TB1CCR0 toggle
    TB1CTL = TBSSEL__ACLK | MC_1 | TBCLR;                     // ACLK, up mode
    
    ADCCTL0 |= ADCENC;                                        // ADC Enable
    //__bis_SR_register(LPM0_bits);
    __bis_SR_register(GIE);                       // Enter LPM3 w/ interrupts
    
    
}




//中断
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
{
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG))
    {
        case ADCIV_NONE:
            break;
        case ADCIV_ADCOVIFG:
            break;
        case ADCIV_ADCTOVIFG:
            break;
        case ADCIV_ADCHIIFG:
            break;
        case ADCIV_ADCLOIFG:
            break;
        case ADCIV_ADCINIFG:
            break;
        case ADCIV_ADCIFG:
          //计数COUNT次取平均值
            AdcResults[adcIndex] = ADCMEM0;
            if(adcIndex == 1)   //通道1  传感器信号电压值
            {
              AdcValues[count]= AdcResults[adcIndex];
              if(++count >= COUNT)
                count=0;
              if(count==0 && adcFinished==0)
              {
                adcFinished=1;
                //P2OUT ^= 0x01; 
              }
            }
            
            else if(adcIndex == 8)  //通道8   电流对应的电压值
            {
              currentAdcValue = AdcResults[adcIndex];
            }
            
            
//            Set_Bit(P2OUT, ~Get_Bit(P2OUT, BIT0));
//            P2OUT ^= 0x01;  
                         
            ADCIFG = 0;              //中断标志位清零
            
            
            adcIndex--;
            if(adcIndex == 255)
              adcIndex = 8;
            break;         
        default:
            break;
    }
}

