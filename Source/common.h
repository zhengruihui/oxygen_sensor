#ifndef	_COMMON_H_
#define	_COMMON_H_

#define u8 unsigned char
#define u16 unsigned short
#define  u32 unsigned int 

#define		Set_Bit(val, bitn)		(val |= ((bitn)))		
#define		Clr_Bit(val, bitn)		(val &= ~((bitn)))		
#define		Get_Bit(val, bitn)		(val & (1<<(bitn)) )		

#define COUNT 100

#define PGA205_INIT P3DIR|= 0x0c
#define PGA205_1 P3OUT &= 0xf3   //00 1±¶
#define PGA205_2 P3OUT &= 0xf3;P3OUT |= 0x04   //10 2±¶
#define PGA205_4 P3OUT &= 0xf3;P3OUT |= 0x08   //01 4±¶
#define PGA205_8 P3OUT &= 0xf3;P3OUT |= 0x0c   //11 8±¶
#define PGA205_FACTOR (P3OUT & 0x0c)==0x00? 1 : ((P3OUT & 0x0c)==0x04? 2 : ((P3OUT & 0x0c)==0x08? 4 : 8))

#define GATE_INIT P5DIR|= 0x02;
#define GATE_HIGH Set_Bit(P5OUT, BIT1)
#define GATE_LOW Clr_Bit(P5OUT, BIT1);

//#define P1 0.04646
//#define P2 -7.022
//#define P3 0
//#define P4 0

//#define P1 9.81e-12
//#define P2 -5.907e-8
//#define P3 0.0001285
//#define P4 -0.05366
//#define P5 33.78

//#define P1 -1.949e-12
//#define P2 2.203e-08 
//#define P3 -0.0001002
//#define P4 0.2302
//#define P5 -268.8
//#define P6 1.289e+05


#define Pa1 -1.141583e-5
#define Pa2 0.0963621
#define Pa3 -271.318488
#define Pa4 254911.5070


#define Pb1 1.367e-09
#define Pb2 -1.41e-05
#define Pb3 0.0544
#define Pb4 -93.49
#define Pb5 6.08e+04

#define Pc1 6.301e-09
#define Pc2 -4.937e-05
#define Pc3 0.1479
#define Pc4 -202
#define Pc5 1.071e+05

#define Pd1 -6.759e-05 
#define Pd2 0.2825
#define Pd3 -395.2
#define Pd4 1.9e+05


//#define PP1 -2.123
//#define PP2 163.9
//#define PP3 -6907
//#define PP4 1.436e5
//#define PP5 0

#define PP1 1.72e5
#define PP2 -0.0477


#endif
