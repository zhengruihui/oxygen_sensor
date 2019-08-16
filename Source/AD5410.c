#include <msp430.h>
#include <string.h>
#include <Source/AD5410.h>

void AD5410_Init(void)
{
        CONFIG_AD5410_PORT;
}

unsigned char AD5410_Write_Bytes(unsigned char *ptr,unsigned char chipNum)
{
        unsigned char mDat[18];
        unsigned char mchipNum=0;
       
        memcpy(mDat,ptr,chipNum*3);

        AD5410_LATCH_L;
        AD5410_SCLK_L;
        while(mchipNum<chipNum)
                {
            int mPos;
            int mNum;
                        for(mPos=0;mPos<3;mPos++)
                                for(mNum=0;mNum<8;mNum++)
                                        {
                                                AD5410_SCLK_L;
                                                if((mDat[3*mchipNum+2-mPos]&0x80)==0x80)
                                                        AD5410_SDI_H;
                                                else
                                                        AD5410_SDI_L;
                                               
                                                AD5410_SCLK_H;
                                                AD5410_SCLK_L;
                                                mDat[3*mchipNum+2-mPos]<<=1;
                                        }
                        mchipNum++;
                }
        AD5410_LATCH_L;
        AD5410_LATCH_H;
        AD5410_LATCH_L;
        return 1;       
}


unsigned char AD5410_Reset(unsigned char chipNum)
{
        unsigned char mDat[18];
        int i=0;
        //unsigned char mchipNum=0;
        mDat[0]=0x01;
        mDat[1]=0x00;
        mDat[2]=AD5410_RST_REG;
        for(i=0;i<5;i++)
                memcpy(mDat+3+i*3,mDat+i*3,3);
        AD5410_Write_Bytes(mDat,chipNum);
        return 1;
}

unsigned char AD5410_Write_ControlREG(unsigned char chipNum)
{
        unsigned char mDat[18];
        int i=0;
        mDat[0]=AD5410_DCEN+AD5410_R;
        mDat[1]=AD5410_OUTEN;
        mDat[2]=AD5410_CTR_REG;
        for(i=0;i<5;i++)
                memcpy(mDat+3+i*3,mDat+i*3,3);
        AD5410_Write_Bytes(mDat,chipNum);
        return 1;
}

unsigned char AD5410_Write_DataREG(unsigned int *dat,unsigned char chipNum)
{
        unsigned char mDat[18];
        int i=0;
        for(i=0;i<chipNum;i++)
                {
                        
                        mDat[1+i*3]=(unsigned char)(*dat);
                        mDat[0+i*3]=(unsigned char)(*dat >> 8);
                        mDat[2+i*3]=AD5410_DATA_REG;
                }
        AD5410_Write_Bytes(mDat,chipNum);
        return 1;
}

unsigned char AD5410_Current_Output(float *currentArray,unsigned char chipNum)
{
        unsigned int currentDat[6];
        int i=0;
        for(i=0;i<chipNum;i++)
                {
                        currentArray[chipNum]=(currentArray[chipNum]-4)*4096/16;
                        unsigned int mC=(unsigned int)currentArray[chipNum]*10;
                        if((mC%10)>=5)
                                currentArray[chipNum]=currentArray[chipNum]+1;
                        currentDat[chipNum]=(unsigned int)currentArray[chipNum];
                }
        AD5410_Write_DataREG(currentDat,chipNum);
        return 1;
}
