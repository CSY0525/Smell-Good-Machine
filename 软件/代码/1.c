#include "reg51.h"

#define u8 unsigned char
#define u16 unsigned int

sbit Water = P1^0;
sbit USB = P1^4;
sbit SEGL = P2^0;
sbit SEGR = P2^1;
sbit BUTTON1 = P1^1;
sbit BUTTON2 = P1^2;
sbit BUTTON3 = P1^3;
sbit LED1 = P2^4;
sbit LED2 = P2^5;
sbit LED3 = P2^6;
sbit LED4 = P2^7;
sbit LED5 = P2^3;
sbit LED6 = P2^2;

//u8 code tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d, 0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
u8 code tab[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8, 0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
u8 Level = 0,LED_Tick = 0;
u16 FenZhong = 0,MiaoZhong = 0;
/******************************/
void Delay_ms(u16 time);
void Tick(); 
void initT0(); 
void Change_Level();
void SEG_Show();
void Miao_To_Fen();
/******************************/
int main(){
	u8 key_up1 = 1,key_up2 = 1;
	initT0();
	Water = 0;USB = 0;
	BUTTON1 = 1;BUTTON2 = 1;BUTTON3 = 1;//BUTTON4 = 1;BUTTON5 = 1;
	LED1 = 0;LED2 = 0;LED3 = 0;LED4 = 0;LED5 = 0;LED6 = 0;
	while(1){
		Change_Level();
		if(BUTTON1 == 0 && key_up1 == 1){
			Delay_ms(15);
			key_up1 = 0;
			if(BUTTON1 == 0)
				Level++;
		}
		else if(BUTTON1 == 1)
			key_up1 = 1;
		
		if(BUTTON2 == 0)
			Level = 5;
		
		if(BUTTON3 == 0 && key_up2 == 1){
			Delay_ms(15);
			key_up2 = 0;
			if(BUTTON3 == 0)
				USB = ~USB;
		}
		else if(BUTTON3 == 1)
			key_up2 = 1;
		
		SEG_Show();
		Miao_To_Fen();
	}
}

void Miao_To_Fen(){
	if(MiaoZhong >= 60){
		MiaoZhong = 0;
		FenZhong++;
	}
}

void SEG_Show(){
	u8 ShiWei,GeWei;
	ShiWei = FenZhong /10;
	GeWei = FenZhong % 10;
	P0 = tab[ShiWei];
	SEGL = 1;Delay_ms(10);SEGL = 0;
	P0 = tab[GeWei];
	SEGR = 1;Delay_ms(10);SEGR = 0;
}

void Change_Level(){
	if(Level == 0){
		Water = 0;
		LED1 = 0;LED2 = 0;LED3 = 0;LED4 = 0;LED5 = 1;
	}
	
	if(Level == 1){
		LED1 = 1;LED2 = 0;LED3 = 0;LED4 = 0;LED5 = 0;
		if(FenZhong >= 15){
			FenZhong = 0;
			Water = 1;Delay_ms(1500);Water = 0;
		}
	}
	
	if(Level == 2){
		LED1 = 0;LED2 = 1;LED3 = 0;LED4 = 0;LED5 = 0;
		if(FenZhong >= 15){
			FenZhong = 0;
			Water = 1;Delay_ms(3000);Water = 0;
		}
	}
	
	if(Level == 3){
		LED1 = 0;LED2 = 0;LED3 = 1;LED4 = 0;LED5 = 0;
		if(FenZhong >= 15){
			FenZhong = 0;
			Water = 1;Delay_ms(6000);Water = 0;
		}
	}
	
	if(Level == 4)
		Level = 0;
	
	if(Level == 5){
		LED1 = 0;LED2 = 0;LED3 = 0;LED4 = 1;LED5 = 0;
		Water = 1;Delay_ms(15000);Water = 0;
		Level = 0;
		FenZhong = 0;
		Delay_ms(200);
	}
}

void Tick() interrupt 1 {
	u16 num;
	TH0 = (65536 - 10000) / 256;
	TL0 = (65536 - 10000) % 256;
	num++;
	if(num == 100){
		num = 0;
		MiaoZhong++;
		LED6 = ~LED6;
	}
}

void initT0(){
	TMOD = 0x01;
	TH0 = (65536 - 10000) / 256;
	TL0 = (65536 - 10000) % 256;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
}

void Delay_ms(u16 time){
	u16 i,j;
	for(i = 0;i < time;i++)
		for(j = 0;j < 110;j++);
}