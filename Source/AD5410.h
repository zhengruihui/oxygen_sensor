#ifndef _AD5410_H
#define _AD5410_H

#define AD5410_Port_OUT P6OUT
#define AD5410_Port_DIR        P6DIR
#define AD5410_LATCH_BIT BIT1      
#define AD5410_SDI_BIT BIT5       
#define AD5410_SCLK_BIT BIT2
#define AD5410_CLEAR_BIT BIT0

#define AD5410_CLEAR_H                AD5410_Port_OUT|=AD5410_CLEAR_BIT
#define AD5410_CLEAR_L                AD5410_Port_OUT&=~AD5410_CLEAR_BIT


#define AD5410_LATCH_H                AD5410_Port_OUT|=AD5410_LATCH_BIT
#define AD5410_LATCH_L                AD5410_Port_OUT&=~AD5410_LATCH_BIT

#define AD5410_SDI_H                AD5410_Port_OUT|=AD5410_SDI_BIT
#define AD5410_SDI_L                AD5410_Port_OUT&=~AD5410_SDI_BIT

#define AD5410_SCLK_H                AD5410_Port_OUT|=AD5410_SCLK_BIT
#define AD5410_SCLK_L                AD5410_Port_OUT&=~AD5410_SCLK_BIT


#define CONFIG_AD5410_PORT        {AD5410_Port_DIR |= (AD5410_LATCH_BIT+AD5410_SDI_BIT+AD5410_SCLK_BIT+AD5410_CLEAR_BIT);AD5410_LATCH_L;AD5410_SCLK_L;AD5410_SDI_H;AD5410_CLEAR_L;}


#define AD5410_COMMAND_NOP        0
#define AD5410_DATA_REG        1
#define AD5410_RECALL        2
#define AD5410_CTR_REG        0x55
#define AD5410_RST_REG        0x56


#define AD5410_READ_STA_REG        0
#define AD5410_READ_DAT_REG        1
#define AD5410_READ_CTR_REG        2

#define AD5410_REXT        0x20
#define AD5410_OUTEN        0x10
#define AD5410_SR_CLOCK        0x01
#define AD5410_SR_STEP                0xC0
#define AD5410_SREN        0x10
#define AD5410_DCEN        0x08
#define AD5410_R                0x05

void AD5410_Init(void);
unsigned char AD5410_Write_Bytes(unsigned char *ptr,unsigned char chipNum);
unsigned char AD5410_Reset(unsigned char chipNum);
unsigned char AD5410_Write_ControlREG(unsigned char chipNum);
unsigned char AD5410_Write_DataREG(unsigned int *dat,unsigned char chipNum);
unsigned char AD5410_Current_Output(float *currentArray,unsigned char chipNum);



#endif
