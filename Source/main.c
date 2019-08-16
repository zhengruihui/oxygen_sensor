/******************************************************************************/
#include <msp430.h>

#include <string.h>
#include <stdio.h>
#include <Source/adc.h>
#include <Source/AD5410.h>
#include <Source/common.h>
#include <Source/oled.h>
#include <Source/w5500.h>

#include "math.h"


struct Upload
{
	unsigned long id;
	float average;
};


void init_CS(void);

extern u16 AdcValues[COUNT];
extern double AdcAverage;
extern volatile double ppmAverage;
extern double percentAverage;
extern u8 adcFinished;
extern volatile u16 currentAdcValue;
char cAdcAverage[20]="";
char temp[30]="";


double  sum=0;
double  ppm_sum=0;
double  v=0;


int main( void )
 {	

    double x=0;
    int i=0;
    struct Upload upload;
    OLED_SSD1306_RST_IO_INIT;
    WDTCTL = WDTPW + WDTHOLD;
    P1DIR |= 0x01;   //片选信号初始化时
    GATE_INIT; //继电器控制引脚 P5.1
    GATE_LOW;
    PGA205_INIT;

    PGA205_4; //pga205设置成4倍
    
    init_CS( ); 
    OLED_Init();		//初始化OLED  
    SPI_init();
    init_ADC();
    AD5410_Init();
    AD5410_Reset(4);
    AD5410_Write_ControlREG(1);
    unsigned int mdat[1]={0xfff};

    Load_Net_Parameters();		//装载网络参数	
    W5500_Hardware_Reset();		//硬件复位W5500
    W5500_Initialization();		//W5500初始货配置
    upload.id = 3;

    while (1)
	{
                   
		W5500_Socket_Set();//W5500端口初始化配置

        AD5410_Write_DataREG(mdat,1);

        if (adcFinished)
        {
            sum=0;
            ppm_sum=0;
            v=0;

            for (i =0; i < COUNT; i++)
            {

               v=AdcValues[i];// / 4096.0 * 3.3;
               sum += v;
            }

            AdcAverage =sum / COUNT;
            x = AdcAverage;

            if(x >= 2663)
              ppmAverage = Pa1*pow(x,3) + Pa2*pow(x,2) + Pa3*pow(x,1) + Pa4;
            else if(x >= 2079)
              ppmAverage = Pb1*pow(x,4) + Pb2*pow(x,3) + Pb3*pow(x,2) + Pb4*pow(x,1) + Pb5;
            else if(x > 1000)
              ppmAverage = Pc1*pow(x,4) + Pc2*pow(x,3) + Pc3*pow(x,2) + Pc4*pow(x,1) + Pc5;
            else
              ppmAverage = Pd1*pow(x,3) + Pd2*pow(x,2) + Pd3*pow(x,1) + Pd4;

            if(ppmAverage <=5)
              ppmAverage = 5;

            if((AdcAverage/ 4096.0 * 3.3 >= 2.5) && (PGA205_FACTOR == 4))
            {
              PGA205_2;
            }
            else if((AdcAverage/ 4096.0 * 3.3 <= 0.25) && (PGA205_FACTOR == 2))
            {
              PGA205_4;
            }

            percentAverage = ppmAverage*0.0001;
            sprintf(cAdcAverage,"%.3f v---%d",AdcAverage/ 4096.0 * 3.3,PGA205_FACTOR);
            OLED_ShowString(0,0,(u8 *)cAdcAverage);

            sprintf(temp,"%.6f %%",percentAverage);
            OLED_ShowString(0,2,(u8 *)temp);

            sprintf(temp,"%.3f mA",currentAdcValue/ 4096.0 * 3300);
            OLED_ShowString(0,4,(u8 *)temp);

            sprintf(temp,"%.3f---%d",AdcAverage,PGA205_FACTOR);
            OLED_ShowString(0,6,(u8 *)temp);

            upload.average = (float)percentAverage;
            Write_SOCK_Data_Buffer(0, (u8 *)&upload, sizeof(struct Upload));


            adcFinished=0;
        }

	}

}


void init_CS(void) {
    // Configure two FRAM waitstate as required by the device datasheet for MCLK
    // operation at 24MHz(beyond 8MHz) _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_2 ;

    P2SEL1 |= BIT6 | BIT7;                       // P2.6~P2.7: crystal pins
    do
    {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);              // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);                   // Test oscillator fault flag

    __bis_SR_register(SCG0);                     // disable FLL
    CSCTL3 |= SELREF__XT1CLK;                    // Set XT1 as FLL reference source
    CSCTL0 = 0;                                  // clear DCO and MOD registers
    CSCTL1 = DCORSEL_7;                         // Set DCO = 24MHz
    CSCTL2 = FLLD_0 + 731;                       // DCOCLKDIV = 24MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                     // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));   // FLL locked

    CSCTL4 = SELMS__DCOCLKDIV | SELA__XT1CLK;   // set XT1 (~32768Hz) as ACLK source, ACLK = 32768Hz
                                                 // default DCOCLKDIV as MCLK and SMCLK source
}










